# -*- makefile -*-
# configuration for Linux 64-bit x86 with GCC using the regular FPU
ALIGNMENT=16
CC=gcc -m64
CPPFLAGS=
ARCHFLAGS= -mpc64 -march=native #-mfpmath=387 -g
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O3 -ffast-math -fexpensive-optimizations
WARNFLAGS= -Wall -W

LD=$(CC)
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm
