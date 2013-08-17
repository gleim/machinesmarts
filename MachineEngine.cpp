/***************************************************
 *
 *  MachineEngine.cpp
 *
 * 	MachineEngine class - this is the
 *		primary class of the system,
 *		providing the
 *	  	majority of the processing
 *		behind the STE layer.
 *
 **************************************************/
#include "MachineEngine.h"

/* debug compile time flags */
#define ENTRY_DEBUG           0
#define IO_DEBUG              0

/* runtime path flags */
#define COMMUNICATE_WITH_VI   0
#define USE_CANNED_DATA       1

static int uiIterationCount; 
static bool bLocalTrain = FALSE;
unsigned int uiPatternVectorCount;

void InputOutputTask(void *);
		
/* 
 * **************************************************************************
 *
 *     The following data structures are tied to the RTOS and/or hardware
 *
 * **************************************************************************
 */
 
OS_MBOX InputMbox, IterateMbox, OutputMbox;
int HostPC, DeviceDriver;
 

extern "C" {void UserMain(void *pd); }

MachineEngine::MachineEngine( )
{
  bStopRequested = 0;
  bInitialized = 0;
  poMachineParameters = NULL;
  poVars = NULL;
  uiPatternVectorCount = 0;
}

MachineEngine::~MachineEngine( )
{
  stop( );
  bInitialized = 0;
  poMachineParameters = NULL;
  poVars = NULL;
  uiPatternVectorCount = 0;
}

void MachineEngine::configure( MachineParameters* pMachineParameters )
{
#if ENTRY_DEBUG
  iprintf("MachineEngine::configure( ) entry point\n");
#endif  
  if (pMachineParameters)
  {
    if (!bInitialized)
    {
      poMachineParameters = pMachineParameters;

      initialize( );

#if ENTRY_DEBUG
      /* show the client the new network */
      display( );
#endif
    }
    else
    {
      /* warn client that engine is already configured & initialized */
    }
  }
} 

void MachineEngine::start( )
{
#if ENTRY_DEBUG
  iprintf("MachineEngine::start( ) entry point\n");
#endif

  unsigned int uiCycleCounter = 0;
  
  if (bInitialized)
  {
    /* clear this flag just in case user requests a start( ) after a stop( ) */
    bStopRequested = 0;  

    bLocalTrain = poMachineParameters->getMachineTraining( );

#if ENTRY_DEBUG
    iprintf("entering infinite loop in MachineEngine::start( )\n");
#endif

    while ( !bStopRequested )
    {
      unsigned char* pmsg;
      BYTE err;

      /* pend on input pattern */
      pmsg = (unsigned char*)OSMboxPend(&InputMbox, 0, &err);
      
#if 1
//#if IO_DEBUG      
      printf("\n\nData received from device driver: \n");
      poVars->parseInputForDisplay((unsigned char*)pmsg);
      poVars->parseOutputForDisplay((unsigned char*)pmsg);
      printf("\n");
#endif
      
      /* parse and store the input pattern*/
      storePattern((unsigned char *)pmsg);

      if ( training( ) )
      { 
        /* training consists of iterate( ) and train( ) cycles */
        train( );

        OSMboxPost(&OutputMbox, (void *)"ACK\0");
      }    
      else
      {
        /* iteration consists solely of iterate( ) cycles */
        iterate( );
        
        /* pend until the iteration is completed */
        pmsg = (unsigned char*)OSMboxPend(&IterateMbox, 0, &err);

#if 0
        /* send the data (i.e., output advice) to output port */
        OSMboxPost(&OutputMbox, (void *)pmsg);
#endif
      }

      /* increment cycle counter */      
      uiCycleCounter++;
      
      /* compare cycle counter with number of cycles per epoch */
      if ((uiCycleCounter % NUMBER_CANNED) == 0)
      {
        /* epoch is complete - provide debug information */
#if 1
//#if IO_DEBUG
        /* print out epoch error after network internals */
        printf("\n\nError this epoch: %f\n\n", poVars->EpochError);
#endif

        /* epoch is complete - check performance */
#if 0
        if ( training( ) )
#endif
        {
          if (poVars->EpochError < EPOCH_ERROR_THRESHOLD)
          {
            stop();
          }
        }
        /* zero out error total for the epoch */
        poVars->EpochError = 0;
      }      

    }  /* end while() */
  }  /* end if( ) */
  else
  {
    /* warn client that machine has not yet been initialized */
  }
}

void MachineEngine::stop( )
{
#if ENTRY_DEBUG
  iprintf("MachineEngine::stop( ) entry point\n");
#endif
  bStopRequested = 1;  
}

