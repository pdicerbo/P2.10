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
    double *xval, *res0, *res1, *res2, *res3, *rest, *resn;
    float  *xvalf, *res0f, *res1f, *res2f, *res3f, *restf, *resnf;
    double xscale, err, sumerr, start;
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

    GRABMEM(xval,double);
    GRABMEM(res0,double);
    GRABMEM(res1,double);
    GRABMEM(res2,double);
    GRABMEM(res3,double);
    GRABMEM(rest,double);
    GRABMEM(resn,double);

    GRABMEM(xvalf,float);
    GRABMEM(res0f,float);
    GRABMEM(res1f,float);
    GRABMEM(res2f,float);
    GRABMEM(res3f,float);
    GRABMEM(restf,float);
    GRABMEM(resnf,float);

    err = sumerr = 0.0;
    xscale = 1.0 / ((double) RAND_MAX);
    start = wallclock(NULL);
    /* random numbers distributed between 0 and 10.0 */ 
    for (i=0; i < num; ++i) {
        double r1,r2,rsum;
        r1 = xscale * ((double) rand());
        r2 = xscale * ((double) rand());
        rsum = 0.5*(r1+r2);
        xval[i] = 10.0 * rsum ;
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

    RUN_LOOP(xvalf,res0f,invsqrt,float);
    printf("numreps %d\n", rep);

    RUN_LOOP(xvalf,resnf,Q_rsqrt,float);
    DOUBLE_ERROR(res0f,resnf);

    free(xval);
    free(res0);
    free(res1);
    free(res2);
    free(res3);
    free(xvalf);
    free(res0f);
    free(res1f);
    free(res2f);
    free(res3f);

    return 0;

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
