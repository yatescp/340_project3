#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h> 
#include <assert.h>

sem_t count_mutex;
sem_t total_mutex;
int total_word_count;

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

	
	char* line = "test string";

	/*
	//pop from the q and store the text in line
	do {

		q_pop(Q, &line);

		//queue empty
		if (line == NULL) {
			return 0;
		}

		sem_wait(&count_mutex);

		int wordCount = countWords(line);

		printf("task %d: %s, %d words\n", tasknum, line, wordCount);

		total_word_count = total_word_count + wordCount;
		sem_post(&count_mutex);

		printf("task %d ending\n", tasknum);
	
	} while (line != NULL)
	*/

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
	size_t maxLength = 32;
	char c;
	int char_index = 0;
	
	k = (char*)malloc(sizeof(char)*maxLength);

	/* 
	queue Q;
	queue_init(Q);
	*/

	while(getline(&k, &maxLength, stdin) != -1){

		/*
		//push the lineof text to our queue
		q_push(Q, k);
		*/

		//testing
		printf("%s", k);
	}

	free(k);

	//initialize semaphores
	sem_init(&count_mutex, 0, 1);
	sem_init(&total_mutex, 0, 1);

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
	
}
