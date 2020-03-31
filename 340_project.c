#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* task(void* x)
{
	//cast void* x to int and save as task number
	int tasknum = (long)x;
	printf("task %d starting\n", tasknum);
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
	
	k = (char*)malloc(sizeof(char)*maxLength);

	//await input, right now takes a single line of text
	printf("enter a line of text: ");
	getline(&k, &maxLength, stdin);
	printf("you typed: %s\n", k);

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

	
}
