/*
    Author: Damon(Bao) Vu
    Student Number: 11261393
    NSID: bav965

    Partner: None
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "petgroomsynch.h"

/*
    A routine that will be called by pthread_create().
    It will call newpet(), then sleep for 2 seconds before calling petdone()
    right after
*/
void *groom(void *arg) {
    pet_t pet = (pet_t) arg; //casting the arg to pet_t

    // pet arrived 
    if (newpet(pet) != 0) {
        return (void *) 1;
    }
    // print out the message that indicate what type of pet arrives 
    switch(pet) {
        case cat: {
            fprintf(stdout, "Arrives - CAT\n");
            break;
        }
        case dog: {
            fprintf(stdout, "Arrives - DOG\n");
            break;
        }
        case other: {
            fprintf(stdout, "Arrives - OTHER\n");
            break;
        }
        default: {
            fprintf(stderr, "Error!\n");
            break;
        }
    }

    // grooming
    // Assuming it takes 2 minutes to groom a cat, 1 minute to groom any other type (dog & other)
    switch(pet) {
        case cat: {
            sleep(2);
            break;
        }
        case dog: {
            sleep(1);
            break;
        }
        case other: {
            sleep(1);
            break;
        }
        default: {
            fprintf(stderr, "Error!\n");
            break;
        }
    }

    // done
    if (petdone(pet) != 0) {
        return (void *) 1;
    }
    // print out which type of pet has just finished to be groomed
    switch(pet) {
        case cat: {
            fprintf(stdout, "Done Grooming a CAT\n");
            break;
        }
        case dog: {
            fprintf(stdout, "Done grooming a DOG\n");
            break;
        }
        case other: {
            fprintf(stdout, "Done grooming an OTHER\n");
            break;
        }
    }


    return (void *) 0;
}

int main(int argc, char const *argv[])
{
    int number_of_stations, number_of_cat, number_of_dog, number_of_other;
    pthread_t *pets;

    if (argc < 5) {
        fprintf(stderr, "Please provide 4 integers: number_of_stations number_of_cat number_of_dog number_of_other\n");
        return 1;
    }

    number_of_stations = atoi(argv[1]);
    number_of_cat = atoi(argv[2]);
    number_of_dog = atoi(argv[3]);
    number_of_other = atoi(argv[4]);

    // Check the petgroom_init()
    if (petgroom_init(number_of_stations) != 0) {
        fprintf(stderr, "Error! Please check petgroom_init()\n");
        return 1;
    }

    fprintf(stdout, "\n***Pet Grooming Facality has been initialized successfully***\n\n");

    // create pets
    int number_of_pets = number_of_cat + number_of_dog + number_of_other;
    pets = (pthread_t *) malloc(number_of_pets * sizeof(pthread_t));

    for (int i = 0; i < number_of_cat; i++) {
        int index = i;
        if (pthread_create( &pets[index], NULL, groom, (void *) cat) != 0) {
            free(pets);
            return 1;
        }
    }

    for (int j = 0; j < number_of_dog; j++) {
        int index = number_of_cat + j;
        if (pthread_create(&pets[index], NULL, groom, (void *) dog) != 0) {
            free(pets);
            return 1;
        }
    }

    for (int k = 0; k < number_of_other; k++) {
        int index = number_of_cat + number_of_dog + k;
        if (pthread_create(&pets[index], NULL, groom, (void *) other) != 0) {
            free(pets);
            return 1;
        }
    }

    // waiting for all threads to finish
    for (int l = 0; l < number_of_pets; l++) {
        if (pthread_join(pets[l], NULL) != 0) {
            fprintf(stderr, "Join Error\n");
            return 1;
        }
    }

    // clean up
    if (petgroom_done() != 0) {
        fprintf(stderr, "Error! Please check petgroom_done()\n");
    }

    fprintf(stdout, "\nClean up successfully\n");
    fprintf(stdout, "\n***End of test program***\n\n");

    // free the list
    free(pets);

    return 0;
}