//------------------------------------------------------------------------------
//
// Module : CLI
//
// Description : NCurses User Interface Module
//
//------------------------------------------------------------------------------
#define CLI_M


//------------------------------------------------------------------------------
//                                                                      Includes
//------------------------------------------------------------------------------
#include <unistd.h>
#include <termios.h>  //For configuring the terminal (see CLI_Init())
#include <ctype.h>
#include <curses.h>

#include "RDM.h"
#include "GRD.h"
#include "CLI.h"

//------------------------------------------------------------------------------
//                                                               Defines & Types
//------------------------------------------------------------------------------
#define X_OFFSET     1
#define Y_OFFSET     1
#define BLOCK_WIDTH  3

#define X_STEP         (1 * BLOCK_WIDTH)
#define Y_STEP         (2 * BLOCK_WIDTH)

#define X_GRDWIN_SIZE  ((4 * (GRID_SIZE) * X_STEP) / 3 - X_STEP)
#define Y_GRDWIN_SIZE  ((4 * (GRID_SIZE) * Y_STEP) / 3 - Y_STEP)
//------------------------------------------------------------------------------
//                                                                     Variables
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static
static struct termios old_tio, new_tio;
static WINDOW* s_ncWin = NULL; //NCurses main display window.
static WINDOW* s_grdWin = NULL;
static BOOL s_bAutoPlay = FALSE;
//------------------------------------------------------------------------------
//                                             Prototypes des fonctions internes
//------------------------------------------------------------------------------
// Note : OBLIGATOIREMENT static
static void CLI_GetDir(E_COM_DIRECTION* pDir, BOOL* pbContinue);
static void CLI_PrintFormatedGrid();
static void CLI_PrintArrow(E_COM_DIRECTION eDir);
static void CLI_DrawBlock(u32 x, u32 y, S_BLK* pBlk);

//------------------------------------------------------------------------------
//                                                           Fonctions exportees
//------------------------------------------------------------------------------

/*****************************************************************************
NOM : CLI_Init
------------------------------------------------------------------------------
DESCRIPTION :
- Configuration of the terminal so that there is no echo of the
pressed key, and no buffering (avoids waiting for enter when waiting for input).
Probably OS-dependent. Works well under Linux w/ gnome-terminal. Needs testing
on other platform.
- Initialization of nCurses.
*****************************************************************************/
void CLI_Init(BOOL bAutoPlayMode)
{
  s_bAutoPlay = bAutoPlayMode;

  /* get the terminal settings for stdin */
  tcgetattr(STDIN_FILENO,&old_tio);

  /* we want to keep the old setting to restore them a the end */
  new_tio=old_tio;

  /* disable canonical mode (buffered i/o) and local echo */
  new_tio.c_lflag &=(~ICANON & ~ECHO);

  /* set the new settings immediately */
  tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

  /*  Initialize ncurses  */
  s_ncWin = initscr();
  if (s_ncWin == NULL)
  {
    fprintf(stderr, "Error initialising ncurses.\n");
    return;
  }
  start_color();
  init_pair(1,  COLOR_RED,     COLOR_BLACK);
  init_pair(2,  COLOR_GREEN,   COLOR_BLACK);
  init_pair(3,  COLOR_YELLOW,  COLOR_BLACK);
  init_pair(4,  COLOR_BLUE,    COLOR_BLACK);
  init_pair(5,  COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6,  COLOR_CYAN,    COLOR_BLACK);
  init_pair(7,  COLOR_BLUE,    COLOR_WHITE);
  init_pair(8,  COLOR_WHITE,   COLOR_RED);
  init_pair(9,  COLOR_BLACK,   COLOR_GREEN);
  init_pair(10, COLOR_BLUE,    COLOR_YELLOW);
  init_pair(11, COLOR_WHITE,   COLOR_BLUE);
  init_pair(12, COLOR_WHITE,   COLOR_MAGENTA);
  init_pair(13, COLOR_BLACK,   COLOR_CYAN);

  s_grdWin = newwin(X_GRDWIN_SIZE + 5,  //Height
                    Y_GRDWIN_SIZE ,  //Width
                    X_OFFSET,
                    Y_OFFSET);
//  box(s_grdWin, '|','-');
  wrefresh(s_grdWin);
  curs_set(0); //Hide the blinking cursor
}

/*****************************************************************************
NOM : CLI_Process
------------------------------------------------------------------------------
DESCRIPTION : CLI Main Loop

*****************************************************************************/
void CLI_Process()
{
  E_COM_DIRECTION eDir;
  BOOL bGameOver = FALSE,
       bContinue = TRUE;

//  GRD_Print();
  CLI_PrintFormatedGrid();
  while(bContinue)
  {
    if (s_bAutoPlay)
    {
      eDir = RDM_Get(4);
      usleep((unsigned long)300E3);
    }
    else
    {
      CLI_GetDir(&eDir, &bContinue);
    }

    if (eDir != DIR_MAX)
    {
      bGameOver = GRD_Move(eDir);
      CLI_PrintFormatedGrid();
      CLI_PrintArrow(eDir);
      eDir = DIR_MAX;
    }

    if (bGameOver)
    {
      mvwprintw(s_grdWin, X_GRDWIN_SIZE /2, 8, "Game Over");
      wrefresh(s_grdWin);
      CLI_GetDir(&eDir, &bContinue);
      break;
    }
  }
}

