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
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXBUFLEN 1024

/*
    get sockaddr, IPv4, IPv6
    (copied from Beej's Guide Book)
*/
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char const *argv[]) {
    // NOTE: copied from section 6.2 & 6.3 of Beej's Guide Book
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect 
    for (p = servinfo; p != NULL; p = p->ai_next) {

        // Socket Settings
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            perror("receiver_client: socket");
            continue;
        }

        // connect
        int err = connect(sockfd, p->ai_addr, p->ai_addrlen);
        if (err == -1) {
            close(sockfd);
            perror("receiver_client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "receiver_client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("[RECEIVER_CLIENT] Connecting to %s\n", s);
    
    freeaddrinfo(servinfo);

    while (1) {
        if ((numbytes = recv(sockfd, buf, MAXBUFLEN - 1, 0)) == -1) {
            perror("receiver");
            exit(1);
        }

        // buf[numbytes] = '\0'; // add zero character to the end of the message string
        if (strcmp(buf, "") != 0) {
            printf("[RECEIVER_CLIENT] Message Received: \'%s\'\n", buf);
        }
        
    }

    close(sockfd);

    return 0;
    

}