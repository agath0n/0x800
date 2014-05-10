//------------------------------------------------------------------------------
//
// Module : Grid (not Groland)
// 
// Description : Grid mgmt module (core of the game)
//
//------------------------------------------------------------------------------
#define GRD_M


//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "RDM.h"
#include "GRD.h"

//------------------------------------------------------------------------------
//                                                               Defines & Types
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//                                                                     Variables
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static
static S_BLK sa_Grid[GRID_SIZE][GRID_SIZE];
/*sa_Grid is a reprensation of the grid, as follow:
 *
 *   0------------>y
 *   | .  .  .  .
 *   | .  .  .  .
 *   | .  .  .  .
 *   | .  .  .  .
 *   v
 *   x
 */
static S_BLK* sa_pBlks[GRID_MAX_ITEMS]; //Same as sa_Grid, but with a linear representation.

//------------------------------------------------------------------------------
//                                             Prototypes des fonctions internes
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static
static void GRD_InitBlock(S_BLK* pBlk, int x, int y);
static BOOL GRD_RandomInsert();
static BOOL GRD_Reduce(const E_COM_DIRECTION eDir, S_BLK* pBlk);
static BOOL GRD_CheckGameOver();


//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

/*****************************************************************************
NOM : GRD_Init
------------------------------------------------------------------------------
DESCRIPTION : Initialization of the module

------------------------------------------------------------------------------
ENTREES  : /

------------------------------------------------------------------------------
SORTIES  : /

------------------------------------------------------------------------------
 *****************************************************************************/
void GRD_Init()
{
  u32 k = 0;
  //Initialize the neigbours
  for (int i = 0; i < GRID_SIZE; i++)
  {
    for (int j = 0; j < GRID_SIZE; j++)
    {
      GRD_InitBlock(&sa_Grid[i][j], i, j);
      sa_pBlks[k++] = &sa_Grid[i][j];
    }
  }
  GRD_RandomInsert();
}

/*****************************************************************************
NOM : GRD_Print
------------------------------------------------------------------------------
DESCRIPTION : Prints the grid. Formated for Stdout
  Intended for debug purposes mostly
 *****************************************************************************/
void GRD_Print()
{
  printf("-----------------\n");
  for (int i = 0; i < GRID_SIZE; i++)
  {
    for (int j = 0; j < GRID_SIZE; j++)
    {
      printf("|");
      if (sa_Grid[i][j].u32Value != 0)
      {
        printf(" %d ", sa_Grid[i][j].u32Value);
      }
      else
      {
        printf(" . ");
      }
    }
    printf("|\n-----------------\n");
  }
}

/*****************************************************************************
NOM : GRD_Move
------------------------------------------------------------------------------
DESCRIPTION : Computes the new grid, given the force direction
------------------------------------------------------------------------------
OUTPUT : TRUE if Game if over. False otherwise.
 *****************************************************************************/
BOOL GRD_Move(const E_COM_DIRECTION eDir)
{

#define COUNT(I, Dir) ((Dir==DIR_RIGHT||Dir==DIR_DOWN)?I--:I++)

u32 u32_XStart,
    u32_YStart;
BOOL bMergedOrMovedCell = FALSE;

switch (eDir)
{
  case DIR_RIGHT:
    u32_XStart = GRID_END;
    u32_YStart = GRID_END;
    break;

  case DIR_LEFT:
  case DIR_UP:
    u32_XStart = GRID_START;
    u32_YStart = GRID_START;
    break;

  case DIR_DOWN:
    u32_XStart = GRID_END;
    u32_YStart = GRID_END;
    break;

  default:
    break;
}
  //TODO: optimize this piece of code with pointers to x & y, in order to avoid the 4 loops.
  if ((eDir == DIR_UP) || (eDir == DIR_DOWN))
  {
    for (u32 y = u32_YStart; y >= 0 && y < GRID_SIZE; COUNT(y, eDir))
    {
      for (u32 x = u32_XStart; x >= 0 && x < GRID_SIZE; COUNT(x, eDir))
      {
        bMergedOrMovedCell |= GRD_Reduce(eDir, &sa_Grid[x][y]);
      }
    }
  }
  else // Left or Right
  {
    for (u32 x = u32_XStart; x >= 0 && x < GRID_SIZE; COUNT(x, eDir))
    {
      for (u32 y = u32_YStart; y >= 0 && y < GRID_SIZE; COUNT(y, eDir))
      {
        bMergedOrMovedCell |= GRD_Reduce(eDir, &sa_Grid[x][y]);
      }
    }
  }

  if (!GRD_RandomInsert())
  {
    return GRD_CheckGameOver();
  }
  return FALSE;
}

