/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include "LIST.h"
#define MAX_NUM_NODES 20
#define MAX_NUM_HEADS 2

struct Node nodes[MAX_NUM_NODES]= {{0}};
struct LIST lists[MAX_NUM_HEADS]= {{0}};
struct Node listHeads[MAX_NUM_HEADS] = {{0}};
struct LIST nodeTracker;
struct LIST listTracker;
struct Node headNode;
struct Node tailNode;
int isInitialized = -1;

int countTotalHeads(struct LIST *list){
    return MAX_NUM_HEADS;
}

//attach node to list
//*********************************************
void attachNode(struct LIST *list, struct Node *node, int location, int orientation) {
    if (list->size == 0) { // if list empty
        list->head = node;
        list->tail = node;
    } else if (list->size == 1) { // if only 1 item
        if (orientation == 0) {
            node->next = list->tail;
            list->head = node;
        } else {
            list->head->next = node;
            list->tail = node;
        }
    } else if (location == 0) { // head
        if (orientation == 0) { // left
            node->next = list->head;
            list->head = node;
        } else { // right
            node->next = list->head->next;
            list->head->next = node;
        }
    } else if (location == 1) { // tail
        if (orientation == 0) { // left
            list->current = list->tail;
            ListPrev(list);
            list->current->next = node;
            node->next = list->tail;
        } else {//right
            list->tail->next = node;
            list->tail = node;
        }
    } else if (location == 2) { // current
        if(list->current == &headNode || (list->current == list->head && orientation == 0)){
            node->next = list->head;
            list->head = node;
        }else if(list->current == &tailNode || (list->current == list->tail && orientation == 1)){
            list->tail->next = node;
            list->tail = node;
        }else if (orientation == 0) { //left
            ListPrev(list);
            node->next = list->current->next;
            list->current->next = node;
        } else { // right
            node->next = list->current->next;
            list->current->next = node;
        }
    }
    list->size++;
    list->current = node;
}

//initialize Lists that track nodes and heads
//*********************************************
void initializeListTrackers(){
    for(int i = 0; i < MAX_NUM_NODES; i++){ //nodes
        attachNode(&nodeTracker,&nodes[i],1,1);
    }
    
    for(int i = 0; i < MAX_NUM_HEADS; i++){ // attach heads to nodes
        listHeads[i].data = &lists[i];
    }
    
    for(int i = 0; i < MAX_NUM_HEADS; i++){ // heads
        attachNode(&listTracker,&listHeads[i],1,1);
    }
    listTracker.current = listTracker.head;
    nodeTracker.current = nodeTracker.head;
    countTotalHeads(&listTracker);
}

//return node to free node list
//*********************************************
void returnNode(struct Node *node){
    node->data = NULL;
    node->next = NULL;
    attachNode(&nodeTracker, node, 1,1);
}

//NULL all fields of list
//*********************************************
void clearList(struct LIST *list){
    list->current = NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}



int countFreeHeads(){
    return listTracker.size;
}

int countFreeNodes(){
    return nodeTracker.size;
}

int countTotalNodes(){
    return MAX_NUM_NODES;
}



//check for empty nodes
//*********************************************
int isEmptyNodeExists(){
    if(ListCount(&nodeTracker) > 0){
        return 0;
    }else{
        return -1;
    }
}

//get empty nodes
//*********************************************
struct Node *getEmptyNode(){
    if(isEmptyNodeExists() == 0){
        struct Node *temp = nodeTracker.head; // get node from head
        nodeTracker.head = nodeTracker.head->next;
        nodeTracker.current = nodeTracker.head;
        nodeTracker.size--;
        temp->data = NULL;
        temp->next = NULL;
        return temp;
    }else{
        return NULL;
    }
}

//check for empty lists
//*********************************************
int isEmptyListExists(){
    if(ListCount(&listTracker) > 0){
        return 0;
    }else{
        return -1;
    }
}

//get empty list
//*********************************************
struct LIST *getEmptyList(){
    if(isEmptyListExists() == 0){
        void *temp = listTracker.current->data;
        struct LIST *tempList = temp;
        clearList(tempList);
        ListNext(&listTracker);
        listTracker.size--;
        countTotalHeads(&listTracker);
        return temp;
    }else{
        return NULL;
    }
}

//create list
//*********************************************
struct LIST *ListCreate(){
    if(isInitialized != 0){
        initializeListTrackers();
        isInitialized = 0;
    }
    if(isEmptyListExists() == 0){
        return getEmptyList(); 
    }else{
        return NULL;
    }
    
}

//count list
//*********************************************
int ListCount(struct LIST *list){
    if(list == NULL){
        return 0;
    }
    return list->size;
}

//move pointer to first node
//*********************************************
void *ListFirst(struct LIST *list){
    if(list == NULL){
        return NULL;
    }
    list->current = list->head;
    return list->current;
}

//move pointer to tail
//*********************************************
void *ListLast(struct LIST *list){
    if(list == NULL){
        return NULL;
    }
    list->current = list->tail;
    return list->current;
}

//iterate pointer forward
//*********************************************
void *ListNext(struct LIST *list){
    if(list == NULL){
        return NULL;
    }
    if(list->current == list->tail){
        list->current = &tailNode; // beyond tail
    }else if(list->current == &tailNode){
        return list->current; 
    }else if(list->current == &headNode){
        list->current = list->head; // beyond head
    }else{
        list->current = list->current->next;
    }
    return list->current;
}

