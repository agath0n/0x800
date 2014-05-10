//------------------------------------------------------------------------------
//
// Module header : RDM
// 
// Description : Ramdom number generator
//
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef RDM_H
#define RDM_H

// Manages prototyping and declarations
#undef PUBLIC
#ifdef RDM_M
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

//------------------------------------------------------------------------------
//                                                      Defines & Types exportes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                           Exported variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

PUBLIC void RDM_Init();
PUBLIC uint32_t RDM_Get(uint32_t mod);
PUBLIC void RDM_Test(u32 iterations);

#endif // RDM_H
//------------------------------------------------------------------------------
