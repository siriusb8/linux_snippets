#include "string_queue.h"


/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param   queue   a queue structure reference
 * @param elements [description]
 */
void queue_Create(strQueue_t * queue, uint8_t elements)
{
	queue->front        =   NULL;
	queue->rear         =   NULL; 
	queue->nodeCount    =   0;

	if(elements == 0)
	{
		queue->maxElements  =   MAX_ELEMENTS;
	}
	else
	{
		queue->maxElements  =   elements;
	}
}
//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param   queue   a queue structure reference
 * @param   str [description]
 * 
 * @return [description]
 */
uint8_t queue_push(strQueue_t *queue, const char *str)
{
	node_t *temp = malloc(sizeof(node_t));

	temp->data = malloc(strlen(str)+1);
	strcpy(temp->data, str);
	temp->nextNode = NULL;

	if (queue->front == NULL)
	{
		queue->front = temp;
	}
	else
	{
		queue->rear->nextNode = temp;
	}
	queue->rear = temp;

	return ++(queue->nodeCount);
}


#if 0
uint8_t queue_push(strQueue_t *queue, const char *str)
{
    // if first element and last element in queue. i.e. no allocations yet
    if(queue->front == NULL)
    {
        queue->rear             = (node_t *)malloc(sizeof(node_t));
        queue->rear->nextNode   = NULL; // null link
        queue->rear->data       = malloc(strlen(str) + 1);
        queue->rear->data       = strncpy(queue->rear->data, (char *)str, strlen(str));
        queue->front            = queue->rear;
    }
    // if any elements exist so fart
    else
    {
        node_t *temp            = (node_t *)malloc(sizeof(node_t));
        queue->rear->nextNode   = temp; // link to next rear node
        temp->data              = malloc(strlen(str) + 1);
        temp->data              = strncpy(temp->data, (char *)str, strlen(str));
        temp->nextNode          = NULL; //ensure rear maintains null link

        queue->rear = temp;     
    }
    return queue->nodeCount++;
}
#endif // 0
//----------------------------------------------------------------------------------------------------------------------

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param   queue   a queue structure reference
 * @return [description]
 */
char *queue_pop(strQueue_t *queue)
{
	node_t *temp = queue->front;
	char   *ret  = NULL;

	if (queue->front == NULL)
	{
		return NULL;
	}

	queue->front = queue->front->nextNode;
	if (queue->front == NULL)
	{
		queue->rear = NULL;
	}

	queue->nodeCount--;
	ret = temp->data;
	free(temp);
	return ret;
}
#if 0
char * dequeue(strQueue_t * queue)
{
    node_t * temp = queue->front;

    if(queue->front == NULL)
    {
        return NULL;
    } 
    if(queue->front == queue->rear)
    {
        queue->front    = NULL;
        queue->rear     = NULL;
    }
    else
    {
        queue->front = (node_t *)queue->front->nextNode;
    }
    queue->nodeCount--;

    removeData(temp);
    free(temp);

    return queue->front->data;
}
#endif // 0


/**
 * @brief returns string at the front of the queue
 * @details [long description]
 * 
 * @param   queue   a queue structure reference
 * @return pointer to string
 */
const char * queue_Front(const strQueue_t * queue)
{
	if (queue->front == NULL)
	{
		return NULL;
	}
	return queue->front->data;
}
#if 0
char * queue_Front(strQueue_t * queue)
{
    if ((queue->front != NULL) && (queue->rear != NULL))
        return (queue->front->data);
    else
        return 0;
}
#endif // 0

#if 0
/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param   queue   a queue structure reference
 * @return [description]
 */
uint8_t queueIsEmpty(const strQueue_t *queue)
{
    if(queue->front == NULL && queue->rear == NULL && queue->nodeCount == 0)
        return 1;
    else
        return 0;
}
#endif // 0

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param   queue   a queue structure reference
 * @return [description]
 */
void queue_Display(strQueue_t *queue)
{
    node_t * tempNode = queue->front;
    while(tempNode != NULL)
    {
        printf("%s\n", tempNode->data);
        tempNode = tempNode->nextNode;
    }
}


/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param   queue   a queue structure reference
 * @return [description]
 */
uint8_t queue_Size(const strQueue_t * queue)
{
    return queue->nodeCount;
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param   queue   a queue structure reference
 * @return [description]
 */
bool queue_IsEmpty(const strQueue_t *queue)
{
	return ( queue->nodeCount == 0);
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param   queue   a queue structure reference
 * @return [description]
 */
bool queue_IsFull(const strQueue_t *queue)
{
    if(queue->front != NULL && queue->nodeCount >= MAX_ELEMENTS)
    {
        return true;
    }
    else
    {
        return false;
    }
}
