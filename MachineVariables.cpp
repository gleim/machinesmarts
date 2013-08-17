/***************************************************
 *
 *  MachineVariables.cpp
 *
 *  MachineVariables class - 
 *		support of primary class of the system,
 *		providing the variables necessary for
 *	  	majority of the processing
 *		within the STE layer.
 *
 **************************************************/
#include "MachineVariables.h"

/* declare debug flags */
#define VIEW_INTERNALS     0
#define VIEW_IO_AND_ERROR  0
#define VIEW_ERROR_ONLY    1
#define VIEW_ADDRESSES     0
#define ENTRY_DEBUG        0
#define MATH_DEBUG         0

/* recurrent network flags */
#define USING_RECURRENT_LAYER  0

/* define min & max weight values */
#define MIN_WEIGHT_VALUE  -10.0
#define MAX_WEIGHT_VALUE   10.0

MachineVariables::MachineVariables( )
{
  ucInputVectorLength  = 0;
  ucHiddenVectorLength = 0;
  ucOutputVectorLength = 0;
  EpochError           = 0; 
}

MachineVariables::~MachineVariables( )
{ 
  cleanup( );
  
  ucInputVectorLength  = 0;
  ucHiddenVectorLength = 0;
  ucOutputVectorLength = 0;
  EpochError           = 0;
}

double MachineVariables::provideRandomUnitValue( )
{
  double random_value;

  /* RNG * 10^-10 results in small positive pseudo-random value */
  random_value = rand() *.000000001;
    
  /* 50% chance of changing sign of value */
  if (rand() % 2)
  {
    random_value = random_value * -1;
  }

  /* want random weight values between 1.0 and -1.0 */
  random_value = (0.0 + random_value) / 2;

  return random_value;  
}

void MachineVariables::initialize( )
{
  iprintf("MachineVariables::initialize( ) entry point\n");
  
  int i, j, seed = 0;
  BackpropagationNode *poTempBackpropagationNode;

  /* initialize (or seed) random number generator */
  seed = rand();
  srand(seed);
  
  /* ALLOCATION of memory for BackpropagationNode objects */
  for (i = 0; i <= ucInputVectorLength; i++)
  {
    poTempBackpropagationNode = new BackpropagationNode( );
    oInputLayer[i] = *poTempBackpropagationNode;
  }
  
  for (i = 0; i <= ucHiddenVectorLength; i++)
  {
    poTempBackpropagationNode = new BackpropagationNode( );
    oHiddenLayer[i] = *poTempBackpropagationNode;
  }
  
  for (i = 0; i <= ucHiddenVectorLength; i++)
  {
    poTempBackpropagationNode = new BackpropagationNode( );
    oContextLayer[i] = *poTempBackpropagationNode;
  }
  
  for (i = 0; i < ucOutputVectorLength; i++)
  {
    poTempBackpropagationNode = new BackpropagationNode( );
    oOutputLayer[i] = *poTempBackpropagationNode;
  }
  
  /* Bias Node of Input Layer */
  oInputLayer[0].Net = 0;
  oInputLayer[0].Activation = 1.0;
  oInputLayer[0].Error = 0;

  for (j = 0; j <= ucHiddenVectorLength; j++)
  {
    /* assign pseudo-random value to weight vector data element */
    oInputLayer[0].Wts[j] = provideRandomUnitValue( );
    oInputLayer[0].WED[j] = 0.0;
    oInputLayer[0].DeltaWts[j] = 0.0;
  }
  
  /* Input Layer */
  
  for (i = 1; i <= ucInputVectorLength; i++)
  {
    oInputLayer[i].Net = 0;
    oInputLayer[i].Activation = 0;
    oInputLayer[i].Error = 0;
    for (j = 0; j <= ucHiddenVectorLength; j++)
    {
      /* assign pseudo-random value to weight vector data element */
      oInputLayer[i].Wts[j] = provideRandomUnitValue( );
      oInputLayer[i].WED[j] = 0.0;
      oInputLayer[i].DeltaWts[j] = 0.0;
    }
  }
  
  /* Bias Node of Hidden Layer */
  oHiddenLayer[0].Net = 0;
  oHiddenLayer[0].Activation = 1.0;
  oHiddenLayer[0].Error = 0;

  for (j = 0; j < ucOutputVectorLength; j++)
  {
    /* assign pseudo-random value to weight vector data element */
    oHiddenLayer[0].Wts[j] = provideRandomUnitValue( );
    oHiddenLayer[0].WED[j] = 0.0;
    oHiddenLayer[0].DeltaWts[j] = 0.0;
  }

  /* Hidden Layer */
  
  for (i = 1; i <= ucHiddenVectorLength; i++)
  {
    oHiddenLayer[i].Net = 0;
    oHiddenLayer[i].Activation = 0;
    oHiddenLayer[i].Error = 0; 
    for (j = 0; j < ucOutputVectorLength; j++)
    {
      /* assign pseudo-random value to weight vector data element */
      oHiddenLayer[i].Wts[j] = provideRandomUnitValue( );
      oHiddenLayer[i].WED[j] = 0.0;
      oHiddenLayer[i].DeltaWts[j] = 0.0;
    }
  }

#if USING_RECURRENT_LAYER
  /* Context Layer */
  for (i = 0; i <= ucHiddenVectorLength; i++)
  {
    oContextLayer[i].Net = 0;
    oContextLayer[i].Activation   = 0;
    oContextLayer[i].Error = 0;
    for (j = 0; j <= ucHiddenVectorLength; j++)
    {
      /* assign pseudo-random value to weight vector data element */
      oContextLayer[i].Wts[j] = provideRandomUnitValue( );
      oContextLayer[i].WED[j] = 0.0;
      oContextLayer[i].DeltaWts[j] = 0.0;
    }
  }
#endif
  
  /* error vectors (for communication between iterate and training threads) */
  for (i = 0; i < ucOutputVectorLength; i++)
  {
    oOutputLayer[i].Net = 0;
    oOutputLayer[i].Activation = 0;
    oOutputLayer[i].Error = 0;
  }  
}

