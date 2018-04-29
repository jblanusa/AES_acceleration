/*
  Copyright 2002 Colin Percival and the University of Oxford

  Copyright in this software is held by the University of Oxford
  and Colin Percival. Both the University of Oxford and Colin Percival
  have agreed that this software may be distributed free of charge and
  in accordance with the attached licence.

  The above notwithstanding, in order that the University as a
  charitable foundation protects its assets for the benefit of its
  educational and research purposes, the University makes clear that no
  condition is made or to be implied, nor is any warranty given or to
  be implied, as to the proper functioning of this work, or that it
  will be suitable for any particular purpose or for use under any
  specific conditions.  Furthermore, the University disclaims all
  responsibility for any use which is made of this work.

  For the terms under which this work may be distributed, please see
  the adjoining file "LICENSE".
*/
/*
  $Id: aestab.h,v 1.1 2006/02/22 20:38:25 epalza Exp $
*/

#include "ptypes.h"

extern uint8 Sbox[256];
extern uint8 InvSbox[256];
extern uint8 aestab_SBox0[1024];
extern uint8 aestab_SBox1[1024];
extern uint8 aestab_SBox2[1024];
extern uint8 aestab_SBox3[1024];
//extern uint8 aestab_ISBox0[1024];
//extern uint8 aestab_ISBox1[1024];
//extern uint8 aestab_ISBox2[1024];
//extern uint8 aestab_ISBox3[1024];
extern uint8 Xtime2Sbox[256];
extern uint8 Xtime3Sbox[256];
extern uint8 Xtime9[256];
extern uint8 XtimeB[256];
extern uint8 XtimeD[256];
extern uint8 XtimeE[256];