void MachineEngine::initialize( )
{
#if ENTRY_DEBUG
  iprintf("MachineEngine::initialize( ) entry point\n");
#endif

  if (!bInitialized)
  {
    uiIterationCount = 0;
    
    poVars = new MachineVariables( );

    if (poVars)
    {
      if (poMachineParameters)
      {
        /* MachineVariable class initialization */

        if ( poMachineParameters->getInputVectorLength( ) )
        {
          poVars->ucInputVectorLength  = 
                    poMachineParameters->getInputVectorLength( );
          poVars->ucOutputVectorLength = 
                    poMachineParameters->getOutputVectorLength( );
          poVars->ucHiddenVectorLength = 
                    ((poVars->ucInputVectorLength * HIDDEN_LENGTH_MULTIPLIER) / 
                                                   HIDDEN_LENGTH_DIVISOR);
        }
        poVars->initialize( );

        initializeRTOS();
  
        bInitialized = 1;
      }
      else
      {
        /* warn that poMachineParameters is invalid */
        iprintf("NULL pointer [ poMachineParameters ] within ");
        iprintf("MachineEngine::initialize( )\n");
      }
    }
    else
    {
      /* warn that poVars is invalid directly after call to constructor */ 
      iprintf("NULL pointer [ poVars ] within ");
      iprintf("MachineEngine::initialize( )\n");
    }
  }
  else
  {
    /* warn that initialize( ) was called on an already initialized machine */
    iprintf("Attempted to initialize an already initialized system within ");
    iprintf("MachineEngine::initialize( )\n");
  }
}

void MachineEngine::display( )
{
#if ENTRY_DEBUG
  iprintf("MachineEngine::display( )\n");
#endif
  if (bInitialized)
  {
#ifdef UCOS_STACK_CHECK  
    iprintf("\nThese are our tasks: \n\n");
    
    OSDumpTCBStacks();
    
    OSDumpTasks();
#endif
    if (poVars)
    {
      poVars->display( );
    }
    else
    {
      /* warn that poVars is invalid */    
      iprintf("NULL pointer [ poVars ] within ");
      iprintf("MachineEngine::display( )\n");
    }
  }
  else
  {
    /* warn that initialize( ) has not yet been called for machine */
    iprintf("Attempted to display an uninitialized system within ");
    iprintf("MachineEngine::display( )\n");
  }
}

void MachineEngine::iterate( )
{
#if ENTRY_DEBUG
  iprintf("MachineEngine::iterate( ) entry point\n");
#endif
  if (bInitialized)
  {
    /* set input unit activation based on test data */
    for (int i = 1; i <= poVars->ucInputVectorLength; i++)
    {
      poVars->oInputLayer[i].Activation = PatternInputElement[i-1];

#if IO_DEBUG
      printf("Input #%i: %f\n", i, poVars->oInputLayer[i].Activation);
#endif    
    }
    
    poVars->iterate( );
    uiIterationCount++;

    for (int i=0; i < poVars->ucOutputVectorLength; i++)
    {
      printf("Output (network) #%i: %f\n", i, 
              poVars->oOutputLayer[i].Activation);
    }
    
    /* cleanup data structures at end of iteration */
    poVars->endOfIteration();

    for (int i=0; i < poVars->ucOutputVectorLength; i++)
    {
      printf("Output (network) #%i: %f\n", i, 
              poVars->oOutputLayer[i].Activation);
    }
    
#if 0    
    OSMboxPost(&IterateMbox, (void *)"IMP\0");
#else
    unsigned long ulTempPattern = 0x00000000;
    unsigned char temp_data[MAXIMUM_BYTES];
    
    for (int i=0; i < MAXIMUM_BYTES; i++)
    {
      temp_data[i] = 0x00;
    }
    
    for (int i=0; i < poVars->ucOutputVectorLength; i++)
    {
      if (poVars->oOutputLayer[i].Activation == 1)
      {
        ulTempPattern = ulTempPattern | (1 << (INPUT_BITS + i));
      }
    }
    
    for (int i=0; i < MAXIMUM_BYTES; i++)
    {
      temp_data[i] = 0xFF & ulTempPattern;
      ulTempPattern = ulTempPattern >> 8;
    }
    
    /* notify the task scheduler that we are done with the data structures */
    OSMboxPost(&IterateMbox, (void*)temp_data);
    
    /* send the data (i.e., output advice) to output port */
    OSMboxPost(&OutputMbox, (void *)temp_data);
#endif

#if 0
    unsigned long ulOutputPattern = 0x00000000;

    /* transform network output to binary format for device driver */
    for (int i=0; i < poVars->ucOutputVectorLength; i++)
    {
      if (poVars->oOutputLayer[i].Activation > UNIT_ACTIVATION_THRESHOLD)
      {
        ulOutputPattern = ulOutputPattern | (unsigned long)(1 << (1*i));
      }  
    }

    sprintf(buffer, "%x\n", ulOutputPattern, MAXIMUM_BYTES);
#endif
    
#if IO_DEBUG
    printf("\nIteration cycle #%ld\n", uiIterationCount);
    display();
#endif
  }
}

