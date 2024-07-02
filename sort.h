#ifndef SORT_H_
#define SORT_H_

#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
#define bool short
#define CIURA_COUNT 9

const int CIURA_SEQUENCE[CIURA_COUNT] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};

typedef struct {
	int* elems;
	long long size;
	long long end;
} INT_ARRAY;

int
swap(int* vp1, int* vp2) {
	int temp = *vp1;
	*vp1 = *vp2;
	*vp2 = temp;
}

void
transpose(int* arr, long long end) {
	int left = 0, right = end;
	while (left < right) {
		swap(&arr[left], &arr[right]);
		++left;
		--right;
	}
}

long long
double_size(void *vp, long long size, long long tsize) {
	/*	
	Double the size of the allocated space for the pointer and return the 
	new size.
	*/
	long long new_size = size * 2;
	char* temp = (char*) realloc(vp, new_size * tsize);
	if (temp != NULL) {
		vp = temp;	
	}
	else {
		fprintf(stderr, "Unable to allocate sufficient memory for array!\n");
		exit(1);
	}
	return new_size;
}

void
comb_sort(int* arr, long long size, bool reverse) {
	long long gap = size, r;
	int left, right;
	bool swapped = true;
	if (reverse == true) {
		while (gap != 1 || swapped == true) {
			gap = gap / 1.3;
			gap = (1 > gap) ? 1 : gap;
			swapped = false;			
			for (long long l = 0; l < size - gap; ++l) {
				r = l + gap;
				left = arr[l];
				right = arr[r];
				if (left < right) {
					swap(&arr[l], &arr[r]);
					swapped = true;				
				}
			} 		
		}	
	}
	else {
		while (gap != 1 || swapped == true) {
			gap = gap / 1.3;
			gap = (1 > gap) ? 1 : gap;
			swapped = false;			
			for (long long l = 0; l < size - gap; ++l) {
				r = l + gap;
				left = arr[l];
				right = arr[r];
				if (left > right) {
					swap(&arr[l], &arr[r]);
					swapped = true;				
				}
			} 		
		}	
	}
}

void
insertion_sort(int* arr, long long size, bool reverse) {
	int k, temp;
	if (reverse == true) {
		for (int i = 1; i < size; ++i) {
			temp = arr[i];
			k = i;
			while (k >= 1 && arr[k - 1] < temp) {
				arr[k] = arr[k - 1];
				--k;
			}
			arr[k] = temp;
		}
	}
	else {
		for (int i = 1; i < size; ++i) {
			temp = arr[i];
			k = i;
			while (k >= 1 && arr[k - 1] > temp) {
				arr[k] = arr[k - 1];
				--k;
			}
			arr[k] = temp;
		}
	}
}

INT_ARRAY
_shell_gap_sequence(int n) {
	int array_size = (CIURA_COUNT + 1) * 2;
	int gap = 1;
	INT_ARRAY seq;	
	
	seq.elems = (int*) malloc(array_size * sizeof(int));
	if (seq.elems == NULL) {
		fprintf(stderr, "Unable to allocate sufficient memory for array!\n");
		exit(1);
	}
	seq.size = array_size;
	seq.elems[0] = gap;
	seq.end = 0;
	int i = 1;
	for (; i < CIURA_COUNT; ++i) {
		if (gap >= n) { 
			return seq; 
		}
		gap = CIURA_SEQUENCE[i];
		seq.elems[i] = gap;
		++seq.end;
	}
	gap = (int) gap * 2.25;
	gap = 1 | gap;
	while (gap < n) {
		seq.elems[i] = gap;
		gap = (int) gap * 2.25;
		gap = 1 | gap;
		++i;
		++seq.end;	
	}
	return seq;
}

void
shell_sort(int* arr, long long size, bool reverse) {
	INT_ARRAY gaps = _shell_gap_sequence(size);
	transpose(gaps.elems, gaps.end);
	int temp, gap, k;
	if (reverse == true) {
		for (int i = 0; i <= gaps.end; ++i) {
			gap = gaps.elems[i];
			for (int n = gap; n < size; ++n) {
				temp = arr[n];
				k = n;
				while (k >= gap && arr[k - gap] < temp) {
					arr[k] = arr[k - gap];
					k -= gap;				
				}
				arr[k] = temp;				
			}		
		} 
	}
	else {
		for (int i = 0; i <= gaps.end; ++i) {
			gap = gaps.elems[i];
			for (int n = gap; n < size; ++n) {
				temp = arr[n];
				k = n;
				while (k >= gap && arr[k - gap] > temp) {
					arr[k] = arr[k - gap];
					k -= gap;				
				}
				arr[k] = temp;				
			}		
		} 
	}
}

#endif