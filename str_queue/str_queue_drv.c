
#include "string_queue.h"


#define BOOL_STR(val) ( ((val) == 1) ? "true" : "false" )

#ifdef TEST_DRV
int main ( int argc, char **argv )
{
	char *str;
	strQueue_t queue;

	queue_Create( &queue, 5);

	queue_push(&queue, "Hello");
	queue_push(&queue, "World");

	printf("queueIsEmpty = %s\n", BOOL_STR( queue_IsEmpty( &queue ) ) );
	printf("queue_Size=%d\n", queue_Size( &queue ) );

	queue_Display( &queue );

	str = queue_pop( &queue );
	printf ( "In queue:%s\n", str );

	printf("--> queue_Size=%d\n", queue_Size( &queue ) );

	str = queue_pop( &queue );
	printf ( "In queue:%s\n", str );

	printf("--> queue_Size=%d\n", queue_Size( &queue ) );

	printf("queueIsEmpty = %s\n", BOOL_STR( queue_IsEmpty( &queue ) ) );

	//queue_destroy(q1);

	return (0);
}
#endif // TEST_DRV