double MachineVariables::perturbWeight(double weightValue)
{
#if 1
  double resultingWeightValue, randomValue;

  /* RNG * 10^-10 results in small positive pseudo-random value */
  randomValue = rand() *.00000000001;
    
  /* 50% chance of changing sign of value */
  if (rand() % 2)
  {
    randomValue = randomValue * -1;
  }

  /* set initial weights to maximal point of change (i.e., 0.5) */
  resultingWeightValue = weightValue + randomValue;

  return resultingWeightValue;  
#else
  return weightValue;
#endif
}

double MachineVariables::checkWeightBoundary(double weightValue)
{
  double resultingWeightValue;
  
  if (weightValue > MAX_WEIGHT_VALUE)
  {
    resultingWeightValue = MAX_WEIGHT_VALUE;
  }
  else if (weightValue < MIN_WEIGHT_VALUE)
  {
    resultingWeightValue = MIN_WEIGHT_VALUE;
  }
  else
  {
    resultingWeightValue = weightValue;
  }
  return resultingWeightValue;
}

void MachineVariables::iterate( )
{
  /* set hidden unit activation based on input & recurrent layer activations */      
  for (int i = 1; i <= ucHiddenVectorLength; i++)
  {
    /* cumulative sum of input (in this case formal input) unit activations */
    /* multiplied by weights creates hidden unit net                        */
    for (int j = 0; j <= ucInputVectorLength; j++)
    {
      oHiddenLayer[i].Net += oInputLayer[j].Activation *
                                     oInputLayer[j].Wts[i];
    }
#if USING_RECURRENT_LAYER
    /* cumulative sum of input (in this case recurrent) unit activations */
    /* multiplied by weights creates hidden unit net                     */
    for (int j = 0; j <= ucHiddenVectorLength; j++)
    {
      oHiddenLayer[i].Net += oContextLayer[j].Activation *
                                     oContextLayer[j].Wts[i];
    }
#endif
    oHiddenLayer[i].Activation = 
        1 / ( 1 + exp(-1 * oHiddenLayer[i].Net) );
  }  

#if USING_RECURRENT_LAYER
  /* set context layer activation at time (t + 1) to be hidden layer */
  /* activation at time (t)                                          */
  for (int i = 1; i <= ucHiddenVectorLength; i++)
  {
    oContextLayer[i].Activation = oHiddenLayer[i].Activation;
  }
#endif  
  /* set output unit activation based on hidden activations & weight vectors */      
  for (int i = 0; i < ucOutputVectorLength; i++)
  {
    /* cumulative addition of hidden unit activations */
    /* multiplied by weights creates output unit net  */
    for (int j = 0; j <= ucHiddenVectorLength; j++)
    {
      oOutputLayer[i].Net += oHiddenLayer[j].Activation *
                                     oHiddenLayer[j].Wts[i];
    }
    oOutputLayer[i].Activation = 
        1 / ( 1 + exp(-1 * oOutputLayer[i].Net) );
#if MATH_DEBUG
    printf("NET: %f\n", oOutputLayer[i].Net);
    printf("\n1 / ( 1 + exp(-1 * oOutputLayer[i].Net) ): %f\n\n",
           ( 1 / ( 1 + exp(-1 * oOutputLayer[i].Net) ) ) );
#endif
  }  
}

