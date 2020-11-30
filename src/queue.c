#include <semaphore.h>
#include <assert.h>
#include <pthread.h>
#include "queue.h"



Queue queueCreate(unsigned capacity){
  Queue q = malloc(sizeof(queue_t)); 
  sem_init(&(q->lock), 0, 1);
  q->capacity = capacity;
  q->front = q->size = 0;
  q->rear = capacity - 1;
  q->array = malloc(q->capacity * sizeof(*q->array));
  return q;
}

void queueDestroy(Queue q){
  sem_destroy(&(q->lock));
  free(q->array);
  free(q);
}

int queueIsFull(Queue q){
  return (q->size == q->capacity); 
}

int queueIsEmpty(Queue q){
  return (q->size == 0);
}

int queueEnqueue(Queue q, int* value){ 
  if (queueIsFull(q)) return -1;
  sem_wait(&(q->lock));
  q->rear = (q->rear + 1) % q->capacity;
  q->array[q->rear] = *value;
  q->size = q->size + 1;
  sem_post(&(q->lock));
  return 0;
}

int queueDequeueFront(Queue queue, int* container){ 
  if (queueIsEmpty(queue)) return -1; 
  sem_wait(&(queue->lock));
  *container = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  sem_post(&(queue->lock));
  return 0;
}

int queueDequeueRear(Queue queue, int* container){
  if (queueIsEmpty(queue)) return -1; 
  sem_wait(&(queue->lock));
  *container = queue->array[queue->rear];
  queue->rear = (queue->capacity + queue->rear - 1) % queue->capacity;
  queue->size = queue->size - 1; 
  sem_post(&(queue->lock));
  return 0;
}

int* queueFront(Queue queue){
  if (queueIsEmpty(queue)) return NULL;
  return &queue->array[queue->front]; 
}

int* queueRear(Queue queue){ 
  if (queueIsEmpty(queue)) return NULL;
  return &queue->array[queue->rear];
}

void queuePrint(Queue q){
  printf("Queue(^: front, *: rear): [");
  int i;
  for(i = 0; i < q->capacity; i++){
    printf("%d", q->array[i]);
    if(i == q->front) printf("^");
    if(i == q->rear) printf("*");
    i == q->capacity - 1 ? printf("] ") : printf(", ");
  }
  printf("size=%d, capacity=%d\n", q->size, q->capacity);
  fflush(stdout);
} 
