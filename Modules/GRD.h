//------------------------------------------------------------------------------
//
// Module header : GRD
// 
// Description : Grid mgmt module (core of the game)
//
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef GRD_H
#define GRD_H

// Manages prototyping and declarations
#undef PUBLIC
#ifdef GRD_M
#define PUBLIC
#else
#define PUBLIC extern
#endif

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include "Common.h"
#include "BLK.h"

//------------------------------------------------------------------------------
//                                                      Defines & Types exportes
//------------------------------------------------------------------------------
#ifndef GRID_SIZE
#define GRID_SIZE       4
#endif

#define GRID_START      0
#define GRID_END        (GRID_SIZE - 1)
#define GRID_MAX_ITEMS  (GRID_SIZE * GRID_SIZE)

//------------------------------------------------------------------------------
//                                                           Exported variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------
PUBLIC void GRD_Init();
PUBLIC void GRD_Print();
PUBLIC BOOL GRD_Move(const E_COM_DIRECTION eDir);
PUBLIC S_BLK* GRD_GetBlock(u32 x, u32 y);

//------------------------------------------------------------------------------
//                                                                        Macros
//------------------------------------------------------------------------------
#if (GRID_SIZE < 2)
#error Wrong Grid Size
#endif

#endif // GRD_H
