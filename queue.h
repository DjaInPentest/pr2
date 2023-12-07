#ifndef QUEUE_H
#define QUEUE_H

typedef struct QueueElement {
    char* value;
    struct QueueElement* next;
} QueueElement;

typedef struct {
    QueueElement* head;
    QueueElement* tail;
} Queue;

Queue* createQueue();
void qpush(Queue* queue, const char* value);
void qpop(Queue* queue);
void printQueue(Queue* queue);
void saveQueueToFile(Queue* queue, const char* filename);
void loadQueueFromFile(Queue* queue, const char* filename);

#endif // QUEUE_H
