/*
 * commonHeaders.hpp
 *
 *  Created on: 2011.04.12.
 *      Author: Adam
 */

#ifndef COMMONHEADERS_HPP_
#define COMMONHEADERS_HPP_

#ifdef BUILDING_DLL
#define MY_DLL __declspec(dllexport)
#endif

typedef unsigned long UL;

/*  Global static variables: */
 static UL z=362436069, w=521288629, jsr=123456789, jcong=380116160;
 static UL t[256];
 static UL x=0,y=0; static unsigned char c=0;

#define znew  (z=36969*(z&65535)+(z>>16))
#define wnew  (w=18000*(w&65535)+(w>>16))
#define MWC   ( (znew<<16)+wnew )
#define SHR3  (jsr=(jsr=(jsr=jsr^(jsr<<17))^(jsr>>13))^(jsr<<5))
#define CONG  (jcong=69069*jcong+1234567)
#define KISS  ((MWC^CONG)+SHR3)
#define LFIB4 (t[c]=t[c]+t[c+58]+t[c+119]+t[c+178],t[++c])
#define SWB   (t[c+237]=(x=t[c+15])-(y=t[c]+(x<y)),t[++c])
#define UNI   (KISS*2.328306e-10)
#define VNI   ((long) KISS)*4.656613e-10


 /* Random seeds must be used to reset z,w,jsr,jcong and the table t[256].  Here is an example procedure, using KISS: */
 /*void settable(UL i1,UL i2,UL i3,UL i4)
 { int i; z=i1;w=i2,jsr=i3; jcong=i4;
 for(i=0;i<256;i++)  t[i]=KISS;        }*/

/*The generators MWC, KISS, LFIB4 and SWB seem to pass all tests.
By themselves, CONG and SHR3 do not, but using CONG+SHR3 provides one of the fastest combinations that satisfy the DIEHARD battery of tests.*/

/* Any one of KISS, MWC, LFIB4, SWB, SHR3, or CONG  can be used in an expression to provide a random 32-bit integer, while UNI
   provides a real in (0,1) and VNI a real in (-1,1).   */

#endif /* COMMONHEADERS_HPP_ */