void MachineVariables::train( )
{
    /* first thing we do in backpropagation is set delta for each unit */

    /* Delta is equal to the error for the unit */
    /*     multiplied by the derivative         */ 
    /*     of the activation function           */
    
    /* set output layer delta based on output layer error*/
    for (int j = 0; j < ucOutputVectorLength; j++)
    {
      /* the derivative of the activation function is the activation */
      /*     of the unit multiplied by (one minus its activation)    */
      oOutputLayer[j].Delta = oOutputLayer[j].Error *
                                      oOutputLayer[j].Activation *
                                      (1 - oOutputLayer[j].Activation);
    }

    /* determine hidden layer error based on output layer delta */
    for (int i = 0; i <= ucHiddenVectorLength; i++)
    {
      for (int j = 0; j < ucOutputVectorLength; j++)
      {
        oHiddenLayer[i].Error += oOutputLayer[j].Delta *
                                         oHiddenLayer[i].Wts[j];
      }
    }
    
    /* set hidden layer delta based on hidden layer error */
    for (int j = 0; j <= ucHiddenVectorLength; j++)
    {
      /* the derivative of the activation function is the activation */
      /*     of the unit multiplied by (one minus its activation)    */
      oHiddenLayer[j].Delta = oHiddenLayer[j].Error *
                                      oHiddenLayer[j].Activation *
                                      (1 - oHiddenLayer[j].Activation);
    }

    /* determine weight error derivatives for hidden to output layer weights */    
    for (int i = 0; i <= ucHiddenVectorLength; i++)
    {
      for (int j = 0; j < ucOutputVectorLength; j++)
      {
        oHiddenLayer[i].WED[j] += oOutputLayer[j].Delta *
                                          oHiddenLayer[i].Activation;
      }
    }
    
    /* determine weight error derivatives for input to hidden layer weights */    
    for (int i = 0; i <= ucInputVectorLength; i++)
    {
      for (int j = 0; j <= ucHiddenVectorLength; j++)
      {
        oInputLayer[i].WED[j] += oHiddenLayer[j].Delta *
                                         oInputLayer[i].Activation;
      }
    }

#if USING_RECURRENT_LAYER
    /* determine weight error derivatives for context to hidden layer weights */    
    for (int i = 1; i <= ucHiddenVectorLength; i++)
    {
      for (int j = 1; j <= ucHiddenVectorLength; j++)
      {
        oContextLayer[i].WED[j] += oHiddenLayer[j].Delta *
                                           oContextLayer[i].Activation;
      }
    }
#endif
    
    /* use weight error derivatives to determine delta weights */
    /*     for hidden to output layer weights                  */
    for (int i = 0; i <= ucHiddenVectorLength; i++)
    {
      for (int j = 0; j < ucOutputVectorLength; j++)
      {
        double oldDeltaWeight = oHiddenLayer[i].DeltaWts[j];
        
        oHiddenLayer[i].DeltaWts[j] = 
            LearningRate * oHiddenLayer[i].WED[j] +
            Momentum     * oldDeltaWeight;
      }
    }
    
    /* use weight error derivatives to determine delta weights */
    /*     for input to hidden layer weights                   */
    for (int i = 0; i <= ucInputVectorLength; i++)
    {
      for (int j = 0; j <= ucHiddenVectorLength; j++)
      {
        double oldDeltaWeight = oInputLayer[i].DeltaWts[j];
        
        oInputLayer[i].DeltaWts[j] = 
            LearningRate * oInputLayer[i].WED[j] +
            Momentum     * oldDeltaWeight;
      }
    }
    
#if USING_RECURRENT_LAYER
    /* use weight error derivatives to determine delta weights */
    /*     for context to hidden layer weights                   */
    for (int i = 1; i <= ucHiddenVectorLength; i++)
    {
      for (int j = 1; j <= ucHiddenVectorLength; j++)
      {
        double oldDeltaWeight = oContextLayer[i].DeltaWts[j];
        
        oContextLayer[i].DeltaWts[j] = 
            LearningRate * oContextLayer[i].WED[j] +
            Momentum     * oldDeltaWeight;
      }
    }
#endif

    /* use delta weights to set new weight values */
    /*     for hidden to output layer weights     */
    for (int i = 0; i <= ucHiddenVectorLength; i++)
    {
      for (int j = 0; j < ucOutputVectorLength; j++)
      {
        oHiddenLayer[i].Wts[j] += oHiddenLayer[i].DeltaWts[j];
        oHiddenLayer[i].Wts[j] = checkWeightBoundary(oHiddenLayer[i].Wts[j]);
      }
    }
    
    /* use delta weights to set new weight values */
    /*     for input to hidden layer weights      */
    for (int i = 0; i <= ucInputVectorLength; i++)
    {
      for (int j = 0; j <= ucHiddenVectorLength; j++)
      {
        oInputLayer[i].Wts[j] += oInputLayer[i].DeltaWts[j];
        oInputLayer[i].Wts[j] = checkWeightBoundary(oInputLayer[i].Wts[j]);
      }
    }
    
#if USING_RECURRENT_LAYER
    /* use delta weights to set new weight values */
    /*     for context to hidden layer weights      */
    for (int i = 1; i <= ucHiddenVectorLength; i++)
    {
      for (int j = 1; j <= ucHiddenVectorLength; j++)
      {
        oContextLayer[i].Wts[j] += oContextLayer[i].DeltaWts[j];
        oContextLayer[i].Wts[j] = checkWeightBoundary(oContextLayer[i].Wts[j]);
      }
    }
#endif

    for (int i = 0; i < ucOutputVectorLength; i++)
    {
      /* reset net for output layer weights    */
      oOutputLayer[i].Net = 0.0;
    }

    for (int i = 0; i <= ucHiddenVectorLength; i++)
    {
      /* reset net for hidden layer weights    */
      /* reset error for hidden layer weights  */
      oHiddenLayer[i].Net   = 0.0;
      oHiddenLayer[i].Error = 0.0;

      for (int j = 0; j < ucOutputVectorLength; j++)
      {
        /* reset weight error derivative values */
        /*   for hidden to output layer weights */
        oHiddenLayer[i].WED[j] = 0.0;
        
        /* perturb weight                       */
        /*   for hidden to output layer weights */
        oHiddenLayer[i].Wts[j] = perturbWeight(oHiddenLayer[i].Wts[j]);
      }
    }

    for (int i = 0; i <= ucInputVectorLength; i++)
    {
      for (int j = 0; j <= ucHiddenVectorLength; j++)
      {
        /* reset weight error derivative values */
        /*   for input to hidden layer weights  */
        oInputLayer[i].WED[j] = 0.0;
        
        /* perturb weight                       */
        /*   for hidden to output layer weights */
        oInputLayer[i].Wts[j] = perturbWeight(oInputLayer[i].Wts[j]);
      }
    }
#if USING_RECURRENT_LAYER
    for (int i = 0; i <= ucHiddenVectorLength; i++)
    {
      /* reset error for hidden layer weights  */
      oContextLayer[i].Net   = 0.0;
      oContextLayer[i].Error = 0.0;

      for (int j = 0; j <= ucHiddenVectorLength; j++)
      {
        /* reset weight error derivative values */
        /*   for context to hidden layer weights  */
        oContextLayer[i].WED[j] = 0.0;
        
        /* perturb weight                       */
        /*   for hidden to output layer weights */
        oContextLayer[i].Wts[j] = perturbWeight(oContextLayer[i].Wts[j]);
      }
    }
#endif
}

