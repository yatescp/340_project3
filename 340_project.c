#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h> 
#include <assert.h>
#include <unistd.h>
#include <string.h>

sem_t count_mutex;
sem_t total_mutex;
sem_t line_mutex;
int total_word_count;

size_t MAXLENGTH = 100;

typedef struct __node {
    char* line;
    struct node *next;
}node;

typedef struct queue {
    node *head;
    node *tail;
    pthread_mutex_t head_lock,tail_lock;
};
	
struct queue a;
struct queue *Q=&a;

void queue_init(struct queue *q){
    node *tmp = malloc(sizeof(node));
    tmp->next=NULL;
    tmp->line= malloc(sizeof(char)*MAXLENGTH);
    q->head = q->tail = tmp;
    pthread_mutex_init(&q->head_lock, NULL);
    pthread_mutex_init(&q->tail_lock, NULL);
}

void q_push(struct queue *q, char *line){
    if (q->head->next == NULL){
	printf("no next node found, overwriting the head\n");
        strncpy(q->head->line, line, sizeof(line));
        return;
    }
    node *tmp = malloc(sizeof(node));
    assert(tmp != NULL);
    tmp->line = malloc(sizeof(char)*MAXLENGTH);
    strncpy(tmp->line, line, sizeof(line));
    tmp->next = NULL;
    pthread_mutex_lock(&q->tail_lock);
    q->tail->next = tmp;
    q->tail=tmp;
    printf("New node created with value: %s", tmp->line);
    pthread_mutex_unlock(&q->tail_lock);
}

int q_pop(struct queue *q){
    printf("q_pop called, line value: %s\n", q->head->line);
    pthread_mutex_lock(&q->head_lock);
    node *tmp = q->head;
    node *new_head = tmp->next;
    if(new_head == NULL){
        pthread_mutex_unlock(&q->head_lock);
        //signal empty
        return -1;
    }
    q->head=new_head;
    pthread_mutex_unlock(&q->head_lock);
    free(tmp);
    return 0;
}

int countWords(char* line){

	int count = 0;
	bool whitespace = true;

	while(*line){

		//check if this character is a whitespace, if so set whitespace to true
		//this also accounts for multiple whitespaces in succession
		if (*line == ' ' || *line == '\n' || *line == '\t') {
			whitespace = true;
		}

		//this only happens if the last character was a whitespace and this one isn't
		else if(whitespace == true){
			whitespace = false;
			count = count + 1;
		}
		++line;

	}
	return count;
}


void* task(void* x)
{
	//cast void* x to int and save as task number
	int tasknum = (long)x;
	printf("task %d starting\n", tasknum);

	
	char* line;

	line = (char*)malloc(sizeof(char)*MAXLENGTH);
	///*
	//pop from the q and store the text in line
	while (Q->head->next != NULL) {
		/*
		//redundant
		if (total_word_count > 10000) {
			break;
		}
		sem_wait(&total_mutex);
		total_word_count = total_word_count + 1;

		printf("task: %d \tcount: %d\n", tasknum, total_word_count);
		sem_post(&total_mutex);
		*/
		sem_wait(&total_mutex);

		printf("attempting to access head\n");

		line = Q->head->line;
		//printf("asdgjklaalkdgklajg");

		//line = "gaskldgal";

		printf("head value: %s\n", line);

		q_pop(Q);

		int wordCount = countWords(line);

		printf("task %d: %s, %d words\n", tasknum, line, wordCount);

		total_word_count = total_word_count + wordCount;
		sem_wait(&total_mutex);
		//*/


	} //(line != NULL);
	//*/

	printf("task %d ending\n", tasknum);

	return 0;
}

int main(int argc, char **argv)
{
	int tasknum;
	if (argc > 2) {
		printf("too many arguments, please provide only 1\n");
	}
	if (argc < 2) {
		printf("No arguments provided, defaulting task number to 2\n");
		tasknum = 2;
	}
	else {
		tasknum = atoi(argv[1]);
		printf("tasks: %d\n", tasknum);
	}

	//character buffer
	char *k;
	char c;
	int char_index = 0;

	k = (char*)malloc(sizeof(char)*MAXLENGTH);

	queue_init(Q);

	while(getline(&k, &MAXLENGTH, stdin) != -1){
		//push the lineof text to our queue

		q_push(Q, k);

		//testing
		printf("%s", k);
	}

	free(k);

	//initialize semaphores
	sem_init(&count_mutex, 0, 1);
	sem_init(&total_mutex, 0, 1);
	sem_init(&line_mutex, 0, 1);


	//make tasknum threads
	pthread_t tasks[tasknum];

	int i;

	//initialize threads
    	for (i = 0; i < tasknum; i++) {
        	pthread_create(&tasks[i], NULL, task, (void*)(long)(i + 1));
	}
	//wait for threads to finish
	for (i = 0; i < tasknum; i++) {
        	void* res;
        	pthread_join(tasks[i], &res);
	}

	printf("Total Word Count: %d\n", total_word_count);

	return 0;
	
} 
