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
#include <signal.h>

#define MAXBUFLEN 1024


/*
    Trim off the enter character '\n' at the end of the message
*/
void str_trim(char *str, int length) {
    for (int i = 0; i < length; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
}

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

int main(int argc, char const *argv[])
{

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    char *buf = (char *) malloc(MAXBUFLEN * sizeof(char));

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("sender_client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("sender_client: connect");
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "sender_client: failed to create socket\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("[SENDER_CLIENT] Connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    size_t message_size = MAXBUFLEN - 1; // getline required size_t argument
    while(1) {

        printf("> ");
        if ((numbytes = getline(&buf, &message_size, stdin)) == -1) {
            fprintf(stderr, "getline: failed");
            exit(1);
        }

        if (strcmp(buf, "\n") != 0) {
            str_trim(buf, MAXBUFLEN); // trim \n before sending
            printf("[SENDER_CLIENT] - Message being sent: '%s'\n", buf);
            if (send(sockfd, buf, numbytes, 0) == -1) {
                perror("sender");
                exit(1);
            }
        }

    }

    close(sockfd);

    return 0;
}