//iterate pointer backwards
//*********************************************
void *ListPrev(struct LIST *list){
    if(list == NULL){
        return NULL;
    }
    if(list->current == list->head){ //beyond head
        list->current = &headNode;
    }else if(list->current == &headNode){
        return list->current;
    }else if(list->current == &tailNode){ //beyond tail
        list->current = list->tail;
    }else{
        struct Node *temp = list->head;
        while (temp != list->tail){ //iterate through list until next node matches
            if(temp->next == list->current){
                list->current = temp;
                return list->current;
            }else{
                temp = temp->next;
            }
        }
        
    }
    return list->current;
}

//get current item
//*********************************************
void *ListCurr(struct LIST *list){
    if(list == NULL){
        return NULL;
    }
    return list->current;
}

//add to list
//*********************************************
int ListAdd(struct LIST *list, void * item){
    if(list == NULL || item == NULL){
        return -1;
    }
    if(isEmptyNodeExists() == 0){
        struct Node *temp = getEmptyNode();
        temp->data = item;
        attachNode(list,temp,2,1);
        return 0;
    }else{
        return -1;
    }
    
}

//insert to list
//*********************************************
int ListInsert(struct LIST *list, void * item){
    if(list == NULL || item == NULL){
        return -1;
    }
    if(isEmptyNodeExists() == 0){
        struct Node *temp = getEmptyNode();
        temp->data = item;
        attachNode(list,temp,2,0);
        return 0;
    }else{
        return -1;
    }
    
}

struct Action{
    char * msg;
    int action;
};
//append to list
//*********************************************
int ListAppend(struct LIST *list, void * item){
    if(list == NULL || item == NULL){
        return -1;
    }
    if(isEmptyNodeExists() == 0){
        ListPrev(list);
        struct Node *temp = getEmptyNode();
        temp->data = item;
        attachNode(list,temp,1,1);
        return 0;
    }else{
        return -1;
    }
}

//prepend to list
//*********************************************
int ListPrepend(struct LIST *list, void *item){
    if(list == NULL || item == NULL){
        return -1;
    }
    if(isEmptyNodeExists() == 0){
        struct Node *temp = getEmptyNode();
        temp->data = item;
        attachNode(list,temp,0,0);
        return 0;
    }else{
        return -1;
    }
}

//remove from list
//*********************************************
void *ListRemove(struct LIST *list){
    if(list == NULL || list->size == 0){
        return NULL;
    }
    void *temp = list->current->data;
    struct Node *currNode = list->current;
    if(list->current == list->tail){ // if current node at tail
        ListPrev(list);
        list->tail = list->current;
        list->current->next = NULL;
    }else if (list->current == list->head){ // if current node at head
        list->head = list->head->next;
        list->current = list->head;
    }else if(list->current == &headNode || list->current == &tailNode){
        return NULL; // if beyond list
    }else{
        ListPrev(list); //else....
        list->current->next = list->current->next->next;
        ListNext(list);
    }
    list->size--;
    returnNode(currNode);
    
    struct Action * action = (struct Action*)temp;
    return temp;
}

//free item in node
//*********************************************
void freeNode(void * node){
    if(node == NULL){
        return;
    }
    struct Node *temp = node;
    temp->data = NULL;
}

//concatenate 2 lists
//*********************************************
void ListConcat(struct LIST *list1, struct LIST *list2){
    if(list1 == NULL || list2 == NULL){
        return;
    }
    if(list2->size != 0){ // point tail node to head node of list 2
        list1->tail->next = list2->head;
        list1->tail = list2->tail; 
    }
    ListFree(list2, &freeNode); // free list2
}

//free list
//*********************************************
void ListFree(struct LIST *list, void (*itemFree)(void * itemToBeFreed)){
    if (list == NULL){
        return;
    }
    list->current = list->head;
    while(list->current != NULL && list->size != 0){ // free each node
        struct Node *temp = list->current;
        list->current = list->current->next;
        itemFree(temp);
        returnNode(temp);
    }
    clearList(list);
    if(listTracker.size == 0){ // return to free head list
        listTracker.tail->data = list;
        listTracker.current = listTracker.tail;
    }else{
        if(listTracker.current == listTracker.head){
            printf("Error return list to full list\n");
            return;
        }
        ListPrev(&listTracker);
        countTotalHeads(&listTracker);
        listTracker.current->data = list;
    }
    listTracker.size++;  
    list = NULL;
    countTotalHeads(&listTracker);
}

//trim from list
//*********************************************
void *ListTrim(struct LIST *list){
    if(list == NULL){
        return NULL;
    }
    ListLast(list);
    return ListRemove(list);
}

//search list
//*********************************************
void *ListSearch(struct LIST *list, int(*comparator)(void*, void*), void * comparisonArg){
    if(list->size == 0 || list == NULL){
        return NULL;
    }
    list->current = list->head;
    while(list->current != NULL){ //use function pointer to compare
        if(comparator(list->current->data, comparisonArg) == 1){
            return list->current->data;
        }else{
            list->current = list->current->next;
        }
    }
    list->current = &tailNode;
    return NULL;
}

