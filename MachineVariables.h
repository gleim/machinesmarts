 /***************************************************
 *
 *	MachineVariables.h
 *
 * 	MachineVariables header
 *
 *	STE (internal) layer class
 *
 **************************************************/

  #ifndef MACHINEVARIABLES_H
  #define MACHINEVARIABLES_H 1

  #include <stdlib.h>   
  #include <stdio.h>
  #include <time.h>
  
  #include "BackpropagationNode.h"
  #include "MachineEngine.h"  /* for temporary inspection of TCB/uCos facility */
  
  #define MAXIMUM_NODES_PER_LAYER 33

  class MachineVariables
  {
  public:
		MachineVariables( );
		~MachineVariables( );
  protected:
        void initialize( );
        void cleanup( );
        void display( );
        void parseOutputForDisplay(unsigned char*);
        void parseInputForDisplay(unsigned char*);
        unsigned short ucInputVectorLength;        
        unsigned short ucHiddenVectorLength;
        unsigned short ucOutputVectorLength;
        
        double         EpochError;

        BackpropagationNode oInputLayer[MAXIMUM_NODES_PER_LAYER];
        BackpropagationNode oHiddenLayer[MAXIMUM_NODES_PER_LAYER];
        BackpropagationNode oContextLayer[MAXIMUM_NODES_PER_LAYER];  /* specific to recurrent net */
        BackpropagationNode oOutputLayer[MAXIMUM_NODES_PER_LAYER];
  private:
        double provideRandomUnitValue( );
        double checkWeightBoundary(double weightValue);
        double perturbWeight(double weightValue);
        void iterate( );
        void train( );
        void endOfIteration( );
  
        static const double LearningRate = 0.33;
        static const double Momentum     = 0.85;

  friend class MachineEngine;
  };

  #endif  // #ifndef MACHINEVARIABLES_H
  