void MachineVariables::endOfIteration( )
{
/* this routine conducts cleanup typically done in train( )    */
/* that was not being done when iterate( ) was used on its own */
    for (int i = 0; i < ucOutputVectorLength; i++)
    {
      /* reset net for output layer weights    */
      oOutputLayer[i].Net = 0.0;
    }

    for (int i = 0; i <= ucHiddenVectorLength; i++)
    {
      /* reset net for hidden layer weights    */
      /* reset error for hidden layer weights  */
      oHiddenLayer[i].Net   = 0.0;
      oHiddenLayer[i].Error = 0.0;

      for (int j = 0; j < ucOutputVectorLength; j++)
      {
        /* reset weight error derivative values */
        /*   for hidden to output layer weights */
        oHiddenLayer[i].WED[j] = 0.0;
        
        /* perturb weight                       */
        /*   for hidden to output layer weights */
        oHiddenLayer[i].Wts[j] = perturbWeight(oHiddenLayer[i].Wts[j]);
      }
    }

    for (int i = 0; i <= ucInputVectorLength; i++)
    {
      for (int j = 0; j <= ucHiddenVectorLength; j++)
      {
        /* reset weight error derivative values */
        /*   for input to hidden layer weights  */
        oInputLayer[i].WED[j] = 0.0;
        
        /* perturb weight                       */
        /*   for hidden to output layer weights */
        oInputLayer[i].Wts[j] = perturbWeight(oInputLayer[i].Wts[j]);
      }
    }
    
    /* Output State bitmap specification 
    Output 0: OUTPUT_VELOCITY_BACK  
    Output 1: OUTPUT_VELOCITY_STOP  
    Output 2: OUTPUT_VELOCITY_FWD1  
    Output 3: OUTPUT_VELOCITY_FWD2 
    Output 4: OUTPUT_STEER_LEFT     
    Output 5: OUTPUT_STEER_STRAIGHT  
    Output 6: OUTPUT_STEER_RIGHT     
    */

    double velocity_outcome = 0;
    for (int i = 0; i < 4; i++)
    {
      if (oOutputLayer[i].Activation > velocity_outcome)
      {
        velocity_outcome = oOutputLayer[i].Activation;
      }
    }
  
    for (int i = 0; i < 4; i++)
    {
      if (oOutputLayer[i].Activation == velocity_outcome)
      {
        oOutputLayer[i].Activation = 1;
      }
    }
  
    double direction_outcome = 0;
    for (int i = 4; i < 7; i++)
    {
      if (oOutputLayer[i].Activation > direction_outcome)
      {
        direction_outcome = oOutputLayer[i].Activation;
      }
    }
  
    for (int i = 4; i < 7; i++)
    {
      if (oOutputLayer[i].Activation == direction_outcome)
      {
        oOutputLayer[i].Activation = 1;
      }
    }
  
}
void MachineVariables::cleanup( )
{
  iprintf("MachineVariables::cleanup( ) entry point\n");
  
  int i;
  BackpropagationNode *poTempBackpropagationNode = NULL;

  /* DEALLOCATION of memory for BackpropagationNode objects */
  for (i = 0; i <= ucInputVectorLength; i++)
  {
    *poTempBackpropagationNode = oInputLayer[i];
    delete poTempBackpropagationNode;
  }
  
  for (i = 0; i <= ucHiddenVectorLength; i++)
  {
    *poTempBackpropagationNode = oHiddenLayer[i];
    delete poTempBackpropagationNode;
  }
  
  for (i = 0; i <= ucHiddenVectorLength; i++)
  {
    *poTempBackpropagationNode = oContextLayer[i];
    delete poTempBackpropagationNode;
  }
  
  for (i = 0; i < ucOutputVectorLength; i++)
  {
    *poTempBackpropagationNode = oOutputLayer[i];
    delete poTempBackpropagationNode;
  }
}
  
