#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
	// **********
    // Testing tput()
    char *messages_tput[6];
    for (int i = 0; i < 6; i++) {
        messages_tput[i] = (char *)malloc(140);
    }

    for (int i = 0; i < 6; i++) {
        topic_t tag;

        switch(i) {
            case 0: {
                tag = QUIZ;
                messages_tput[i] = "Quiz was easy!";
                break;
            }
            case 1: {
                tag = MIDTERM;
                messages_tput[i] = "Midterm was ok!";
                break;
            }
            case 2: {
                tag = FINAL;
                messages_tput[i] = "Final was hard!";
                break;
            }
            default: {
                tag = QUIZ;
                messages_tput[i] = "Other Quizzes!";
                break;
            }
        }

        printf("Userspace Message Address: %p\n", &messages_tput[i]); // testing
        tput(tag, messages_tput[i]);
        
    }

    int status;
    for (int i = 0; i < 6; i++) {
        wait(&status);
    }
    
    // **********
    // Testing tget()
    topic_t tag = FINAL;
    char buf;

    for (int i = 0; i < 2; i++) {
        if (fork() == 0) {
            if (tget(tag, &buf) == 0) {
                exit(0);
            } else {
                exit(1);
            }
        }
    }
    
    for (int i = 0; i < 2; i++) {
        wait(&status);
    }



    // **********
    // Testing btput
    // char *messages_btput[6];
    // for (int i = 0; i < 6; i++) {
    //     messages_btput[i] = (char *)malloc(140);
    // }

    // for (int i = 0; i < 6; i++) {
    //     topic_t tag;

    //     switch(i) {
    //         case 0: {
    //             tag = QUIZ;
    //             messages_btput[i] = "Quiz was easy!";
    //             break;
    //         }
    //         case 1: {
    //             tag = MIDTERM;
    //             messages_btput[i] = "Midterm was ok!";
    //             break;
    //         }
    //         case 2: {
    //             tag = FINAL;
    //             messages_btput[i] = "Final was hard!";
    //             break;
    //         }
    //         default: {
    //             tag = QUIZ;
    //             messages_btput[i] = "Other Quizzes!";
    //             break;
    //         }
    //     }

    //     printf("Userspace Message Address: %p\n", &messages_btput[i]); // testing
    //     btput(tag, messages_btput[i]);
        
    // }

    
    // topic_t tag;
    // char buf;
    // tag = MIDTERM;
    // if (fork() == 0) {
    //         if (btget(tag, &buf) == 0) {
    //             exit(0);
    //         } else {
    //             exit(1);
    //         }
    // }
    
	exit(0);
}
