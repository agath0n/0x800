//------------------------------------------------------------------------------
// Company
//
// Module header : Common
// 
//------------------------------------------------------------------------------

// Multi-inclusion protection
#ifndef Common_H
#define Common_H

// Manages prototyping and declarations
#undef PUBLIC
#ifdef Common_M
#define PUBLIC
#else
#define PUBLIC extern
#endif

//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>

//------------------------------------------------------------------------------
//                                                      Defines & Types exportes
//------------------------------------------------------------------------------
typedef enum{
  DIR_LEFT = 0,
  DIR_RIGHT,
  DIR_UP,
  DIR_DOWN,

  DIR_MAX
} E_COM_DIRECTION;

//nCurses fucks up on this side. FALSE is already defined there, must do sthg dirty to avoid bugs.
#ifndef FALSE
typedef enum{
  FALSE = 0,
  TRUE
}BOOL;
#else
typedef enum{
  B_FALSE = 0,
  B_TRUE
}BOOL;
#endif

typedef uint8_t  u8;
typedef uint32_t u32;


#endif // Common_H