void MachineVariables::display( )
{
#if ENTRY_DEBUG
  iprintf("MachineVariables::display( )\n");
#endif

#if VIEW_INTERNALS
  int i, j;

  /* Bias Node of Input Layer */
#if VIEW_ADDRESSES
  printf("\nInput layer bias node address:  0x%x\n", &oInputLayer[0]);
#endif
  printf("Input layer bias node NET:  %f\n", oInputLayer[0].Net);
  printf("Input layer bias node ACTIVATION:  %f\n",   
             oInputLayer[0].Activation);
  printf("Input layer bias node ERROR:  %f\n",   
             oInputLayer[0].Error);

  for (j = 1; j <= ucHiddenVectorLength; j++)
  {
    /* weight to hidden layer */
    printf("Weight from bias to hidden layer node  %i\n%f\n\n", j,
               oInputLayer[0].Wts[j]);
  }

  /* Input Layer */

  for (i = 1; i <= ucInputVectorLength; i++)
  {
#if VIEW_ADDRESSES
    printf("\nInput layer node %i address:  0x%x\n", i, &oInputLayer[i]);
#endif
    printf("\nInput layer node %i NET:  %f\n", 
               i, oInputLayer[i].Net);
    printf("Input layer node %i ACTIVATION:  %f\n", 
               i, oInputLayer[i].Activation);
    printf("Input layer node %i ERROR:  %f\n", 
               i, oInputLayer[i].Error);

    for (j = 0; j < ucHiddenVectorLength; j++)
    {
      /* weights to hidden layer */
      iprintf("Weight from input layer node %i to hidden layer node  ", i);
      printf("%i\n%f\n\n", j, oInputLayer[i].Wts[j]);
    }
  }

  /* Bias Node of Hidden Layer */
#if VIEW_ADDRESSES
  printf("\nHidden layer bias node address:  0x%x\n", &oHiddenLayer[0]);
#endif
  printf("Hidden layer bias node NET:  %f\n", oHiddenLayer[0].Net);
  printf("Hidden layer bias node ACTIVATION:  %f\n",   
             oHiddenLayer[0].Activation);
  printf("Hidden layer bias node ERROR:  %f\n",   
             oHiddenLayer[0].Error);

  for (j = 0; j < ucOutputVectorLength; j++)
  {
    /* weight to hidden layer */
    printf("Weight from bias to output layer node  %i\n%f\n\n", j,
               oHiddenLayer[0].Wts[j]);
  }

  /* Hidden Layer */

  for (i = 1; i <= ucHiddenVectorLength; i++)
  {
#if VIEW_ADDRESSES
    printf("\nHidden layer node %i address:  0x%x\n", i, &oHiddenLayer[i]);
#endif
    printf("\nHidden layer node %i NET:  %f\n", 
               i, oHiddenLayer[i].Net);
    printf("Hidden layer node %i ACTIVATION:  %f\n", 
               i, oHiddenLayer[i].Activation);
    printf("Hidden layer node %i ERROR:  %f\n", 
               i, oHiddenLayer[i].Error);

    for (j = 0; j < ucOutputVectorLength; j++)
    {
      /* weights to output layer */
      iprintf("Weight from hidden layer node %i to output layer node  ", i);
      printf("%i\n%f\n\n", j, oHiddenLayer[i].Wts[j]);
    }
  }

  /* Context Layer */

#if USING_RECURRENT_LAYER
  for (i = 0; i <= ucHiddenVectorLength; i++)
  {
#if VIEW_ADDRESSES
    printf("\nContext layer node %i address:  0x%x\n", i, &oContextLayer[i]);
#endif
    printf("\nContext layer node %i NET:  %f\n", 
               i, oContextLayer[i].Net);
    printf("Context layer node %i ACTIVATION:  %f\n", 
               i, oContextLayer[i].Activation);
    printf("Context layer node %i ERROR:  %f\n", 
               i, oContextLayer[i].Error);

    for (j = 1; j <= ucHiddenVectorLength; j++)
    {
      /* weights to hidden layer */
      iprintf("Weight from context layer node %i to hidden layer node  ", i);
      printf("%i\n%f\n\n", j, oContextLayer[i].Wts[j]);
    }
  }
#endif
  
  /* Output Layer */
  
  for (i = 0; i < ucOutputVectorLength; i++)
  {
#if VIEW_ADDRESSES
    printf("\nOutput layer node %i address:  0x%x\n", i, &oOutputLayer[i]);
#endif
    printf("Output layer node %i NET:  %f\n", i, oOutputLayer[i].Net);
    printf("Output layer node %i ACTIVATION:  %f\n", 
               i, oOutputLayer[i].Activation);
    printf("Output layer node %i ERROR:  %f\n", 
               i, oOutputLayer[i].Error);
  }
  
  iprintf("\n\n");

#elif VIEW_IO_AND_ERROR
  int i;
  
  for (i = 1; i <= ucInputVectorLength; i++)
  {
    printf("Input #%i:  %f\n", i, oInputLayer[i].Activation);
  }

  for (i = 0; i < ucOutputVectorLength; i++)
  {
    printf("Output #%i:  %f\n", i, oOutputLayer[i].Activation);
    printf("Output #%i ERROR:  %f\n", i, oOutputLayer[i].Error);
  }
  iprintf("\n\n");
#elif VIEW_ERROR_ONLY
  for (int i = 0; i < ucOutputVectorLength; i++)
  {
    printf("Output ERROR:  %f\n", oOutputLayer[i].Error);
  }
#endif
}

