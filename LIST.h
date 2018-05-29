/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LIST.h
 * Author: Ip
 *
 * Created on January 28, 2018, 3:38 PM
 */

#ifndef LIST_H
#define LIST_H

struct Node{
    void* data;
    struct Node* next;
};

struct LIST{
    struct Node *head;
    struct Node *tail;
    struct Node *current;
    int size;
};

struct LIST *ListCreate();

int ListCount(struct LIST *list);

void *ListFirst(struct LIST *list);

void *ListLast(struct LIST *list);

void *ListNext(struct LIST *list);

void *ListPrev(struct LIST *list);

void *ListCurr(struct LIST *list);

int ListAdd(struct LIST *list, void * item);

int ListInsert(struct LIST *list, void * item);

int ListAppend(struct LIST *list, void * item);

int ListPrepend(struct LIST *list, void *item);

void *ListRemove(struct LIST *list);

void ListConcat(struct LIST *list1,struct LIST *list2);

void ListFree(struct LIST *list, void (*itemFree)(void *itemToBeFreed));

void returnList(struct LIST *itemToBeFreed);

void *ListTrim(struct LIST *list);

void *ListSearch(struct LIST *list, int(*comparator)(void*, void*), void * comparisonArg);


#endif /* LIST_H */

