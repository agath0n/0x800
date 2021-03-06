//------------------------------------------------------------------------------
//
// Module header : BLK
// 
// Description : Dummy Module Template Header
//
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef BLK_H
#define BLK_H

// Manages prototyping and declarations
#undef PUBLIC
#ifdef BLK_M
#define PUBLIC
#else
#define PUBLIC extern
#endif

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include "Common.h"
#include "GRD.h"

//------------------------------------------------------------------------------
//                                                      Defines & Types exportes
//------------------------------------------------------------------------------
typedef struct S_BLK S_BLK;

struct S_BLK{
  S_BLK* nghs[DIR_MAX];
  uint32_t u32Value;
};

//------------------------------------------------------------------------------
//                                                           Exported variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------


#endif // BLK_H
//------------------------------------------------------------------------------
