/*
 * SYSLOG(2)
 * syslog, klogctl - read and/or clear kernel message ring buffer; set console_loglevel
 *
 * Compile: gcc -Wall  klogctl_test.c -o klogctl_test
 */

#include <sys/klog.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

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

int main()
{

	// Check privi

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
