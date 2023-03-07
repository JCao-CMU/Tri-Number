// main file where we run our pipeline and conduct various experiments
#include <stdio.h>
// #include "igen_lib.h"
#include "FFT-Radhard-Sample/libs/fft.h"
#include "complex.h"
#include <time.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {

    // set the rounding mode to make the intervals work
    fesetround(FE_UPWARD);
    // set the random seed
    srand(time(NULL));
    // FILE *timings = fopen("timeperfs.txt", "w");
    // if (timings == NULL) {
    //     printf("Error opening file!\n");
    //     exit(1);
    // }
    long sizeloop = 1 << 9;
    // looping over FFT size
    for (long N = sizeloop; N < sizeloop * 2; N *= 2) {
        // call init functions
        //initializes the twiddle array
        f64_I *twiddle = malloc(sizeof(interval_t) * N * 4);
        //dd_I *twiddle2 = malloc(sizeof(dd_I) * N * 4);
        clock_t pl;
        pl = clock();
        initialize_twiddles(2 * N, twiddle);
        //fulldd_initialize_twiddles(2 * N, twiddle2);
        pl = clock() - pl;
        double time_takenn = ((double)pl)/CLOCKS_PER_SEC;
        //fprintf(timings, "\n----------D TWIDDLES time: %f ---------------", time_takenn);
        // looping over bit widths, iteration amount
        for (int b = 4; b <= 4; b++) {
            double total = 0;
            for (int i = 0; i < 10; i++) {
                // first number malloc
                f64_I *firstnum = malloc(sizeof(f64_I)*4*N);
                if (firstnum == NULL) {
                    printf("Memory allocation failed");
                    return 0;
                }
                // second number malloc
                f64_I *secnum = malloc(sizeof(f64_I)*4*N);
                if (secnum == NULL) {
                    printf("Memory allocation failed");
                    return 0;
                }
                // product malloc
                long *b4 = malloc(sizeof(long)*2*N);
                if (b4 == NULL) {
                    printf("Memory allocation failed");
                    return 0;
                }
                // initializes the string that will hold first factor
                char *firstfactor = malloc(sizeof(char) * N);
                if(firstfactor == NULL) {
                    printf("Memory allocation failed");
                    return 0;
                }
                // initializes the string that will hold second factor
                char *secondfactor = malloc(sizeof(char) * N);
                if (secondfactor == NULL) {
                    printf("Memory allocation failed");
                    return 0;
                }
                // initializes the string that will hold our output
                char *res = malloc(sizeof(char) * 2 * N);
                if (res == NULL) {
                    printf("Memory allocation failed");
                    return 0;
                }
                randinit(firstnum, b, N, firstfactor);
                randinit(secnum, b, N, secondfactor);
                //printf("\ninput[0] %f * %f", firstnum[0].up, secnum[0].up);

                double start;
                double end;
                start = omp_get_wtime();
                int_mult(b, N, firstnum, secnum, twiddle);
                end = omp_get_wtime();
                total += (end-start);
                // for (long j = 0; j < N; j++) {
                //     fprintf(timings, "output[%li] : (%0.6f, %0.6f) + (%0.6f, %0.6f)i\n", j, -firstnum[2*j].lo, firstnum[2*j].up, -firstnum[(2*j)+1].lo, firstnum[(2*j)+1].up);
                // }
                turn_it_back(b, 2*N, firstnum, b4);
                // perform_carry(b, b4, 2*N);
                // write_result(b4, res, 2*N, b);
                // for (int i = 0; i < N; i++) {
                //     printf("%c", firstfactor[i]);
                // }
                // printf("\n");
                // for (int i = 0; i < N; i++) {
                //     printf("%c", secondfactor[i]);
                // }
                // printf("\n");
                // for (int i = 0; i < 2*N; i++) {
                //     printf("%c", res[i]);
                // }
                // printf("\n");

                free(firstnum);
                free(secnum);
                free(res);
                free(secondfactor);
                free(firstfactor);
                free(b4);
                //printf("\nsize: %li, bitw: %i, average time : %0.7f", N, b, amount / 5);
                //fprintf(timings, "\nsize: %li, bitw: %i, average time : %0.7f", N, b, end - start);
            }
            printf("average time : %0.9f", total/10);
        }
        free(twiddle);
        //free(twiddle2);
    }
    //fclose(timings);

    //initializing the input that is being multiplied and writing to the file

    // check the malloc outputs, unit testing capabilities, for ffts and debug, malloclab ifndef

    long size = 4;
    int bti = 4;

    // initializes the interval signal
    dd_I *outA = malloc(sizeof(dd_I) * 4 * size);
    if (outA == NULL) {
        printf("Memory allocation failed");
        return 0;
    }
    // initializes the interval signal
    dd_I *outB = malloc(sizeof(dd_I) * 4 * size);
    if (outB == NULL) {
        printf("Memory allocation failed");
        return 0;
    }
    long *b4 = malloc(sizeof(long)*2*size);
    if (b4 == NULL) {
        printf("Memory allocation failed");
        return 0;
    }
    // initializes the string that will hold first factor
    char *firstnum = malloc(sizeof(char) * size);
    if(firstnum == NULL) {
        printf("Memory allocation failed");
        return 0;
    }
    // initializes the string that will hold second factor
    char *secondnum = malloc(sizeof(char) * size);
    if (secondnum == NULL) {
        printf("Memory allocation failed");
        return 0;
    }
    // initializes the string that will hold our output
    char *res = malloc(sizeof(char) * 2 * size);
    if (res == NULL) {
        printf("Memory allocation failed");
        return 0;
    }
    //initializes the twiddle array
    dd_I *twiddle = malloc(sizeof(dd_I) * size);
    fesetround(FE_UPWARD);

    fulldd_randinit(outA, bti, size, firstnum);
    fulldd_randinit(outB, bti, size, secondnum);

    clock_t pl;
    pl = clock();
    // perform the multiplication
    fulldd_initialize_twiddles(2 * size, twiddle);
    pl = clock() - pl;
    double time_takenn = ((double)pl)/CLOCKS_PER_SEC;
    printf("\nD TWIDDLES took %f seconds to execute \n", time_takenn);

    clock_t t;
    t = clock();
    fulldd_int_mult(bti, size, outA, outB, twiddle, b4);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("\nINT_MULT took %0.7f seconds to execute \nsize: %li\nbit: %i\n", time_taken, size, bti);
    //test_fft(size, test, A_in, outA);
    //TURN IT BACK AND CARRY FUNCTIONS
    perform_carry(bti, b4, 2*size);
    write_result(b4, res, 2*size, bti);

    printf("\nFIRSTNUM = %s\n", firstnum);
    printf("SECONDNUM = %s\n", secondnum);
    printf("result = %s\n", res);

    // char str[3] = "3A2";
    // char *ptr;
    // long ret;
    // ret = strtol(str+1, &ptr, 16);
    // printf("\nHEX: %x", ret);
    // printf("\nINT: %li", str[0]);
    // printf("rest %s", ptr);

    free(outA);
    free(outB);
    free(twiddle);
    //free(firstnum);
    //free(secondnum);
    free(res);
    double data11[4] = {1,2,3,4};
    double data22[4] = {1,2,3,4};
    double complex fft11[4];
    double complex fft22[4];
    int n = 4;
    realfft(data11, data22, fft11, fft22, n);

    long NN = 4;
    f64_I *twiddle = malloc(sizeof(f64_I) * NN * 2);
    initialize_twiddles(NN, twiddle);
    f64_I data1[NN*2];
    f64_I data2[NN*2];
    char firstnumy[NN];
    char secondnumy[NN];
    f64_I *fft11 = malloc(sizeof(f64_I) * 2 * NN);
    f64_I *fft22 = malloc(sizeof(f64_I) * 2 * NN);
    real_randinit(data1, 3, NN, firstnumy);
    real_randinit(data2, 3, NN, secondnumy);

    // f64_I inv_data1[4];
    // f64_I inv_data2[4];

    // f64_I inv_fft1[8] = {_ia_set_pointed_f64(18.0), _ia_set_pointed_f64(0.0),
    //                  _ia_set_pointed_f64(2.0), _ia_set_pointed_f64(2.0),
    //                  _ia_set_pointed_f64(6.0), _ia_set_pointed_f64(0.0),
    //                  _ia_set_pointed_f64(2.0), _ia_set_pointed_f64(-2.0)};
    // f64_I inv_fft2[8] = {_ia_set_pointed_f64(18.0), _ia_set_pointed_f64(0.0),
    //                  _ia_set_pointed_f64(2.0), _ia_set_pointed_f64(2.0),
    //                  _ia_set_pointed_f64(-10.0), _ia_set_pointed_f64(0.0),
    //                  _ia_set_pointed_f64(2.0), _ia_set_pointed_f64(-2.0)};
    interval_realfft(data1, data2, fft1, fft2, NN, twiddle);
    //interval_inverse_realfft(inv_data1, inv_data2, inv_fft1, inv_fft2, NN, twiddle);
    //printf("\nFIRST real signal");
    for (int i = 0; i < NN; i++) {
        // int j = i * 2;
        // printf("\n%f + %fj", fft1[j].up, fft1[j+1].up);
        //printf("\n%f", data1[i].up);
    }
    //printf("\nSECOND real signal");
    for (int i = 0; i <NN; i++) {
        // int j = i * 2;
        // printf("\n%f + %fj", fft2[j].up, fft2[j+1].up);
        //printf("\n%f" , data2[i].up);
    }

    long N = 8;
    f64_I *twiddles = malloc(sizeof(f64_I) * N);
    initialize_twiddles(N*2, twiddles);
    f64_I sig[N*2];
    f64_I sig1[N*2];
    char firstnumy[N];
    char secondnumy[N];
    f64_I *fft11 = malloc(sizeof(f64_I) * 4 * N);
    f64_I *fft22 = malloc(sizeof(f64_I) * 4 * N);
    real_randinit(sig, 3, N, firstnumy);
    real_randinit(sig1, 3, N, secondnumy);
    printf("\nfirstn %s", firstnumy);
    printf("\nsecondn %s", secondnumy);
    real_int_mult(3, N, sig, sig1, fft11, fft22, twiddles);
    free(fft11);
    free(fft22);

    return 0;
}