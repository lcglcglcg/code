
#include "lg_queue.h"

lg_queue_t *lg_queue_init(lg_queue_t *queue, unsigned int size)
{

    if (queue == NULL)
	if (!(queue = (lg_queue_t *)malloc(sizeof(lg_queue_t))))
	    return NULL;

    if (!(queue->buffer = (char *)malloc(size))) {

	free(queue);
	return NULL;
    }

    queue->buffer_size = size;
    queue->start_pos = 0;
    queue->end_pos = 0;
    return queue;
}

void lg_queue_free(lg_queue_t  *queue)
{
    free(queue->buffer);
    free(queue);
}

int lg_queue_enqueue(lg_queue_t *queue, char *buffer, unsigned int size)
{

    unsigned int surplus = 0;
    if (queue->start_pos <=  queue->end_pos)
	surplus = queue->buffer_size - queue->end_pos + queue->start_pos;
    else
	surplus = queue->start_pos - queue->end_pos;

    if (surplus <= size)
	return -1;

    unsigned int end_surplus = queue->buffer_size - queue->end_pos;
    if (end_surplus < size) {

	unsigned int temp_size = size - end_surplus;
	memcpy((queue->buffer + queue->end_pos), buffer, end_surplus);
	memcpy((queue->buffer), (buffer + end_surplus), temp_size);
	queue->end_pos = temp_size;

    } else {

	memcpy((queue->buffer + queue->end_pos), buffer, size);
	queue->end_pos += size;
    }

    return 0;
}

int lg_queue_dequeue(lg_queue_t *queue, char *buffer, unsigned int size)
{

    unsigned int used = 0;
    if (queue->start_pos < queue->end_pos)
	used = queue->end_pos - queue->start_pos;
    else if (queue->start_pos > queue->end_pos)
	used = queue->buffer_size - queue->start_pos + queue->end_pos;
    else
	return -1;

    if (used < size)
	return -1;

    unsigned int start_used = queue->buffer_size - queue->start_pos;
    if (start_used < size) {

	unsigned int temp_size = size - start_used;
	memcpy(buffer, (queue->buffer + queue->start_pos), start_used);
	memcpy((buffer + start_used), queue->buffer, temp_size);
	queue->start_pos = temp_size;

    } else {

	memcpy(buffer, (queue->buffer + queue->start_pos), size);
	queue->start_pos += size;
    }

    return 0;
}

