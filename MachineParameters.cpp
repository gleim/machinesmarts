/***************************************************
 *
 *	MachineParameters.cpp
 *
 * 	MachineParameters class:
 *		this class holds the
 *		variables specified by the
 *		client to determine the
 *		current runtime Machine
 *		specification, STE layer
 *
 **************************************************/
#include "MachineParameters.h"

MachineParameters::MachineParameters( )
{
  bTrain = 0;
  ucInputVectorLength = 0;
  ucOutputVectorLength = 0;
}

MachineParameters::~MachineParameters( )
{
  bTrain = 0;
  ucInputVectorLength = 0;
  ucOutputVectorLength = 0;
}

unsigned short MachineParameters::getInputVectorLength( )
{
  return ucInputVectorLength;
}

void MachineParameters::setInputVectorLength(unsigned short ucLocalInputVectorLength)
{
  ucInputVectorLength = ucLocalInputVectorLength;
}

unsigned short MachineParameters::getOutputVectorLength( )
{
  return ucOutputVectorLength;
}

void MachineParameters::setOutputVectorLength(unsigned short ucLocalOutputVectorLength)
{
  ucOutputVectorLength = ucLocalOutputVectorLength;
}

bool MachineParameters::getMachineTraining( )
{
  return bTrain;
}

void MachineParameters::setMachineTraining( bool bLocalTrain )
{
  bTrain = bLocalTrain;
}
