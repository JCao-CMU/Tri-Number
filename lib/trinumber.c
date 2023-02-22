#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include "igen_lib.h"

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
    dd_t real_center = _ia_cast_int_to_dd(real);
    dd_t img_center = _ia_cast_int_to_dd(0);
    itv.real.interval = real_itv;
    itv.imag.interval = imag_itv;
    itv.real.center = real_center;
    itv.imag.center = imag_center;
    return itv;
}

interval_t complex_add(interval_t A, interval_t B) {
    interval_t result;
    result.real.interval = _ia_add_f64(A.real.interval, B.real.interval);
    result.imag.interval = _ia_add_f64(A.imag.interval, B.imag.interval);
    result.real.center = _ia_add_dd(A.real.center, B.real.center);
    result.imag.center = _ia_add_dd(A.imag.center, B.imag.center);
    return result
}

interval_t complex_sub(interval_t A, interval_t B) {
    interval_t result;
    result.real.interval = _ia_sub_f64(A.real.interval, B.real.interval);
    result.imag.interval = _ia_sub_f64(A.imag.interval, B.imag.interval);
    result.real.center = _ia_sub_dd(A.real.center, B.real.center);
    result.imag.center = _ia_sub_dd(A.imag.center, B.imag.center);
    return result
}

interval_t complex_mul(interval_t A, interval_t B) {
//   FPIntervalMult64 aRbR_mult (.clock, .reset, .a(aReal), .b(bReal), .out(aRbR), .invalid(invalid_bit[0]));
//   FPIntervalMult64 aCbC_mult (.clock, .reset, .a(aComplex), .b(bComplex), .out(aCbC), .invalid(invalid_bit[1]));
//   FPIntervalMult64 aRbC_mult (.clock, .reset, .a(aReal), .b(bComplex), .out(aRbC), .invalid(invalid_bit[2]));
//   FPIntervalMult64 aCbR_mult (.clock, .reset, .a(aComplex), .b(bReal), .out(aCbR), .invalid(invalid_bit[3]));

//   logic [1:0][63:0] aCbCInv;
//   FPIntervalInverter64 aCbC_inv (.in(aCbC), .out(aCbCInv));
//   FPIntervalAdd64 real_adder (.clock, .reset, .a(aRbR), .b(aCbCInv), .out(outReal));
//   FPIntervalAdd64 complex_adder (.clock, .reset, .a(aRbC), .b(aCbR), .out(outComplex));

    interval_t result;

    f64_I aRbR, aCbC, aRbC, aCbR;
    aRbR = _ia_mul_f64(A.real.interval, B.real.interval);
    aCbC = _ia_mul_f64(A.imag.interval, B.imag.interval);
    aRbC = _ia_mul_f64(A.real.interval, B.imag.interval);
    aCbR = _ia_mul_f64(A.imag.interval, B.real.interval);

    result.real.interval = _ia_sub_f64(aRbR, aCbC);
    result.imag.interval = _ia_add_f64(aRbC, aCbR);

    dd_t aRbRd, aCbCd, aRbCd, aCbRd;
    aRbRd = _ia_mul_dd(A.real.center, B.real.center);
    aCbCd = _ia_mul_dd(A.imag.center, B.imag.center);
    aRbCd = _ia_mul_dd(A.real.center, B.imag.center);
    aCbRd = _ia_mul_dd(A.imag.center, B.real.center);

    result.real.center = _ia_sub_dd(aRbRd, aCbCd);
    result.imag.center = _ia_add_dd(aRbCd, aCbRd);
    return result
}

int main(){

    return 0;
}