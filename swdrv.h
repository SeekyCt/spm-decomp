#ifndef SWDRV_H
#define SWDRV_H
/*
  0007e9dc 000078 80083fbc  4 swInit 	swdrv.o 
  0007e998 000044 80083f78  4 swReInit 	swdrv.o 
  0007e958 000040 80083f38  4 swSet 	swdrv.o 
  0007e910 000048 80083ef0  4 swGet 	swdrv.o 
  0007e8d0 000040 80083eb0  4 swClear 	swdrv.o 
  UNUSED   000054 ........ swToggle swdrv.o 
  0007e8ac 000024 80083e8c  4 swByteSet 	swdrv.o 
  0007e888 000024 80083e68  4 swByteGet 	swdrv.o 
  0007e848 000040 80083e28  4 _swSet 	swdrv.o 
  0007e800 000048 80083de0  4 _swGet 	swdrv.o 
  0007e7c0 000040 80083da0  4 _swClear 	swdrv.o 
  UNUSED   000054 ........ _swToggle swdrv.o 
  0007e7b0 000010 80083d90  4 _swByteSet 	swdrv.o 
  0007e7a0 000010 80083d80  4 _swByteGet 	swdrv.o 
*/

int swGet(int n);
int swByteGet(int n);
int _swGet(int n);
int _swByteGet(int n);

#endif