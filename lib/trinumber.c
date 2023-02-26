#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include "../IGen/igen_lib/igen_lib.h"
#include "../IGen/igen_lib/igen_dd_lib.h"
#include <stdbool.h>

typedef struct trinumber_real {
    f64_I interval;
    dd_t center;
} tnum_real_t;

typedef struct trinumber_complex {
    tnum_real_t real;
    tnum_real_t imag;
} interval_t;

interval_t create_interval(int real){
    interval_t itv;
    f64_I real_itv = _ia_cast_int_to_f64(real);
    f64_I imag_itv = _ia_cast_int_to_f64(0);
    dd_t real_center = veltkampSplit(real);
    dd_t img_center = veltkampSplit(0);
    itv.real.interval = real_itv;
    itv.imag.interval = imag_itv;
    itv.real.center = real_center;
    itv.imag.center = img_center;
    itv.real.interval.lo = -itv.real.interval.lo;
    itv.imag.interval.lo = -itv.imag.interval.lo;
    return itv;
}

interval_t interval_add(interval_t A, interval_t B) {
    interval_t result;
    result.real.interval = _ia_add_f64(A.real.interval, B.real.interval);
    result.imag.interval = _ia_add_f64(A.imag.interval, B.imag.interval);
    result.real.center = dd_add(A.real.center, B.real.center);
    result.imag.center = dd_add(A.imag.center, B.imag.center);
    
    return result;
}

interval_t interval_sub(interval_t A, interval_t B) {
    interval_t result;
    result.real.interval = _ia_sub_f64(A.real.interval, B.real.interval);
    result.imag.interval = _ia_sub_f64(A.imag.interval, B.imag.interval);
    result.real.center = dd_add(A.real.center, dd_neg(B.real.center));
    result.imag.center = dd_add(A.imag.center, dd_neg(B.imag.center));
    return result;
}

interval_t interval_mul(interval_t A, interval_t B) {
    interval_t result;

    f64_I aRbR, aCbC, aRbC, aCbR;
    aRbR = _ia_mul_f64(A.real.interval, B.real.interval);
    aCbC = _ia_mul_f64(A.imag.interval, B.imag.interval);
    aRbC = _ia_mul_f64(A.real.interval, B.imag.interval);
    aCbR = _ia_mul_f64(A.imag.interval, B.real.interval);

    result.real.interval = _ia_sub_f64(aRbR, aCbC);
    result.imag.interval = _ia_add_f64(aRbC, aCbR);

    dd_t aRbRd, aCbCd, aRbCd, aCbRd;
    aRbRd = dd_mul(A.real.center, B.real.center);
    aCbCd = dd_mul(A.imag.center, B.imag.center);
    aRbCd = dd_mul(A.real.center, B.imag.center);
    aCbRd = dd_mul(A.imag.center, B.real.center);

    result.real.center = dd_add(aRbRd, dd_neg(aCbCd));
    result.imag.center = dd_add(aRbCd, aCbRd);
    return result;
}

void print_interval(interval_t A){
    double ru = A.real.interval.up;
    double rl = A.real.interval.lo;
    double iu = A.imag.interval.up;
    double il = A.imag.interval.lo;
    double rc = A.real.center.h;
    double ic = A.imag.center.l;
    printf("({%f, %f, %f}, {%f, %f, %f})\n", rl, rc, ru, il, ic, iu);
}

bool is_valid_interval(interval_t A) {
    double ru = A.real.interval.up;
    double rl = A.real.interval.lo;
    double iu = A.imag.interval.up;
    double il = A.imag.interval.lo;
    dd_t rc = A.real.center;
    dd_t ic = A.imag.center;

    if ((ru < rl) || (iu < il))
        return false;

    dd_t ru_dd = dd_set(ru, 0.0);
    dd_t rl_dd = dd_set(rl, 0.0);
    dd_t iu_dd = dd_set(iu, 0.0);
    dd_t il_dd = dd_set(il, 0.0);

    // if c-l-u or l-u-c
    if (dd_cmpgt(rl_dd, rc) || dd_cmpgt(rc, ru_dd)) 
        return false;

    if (dd_cmpgt(il_dd, ic) || dd_cmpgt(ic, iu_dd)) 
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
        printf("yes!\n");
    }
    printf("%lx\n", twenty.imag.interval.up);
    printf("%lx\n", twenty.imag.interval.lo);
    if (twenty.imag.interval.up >= twenty.imag.interval.lo){
        printf("yes!\n");
    }
    return 0;
}