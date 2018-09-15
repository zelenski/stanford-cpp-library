/*
 * File: sound.c
 * -------------
 * This file implements the sound.h interface.
 */

#include <stdio.h>
#include "cslib.h"
#include "platform.h"
#include "sound.h"

/*
 * Type SoundCDT
 * -------------
 * This type is the concrete type for sounds.
 */

struct SoundCDT {
   string filename;
};

Sound newSound(string filename) {
   Sound sound;

   sound = newBlock(Sound);
   sound->filename = filename;
   createSoundOp(sound, filename);
   return sound;
}

void freeSound(Sound sound) {
   deleteSoundOp(sound);
}

void playSound(Sound sound) {
   playSoundOp(sound);
}
