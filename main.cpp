/******************************************************************************
 *****************************************************************************/
/*
**   Machine Training application
*    System initialization, command dispatch, and diagnostic stuff is here.
*    All HTML interface code is in htmlinterface.cpp
*
 *****************************************************************************
 *****************************************************************************/


#include "predef.h"
#include <stdio.h>
#include <ctype.h>
#include <startnet.h>
#include <http.h>
#include <ucosmcfc.h>
#include <system.h>
#include <iosys.h>
#include <serial.h>
#include <dhcpclient.h>
#include <taskmon.h>

#include "MachineEngine.h"

extern const char *PlatformName;

extern "C"
{
   void UserMain( void *pd );
   void OSDumpTCBs();
}


const char *AppName = "Machine Training test application";

void UserMain( void *pd )
{
   InitializeStack();
   OSChangePrio( MAIN_PRIO );

   iprintf( "Machine Training application started on %s\r\n", PlatformName );

  /* This routine is the entry point for application */
  iprintf("Calling MachineEngine class constructor\n");
  MachineEngine * poME = new MachineEngine();
  iprintf("Calling MachineParameters class constructor\n");
  MachineParameters * poMP = new MachineParameters();

  unsigned short ucInputVectorLength  = INPUT_BITS;
  unsigned short ucOutputVectorLength = OUTPUT_BITS;
  
  poMP->setInputVectorLength( ucInputVectorLength+1 );
  poMP->setOutputVectorLength( ucOutputVectorLength );
  poMP->setMachineTraining( TRUE );

  /* Set parameters before calling configure & start */
  iprintf("Calling MachineEngine::configure( )\n");
  poME->configure(poMP);

  iprintf("Calling MachineEngine::start( )\n");
  poME->start();

  iprintf("Training completed.  Error threshold reached.\n");
  iprintf("Control returned to application.\n");

  poMP->setMachineTraining( FALSE );
  poME->start();

  iprintf("End bumper.  We only get here is there is a problem");
  iprintf(" in MachineEngine::start( )\n");
  
   while ( 1 )
   {
     /* empty while loop as backup */
   }//While
}

