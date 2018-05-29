#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

//printing errors and exit program
void error(char *msg) {
    printf("Error: %s\n", msg);
    exit(1);
}

//host variables
int client, myPort, myLen;
struct sockaddr_in peerAddr, myAddr;
struct hostent *hostp;
char buf[BUFSIZE];
char *hostaddrp;
int optval, n;

//peer variables
struct addrinfo peerHints;
struct addrinfo* peerRes;
char portString[10];
int peerLen;
// create the socket
void createSocket(int portNum1, int portNum2, char machineName[]) {
    //CLIENT*********************************************************
    myPort = portNum1;
    client = socket(AF_INET, SOCK_DGRAM, 0);
    if (client < 0)
        error("ERROR opening socket");
    
    //for optimization on next re run
    optval = 1;
    setsockopt(client, SOL_SOCKET, SO_REUSEADDR,
            (const void *) &optval, sizeof (int));
    
    memset((char*)&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddr.sin_port = htons((unsigned short) myPort);
    
    //bind
    if (bind(client, (struct sockaddr *) &myAddr,
            sizeof (myAddr)) < 0)
        error("Error on binding");
    
    
    //PEER***********************************************************
    memset(&peerHints,0,sizeof(peerHints));
    peerHints.ai_family= AF_UNSPEC;
    peerHints.ai_socktype=SOCK_DGRAM;
    peerHints.ai_protocol = 0;
    peerHints.ai_flags=AI_ADDRCONFIG;
    peerRes = 0;
    
    sprintf(portString, "%d", portNum2);
    int err = getaddrinfo(machineName,portString,&peerHints,&peerRes);
    if(err!=0){
        printf("Peer not found.\n");
        exit(0);
    }   
}

//send msg to peer
void sendMsg(char * msg) {
    memset(&buf,0,BUFSIZE);
    strcpy(buf, msg);
    n = sendto(client, buf, strlen(buf), 0, peerRes->ai_addr,peerRes->ai_addrlen);
    if (n < 0)
        error("ERROR in sendto");
}

//receive msg
char * receiveMsg() {
    memset(&buf,0,BUFSIZE);
    peerLen = sizeof(peerRes->ai_addrlen);
    n = recvfrom(client, buf, BUFSIZE, 0, peerRes->ai_addr,&peerLen);
    if (n < 0)
        error("ERROR in recvfrom");
    return buf;
}