void MachineVariables::parseOutputForDisplay(unsigned char* msg)
{
  unsigned long ulTempPattern = 0x00000000;

  for (int i=0; i < MAXIMUM_BYTES; i++)
  {
    ulTempPattern = ulTempPattern | (msg[i] << (8*i));
  }

  if (ulTempPattern & OUTPUT_VELOCITY_BACK)
  {
    printf("Output state:  OUTPUT_VELOCITY_BACK state received\n");
  }

  if (ulTempPattern & OUTPUT_VELOCITY_STOP)
  {
    printf("Output state:  OUTPUT_VELOCITY_STOP state received\n");
  }

  if (ulTempPattern & OUTPUT_VELOCITY_FWD1)
  {
    printf("Output state:  OUTPUT_VELOCITY_FWD1 state received\n");
  }

  if (ulTempPattern & OUTPUT_VELOCITY_FWD2)
  {
    printf("Output state:  OUTPUT_VELOCITY_FWD2 state received\n");
  }

  if (ulTempPattern & OUTPUT_STEER_LEFT)
  {
    printf("Output state:  OUTPUT_STEER_LEFT state received\n");
  }

  if (ulTempPattern & OUTPUT_STEER_STRAIGHT)
  {
    printf("Output state:  OUTPUT_STEER_STRAIGHT state received\n");
  }

  if (ulTempPattern & OUTPUT_STEER_RIGHT)
  {
    printf("Output state:  OUTPUT_STEER_RIGHT state received\n");
  }
}


