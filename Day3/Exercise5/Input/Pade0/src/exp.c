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

/* faster versions of 2**x, e**x, and 10**x in single and double precision.
 *
 * Based on the Cephes math library 2.8
 */

#include "fastermath.h"
#include "fm_internal.h"
#include <math.h>

/*  Seriani: basic Pade` implementation of exp(x).
 */

static const double pad_exp_p[] __attribute__ ((aligned(_FM_ALIGN))) = {
    2.30933477057345225087e-2,
    2.02020656693165307700e1,
    1.51390680115615096133e3,
    2.33184211722314911771e2,
    4.36821166879210612817e3,
};

double pad_exp(double x)
{
    double  px, qx;

    x *= log2(2.71828182846);

    int s = (x > 0 ? (int) x : (int) x-1);
    long int t = (s + 127) << 23;
    double ipow = *((float*)(&t));

    x -= s;
    
    px = pad_exp_p[0]*x*x*x*x*x +
        pad_exp_p[1]*x*x*x + pad_exp_p[2]*x;

    qx = x*x*x*x + pad_exp_p[3]*x*x + pad_exp_p[4];

    x = 1. + 2.*px/(qx-px);

    return ipow*x;
}

/***
     // Old Code 
     static const double pad_exp_p[] __attribute__ ((aligned(_FM_ALIGN))) = {
         5.95238095238095238095e-4,
         1.19047619047619047619e-2,
         1.07142857142857142857e-1,
         5.00000000000000000000e-1,
         1.00000000000000000000e0,
         };
                   
       double pad_exp(double x)
          {
       double  px, qx;

       px =         pad_exp_p[0];
       qx =         pad_exp_p[0];
       px = px*x + pad_exp_p[1];
       qx = -qx*x + pad_exp_p[1];
       px = px*x + pad_exp_p[2];
       qx = -qx*x + pad_exp_p[2];
       px = px*x + pad_exp_p[3];
       qx = -qx*x + pad_exp_p[3];
       px = px*x + pad_exp_p[4];
       qx = -qx*x + pad_exp_p[4];
       x = px/qx;
      return x;
     }
***/




static const double fm_exp_t[] __attribute__ ((aligned(_FM_ALIGN))) = {
    1.00000000000000000000e0,
    5.00000000000000000000e-1,
    1.66666666666666666666e-1,
    4.16666666666666666666e-2,
    8.33333333333333333333e-3,
    1.38888888888888888888e-3,
    1.98412698412698412698e-4
};
    
 
double t_exp(double x)
{
    double   tx;
           tx =        fm_exp_t[6];
           tx = tx*x + fm_exp_t[5];
           tx = tx*x + fm_exp_t[4];
           tx = tx*x + fm_exp_t[3];
           tx = tx*x + fm_exp_t[2];
           tx = tx*x + fm_exp_t[1];
           tx = tx*x + fm_exp_t[0];
           tx = tx*x + fm_exp_t[0];
 return tx;
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
