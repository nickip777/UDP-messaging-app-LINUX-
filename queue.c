#include <stdio.h>
#include <stdlib.h>
#include "LIST.h"


struct LIST* list;

//create queue
void createQueue(){
	list = ListCreate();
}

//add to queue
int enqueue(void * item){
	return ListAppend(list,item);
}

//remove from queue
void* dequeue(){
	return ListTrim(list);
}

//peek top
void * peek(){
    struct Node * node = ListLast(list);
    return node->data;
}

//check if empty
int isEmpty(){
	if(ListCount(list) == 0)
		return 1;
	return 0;
}