/*****************************************************************************
NOM : GRD_GetBlock
------------------------------------------------------------------------------
DESCRIPTION : Returns a pointer to the grid.
*****************************************************************************/
S_BLK* GRD_GetBlock(u32 x, u32 y)
{
  if ((x < GRID_SIZE) && (y < GRID_SIZE))
    return &sa_Grid[x][y];
  return NULL;
}

//------------------------------------------------------------------------------
//                                                           Fonctions internes
//------------------------------------------------------------------------------

/*****************************************************************************
NOM : GRD_InitBlock
------------------------------------------------------------------------------
DESCRIPTION : Initialization of an element of the grid
*****************************************************************************/
void GRD_InitBlock(S_BLK* pBlk, int x, int y)
{
  memset(pBlk->nghs, 0, sizeof(pBlk->nghs));

  if ((x+1) < GRID_SIZE)
    pBlk->nghs[DIR_DOWN] = &sa_Grid[x+1][y];
  if (x > 0)
    pBlk->nghs[DIR_UP] = &sa_Grid[x-1][y];
  if ((y+1) < GRID_SIZE)
    pBlk->nghs[DIR_RIGHT] = &sa_Grid[x][y+1];
  if (y > 0)
    pBlk->nghs[DIR_LEFT] = &sa_Grid[x][y-1];

  pBlk->u32Value = 0;
}

/*****************************************************************************
NOM : GRD_RandomInsert
------------------------------------------------------------------------------
DESCRIPTION : Add an item randomly in the grid
------------------------------------------------------------------------------
SORTIE: True if an item was inserted
*****************************************************************************/
BOOL GRD_RandomInsert()
{
  u32 u32Val = (RDM_Get(1000)>800?4:2);
  u32 u32ZeroItems = 0;
  S_BLK* a_pZeroBlocks[GRID_MAX_ITEMS];

  for (u32 i = 0; i < GRID_MAX_ITEMS; ++i)
  {
    if (sa_pBlks[i]->u32Value == 0)
    {
      a_pZeroBlocks[u32ZeroItems++] = sa_pBlks[i];
    }
  }

  if (u32ZeroItems > 0)
  {
    u32 u32Idx = RDM_Get(u32ZeroItems);
    a_pZeroBlocks[u32Idx]->u32Value = u32Val;
    return TRUE;
  }
  return FALSE;
}

/*****************************************************************************
NOM : GRD_Reduce
*****************************************************************************/
BOOL GRD_Reduce(const E_COM_DIRECTION eDir, S_BLK* pBlk)
{
  S_BLK* pNextBlock = pBlk;
  BOOL bMergedOrMovedCell = FALSE;

  if (pBlk->u32Value != 0)
  {
    //Move the block if necessary
    BOOL bContinue = FALSE;
    do
    {
      bContinue = FALSE;
      if (pNextBlock->nghs[eDir] != NULL)
      {
        if (pNextBlock->nghs[eDir]->u32Value == 0)
        {
          pNextBlock = pNextBlock->nghs[eDir];
          bContinue = TRUE;
        }
      }
    } while (bContinue);

    if (pNextBlock != pBlk)
    {
      pNextBlock->u32Value = pBlk->u32Value;
      pBlk->u32Value = 0;
      bMergedOrMovedCell = TRUE;
    }

    //Merge?
    if (pNextBlock->nghs[eDir] != NULL)
    {
      if (pNextBlock->u32Value == pNextBlock->nghs[eDir]->u32Value)
      {
        pNextBlock->nghs[eDir]->u32Value = 2 * pNextBlock->nghs[eDir]->u32Value;
        pNextBlock->u32Value = 0;
        bMergedOrMovedCell = TRUE;
      }
    }
  }
  return bMergedOrMovedCell;
}

/*****************************************************************************
NOM : GRD_CheckGameOver
OUTPUT: TRUE if Game is over, FALSE otherwise.
*****************************************************************************/
BOOL GRD_CheckGameOver()
{
  //Not the most efficient way to do. Some blocks are checked more than once.
  for (u32 i = 0; i < GRID_MAX_ITEMS; i++)
  {
    S_BLK* pBlk = sa_pBlks[i];
    for (E_COM_DIRECTION eDir = 0; eDir < DIR_MAX; eDir++)
    {
      S_BLK* pNghb = pBlk->nghs[eDir];
      if (pNghb != NULL)
      {
        if (pNghb->u32Value == pBlk->u32Value)
        {
          return FALSE;
        }
      }
    }
  }
  return TRUE;
}
//------------------------------------------------------------------------------
