/*
    Author: Damon(Bao) Vu
    Student Number: 11261393
    NSID: bav965

    Partner: None
*/

#ifndef PETGROOMSYNCH_H
#define PETGROOMSYNCH_H

/*
    Types of pet
*/
typedef enum {cat, dog, other} pet_t;

/*
    This function is called to initialize any required variables
    Params:
    - int numstations: an integer number represents for the number of grooming 
    station
    Return: 0 on success, -1 on failure
*/
int petgroom_init(int numstations);

/*
    This function is called when a new pet of type pet_t arrives at the 
    facility needing to be groomed
    Params:
    - pet_t pet: a pet of type pet_t that needs to be groomed
    Return: 0 on success, -1 on failure
*/
int newpet(pet_t pet);


/*
    This function is called when a pet of type pet_t that had been allocated a 
    station is done being groomed and the station is now free
    Params:
    - pet_t pet: a pet of type pet_t
    Return: 0 on success, -1 on failure

*/
int petdone(pet_t pet);

/*
    Perform any required data cleanup actions
    Params: None
    Return: 0 on success, -1 on failure
*/
int petgroom_done();


#endif