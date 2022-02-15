/*
    Author: Damon(Bao) Vu
    NSID: bav965
    Student #: 11261393

    Partner: Eyan Cunningham
    NSID: esc568
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#define SENDER_PORT 33333
#define RECEIVER_PORT 34444
#define MAXDATASIZE 1024
#define BACKLOG 10
#define METADATASIZE 64
#define MAX_RECEIVER_CLIENTS 100
#define MAX_MESSAGES 100

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER; // message queue mutex
pthread_cond_t new_message_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t receiver_client_mutex = PTHREAD_MUTEX_INITIALIZER; // receiver clients queue mutex

typedef struct {
    int sockfd;
    char *addr;
    unsigned short port;
} connection_t;

typedef struct {
    char *data;
} message_t;

message_t *messages[MAX_MESSAGES];

volatile int num_messages;
volatile int message_index;
volatile int num_receivers;
volatile int *receiver_clients[MAX_RECEIVER_CLIENTS];

/*
    Add a new client to the client queue
    Params:
    - int *sockfd: the sockfd of the receiver client
    Return:
    - None
*/
void add_client(int *sockfd) {
    if (pthread_mutex_lock(&receiver_client_mutex) != 0) {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    for (int i = 0; i < MAX_RECEIVER_CLIENTS; i++) {
        if (!receiver_clients[i]) {
            receiver_clients[i] = sockfd;
            break;
        }
    }

    if (pthread_mutex_unlock(&receiver_client_mutex) != 0) {
        perror("pthread_mutex_unlock");
        pthread_exit(NULL);
    }
}

/*
    Add a message to the message queue.
    Params: 
    - message_t *msg: pointer to the message_t struct
    Return:
    - None
*/
void add_message(message_t *msg) {
    // Get the queue mutex
    if(pthread_mutex_lock(&queue_mutex) != 0) {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    // Add the message to the queue
    messages[message_index] = msg;
    num_messages++;
    message_index++;

    // Wake up all threads that are waiting for a new message to arrive
    if (pthread_cond_broadcast(&new_message_cond) != 0) {
        perror("pthread_cond_broadcast");
        pthread_exit(NULL);
    }

    // Release the queue mutex
    if(pthread_mutex_unlock(&queue_mutex) != 0){
        perror("pthread_mutex_unlock");
        pthread_exit(NULL);
    }
}

/*
    Get sockaddr, IPv4 or IPv6
    Copied from Beej's Guide Book
*/
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/*
    This is the sender client routine that will be called when 
    a sender client thread is created
    Params:
    - (void *)arg: This should be a connection_t pointer
*/
void *sender_client_routine(void *arg) {


    int  numbytes;
    char buf[MAXDATASIZE];
    char new_message[MAXDATASIZE + METADATASIZE];

    connection_t con = *(connection_t *)arg;
    
    printf("[SERVER] - Sender Client IP: %s\n", con.addr);
    printf("[SERVER] - Sender Client Port: %d\n", con.port);

    while(1) {

        if (numbytes = recv(con.sockfd, buf, MAXDATASIZE, 0) == -1) {
            perror("server: recv - sender_client_routine");
            close(con.sockfd);
            pthread_exit(NULL);
        }

        printf("\n[SERVER] - Message received: '%s'\n", buf);

        if (num_receivers != 0) {

            printf("[SERVER] - Preparing message to send to the receiver clients\n");
            printf("[SERVER] - Number of receivers is: %d\n", num_receivers);

            sprintf(new_message, "%s, %d: %s", con.addr, con.port, buf);
            printf("[SERVER] - The new message is: '%s'\n", new_message);

            message_t *msg = (message_t *)malloc(sizeof(message_t));
            msg->data = new_message;
            add_message(msg);

        } else { // no receivers
            printf("[SERVER] - No Receiver Client - Message was discarded\n");
        }
        
    }

    pthread_exit(NULL);

    return NULL;

}

/*
    This is the receiver client routine that will be called when 
    a receiver client thread is created
    Params:
    - (void *)arg: This should be an integer represents for sockfd
*/
void *receiver_client_routine(void *arg) {

    char buf[MAXDATASIZE + METADATASIZE];

    int sockfd = *(int *)arg;
    int *new_receiver_client = (int *)malloc(sizeof(int));
    new_receiver_client = &sockfd;
    add_client(new_receiver_client);

    // Get the queue_mutex
    if (pthread_mutex_lock(&queue_mutex) != 0) {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    num_receivers++;

    while(1) {

        while(num_messages == 0) {
            
            // printf("\n[SERVER] - No Message! Putting Receiver Client to Sleep\n"); // For Testing
            
            if (pthread_cond_wait(&new_message_cond, &queue_mutex) != 0) {
                perror("pthread_cond_wait");
                pthread_exit(NULL);
            }
        }
        
        sprintf(buf, "%s", messages[message_index-1]->data);

        num_messages--; 

        if (num_receivers != 0) {
            
            pthread_mutex_lock(&receiver_client_mutex);
            for (int i = 0; i < MAX_RECEIVER_CLIENTS; i++) {
                if (receiver_clients[i]) {
                    if (send(*receiver_clients[i], buf, sizeof buf, 0) == -1) {
                        perror("send");
                        num_receivers--;
                        pthread_exit(NULL);
                    }
                } else {
                    break;
                }
            }
            pthread_mutex_unlock(&receiver_client_mutex);

        }
        
        if (pthread_mutex_unlock(&queue_mutex) != 0) {
            perror("pthread_mutex_unlock");
            pthread_exit(NULL);
        }

    }
    

    pthread_exit(NULL);
    
    return NULL;

}

/*
    This is the server routine that will be called when 
    the server thread is created
    Params:
    - (void *)arg: This should be the port number of the clients. Depending
    the client port number, another routine will be called with a new 
    thread created.
    Return:
    - None
*/
void *server_routine(void *arg) {
    // printf("server_routine\n"); // for testing

    // Need to do casting since getaddrinto require char *
    char port[8];
    sprintf(port, "%ld", (long) arg);

    int sockfd, numbytes, new_fd;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    int rv;
    char s[INET6_ADDRSTRLEN];
    socklen_t addr_size;
    int opt = 1;

    connection_t con;
    pthread_t th; // connection thread

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // getaddrinfo
    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        pthread_exit(NULL); // terminate the calling thread
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        
        // socket
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))== -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), &opt, sizeof(opt)) == -1) {
            perror("server: setsockopt");
            pthread_exit(NULL);
        }

        // Bind
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;

    }

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind socket\n");
        pthread_exit(NULL);
    }

    freeaddrinfo(servinfo);

    // Listen
    if (listen(sockfd, BACKLOG) == -1) {
        perror("server: listen");
        pthread_exit(NULL);
    }

    //accept
    while(1) {
        addr_size = sizeof their_addr;
        // printf("sockfd before: %d\n", sockfd); // for testing
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
        if (new_fd == -1) {
            perror("server: accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);

        //switch cases
        // sender_client
        if ((long) arg == SENDER_PORT) {
            printf("\n[SERVER] - Got connection from SENDER_CLIENT at %s\n", s);
            
            con.sockfd = new_fd;
            con.addr = s;
            con.port = ntohs(((struct sockaddr_in *)&their_addr)->sin_port);

            if (pthread_create(&th, NULL, (void *) sender_client_routine, (void *)&con) != 0) {
                perror("server: pthread_create sender");
            }

            // as specified in the description, each thread created 
            // as a detached thread
            pthread_detach(th); 

        } 

        // receiver_client
        else { // port = RECEIVER_PORT 

            printf("\n[SERVER] - Got connection from RECEIVER_CLIENT at %s\n", s);

            if (pthread_create(&th, NULL, (void *) receiver_client_routine, (void *)&new_fd) != 0) {
                perror("server: pthread_create receiver");
            }

            // as specified in the description, each thread created 
            // as a detached thread
            pthread_detach(th); 

        }

    }

    pthread_exit(NULL);

}

/*
    The main program
*/
int main(int argc, char const *argv[])
{
    printf("\nUse Port %d for Sender Clients\n", SENDER_PORT);
    printf("Use Port %d for Receiver Clients\n", RECEIVER_PORT);

    pthread_t sender_server_thread;
    pthread_t receiver_server_thread;

    signal(SIGPIPE, SIG_IGN);

    // create the two threads that accept connections
    if (pthread_create(&sender_server_thread, NULL, (void *)server_routine, (void *)SENDER_PORT) != 0) {
        fprintf(stderr, "Error: pthread_create\n");
    }

    if (pthread_create(&receiver_server_thread, NULL, (void *)server_routine, (void *)RECEIVER_PORT) != 0) {
        fprintf(stderr, "Error: pthread_create\n");
    }

    // join the two threads 
    if (pthread_join(sender_server_thread, NULL) != 0) {
        fprintf(stderr, "Error: pthread_join\n");
    }

    if (pthread_join(receiver_server_thread, NULL) != 0) {
        fprintf(stderr, "Error: pthread_join\n");
    }

    num_messages = 0;
    message_index = 0;
    num_receivers = 0;
    
    return EXIT_SUCCESS;
}
