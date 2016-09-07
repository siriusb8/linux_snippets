#include <stdio.h>
#include <stdlib.h>
 
struct buffer {
	int size;
	int start;
	//int end;  // position of last element
	/* Tracking start and end of buffer would waste
	 * one position. A full buffer would always have
	 * to leave last position empty or otherwise
	 * it would look empty. Instead this buffer uses
	 * count to track if buffer is empty or full
	 */
	int count; // number of elements in buffer
	/* Two ways to make buffer element type opaque
	 * First is by using typedef for the element
	 * pointer. Second is by using void pointer.
	 */
	/* different types of buffer: 
	int *element;   // array of integers
	char *element;  // array of characters 
	void *element;  // array of void type (could cast to int, char, etc)
	char **element; //array of char pointers (array of strings)
	void **element; // array of void pointers
	Choosing array of void pointers since it's the most flexible */
	void **element;
};
 
typedef struct buffer buffer_t;
 
/**
 * @return  0 success 
 *		 -1 error , ou of mem
 */
int init(buffer_t *buffer, int size) 
{
	buffer->size = size;
	buffer->start = 0;
	buffer->count = 0;
	buffer->element = malloc( sizeof(buffer->element) * size );
	
	if ( NULL == buffer->element )
	{
		return -1;
	}
	/* allocated array of void pointers. Same as below */
	//buffer->element = malloc(sizeof(void *) * size);
	return 0;
}
 
int full(buffer_t *buffer) 
{
	if (buffer->count == buffer->size) { 
		return 1;
	} else {
		return 0;
	}
}
 
int empty(buffer_t *buffer) 
{
	if (buffer->count == 0) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * ring buffer
 */
void push(buffer_t *buffer, void *data) 
{
	int index;

	index = buffer->start + buffer->count;
	buffer->count++;
	
	if (index >= buffer->size) 
	{
		index = 0;
		buffer->count = buffer->size;
	}
	buffer->element[index] = data;
}

#if 0
	int index;

	if (full(buffer)) 
	{
		printf("Buffer overflow\n");
	} else {
		index = buffer->start + buffer->count++;
		if (index >= buffer->size) {
			index = 0;
		}
		buffer->element[index] = data;
	}
}
#endif // 0

// FIFO
void * popqueue(buffer_t *buffer) 
{
	void * element;
	if (empty(buffer)) {
		printf("Buffer underflow\n");
		return "0";
	} else {
	   /* FIFO implementation */
	   element = buffer->element[buffer->start];
	   buffer->start++;
	   buffer->count--;
	   if (buffer->start == buffer->size) {
		   buffer->start = 0;
	   }
		
	   return element;
	}
}

// LIFO
void * popstack(buffer_t *buffer) 
{
	int index;
	if ( empty(buffer) ) 
	{
		printf("Buffer underflow\n");
		return "0";
	} 
	else 
	{
		/* LIFO implementation */
		index = buffer->start + buffer->count - 1;
		if (index >= buffer->size) 
		{
		   index = buffer->count - buffer->size - 1;
		}
		buffer->count--;
		return buffer->element[index];
	}
}

int main() 
{
	buffer_t buffer;
	init( &buffer, 5);
	char *data[] = {"first" , "second", "third", "fourth" , "fifth", "sixth"};
	int i;
	
	for (i = 0; i < 6; i++) 
	{
		printf("push: %s\n", data[i]);
		push(&buffer, data[i]);
	}
	printf("\n");
	for (i = 0; i < 6; i++) 
	{
		/* need to cast void pointers to char pointers */
		printf("pop from queue: %s\n", (char *)popqueue(&buffer));
	}
	 
	for (i = 0; i < 6; i++) 
	{
		printf("push: %s\n", data[i]);
		push(&buffer, data[i]);
	}
	for (i = 0; i < 6; i++) {
		printf("pop from stack: %s\n", (char *)popstack(&buffer));
	}
	 
	 
}
