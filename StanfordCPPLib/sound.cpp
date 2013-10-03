/*
 * File: sound.cpp
 * ---------------
 * Implementation of the Sound class.
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "gevents.h"
#include "gtypes.h"
#include "sound.h"
#include "vector.h"
#include "platform.h"
using namespace std;

static Platform *pp = getPlatform();

Sound::Sound(string filename) {
   pp->createSound(this, filename);
}

Sound::~Sound() {
   pp->deleteSound(this);
}

void Sound::play() {
   pp->playSound(this);
}

