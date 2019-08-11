#ifndef NETWORK_H
#define NETWORK_H

#include <ijvm.h>
#include <stack.h>
#include <machine.h>

#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define OP_NETBIND        ((byte_t) 0xE1)        
#define OP_NETCONNECT     ((byte_t) 0xE2)    
#define OP_NETIN          ((byte_t) 0xE3)
#define OP_NETOUT         ((byte_t) 0xE4)
#define OP_NETCLOSE       ((byte_t) 0xE5)

int socket_file_descriptor;
int new_fd;

//act as the server, bind with a port and listen for incoming clients,
//accept if someone wants to connect
void netbind(struct stack *stack);

//you are the client, connect with an IP on a certain port number
void netconnect(struct stack *stack);

//pop netref from the stack, reads a character from the network
void netin(struct stack *stack);

//pops netref and message from the stack and send that over the network
void netout(struct stack *stack);

//close the connection
void netclose(void);

#endif
