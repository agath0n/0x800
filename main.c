#include <stdlib.h>
#include <stdio.h>

#include "GRD.h"
#include "RDM.h"
#include "CLI.h"

int main(int argc, char *argv[])
{
  //Initialization
  RDM_Init();
  GRD_Init();
  //TODO Write arg parser for auto play.
  CLI_Init(FALSE);

  //Main software
  CLI_Process();

  //End
  CLI_Quit();

  return EXIT_SUCCESS;
}
