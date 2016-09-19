#ifndef STRING_QUEUE_H_
#define STRING_QUEUE_H_ 1

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <stdint.h>
#include <stdbool.h>

#define MAX_ELEMENTS 32

//!<@struct node_t
typedef struct node_Tag
{
	char *data;                //!< string for node data
	struct node_Tag *nextNode;   //!< link to next node, may be NULL to indicate rear
}node_t;

//!<@strQueue_t
typedef struct strQueue_tag
{
	node_t *front;          //!< front node
	node_t *rear;           //!< rear node
	uint8_t nodeCount;      //!< current number of nodes
	uint8_t maxElements;    //!< maximum number of nodes
}strQueue_t; 


void queue_Create(strQueue_t * queue, uint8_t elements);

uint8_t queue_push(strQueue_t * queue, const char * str);

char *queue_pop(strQueue_t * queue);



bool queue_IsEmpty(const strQueue_t * queue);
bool queue_IsFull(const strQueue_t * queue);

void queue_Display(strQueue_t * queue);
uint8_t queue_Size(const strQueue_t * queue);


// :BAD: char *queue_Front(strQueue_t * queue);

__inline__ void queue_removeData(node_t * node)
{
	char *str = node->data;
	free(str);
}

#endif // STRING_QUEUE_H_
