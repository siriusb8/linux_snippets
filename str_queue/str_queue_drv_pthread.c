/**
 *
 * Communication via string queue between main process and thread
 * Datatsource: klog ringbuffer
 *
 */

#include "string_queue.h"

#include <pthread.h>
#include <semaphore.h>

#include <unistd.h>

#include <errno.h>

#include <sys/klog.h>
#include <stdio.h>
#include <stdlib.h>


#include <unistd.h>		// getuid()
#include <sys/types.h>

#define SYSLOG_ACTION_CLOSE          0 /* Close the log.  Currently a NOP. */
#define SYSLOG_ACTION_OPEN           1 /* Open the log. Currently a NOP. */
#define SYSLOG_ACTION_READ           2 /* Read from the log. */
#define SYSLOG_ACTION_READ_ALL       3 /* Read all messages remaining in the ring buffer. (allowed for non-root) */
#define SYSLOG_ACTION_READ_CLEAR     4 /* Read and clear all messages remaining in the ring buffer */
#define SYSLOG_ACTION_CLEAR          5 /* Clear ring buffer. */
#define SYSLOG_ACTION_CONSOLE_OFF    6 /* Disable printk's to console */
#define SYSLOG_ACTION_CONSOLE_ON     7 /* Enable printk's to console */
#define SYSLOG_ACTION_CONSOLE_LEVEL  8 /* Set level of messages printed to console */
#define SYSLOG_ACTION_SIZE_UNREAD    9 /* Return number of unread characters in the log buffer */
#define SYSLOG_ACTION_SIZE_BUFFER   10 /* Return size of the log buffer */


#define DEFAULT_MINIMUM LOG_DEBUG
#define DEFAULT_MAXIMUM INT_MAX

#define DEFAULT_CONSOLE_LEVEL 7
#define MIN_CONSOLE_LEVEL 0
#define MAX_CONSOLE_LEVEL 7


#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define warn(fmt, args...) \
    fprintf(stderr, "%s: " fmt "\n", "metalog" , ## args)
#define warnf(fmt, args...) warn("%s():%i: " fmt, __func__, __LINE__ , ## args)
#define warnp(fmt, args...) warn(fmt ": %s" , ## args , strerror(errno))
#define _err(wfunc, fmt, args...) \
    do { \
        wfunc("error: " fmt, ## args); \
        exit(EXIT_FAILURE); \
    } while (0)
#define err(fmt, args...) _err(warn, fmt, ## args)
#define errf(fmt, args...) _err(warnf, fmt, ## args)
#define errp(fmt, args...) _err(warnp, fmt , ## args)


#define LINE_MAX 4096



#define DEBUG 0

#define BOOL_STR(val) ( ((val) == 1) ? "true" : "false" )


// Always check the code below by compiler
// 
#define DPRINT(fmt, ...) \
        do \
        { \
        if (DEBUG) { fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); } \
        } while (0)


static 	strQueue_t queue_st;

static sem_t full, empty;			// the semaphores
static pthread_attr_t attr_at;		// Set of thread attr_atibutes
static pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;

#if 0
static pthread_mutex_t queueMutex;
pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_mutex_t mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
#endif // 0



/*
 * klog buffer Thread
 */
void *klogBufferRead (void *arg) 
{
	int loop = 1;
	char line[LINE_MAX];
	int status;	// klogctl
	static int console_level = DEFAULT_CONSOLE_LEVEL;

	if ( ( klogctl(SYSLOG_ACTION_OPEN, NULL, 0) < 0 )                       ||
		 ( klogctl(SYSLOG_ACTION_CONSOLE_LEVEL, NULL, console_level) < 0 )  
	   ) 
	{
		perror("Unable to control the kernel logging device");
		return NULL;
	}

	fprintf( stdout, "Start reading kernel messages...\n" );
	while (loop) 
	{
		while ( ( (status = klogctl(SYSLOG_ACTION_READ, line, sizeof (line) ) ) == 0)  && (errno == EINTR ) )
		{
			continue;
		}
		if (status == -1) 
		{
			perror("klogctl");
			loop = 0;
			break;
		}
		// --->>> <<<---
		DPRINT("Waiting...");
		sem_wait(&empty);					/* acquire the empty lock */
		pthread_mutex_lock(&queueMutex);	/* acquire the mutex lock */
		queue_push( &queue_st, line );
		DPRINT("queue filled\n");
		sleep(1);
		pthread_mutex_unlock(&queueMutex);
		DPRINT(" mutex unlock\n");
		sem_post(&full);		/* signal full */
		// --->>> <<<---

	} 	// loop until error
	klogctl( SYSLOG_ACTION_CONSOLE_ON, NULL, 0 );
	klogctl( SYSLOG_ACTION_CLOSE, NULL, 0 );

	return NULL;
}



/**
* Producer thread
*/
void *producer(void *arg)
{

	DPRINT("I'm the producer\n");
	while(1)
	{
		printf("Waiting...");
		sem_wait(&empty);		/* acquire the empty lock */
		pthread_mutex_lock(&queueMutex);		/* acquire the mutex lock */
		queue_push( &queue_st, "Hello" );
		DPRINT("queue filled\n");
		sleep(1);
		pthread_mutex_unlock(&queueMutex);
		DPRINT(" mutex unlock\n");
		sem_post(&full);		/* signal full */
	
	}
	
	return NULL;
}

// Main is "consumer"
int main ( int argc, char **argv )
{
	pthread_t thread; 	// Thread ID


	// Check if executed under root permissions (klogctl) 
	if ( getuid() != 0 )
	{
		// Tell user to run app as root, then exit.
		fprintf( stdout, "Needs to be run with root permissions\n");
		exit(EXIT_FAILURE);
	}

	// :DYNAMIC: pthread_mutex_init (&queueMutex, NULL);

	/* Create the full semaphore and initialize to 0 */
	sem_init(&full, 0, 0);

	/* Create the empty semaphore and initialize to BUFFER_SIZE */
	sem_init(&empty, 0, MAX_ELEMENTS);

	/* Get the default attributes */
	pthread_attr_init(&attr_at);



	queue_Create( &queue_st, MAX_ELEMENTS );

	pthread_create ( &thread, NULL, klogBufferRead, NULL );

	fprintf( stdout, "Waiting for data...\n" );
	// :TODO: check if thread is still running
	while(1)
	{
		sleep(1);
		sem_wait(&full);		/* aquire the full lock */
		pthread_mutex_lock(&queueMutex);

		if ( queue_IsEmpty( &queue_st ) == false )
		{
			char *str = queue_pop( &queue_st );
			printf ( "Consumer:%s\n", str );
		}
		pthread_mutex_unlock(&queueMutex);
		sem_post(&empty); /* signal empty */
	}

	pthread_join( thread, NULL );

	return (0);
}
