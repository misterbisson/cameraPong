/*
railPlan.h - Library for managing the rail in cameraPong
Copyright 2013 by Casey Bisson
*/

#ifndef railPlan_h
#define railPlan_h

#include <stdlib.h>
#include <Arduino.h>
#include <AccelStepper.h>

class railPlan
{
  public:
    railPlan( AccelStepper& stepper, int positionStart_pin, int positionEnd_pin );
    int   resetPosition();
    long  positionNow();
    long  positionStart();
    long  positionLeft();
    long  positionEnd();
    long  positionRight();
  private:
    int   setPositionStart();
    int   setPositionEnd();
    long  _positionStart;
    int   _positionStart_pin;
    int   _positionStart_state;
    long  _positionEnd;
    int   _positionEnd_pin;
    int   _positionEnd_state;
  AccelStepper& _stepper;
};

#endif
