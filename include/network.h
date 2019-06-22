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

void netbind(struct stack *stack);

void netconnect(struct stack *stack);

void netin(struct stack *stack);

void netout(struct stack *stack);

void netclose(void);

#endif