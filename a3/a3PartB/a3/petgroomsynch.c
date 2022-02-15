/*
    Author: Damon(Bao) Vu
    Student Number: 11261393
    NSID: bav965

    Partner: None
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "petgroomsynch.h"

// Variables
pthread_mutex_t mutex;
pthread_cond_t cond;

typedef enum {CAT, DOG, ANY} turn_t;

volatile turn_t turn;
volatile int running_stations;
volatile int available_stations;
volatile int ntimes;

volatile int dog_waiting;
volatile int dog_grooming;
volatile int num_dog_get_service;

volatile int cat_waiting;
volatile int cat_grooming;
volatile int num_cat_get_service;


int petgroom_init(int numstations) {
    available_stations = numstations;
    turn = ANY;
    dog_waiting = 0;
    dog_grooming = 0;
    cat_waiting = 0;
    cat_grooming = 0;
    ntimes = numstations;

    // Initialize mutex and cond
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        return -1;
    }

    if (pthread_cond_init(&cond, NULL) != 0) {
        return -1;
    }

    return 0;
}

int newpet(pet_t pet) {

    // Get the lock
    if (pthread_mutex_lock(&mutex) != 0) {
        return -1;
    }

    // Have the lock now
    // Do the logic
    if (pet == cat) {
        
        if (turn == ANY) {
            turn = CAT;
        }
        cat_waiting++;

        while( (turn != CAT) || (available_stations <= 0) || (dog_grooming > 0) ) {

            // wait
            if (pthread_cond_wait(&cond, &mutex) != 0) {
                return -1;
            }

        }

        // assign the cat to a grooming station
        cat_waiting--;
        available_stations--;
        cat_grooming++;
        num_cat_get_service++;


    } else if (pet == dog) {

        if (turn == ANY) {
            turn = DOG;
        }
        dog_waiting++;

        while ( (turn != DOG) || (available_stations <= 0) || (cat_grooming > 0) ) {

            // wait
            if (pthread_cond_wait(&cond, &mutex) != 0) {
                return -1;
            }

        }

        // assign the dog to a grooming station
        dog_waiting--;
        available_stations--;
        dog_grooming++;
        num_dog_get_service++;

    } else { // pet == other

        while(available_stations <= 0) {
            if (pthread_cond_wait(&cond, &mutex) != 0) {
                return -1;
            }
        }

        available_stations--;
    }


    // release the lock
    if (pthread_mutex_unlock(&mutex) != 0) {
        return -1;
    }
    return 0;
}

int petdone(pet_t pet) {

    // get the lock
    if (pthread_mutex_lock(&mutex) != 0) {
        return -1;
    }

    if (pet == cat) {

        cat_grooming--;
        available_stations++;

        if (num_cat_get_service >= ntimes) {
            if (dog_waiting > 0) {
                turn = DOG;
            }
            num_cat_get_service = 0; //reset the value
        }

        if (cat_waiting == 0) {
            if (dog_waiting > 0) {
                turn = DOG;
            } else if (dog_waiting == 0) {
                turn = ANY;
            }
        }

    } else if (pet == dog) {

        dog_grooming--;
        available_stations++;

        if (num_dog_get_service >= ntimes) {
            if (cat_waiting > 0) {
                turn = CAT;
            }
            num_dog_get_service = 0;
        }

        if (dog_waiting == 0) {
            if (cat_waiting > 0) {
                turn = CAT;
            } else if (cat_waiting == 0) {
                turn = ANY;
            }
        }
    
    } else { // other
        available_stations++;
    }

    // broadcast
    // wake all threads up to check their conditions
    if (pthread_cond_broadcast(&cond) != 0) {
        return -1;
    }

    // release the unlock 
    if (pthread_mutex_unlock(&mutex) != 0) {
        return -1;
    }
    return 0;
}

int petgroom_done() {

    // clean up
    if (pthread_mutex_destroy(&mutex) != 0) {
        return -1;
    }

    if (pthread_cond_destroy(&cond) != 0) {
        return -1;
    }

    return 0;
}