void MachineEngine::train( )
{
#if ENTRY_DEBUG
  iprintf("MachineEngine::train( ) entry point\n");
#endif

  if (bInitialized)
  {
    /* set input unit activation based on test data */
    for (int i = 1; i <= poVars->ucInputVectorLength; i++)
    {
      poVars->oInputLayer[i].Activation = PatternInputElement[i-1];

#if IO_DEBUG
      printf("Input (network) #%i: %f\n", i, poVars->oInputLayer[i].Activation);
#endif    
    }
    
    poVars->iterate( );
    uiIterationCount++;

    /* set output unit error based on difference */
    /* between target and actual output values   */      
    
    /* the only reason we are doing this here */
    /* (and not in MachineVariables class)    */
    /* is that this class is currently the    */
    /* only place we store the train pattern  */
    /* target outputs;  eventually, this      */
    /* may best be relayed to the Machine     */
    /* Variables class directly, and this     */
    /* computation can be pushed to the       */
    /* MachineVariables class as well.        */
    for (int i = 0; i < poVars->ucOutputVectorLength; i++)
    {
      double localUnitError = PatternTargetElement[i] - 
                              poVars->oOutputLayer[i].Activation;

//#if IO_DEBUG
#if 1
      printf("Output (network) #%i: %f\n", i, 
              poVars->oOutputLayer[i].Activation);
#endif                              

      poVars->oOutputLayer[i].Error = localUnitError;
      poVars->EpochError           += fabs(localUnitError);
    }

    poVars->train( );
  }
}


void MachineEngine::storePattern(unsigned char* DataVector)
{
/* AT THIS POINT, WE MUST TRANSFORM THE 
                                     10-byte representation of TrainVector 

   TO THE                                        
                                     40-bit representation required by the 
                                     PatternInputElement data structure 
   AND THE                                      
                                     14-bit representation required by the 
                                     PatternTargetElement data structure */

  unsigned int uiSourceIndex = 0;
  unsigned int uiDestinationIndex = 0;

  unsigned long ulPattern = 0x00000000;
  
  for (int i=0; i < MAXIMUM_BYTES; i++)
  {
    ulPattern = (unsigned long)ulPattern | (DataVector[uiSourceIndex] << (8*i));

#if IO_DEBUG
    printf("Input Element #%i:\n", i);
    /* post the target (training) vector to the debug port */
    printf("  SOURCE data: 0x%x\n", (unsigned int)DataVector[uiSourceIndex]);
#endif

    uiSourceIndex++;
  }


  for (int i=0; i < (poVars->ucInputVectorLength - 1); i++)
  {
    unsigned long ulTempElement = 0x00000000;
    ulTempElement = ulTempElement | 
                     (unsigned long)(1 << (1 * uiDestinationIndex));

    if (ulPattern & ulTempElement)
    {
      PatternInputElement[i] = 1;
    }
    else
    {
      PatternInputElement[i] = 0;
    }

#if IO_DEBUG
    /* post the input (training) vector to the debug port */
    printf("Input (canned)  #%i: 0x%x\n", i, PatternInputElement[i]);
#endif                                       

    uiDestinationIndex++;
  }

  for (int i=0; i < poVars->ucOutputVectorLength; i++)
  {
    unsigned long ulTempElement = 0x00000000;
    ulTempElement = ulTempElement |  
                     (unsigned long)(1 << (1 * uiDestinationIndex));
    if (ulPattern & ulTempElement)
    {
      PatternTargetElement[i] = 1;
    }
    else
    {
      PatternTargetElement[i] = 0;
    }
#if 1
//#if IO_DEBUG
    /* post the target (training) vector to the debug port */
    printf("Output (canned)  #%i: 0x%x\n", i, PatternTargetElement[i]);
#endif

    uiDestinationIndex++;
  }
}

bool MachineEngine::training( )
{
  return bLocalTrain;
}
 
