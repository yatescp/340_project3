#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <assert.h>
#include <unistd.h>

typedef struct __node {
    char* line=NULL;
    struct node *next=NULL;
}node;

typedef struct queue {
    node *head;
    node *tail;
    pthread_mutex_t head_lock,tail_lock;
};

void queue_init(struct queue *q){
    node *tmp = malloc(sizeof(node));
    tmp->next=NULL;
    q->head = q->tail = tmp;
	printf("init");
    pthread_mutex_init(&q->head_lock, NULL);
    pthread_mutex_init(&q->tail_lock, NULL);
}

void q_push(struct queue *q, char *line){
    node *tmp = malloc(sizeof(node));
    assert(tmp != NULL);
    tmp->line = line;
    tmp->next = NULL;
	printf("push");
    pthread_mutex_lock(&q->tail_lock);
    q->tail->next = tmp;
    q->tail=tmp;
    pthread_mutex_unlock(&q->tail_lock);
}

int q_pop(struct queue *q, char* *line){
    pthread_mutex_lock(&q->head_lock);
    node *tmp = q->head;
    node *new_head = tmp->next;
	printf("pop");
    if(new_head == NULL){
        pthread_mutex_unlock(&q->head_lock);
        //signal empty
        return -1;
    }
    *line = new_head->line;
    q->head=new_head;
    pthread_mutex_unlock(&q->head_lock);
    free(tmp);
    return 0;
}
/*
int main(int argc, char **argv){
	struct queue q;
	struct queue *a=&q;
	queue_init(a);
	char* l = "abc";
	char** b = &l;
	q_push(a, l);
	q_pop(a, b);
}
*/
