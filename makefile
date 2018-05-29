s-talk: s-talk.o udpSocket.o LIST.o queue.o
	gcc -pthread -o s-talk s-talk.o udpSocket.o LIST.o queue.o
    
s-talk.o: s-talk.c udpSocket.c queue.c LIST.c
	gcc -c s-talk.c
    
udpSocket.o: udpSocket.c udpSocket.h
	gcc -c udpSocket.c
    
queue.o: queue.c queue.h LIST.c
	gcc -c queue.c
    
LIST.o: LIST.c LIST.h
	gcc -c LIST.c
    
clean:
	rm s-talk.o udpSocket.o s-talk LIST.o queue.o
