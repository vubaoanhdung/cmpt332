Author: Damon(Bao) Vu
Student Number: 11261393
NSID: bav965

Partner: None

**********

README - Part B

**********

- To Run The Program:
	+ extract the files from the tar file
	+ run "make clean"
	+ run "make" - This will create a executable "test_program"
	+ run "./test_program <number_of_stations> <number_of_cat> <number_of_dog> <number_of_other>" with each argument as an integer
	
=> For example: ./test_program 3 4 5 6
	
- Note: Everything works as expected.
- Files:
	+ petgroomsynch.h: includes the function declarations and the 
	type definition typedef enum {cat, dog, other} pet_t;
	
	+ petgroomsynch.c: includes the actual implementations
	
	+ test_program.c: used to test the program. You will see some print statements below as you run the test program
		
		++ Arrives - <pet_t> : this means that a pet of type pet_t just arrives
		
		++ Done grooming a/an <pet_t>: this means that a pet of type pet_t has been done being groomed and was remove from the station. The station now empty and ready to take a new pet (with respective to constraints)


