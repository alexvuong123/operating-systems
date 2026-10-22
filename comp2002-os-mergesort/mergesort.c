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
		return NULL;
}

/* we build the argument for the parallel_mergesort function. VISHU */
struct argument * buildArgs(int left, int right, int level){
		return NULL;
}

