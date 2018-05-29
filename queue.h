#include <stdio.h>
#include <stdlib.h>
#include "LIST.h"

#ifndef QUEUE_H
#define QUEUE_H

void createQueue();

int enqueue(void * item);

void* dequeue();

void * peek();

int isEmpty();

#endif /* QUEUE_H */

