#ifndef _TRINUM_
#define _TRINUM_

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include "../IGen/igen_lib/igen_lib.h"
#include "../IGen/igen_lib/igen_dd_lib.h"
#include <stdbool.h>

typedef struct trinumber_real {
    f32_I interval;
    double center;
} tnum_real_t;

typedef struct trinumber_complex {
    tnum_real_t real;
    tnum_real_t imag;
} interval_t;

interval_t create_interval(int real);

interval_t interval_add(interval_t A, interval_t B);

interval_t interval_sub(interval_t A, interval_t B);

interval_t interval_mul(interval_t A, interval_t B);

void print_interval(interval_t A);

bool is_valid_interval(interval_t A);

tnum_real_t create_real(long A);

tnum_real_t real_mult(tnum_real_t A, tnum_real_t B);

tnum_real_t real_div(tnum_real_t A, tnum_real_t B);

interval_t complex_exp(tnum_real_t A);

#endif

