/* 
   Copyright (c) 2012,2013   Axel Kohlmeyer <akohlmey@gmail.com> 
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   * Neither the name of the <organization> nor the
     names of its contributors may be used to endorse or promote products
     derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define _GNU_SOURCE 1
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

extern double exp10(double);
extern float exp10f(float);

extern double genspline(int, double, double, double *, double *, double *);
extern double spl_exp(double, double, double, double *, double *, double *);

#include "fastermath.h"

#include "config.c"

/* compute high precision walltime and walltime difference */
static double wallclock(const double * __restrict ref)
{
    struct timespec t;
    double ret;

    clock_gettime(CLOCK_REALTIME, &t);
    ret = ((double) t.tv_sec)*1.0e6 + 1.0e-3*((double) t.tv_nsec);

    return ref ? (ret - *ref) : ret;
}

int main(int argc, char **argv)
{
    double *xval, *res0, *res1, *res2, *res3, *rest;
    float  *xvalf, *res0f, *res1f, *res2f, *res3f;
    double xscale, err, sumerr, start;
    double x0, xfin, delta; 
    double *xar, *a, *b;
    int nmax;
    int num, rep, i, j;
    unsigned int seed;

    puts("\n============================="
         "\nfastermath test and benchmark"
         "\n=============================");
    puts(config);

    if (argc < 3) {
        printf("usage %s <num tests> <num reps> [<rng seed>]\n",argv[0]);
        return -1;
    }

    num = atoi(argv[1]);
    rep = atoi(argv[2]);
    if (argc < 4) {
        seed = 1966;
    } else {
        seed = atoi(argv[3]);
    }
    srand(seed);

#define GRABMEM(var,type)                                          \
    posix_memalign((void **)&var, _FM_ALIGN, num*sizeof(type)); \
    memset(var, 0, num*sizeof(type))

    nmax=100;

#define GRABMEM2(var,type)                                          \
    posix_memalign((void **)&var, _FM_ALIGN, nmax*sizeof(type)); \
    memset(var, 0, nmax*sizeof(type))

    GRABMEM2(xar,double);
    GRABMEM2(a,double);
    GRABMEM2(b,double);

    GRABMEM(xval,double);
    GRABMEM(res0,double);
    GRABMEM(res1,double);
    GRABMEM(res2,double);
    GRABMEM(res3,double);
    GRABMEM(rest,double);

    GRABMEM(xvalf,float);
    GRABMEM(res0f,float);
    GRABMEM(res1f,float);
    GRABMEM(res2f,float);
    GRABMEM(res3f,float);
 
    puts("-------------------------\ntesting exponentiation functions");
    err = sumerr = 0.0;
    xscale = 1.0 / ((double) RAND_MAX);
    start = wallclock(NULL);
    x0 = -10.0;
    xfin = 10.0;
    delta = genspline(nmax, x0, xfin, xar, a, b);    

    /* random numbers distributed between x0 and xfin */ 
    for (i=0; i < num; ++i) {
        double r1,r2,rsum;
        r1 = xscale * ((double) rand());
        r2 = xscale * ((double) rand());
        rsum = 0.5*(r1+r2);
        xval[i] = (xfin-x0) * rsum  + x0;
        xvalf[i] = (float) xval[i];
        err += xval[i];
        sumerr += xval[i]*xval[i];
    }
    err /= (double) num;
    sumerr /= (double) num;
    printf("time/set for %d x-values : %8.4gus\n", num, wallclock(&start)/num);
    printf("<x>: %.6g    <x**2> - <x>**2: %.15g\n", err, sumerr-(err*err));

    xscale = 1.0/(rep*num);


#define DOUBLE_ERROR(ref,res)                           \
    sumerr = 0.0;                                       \
    for (i=0; i < num; ++i) {                           \
        sumerr += fabs(((double)res[i]- (double)ref[i])/((double) ref[i])); \
    }                                                   \
    printf("avgerr  %.6g\n", sumerr/((double) num))

#define FLOAT_ERROR(ref,res)                            \
    sumerrf = 0.0f;                                     \
    for (i=0; i < num; ++i) {                           \
        sumerrf += fabsf((res[i]-ref[i])/ref[i]);       \
    }                                                   \
    printf("avgerr  %.6g\n", sumerrf/((double) num))

#define RUN_LOOP(x,result,func,type)            \
    memset(result, 0, num*sizeof(type));        \
    start = wallclock(NULL);                    \
    for (j=0; j < rep; ++j) {                   \
        for (i=0; i < num; ++i)                 \
            result[i] += func(x[i]);            \
    }                                           \
    printf("time for% 20s(): %8.4fus  ",        \
           #func, xscale*wallclock(&start))

#define RUN_LOOP2(x,result,func,type, x01, delta2, xar1, a1, b1)            \
    memset(result, 0, num*sizeof(type));        \
    start = wallclock(NULL);                    \
    for (j=0; j < rep; ++j) {                   \
        for (i=0; i < num; ++i)                 \
            result[i] += func(x[i], x01, delta2, xar1, a1, b1);            \
    }                                           \
    printf("time for% 20s(): %8.4fus  ",        \
           #func, xscale*wallclock(&start)) 

    RUN_LOOP(xval,res0,exp,double);
    printf("numreps %d\n", rep);

    RUN_LOOP2(xval,rest,spl_exp,double, x0, delta, xar, a, b);
    DOUBLE_ERROR(res0,rest); 

/*    for (i=0; i < num; ++i) {
        printf("xval222,   %.15g \n", xval[i]);
    }
*/
    
    return 0;

#if 0
    /* special cases */


    puts("testing special cases. libm exp10() / exp10f()");
    printf("exp10(0.0)-1.0 = %.15g\n", exp10(0.0)-1.0);
    printf("exp10(1.0)-10.0 = %.15g\n", exp10(1.0)-10.0);
    printf("exp10(-1.0)-0.1 = %.15g\n", exp10(-1.0)-0.1);
    printf("exp10(10.0)-1e10 = %.15g\n", exp10(10.0)-1e10);

    printf("exp10f(0.0f)-1.0f = %.15g\n", exp10f(0.0f)-1.0f);
    printf("exp10f(1.0f)-10.0f = %.15g\n", exp10f(1.0f)-10.0f);
    printf("exp10(-1.0f)-0.1f = %.15g\n", exp10f(-1.0f)-0.1f);
    printf("exp10(10.0f)-1e10f = %.15g\n", exp10f(10.0f)-1e10f);
    printf("exp10(10.0f) = %.15g\n", exp10f(10.0f));

    puts("testing special cases. libm fm_exp10() / fm_exp10f()");
    printf("exp10(0.0)-1.0 = %.15g\n", fm_exp10(0.0)-1.0);
    printf("exp10(1.0)-10.0 = %.15g\n", fm_exp10(1.0)-10.0);
    printf("exp10(-1.0)-0.1 = %.15g\n", fm_exp10(-1.0)-0.1);
    printf("exp10(10.0)-1e10 = %.15g\n", fm_exp10(10.0)-1e10);

    printf("exp10f(0.0f)-1.0f = %.15g\n", fm_exp10f(0.0f)-1.0f);
    printf("exp10f(1.0f)-10.0f = %.15g\n", fm_exp10f(1.0f)-10.0f);
    printf("exp10(-1.0f)-0.1f = %.15g\n", fm_exp10f(-1.0f)-0.1f);
    printf("exp10(10.0f)-1e10f = %.15g\n", fm_exp10f(10.0f)-1e10f);
    printf("exp10(10.0f) = %.15g\n", fm_exp10f(10.0f));

    puts("testing special cases. libm fm_exp10_alt() / fm_exp10f_alt()");
    printf("exp10(0.0)-1.0 = %.15g\n", fm_exp10_alt(0.0)-1.0);
    printf("exp10(1.0)-10.0 = %.15g\n", fm_exp10_alt(1.0)-10.0);
    printf("exp10(-1.0)-0.1 = %.15g\n", fm_exp10_alt(-1.0)-0.1);
    printf("exp10(10.0)-1e10 = %.15g\n", fm_exp10_alt(10.0)-1e10);
    printf("exp10(10.0f) = %.15g\n", exp10f(10.0f));

    printf("exp10f(0.0f)-1.0f = %.15g\n", fm_exp10f_alt(0.0f)-1.0f);
    printf("exp10f(1.0f)-10.0f = %.15g\n", fm_exp10f_alt(1.0f)-10.0f);
    printf("exp10(-1.0)-0.1f = %.15g\n", fm_exp10f_alt(-1.0f)-0.1f);
    printf("exp10(10.0f)-1e10f = %.15g\n", fm_exp10f_alt(10.0f)-1e10f);

#endif

}

/* 
 * Local Variables:
 * mode: c
 * compile-command: "make -C .."
 * c-basic-offset: 4
 * fill-column: 76 
 * indent-tabs-mode: nil 
 * End: 
 */
