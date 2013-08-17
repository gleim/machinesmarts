/***************************************************
 *
 * 	BackpropagationNode.h
 *
 *
 **************************************************/

  #ifndef BACKPROPAGATIONNODE_H
  #define BACKPROPAGATIONNODE_H 1
  
  #define MAXIMUM_UNITS 50
		                /*   value of fifty will have to change */
		                /*   for larger input vectors           */
		                
		                /* This value should be no smaller than: */
		                
		                /* input_vector_length multiplied by     */
                        /* hidden_layer_multiplier divided by    */
                        /* hidden_layer_divisor                  */


  class BackpropagationNode
  {
  public:
		BackpropagationNode( );
		~BackpropagationNode( );

  protected:
		double Net;
		double Activation;
		double Derivative;
		double Error;
		double Delta;
		double DeltaWts[MAXIMUM_UNITS];
		double WED[MAXIMUM_UNITS];
		double Wts[MAXIMUM_UNITS]; /* wts are TO next layer */
		
  private:

  friend class MachineEngine;
  friend class MachineVariables;
  };

  #endif