void MachineEngine::initializeRTOS( )
{
#if ENTRY_DEBUG
  iprintf("MachineEngine::initializeRTOS( ) entry point\n");
#endif

  SerialClose( 1 );
  DeviceDriver = OpenSerial( 1, 115200, 2, 8, eParityNone );
  write( DeviceDriver, "Hello Device Driver\0", 20 );

  /* Initialize Input, Iterate, & Train mailboxes */
  OSMboxInit(&InputMbox,   NULL);
  OSMboxInit(&IterateMbox, NULL);
  OSMboxInit(&OutputMbox,  NULL);

  if (OSTaskCreate (	InputOutputTask,
  			NULL,
			(void *) &InputOutputTaskStack[USER_TASK_STK_SIZE],
			(void *) InputOutputTaskStack,
			INPUT_OUTPUT_PRIORITY) != OS_NO_ERR )
  {
    /* warn of error creating task */
    iprintf("Error initializing InputOutputTask RTOS data structure\n");
  }
}

/*
 *****************************************************************************
 *    
 *    This is the division line between MachineEngine proper & the RTOS / HW
 *    
 *****************************************************************************
 */


/*------------------------------------------------------------------------

  Functions for Task Mailbox interactions with main task.

 ------------------------------------------------------------------------*/
void InputOutputTask(void *pdata)
{
#if USE_CANNED_DATA
  int count = 0;
#endif

  while (1)
  {
    OSTimeDly(1); // Delay for one tick
#if COMMUNICATE_WITH_VI
    //Set up a file set so we can select on the serial ports...
    fd_set read_fds;
    FD_ZERO( &read_fds );
    FD_SET( DeviceDriver, &read_fds );
    if ( select( FD_SETSIZE,
                 &read_fds,
                 ( fd_set * ) 0,
                 ( fd_set * ) 0,
                 TICKS_PER_SECOND * 10 ) )
    {
       if ( FD_ISSET( DeviceDriver, &read_fds ) )
       {
          char ack[2]     = {0xCC};
          char buffer[MAXIMUM_BYTES];
          
          for (int i=0; i < MAXIMUM_BYTES; i++)
          {
            buffer[i] = 0x00;
          } 

          int n = read( DeviceDriver, buffer, MAXIMUM_BYTES );
          
#if 1
          /* echo data back to device driver */
          write( DeviceDriver, buffer, n );
#else
          /* write ack byte(s) to device driver */
          write( DeviceDriver, ack, 2 );
#endif

#if IO_DEBUG
          /* post the input (i.e., training) vector to the debug port */
          printf("Input vector: 0x");
          
          for (int i=0; i < MAXIMUM_BYTES; i++)
          {
            printf("%2x", buffer[i]);
          } 

          printf("\n");
#endif
          
          /* post the input (i.e., training) vector for processing */
          OSMboxPost(&InputMbox, (void *)buffer);
       }
    }
    else
    {
      // we timed out... nothing to send
    }
#elif USE_CANNED_DATA
  unsigned long ulTempPattern = 0x00000000;
  unsigned char buffer[MAXIMUM_BYTES];

#if IO_DEBUG
  /* we want two canned training patterns per second - */
  /*   this rate is quick but readable.                */
  OSTimeDly(TICKS_PER_SECOND); 
#endif
  
  for (int i=0; i < MAXIMUM_BYTES; i++)
  {
    buffer[i] = 0x00;
  } 

  for (int i=0; i < MAXIMUM_ON_BITS; i++)
  {
    /* each canned train vector includes just a single bit */
    ulTempPattern = ulTempPattern | CannedVectors[count][i]; 
  }

  for (int i=0; i < MAXIMUM_BYTES; i++)
  {
    /* each buffer element is one byte of the entire pattern */
    buffer[i] = 0xFF & ulTempPattern;
    ulTempPattern = ulTempPattern >> 8;
  }

#if USE_CANNED_DATA
  /* increment counter to step through training patterns */
  count++;
  if (count >= NUMBER_CANNED)
  {
    /* go back to the zero element of our stored training vectors */
    count = 0;
  }
#endif

#if IO_DEBUG
  /* post the input (i.e., training) vector to the debug port */
  printf("Input vector: 0x");
          
  for (int i=0; i < MAXIMUM_BYTES; i++)
  {
    printf("%2x", buffer[i]);
  } 

  printf("\n");
#endif
    
  /* post the input (i.e., training) vector for processing */
  OSMboxPost(&InputMbox, (void *)buffer);

  void* pmsg;
  BYTE err;
      
  /* pend on input pattern */
  pmsg = OSMboxPend(&OutputMbox, 0, &err);

  write( DeviceDriver, (char *)pmsg, MAXIMUM_BYTES);
#endif
  }
}


