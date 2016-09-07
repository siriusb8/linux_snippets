/* 
 * SYSLOG(3)
 * closelog, openlog, syslog, vsyslog - send messages to the system logger
 */

#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	openlog("syslog test", LOG_PID | LOG_CONS, LOG_USER );

	syslog( LOG_EMERG, "system is unusable, pid=%d", getpid() );
	syslog( LOG_ALERT, "action must be takenimmediately, pid=%d", getpid() );
	syslog( LOG_CRIT, "critical conditions, pid=%d", getpid() );
	
	syslog( LOG_ERR, "error conditions, pid=%d", getpid() );
	syslog( LOG_WARNING, "warning conditions, pid=%d", getpid() );
	syslog( LOG_NOTICE, "normal, but significant, condition, pid=%d", getpid() );
	syslog( LOG_INFO, "informal message, pid=%d", getpid() );
	syslog( LOG_DEBUG, "debug-level message, pid=%d", getpid() );

	closelog();
}