void MachineVariables::parseInputForDisplay(unsigned char* msg)
{
  unsigned long ulTempPattern = 0x00000000;
  
  for (int i=0; i < MAXIMUM_BYTES; i++)
  {
    ulTempPattern = ulTempPattern | (msg[i] << (8*i));
  }

  printf("\n");

  if (ulTempPattern & INPUT_VELOCITY_BACK)
  {
    printf("Input state:  INPUT_VELOCITY_BACK state received\n");
  }

  if (ulTempPattern & INPUT_VELOCITY_STOP)
  {
    printf("Input state:  INPUT_VELOCITY_STOP state received\n");
  }

  if (ulTempPattern & INPUT_VELOCITY_FWD1)
  {
    printf("Input state:  INPUT_VELOCITY_FWD1 state received\n");
  }

  if (ulTempPattern & INPUT_VELOCITY_FWD2)
  {
    printf("Input state:  INPUT_VELOCITY_FWD2 state received\n");
  }

  if (ulTempPattern & INPUT_GOAL_HEADING_N)
  {
    printf("Input state:  INPUT_GOAL_HEADING_N state received\n");
  }

  if (ulTempPattern & INPUT_GOAL_HEADING_E)
  {
    printf("Input state:  INPUT_GOAL_HEADING_E state received\n");
  }

  if (ulTempPattern & INPUT_GOAL_HEADING_S)
  {
    printf("Input state:  INPUT_GOAL_HEADING_S state received\n");
  }

  if (ulTempPattern & INPUT_GOAL_HEADING_W)
  {
    printf("Input state:  INPUT_GOAL_HEADING_W state received\n");
  }

  if (ulTempPattern & INPUT_HEADING_N)
  {
    printf("Input state:  INPUT_HEADING_N state received\n");
  }

  if (ulTempPattern & INPUT_HEADING_NE)
  {
    printf("Input state:  INPUT_HEADING_NE state received\n");
  }

  if (ulTempPattern & INPUT_HEADING_E)
  {
    printf("Input state:  INPUT_HEADING_E state received\n");
  }

  if (ulTempPattern & INPUT_HEADING_SE)
  {
    printf("Input state:  INPUT_HEADING_SE state received\n");
  }

  if (ulTempPattern & INPUT_HEADING_S)
  {
    printf("Input state:  INPUT_HEADING_S state received\n");
  }

  if (ulTempPattern & INPUT_HEADING_SW)
  {
    printf("Input state:  INPUT_HEADING_SW state received\n");
  }

  if (ulTempPattern & INPUT_HEADING_W)
  {
    printf("Input state:  INPUT_HEADING_W state received\n");
  }

  if (ulTempPattern & INPUT_HEADING_NW)
  {
    printf("Input state:  INPUT_HEADING_NW state received\n");
  }

  if (ulTempPattern & INPUT_OBSTACLE_FIELD_A)
  {
    printf("Input state:  INPUT_OBSTACLE_FIELD_A state received\n");
  }

  if (ulTempPattern & INPUT_OBSTACLE_FIELD_B)
  {
    printf("Input state:  INPUT_OBSTACLE_FIELD_B state received\n");
  }

  if (ulTempPattern & INPUT_OBSTACLE_FIELD_C)
  {
    printf("Input state:  INPUT_OBSTACLE_FIELD_C state received\n");
  }

  if (ulTempPattern & INPUT_OBSTACLE_FIELD_D)
  {
    printf("Input state:  INPUT_OBSTACLE_FIELD_D state received\n");
  }

  if (ulTempPattern & INPUT_OBSTACLE_FIELD_E)
  {
    printf("Input state:  INPUT_OBSTACLE_FIELD_E state received\n");
  }

  if (ulTempPattern & INPUT_OBSTACLE_FIELD_F)
  {
    printf("Input state:  INPUT_OBSTACLE_FIELD_F state received\n");
  }

  if (ulTempPattern & INPUT_OBSTACLE_FIELD_G)
  {
    printf("Input state:  INPUT_OBSTACLE_FIELD_G state received\n");
  }

  if (ulTempPattern & INPUT_OBSTACLE_FIELD_H)
  {
    printf("Input state:  INPUT_OBSTACLE_FIELD_H state received\n");
  }
}

