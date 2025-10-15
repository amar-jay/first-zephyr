#ifndef MATHS_H
#define MATHS_H

typedef enum {
		SUCCESS = 0,
		FAILURE = -1,
		DIVIDE_BY_ZERO = -2
} Result_t;

// Function prototypes
Result_t add(int a, int b, int *result);
Result_t subtract(int a, int b, int *result);
Result_t multiply(int a, int b, int *result);
Result_t divide(int a, int b, int *result);

#endif