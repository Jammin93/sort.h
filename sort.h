#ifndef SORT_H
#define SORT_H

#define VERSION "1.0.0"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_ERROR -1

#define CIURA_SIZE 9

const int const CIURA_SEQ[CIURA_SIZE] = {
	1, 4, 10, 23, 57, 132, 301, 701, 1750
};

typedef struct {
	int* elems;
	long long size;
	long long logsize;
} ARRAY;

static void
swap(void *vp1, void *vp2, long long elemsize) {
	/*	Swap the bit patterns of the two void pointers. The number of bytes to 
		swap is determined by `elemsize`. 
	*/
	unsigned char *p = vp1, *r = vp2, temp;
	for (long long i = 0; i < elemsize; ++i) {
		temp = p[i];
		p[i] = r[i];
		r[i] = temp;
	}
}

static void
transpose(void *arr, long long upper, long long elemsize) {
	// Reverse the order of the dynamic array elements.
	int left = 0, right = upper;
	int l, r;
	char *ap = (char*) arr;
	while (left < right) {
		l = left * elemsize;
		r = right * elemsize;
		swap(&ap[l], &ap[r], elemsize);
		++left;
		--right;
	}
}

static long long
array_double(void *arr, long long size, long long elemsize) {
	// Double the size of the dynamic array.
	size *= 2;
	arr = realloc(arr, size * elemsize);
	if (! arr) {
		return ALLOC_ERROR;
	}
	return size;
}

static long
floor_log2(long long n) {
	// Return the floor of log2(`n`).
	long res = (long) log(n) / log(2);
	return res;
}

void 
comb_sort(
		void *arr, 
		long long logsize, 
		long long elemsize, 
		int (*cmp_fn)(void*, void*),
		bool asc) {
	long long gap = logsize;
	long long l, r;
	void *left, *right;
	bool swapped = true;
	char *ap = (char*) arr;
	if (asc) {
		while (gap != 1 || swapped) {
			gap = gap / 1.3;
			gap = (1 > gap) ? 1 : gap;
			swapped = false;
			for (long long k = 0; k < logsize - gap; ++k) {
				l = k * elemsize;
				r = (k + gap) * elemsize;
				left = &ap[l];
				right = &ap[r];
				if (cmp_fn(left, right) > 0) {
					swap(left, right, elemsize);
					swapped = true;
				}	
			}
		}
	}
	else {
		while (gap != 1 || swapped) {
			gap = gap / 1.3;
			gap = (1 > gap) ? 1 : gap;
			swapped = false;
			for (long long k = 0; k < logsize - gap; ++k) {
				l = k * elemsize;
				r = (k + gap) * elemsize;
				left = &ap[l];
				right = &ap[r];
				if (cmp_fn(left, right) < 0) {
					swap(left, right, elemsize);
					swapped = true;
				}	
			}
		}
	}
}

void
insertion_sort(
		void *arr,
		long long logsize,
		long long elemsize,
		int (*cmp_fn)(void*, void*),
		bool asc) {
	void *temp = malloc(elemsize);
	long long k, idx;
	char *ap = (char*) arr;
	if (asc) {
		for (long long i = 1; i < logsize; ++i) {
			k = i;
			idx = i * elemsize;
			memcpy(temp, ap + idx, elemsize);
			while (k >= 1 && cmp_fn(ap + idx - elemsize, temp) > 0) {
				memcpy(ap + idx, ap + idx - elemsize, elemsize);
				--k;
				idx = k * elemsize;
			}
			memcpy(ap + idx, temp, elemsize);
		}
	}
	else {
		for (long long i = 1; i < logsize; ++i) {
			k = i;
			idx = i * elemsize;
			memcpy(temp, ap + idx, elemsize);
			while (k >= 1 && cmp_fn(ap + idx - elemsize, temp) < 0) {
				memcpy(ap + idx, ap + idx - elemsize, elemsize);
				--k;
				idx = k * elemsize;
			}
			memcpy(ap + idx, temp, elemsize);
		}
	}
	free(temp);
}

static ARRAY
shell_gap_sequence(int n) {
	// Generate the gap sequence for shell sort using Ciura255Odd.

	/*	Since we begin with a sequence of numbers whose values, not including 
		the first, are bigger than log2(`i`) and then multiply additional gaps 
		by 2.25, the size of the array will always be smaller than the floor of 
		log2(`n`). We add floor of log2(`n`) to `CIURA_SIZE` to get a 
		sufficiently sized array which does not need to be resized during 
		iteration.
	*/
	long long arr_size = CIURA_SIZE + floor_log2(n);
	short elemsize = sizeof(int);
	long long gap = 1;
	ARRAY seq;

	seq.elems = (int*) malloc(arr_size * sizeof(int));
	if (! seq.elems) {
		seq.size = ALLOC_ERROR;
		seq.logsize = ALLOC_ERROR;
		return seq;
	}
	seq.size = arr_size;
	seq.elems[0] = gap;
	seq.logsize = 1;
	long long i = 1;
	for (; i < CIURA_SIZE; ++i) {
		if (gap >= n) {
			return seq;
		}
		gap = CIURA_SEQ[i];
		seq.elems[i] = gap;
		++seq.logsize;
	}
	gap = (long long) gap * 2.25;
	gap = 1 | gap;
	while (gap < n) {
		seq.elems[i] = gap;
		gap = (int) gap * 2.25;
		gap = 1 | gap;
		++i;
		++seq.logsize;
	}
	return seq;
}

void
shell_sort(
		void *arr, 
		long long logsize, 
		long long elemsize,
		int (*cmp_fn)(void*, void*),
		bool asc) {
	ARRAY gaps = shell_gap_sequence(logsize);
	transpose(gaps.elems, gaps.logsize - 1, elemsize);
	void *temp = malloc(elemsize);
	char *ap = (char*) arr;
	long long gap, idx, k, g;
	if (asc) {
		for (int i = 0; i < gaps.logsize; ++i) {
			gap = gaps.elems[i];
			g = gap * elemsize;
			for (int n = gap; n < logsize; ++n) {
				k = n;
				idx = k * elemsize;
				memcpy(temp, ap + n * elemsize, elemsize);
				while (k >= gap && cmp_fn(ap + idx - g, temp) > 0) {
					memcpy(ap + idx, ap + idx - g, elemsize);
					k -= gap;
					idx = k * elemsize;
				}
				memcpy(ap + idx, temp, elemsize);
			}
		}
	}
	else {
		for (int i = 0; i < gaps.logsize; ++i) {
			gap = gaps.elems[i];
			g = gap * elemsize;
			for (int n = gap; n < logsize; ++n) {
				k = n;
				idx = k * elemsize;
				memcpy(temp, ap + n  * elemsize, elemsize);
				while (k >= gap && cmp_fn(ap + idx - g, temp) < 0) {
					memcpy(ap + idx, ap + idx - g, elemsize);
					k -= gap;
					idx = k * elemsize;
				}
				memcpy(ap + idx, temp, elemsize);
			}
		}
	}
}

#endif // SORT_H
