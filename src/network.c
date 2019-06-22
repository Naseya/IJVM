#include <network.h>

void netbind(struct stack *stack) {
    int port = pop(stack);

    struct sockaddr_in server_address;

	socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1) {
        printf("Error: no socket created.");
        return;
    } else {
        printf("Succesfully created socket server: %d\n", socket_file_descriptor);
    }

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(socket_file_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        printf("Unable to bind the socket to the port\n");
        return;
    } else {
        printf("Succesfully binded the socket to the port.\n");
    }
	
	if(listen(socket_file_descriptor, SOMAXCONN) < 0) {
        push(stack, 0);
        printf("Unable to listen for incoming connections\n");
        return;
    } else {
        push(stack, socket_file_descriptor);        
        printf("Server is listening for incoming connections\n");
    }

    new_fd = accept(socket_file_descriptor, NULL, NULL);
    printf("The new fd is: %d\n", new_fd);
}

void netconnect(struct stack *stack) {
    
    uint32_t address = (uint32_t)swap_uint32(pop(stack));
    int port = pop(stack);
    
    struct sockaddr_in server_address;
    
    struct in_addr ipv4address;
    ipv4address = (struct in_addr) {
        address
    };

    server_address.sin_family = AF_INET; 
    
    server_address.sin_port = htons(port);
    
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    if(inet_pton(AF_INET, inet_ntoa(ipv4address), &server_address.sin_addr.s_addr) <= 0) {
        printf("Invalid address\n");
        push(stack, 0);
        return;
    }
    
    if (connect(socket_file_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { 
        printf("Connection Failed\n"); 
        push(stack, 0);
        return;
    } else {
        printf("Connection Succesful\n");
        push(stack, socket_file_descriptor);
        return;
    }
}

void netin(struct stack *stack) {
    char message_recv;
	word_t netref = pop(stack);

	recv(netref, &message_recv, sizeof(message_recv), 0);
	push(stack, (word_t)message_recv);
}

void netout(struct stack *stack) {
    char message_send = pop(stack);
    word_t netref = pop(stack);

	send(netref, &message_send, sizeof(message_send), 0);
}
 
void netclose() {
    close(socket_file_descriptor);
}
