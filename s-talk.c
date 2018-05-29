#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "udpSocket.h"
#include <pthread.h>
#include "queue.h"
#include <errno.h>
#define BUFSIZE 1024
#define MAX_STRING_LENGTH 100
#define MAX_ACTIONS 20

int myPortNum;
char *machineName;
int remotePortNum;
char * user = "You";
pthread_t thread[4];
pthread_t input_t,send_t,output_t,rcv_t;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t send_c, output_c, nodes_c;
struct LIST * actions;

//struct for keeping info about the msg and type of action
struct Action{
    char * msg;
    int action;
}actionArr[MAX_ACTIONS];

//this monitor function helps with signaling wait conditions
void monitor(){
    if(isEmpty() == 0){
        struct Action * action = peek();
        if (action->action == 0){ // for sending
            pthread_cond_signal(&send_c);
        }else if (action->action == 1){
            pthread_cond_signal(&output_c); // for printing
        }
    }
}

//time stamp for printing
char * timestamp()
{
    time_t ltime; // calendar time
    ltime=time(NULL); // get current time
    char* time = asctime( localtime(&ltime));
    time[strlen(time)-1] = 0; // remove newline
    return time;
}

//print msg with time stamp
void print(char * name, char * msg){
    printf("[%s] %s: %s",timestamp(), name, msg);
}

//function for input thread
void * input(){
    while(1){
        char input[BUFSIZE];
        fgets(input, BUFSIZE, stdin);
        print(user, input); // get user input and print
        //for exiting program
        if(strcmp("!\n", input) == 0){ 
            printf("Exiting......\n");
            exit(0);
        }
        
        //critical section
        pthread_mutex_lock(&mutex); // lock mutex
        struct Action * action = (struct Action*)ListTrim(actions); // get action
        while(action == NULL){
            pthread_cond_wait(&nodes_c,&mutex);
            action = (struct Action*)ListTrim(actions);
        }
        action->action = 0; // set to send
        action->msg = input; // set msg
        enqueue(action); // add action to queue
        pthread_mutex_unlock(&mutex); // unlock mutex
        //crit section end
        monitor(); 
    }
}

// function for send thread
void * send(){
    while(1){
        //critical section
        pthread_mutex_lock(&mutex);//lock mutex
        pthread_cond_wait(&send_c,&mutex); // wait for signal
        struct Action * action = (struct Action*)dequeue(); //get action
        ListAppend(actions,action);//add action to empty action list
        if(ListCount(actions) == 1){
            pthread_cond_signal(&nodes_c);
        }
        char * msg = action->msg; // get msg
        pthread_mutex_unlock(&mutex);
        //crit section end
        monitor();
        sendMsg(msg); // send msg
    }
}

//function for output thread
void * output(){
    while(1){
        //critical section
        pthread_mutex_lock(&mutex);//lock mutex
        pthread_cond_wait(&output_c,&mutex);//wait for signal
        struct Action * action = (struct Action*)dequeue(); // get action
        ListAppend(actions, action); // add action to empty list
        if(ListCount(actions) == 1){
            pthread_cond_signal(&nodes_c);
        }
        char * msg = action->msg;
        pthread_mutex_unlock(&mutex);//unlock mutex
        //crit section end
        monitor();
        
        //print to screen msg
        print(machineName, msg);
    }
}

//function for receiving thread
void * rcv(){
    while(1){
        char string[BUFSIZE];
        char * msg = receiveMsg(); // receive msg
        strcpy(string, msg); // copy string to local for use
        
        //critical section
        pthread_mutex_lock(&mutex);//lock mutex
        struct Action * action = (struct Action*)ListTrim(actions); // get empty action
        while(action == NULL){
            pthread_cond_wait(&nodes_c,&mutex);
            action = (struct Action*)ListTrim(actions);
        }
        action->action = 1; // set action to output
        action->msg = string;
        //enqueue with action
        enqueue(action);
        pthread_mutex_unlock(&mutex);//unlock mutex
        //crit section end
        monitor();
    }
}

int main(int argc, char *argv[]){
	if(argc < 4){
		printf("Usage: s-talk [my port number] [remote machine name] [remote port number]\n");
		return 0;
	}
	myPortNum = atoi(argv[1]);
	machineName = argv[2];
	remotePortNum = atoi(argv[3]);
        if (remotePortNum <= 0|| myPortNum <= 0){
            printf("Port Numbers must be valid positive integers.\n");
            exit(0);
        }
            
	printf("My Port Number = %d\n",myPortNum);
	printf("Machine Name = %s\n", machineName);
	printf("Remote Port Name = %d\n", remotePortNum);
        //create the queue for threads
        createQueue();
        //create empty list of actions
        actions = ListCreate();
        //initialize empty actions
        for(int i = 0; i < MAX_ACTIONS; i++){
            ListAppend(actions,&actionArr[i]);
        }
        
        // create socket
	createSocket(myPortNum, remotePortNum, machineName);
        
        //initialize conditions
        pthread_cond_init(&output_c,NULL);
        pthread_cond_init(&send_c,NULL);
        pthread_cond_init(&nodes_c,NULL);
        //create threads
        pthread_create(&input_t,NULL,input,NULL);
        pthread_create(&send_t,NULL,send,NULL);
        pthread_create(&output_t,NULL,output,NULL);
        pthread_create(&rcv_t,NULL,rcv,NULL);
        //join threads
        pthread_join(input_t, NULL);
        pthread_join(rcv_t, NULL);
        pthread_join(output_t, NULL);
        pthread_join(send_t,NULL);
        
}
