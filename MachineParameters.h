/***************************************************
 *
 * 	MachineParameters.h
 *
 **************************************************/

  #ifndef MACHINEPARAMETERS_H
  #define MACHINEPARAMETERS_H 1

  class MachineParameters
  {
  public:
		// CONSTRUCTORS
		MachineParameters( );
		~MachineParameters( );

		unsigned short getInputVectorLength();
		unsigned short getOutputVectorLength();
		void setInputVectorLength(unsigned short);
		void setOutputVectorLength(unsigned short);
        bool getMachineTraining( );
        void setMachineTraining( bool );
  private:
		unsigned short ucInputVectorLength;
		unsigned short ucOutputVectorLength;
        bool bTrain;
  };

  #endif  // #ifndef MACHINEPARAMETERS_H


