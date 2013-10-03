/*
 * File: gtimer.cpp
 * ----------------
 * This file implements the gtimer.h interface.
 */

#include <iostream>
#include <sstream>
#include <string>
#include "platform.h"
using namespace std;

/* Global variables */

static Platform *pp = getPlatform();

/* Implementation of the GTimer class */

GTimer::GTimer(double milliseconds) {
   gtd = new GTimerData();
   gtd->refCount = 1;
   pp->createTimer(*this, milliseconds);
}

GTimer::~GTimer() {
   if (--gtd->refCount == 0) delete gtd;
}

void GTimer::start() {
   pp->startTimer(*this);
}

void GTimer::stop() {
   pp->stopTimer(*this);
}

bool GTimer::operator==(GTimer t2) {
   return gtd == t2.gtd;
}

bool GTimer::operator!=(GTimer t2) {
   return gtd != t2.gtd;
}

GTimer::GTimer(GTimerData *gtd) {
   this->gtd = gtd;
   gtd->refCount++;
}

GTimer::GTimer(const GTimer & src) {
   this->gtd = src.gtd;
   this->gtd->refCount++;
}

GTimer & GTimer::operator=(const GTimer & src) {
   if (this != &src) {
      if (--gtd->refCount == 0) delete gtd;
      this->gtd = src.gtd;
      this->gtd->refCount++;
   }
   return *this;
}
