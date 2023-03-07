#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include "../IGen/igen_lib/igen_lib.h"
#include "../IGen/igen_lib/igen_dd_lib.h"
#include "../IGen/igen_lib/igen_math.h"
#include <stdbool.h>

typedef struct trinumber_real {
    f32_I interval;
    double center;
} tnum_real_t;

typedef struct trinumber_complex {
    tnum_real_t real;
    tnum_real_t imag;
} interval_t;

interval_t create_interval(int real){
    interval_t itv;
    f32_I real_itv = _ia_cast_int_to_f32(real);
    f32_I imag_itv = _ia_cast_int_to_f32(0);
    double real_center = (double)real;
    double img_center = (double)0;
    itv.real.interval = real_itv;
    itv.imag.interval = imag_itv;
    itv.real.center = real_center;
    itv.imag.center = img_center;
    itv.real.interval.lo = -itv.real.interval.lo;
    itv.imag.interval.lo = -itv.imag.interval.lo;
    itv.real.interval.up = itv.real.interval.up;
    itv.imag.interval.up = itv.imag.interval.up;
    return itv;
}

interval_t interval_add(interval_t A, interval_t B) {
    interval_t result;
    result.real.interval = _ia_add_f32(A.real.interval, B.real.interval);
    result.imag.interval = _ia_add_f32(A.imag.interval, B.imag.interval);
    result.real.center = A.real.center + B.real.center;
    result.imag.center = A.imag.center + B.imag.center;
    
    return result;
}

interval_t interval_sub(interval_t A, interval_t B) {
    interval_t result;
    result.real.interval = _ia_sub_f32(A.real.interval, B.real.interval);
    result.imag.interval = _ia_sub_f32(A.imag.interval, B.imag.interval);
    result.real.center = A.real.center - B.real.center;
    result.imag.center = A.imag.center - B.imag.center;
    return result;
}

interval_t interval_mul(interval_t A, interval_t B) {
    interval_t result;

    f32_I aRbR, aCbC, aRbC, aCbR;
    A.real.interval.lo = -A.real.interval.lo;
    A.imag.interval.lo = -A.imag.interval.lo;
    B.real.interval.lo = -B.real.interval.lo;
    B.imag.interval.lo = -B.imag.interval.lo;
    aRbR = _ia_mul_f32(A.real.interval, B.real.interval);
    aCbC = _ia_mul_f32(A.imag.interval, B.imag.interval);
    aRbC = _ia_mul_f32(A.real.interval, B.imag.interval);
    aCbR = _ia_mul_f32(A.imag.interval, B.real.interval);

    result.real.interval = _ia_sub_f32(aRbR, aCbC);
    result.imag.interval = _ia_add_f32(aRbC, aCbR);
    result.real.interval.lo = -result.real.interval.lo;
    result.imag.interval.lo = -result.imag.interval.lo;
    
    double aRbRd, aCbCd, aRbCd, aCbRd;
    aRbRd = (A.real.center * B.real.center);
    aCbCd = (A.imag.center * B.imag.center);
    aRbCd = (A.real.center * B.imag.center);
    aCbRd = (A.imag.center * B.real.center);

    result.real.center =  aRbRd - aCbCd;
    result.imag.center = aRbCd + aCbRd;
    return result;
}

tnum_real_t create_real(long A){
    tnum_real_t result;
    result.interval = _ia_cast_int_to_f32(A);
    result.interval.lo = -result.interval.lo;
    result.center = (double)A;
    return result;
}

tnum_real_t real_mult(tnum_real_t A, tnum_real_t B){
    A.interval.lo = -A.interval.lo;
    B.interval.lo = -B.interval.lo;
    f32_I result;
    result = _ia_mul_f32(A.interval, B.interval);
    double result_c = A.center * B.center;
    result.lo = -result.lo;
    tnum_real_t final_result;
    final_result.center = result_c;
    final_result.interval = result;
    return final_result;
}

tnum_real_t real_div(tnum_real_t A, tnum_real_t B){
    A.interval.lo = -A.interval.lo;
    B.interval.lo = -B.interval.lo;
    f32_I result;
    result = _ia_div_f32(A.interval, B.interval);
    double result_c = A.center / B.center;
    result.lo = -result.lo;
    tnum_real_t final_result;
    final_result.center = result_c;
    final_result.interval = result;
    return final_result;
}

interval_t complex_exp(tnum_real_t A){
    A.interval.lo = -A.interval.lo;
    tnum_real_t cosine;
    cosine.interval = _ia_cos_f32(A.interval);
    cosine.interval.lo = -cosine.interval.lo;
    cosine.center = cos(A.center);

    tnum_real_t sine;
    sine.interval = _ia_sin_f32(A.interval);
    sine.interval.lo = -sine.interval.lo;
    sine.interval = _ia_neg_f32(sine.interval);
    sine.center = -sin(A.center);

    interval_t result;
    result.real = cosine;
    result.imag = sine;
    return result;
    // we return cos(A) - jsin(A)

}

void print_interval(interval_t A){
    float ru = A.real.interval.up;
    float rl = A.real.interval.lo;
    float iu = A.imag.interval.up;
    float il = A.imag.interval.lo;
    double rc = A.real.center;
    double ic = A.imag.center;
    printf("({%f, %f, %f}, {%f, %f, %f})\n", rl, rc, ru, il, ic, iu);
}

bool is_valid_interval(interval_t A) {
    float ru = A.real.interval.up;
    float rl = A.real.interval.lo;
    float iu = A.imag.interval.up;
    float il = A.imag.interval.lo;
    double rc = A.real.center;
    double ic = A.imag.center;

    if ((ru < rl) || (iu < il))
        return false;

    double ru_dd = (double)ru;
    double rl_dd = (double)rl;
    double iu_dd = (double)iu;
    double il_dd = (double)il;

    // if c-l-u or l-u-c
    if ((rl_dd > rc) || (rc > ru_dd)) 
        return false;

    if ((il_dd > ic) || (ic > iu_dd)) 
        return false;
    if (ceil(rl_dd) != floor(ru_dd) || ceil(il_dd) != floor(iu_dd))
        return false;
    return true;

}

int main(){
    interval_t four = create_interval(4);
    interval_t five = create_interval(5);
    interval_t nine = interval_add(four, five);
    interval_t twenty = interval_mul(four, five);
    print_interval(four);
    print_interval(five);
    print_interval(nine);
    print_interval(twenty);
    if (is_valid_interval(twenty)){
        printf("Valid Interval!\n");
    }
    // printf("%lx\n", twenty.imag.interval.up);
    // printf("%lx\n", twenty.imag.interval.lo);
    // if (twenty.imag.interval.up >= twenty.imag.interval.lo){
    //     printf("yes!\n");
    // }
    return 0;
}