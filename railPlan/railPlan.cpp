/*
railPlan.cpp - Library for managing the rail in cameraPong
Copyright 2013 by Casey Bisson
*/

#include "Arduino.h"
#include "railPlan.h"

railPlan::railPlan( AccelStepper& stepper, int positionStart_pin, int positionEnd_pin ) :
  _stepper( stepper )
{
  delay( 15 );
};
