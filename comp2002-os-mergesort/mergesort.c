/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). ALEX */
void merge(int leftstart, int leftend, int rightstart, int rightend){

	int i = leftstart, j = rightstart, k = leftstart;

	// Merge the two sorted halves into B
	while (i <= leftend && j <= rightend){
		if (A[i] <= A[j]) {
			B[k] = A[i];
			i++;
		} else {
			B[k] = A[j];
			j++;
		}
		k++;
	}

	// Copy any remaining elements from left half
	while (i <= leftend){
		B[k] = A[i];
		i++;
		k++;
	}

	// Copy any remaining elements from right half
	while(j <= rightend){
		B[k] = A[j];
		j++;
		k++;
	}

	// Copy sorted elements into A
	for (i = leftstart; i <= rightend; i++){
		A[i] = B[i];
	}
}

/* this function will be called by parallel_mergesort() as its base case. ALEX */
void my_mergesort(int left, int right){
	if (left < right){
		int mid = left + (right - left) / 2;

		// Sort first and second halves
		my_mergesort(left, mid);
		my_mergesort(mid + 1, right);

		// Merge the two sorted halves
		merge(left, mid, mid + 1, right);
	}
}

/* this function will be called by the testing program. MIHIR */
void * parallel_mergesort(void *arg){

    struct argument *args = (struct argument *)arg;

    if (args == NULL){
        return NULL;
    }

    int left = args->left;
    int right = args->right;
    int level = args->level;

    if (left < right){

        // Use serial merge sort when cutoff is reached
        if (level >= cutoff){
            my_mergesort(left, right);
        } else {
            int mid = left + (right - left) / 2;

            pthread_t t1, t2;
            int created1, created2;

            // Build arguments for the two halves
            struct argument *arg1 = buildArgs(left, mid, level + 1);
            struct argument *arg2 = buildArgs(mid + 1, right, level + 1);

            if (arg1 == NULL || arg2 == NULL){
                free(arg1);
                free(arg2);
                my_mergesort(left, right);
            } else {

                // Create threads for both halves
                created1 = (pthread_create(&t1, NULL,
                            parallel_mergesort, arg1) == 0);

                created2 = (pthread_create(&t2, NULL,
                            parallel_mergesort, arg2) == 0);

                // Sort serially if a thread cannot be created
                if (!created1){
                    free(arg1);
                    my_mergesort(left, mid);
                }

                if (!created2){
                    free(arg2);
                    my_mergesort(mid + 1, right);
                }

                // Wait for both threads to finish
                if (created1 && pthread_join(t1, NULL) != 0){
                    abort();
                }

                if (created2 && pthread_join(t2, NULL) != 0){
                    abort();
                }

                // Merge the sorted halves
                merge(left, mid, mid + 1, right);
            }
        }
    }

    // Free child arguments (main frees the original argument)
    if (level > 0){
        free(args);
    }

    return NULL;
}


/* we build the argument for the parallel_mergesort function. VISHU */
struct argument * buildArgs(int left, int right, int level){
		struct argument *newArg = malloc(sizeof *newArg);
		if (newArg == NULL) {
			return NULL;
		
		}

		newArg->left = left;
		newArg->right = right;
		newArg->level = level;

		return newArg;
}