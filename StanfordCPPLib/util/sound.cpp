/*
 * File: sound.cpp
 * ---------------
 * Implementation of the Sound class.
 * 
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - removed unused include statements
 */

#include "sound.h"
#include "private/platform.h"

Sound::Sound(std::string filename) {
    stanfordcpplib::getPlatform()->sound_constructor(this, filename);
}

Sound::~Sound() {
    stanfordcpplib::getPlatform()->sound_delete(this);
}

void Sound::play() {
    stanfordcpplib::getPlatform()->sound_play(this);
}

