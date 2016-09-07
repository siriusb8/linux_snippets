/*
 * SYSLOG(2)
 * syslog, klogctl - read and/or clear kernel message ring buffer; set console_loglevel
 *
 * Compile: gcc -Wall  klogctl_test.c -o klogctl_test
 *
 * http://embedjournal.com/implementing-circular-buffer-embedded-c/
 */

// ----- Includes
#include <sys/klog.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <stdint.h>
#include <stdbool.h>

// ----- Defines

// Redefinition: symbolic names are defined in the kernel source, but are not exported to user space

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

// ----- Macros

#define CIRCBUF_DEF(x,y) uint8_t x##_space[y]; circBuf_t x = { x##_space,0,0,y}

#define DBG(format, ...) ( printf("DEBUG: " #format "\n", ##__VA_ARGS__) )

// ----- Prototypes

 
typedef struct
{
	uint8_t * const buffer;
	int head;
	int tail;
	const int maxLen;
} circBuf_t;

/**
 * :TODO:
 * @param c
 * @param data
 *
 * @return   0 success
 *          -1 buf full
 */
int circBufPush(circBuf_t *c, uint8_t data)
{
	int next = c->head + 1;
	if (next >= c->maxLen)
	{
		next = 0;
	}

	// Cicular buffer is full
	if (next == c->tail)
	{
		return -1;  // quit with an error
	}

	c->buffer[c->head] = data;
	c->head = next;
	return 0;
}

/**
 * :TODO:
 * @param c
 * @param data
 *
 * @return   0 success
 *          -1 we don't have any character
 */
int circBufPop(circBuf_t *c, uint8_t *data)
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (c->head == c->tail)
	{
		return -1;  // quit with an error
	}

	*data = c->buffer[c->tail];
	c->buffer[c->tail] = 0;  // clear the data (optional)

	int next = c->tail + 1;
	if(next >= c->maxLen)
	{
		next = 0;
	}
	c->tail = next;

	return 0;
}


CIRCBUF_DEF(cb, 32);
 
int main()
{
	uint8_t data_u8 = 0;

	for ( int i = 0; i<32; i++ )
	{
		data_u8 = i;
		
		if ( circBufPush( &cb, data_u8 ) == -1 ) 
		{
			DBG("Out of space in CB");
		}
	}

	while ( circBufPop( &cb, &data_u8 ) != -1 ) 
	{
		printf( "%d\n", data_u8 );
	}
	DBG("CB is empty");
	

	return (0); 
}

#if 0
int main()
{
	pthread_t 


	// :TODO: Check root

	int size = klogctl( SYSLOG_ACTION_SIZE_BUFFER, NULL, 0 );
	char *buf = malloc( size );

	if ( buf == NULL )
	{
		perror("No mem");
		exit(EXIT_FAILURE);
	}


	printf( "kernel ring buf size: %d bytes\n", size );

	int unread = klogctl( SYSLOG_ACTION_SIZE_UNREAD, NULL, 0 );
	if ( unread == -1 )
	{
		perror("klogctl SYSLOG_ACTION_SIZE_UNREAD");
		exit(EXIT_FAILURE);
	}
	printf( "SYSLOG_ACTION_SIZE_UNREAD: %d bytes\n", unread );


	klogctl( SYSLOG_ACTION_OPEN, NULL, 0 );
	
	char myBuf_sz[21];
	int len = klogctl( SYSLOG_ACTION_READ, myBuf_sz, sizeof(myBuf_sz)-1 );
	
	if ( len == -1 )
	{
		perror("klogctl SYSLOG_ACTION_READ");
		exit(EXIT_FAILURE);
	}
	myBuf_sz[20]='\0';
	printf( " %s \n", myBuf_sz );

	klogctl( SYSLOG_ACTION_CLOSE, NULL, 0 );

	// klogctl( SYSLOG_ACTION_READ_ALL, buf, size );

#if 0
	klogctl( SYSLOG_ACTION_READ_ALL, buf, size );
	printf( "%s", buf );
#endif // 0

	free( buf );

	return 0;
}
#endif // 0
