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

#define INTERNAL_INCLUDE 1
#include "sound.h"
#undef INTERNAL_INCLUDE

Sound::Sound(std::string /*filename*/) {
    // TODO
    // stanfordcpplib::getPlatform()->sound_constructor(this, filename);
}

Sound::~Sound() {
    // TODO
    // stanfordcpplib::getPlatform()->sound_delete(this);
}

void Sound::play() {
    // TODO
    // stanfordcpplib::getPlatform()->sound_play(this);
}