/*****************************************************************************
NOM : CLI_Quit
------------------------------------------------------------------------------
DESCRIPTION : Un-initilization of the module

------------------------------------------------------------------------------
ENTREES  : /

------------------------------------------------------------------------------
SORTIES  : /

------------------------------------------------------------------------------
 *****************************************************************************/
void CLI_Quit()
{
  /* restore the former settings */
  tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

  /*  Clean up the NCurses thing*/
  delwin(s_ncWin);
  endwin();
  refresh();
}

//------------------------------------------------------------------------------
//                                                           Fonctions internes
//------------------------------------------------------------------------------

/*****************************************************************************
NOM : CLI_GetDir
------------------------------------------------------------------------------
DESCRIPTION : Handles user input and return which arrow key was pressed, and
              if the user wants to quit
*****************************************************************************/
void CLI_GetDir(E_COM_DIRECTION* pDir, BOOL* pbContinue)
{
#define KEYB_ARROW_START_SEQ  27
#define KEYB_ARROW_MID_SEQ    91
#define KEYB_UP    65
#define KEYB_DOWN  66
#define KEYB_RIGHT 67
#define KEYB_LEFT  68

  BOOL bContinue = TRUE;
  while(bContinue)
  {
    int c = getchar();
    if (tolower(c) == 'q')
    {
      *pbContinue = FALSE;
      bContinue = FALSE;
    }
    else if  (KEYB_ARROW_START_SEQ == c
           && KEYB_ARROW_MID_SEQ == getchar())
    {
      *pDir = DIR_MAX;
      switch (getchar())
      {
       case KEYB_UP:
         *pDir = DIR_UP;
         break;
       case KEYB_DOWN:
         *pDir = DIR_DOWN;
         break;
       case KEYB_RIGHT:
         *pDir = DIR_RIGHT;
         break;
       case KEYB_LEFT:
         *pDir = DIR_LEFT;
         break;
      }
      if (*pDir != DIR_MAX)
        bContinue = FALSE;
    }
  }
}

/*****************************************************************************
NOM : CLI_PrintFormatedGrid
------------------------------------------------------------------------------
DESCRIPTION : Prints the grid in a 'fancy' way, using nCurses library.
*****************************************************************************/
void CLI_PrintFormatedGrid()
{
  u32 x = 0,
      y = 0;
  S_BLK *pBlk;
  wclear(s_grdWin);
  //draw grid
  for (x = 0, y = 0;
        x <= X_GRDWIN_SIZE && y <= Y_GRDWIN_SIZE;
        x += X_STEP, y += Y_STEP)
  {
    mvwhline(s_grdWin,x,1, '-', Y_GRDWIN_SIZE - 3);
    mvwvline(s_grdWin,1,y, '|', X_GRDWIN_SIZE - 2); /*(y==0||y==Y_GRDWIN_SIZE?X_GRDWIN_SIZE - 1 : X_GRDWIN_SIZE - 2)*/
  }

  for (x = 0; x < GRID_SIZE; x++)
  {
    for (y = 0; y < GRID_SIZE; y++)
    {
      pBlk = GRD_GetBlock(x, y);
      CLI_DrawBlock(x, y, pBlk);
    }
  }
  mvwprintw(s_grdWin, 0, (Y_GRDWIN_SIZE - 5) / 2, "0x800");
  wmove(s_grdWin,0,0);
  touchwin(s_grdWin);
  wrefresh(s_grdWin);
}

/*****************************************************************************
NOM : CLI_PrintArrow
------------------------------------------------------------------------------
DESCRIPTION : Prints a nice arrow to give some feedback to the user.
*****************************************************************************/
void CLI_PrintArrow(E_COM_DIRECTION eDir)
{
  chtype ucSym[DIR_MAX] = {
      /* DIR_LEFT  */ ACS_LARROW,
      /* DIR_RIGHT */ ACS_RARROW,
      /* DIR_UP    */ ACS_UARROW,
      /* DIR_DOWN  */ ACS_DARROW,
  };

  mvwaddch(s_grdWin, X_GRDWIN_SIZE + 1, Y_GRDWIN_SIZE / 2 - 1, ucSym[eDir]);
  wrefresh(s_grdWin);
}

/*****************************************************************************
NOM : CLI_DrawBlock
------------------------------------------------------------------------------
DESCRIPTION : Prints a single block of the grid.
------------------------------------------------------------------------------
INPUTS : x, y: coordinates in the grid (not on the screen)
*****************************************************************************/
void CLI_DrawBlock(u32 x, u32 y, S_BLK* pBlk)
{
  u32 x_c = x * X_STEP + X_STEP/2;
  u32 y_c = y * Y_STEP + Y_STEP/2;

  wmove(s_grdWin, x_c, y_c);
  if (pBlk->u32Value > 0)
  {
    wprintw(s_grdWin, "%d", pBlk->u32Value);
  }
  else
  {
    wprintw(s_grdWin, " . ");
  }

}
//------------------------------------------------------------------------------
