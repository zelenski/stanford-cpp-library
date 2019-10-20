/*
 * File: sound.h
 * -------------
 * This interface defines an abstract type that represents a sound.
 */

#ifndef _sound_h
#define _sound_h

/*
 * Type: Sound
 * -----------
 * This type encapsulates a sound file.  The sound file is specified in the
 * constructor and must be a file in either the current directory or a
 * subdirectory named <code>sounds</code>.
 *
 * <p>The following code, for example, plays the sound file
 * <code>ringtone.wav</code>:
 *
 *<pre>
 *    Sound ringtone = newSound("ringtone.wav");
 *    play(ringtone);
 *</pre>
 */

typedef struct SoundCDT *Sound;

/*
 * Function: newSound
 * Usage: sound = newSound(filename);
 * ----------------------------------
 * Creates a <code>Sound</code> object from the specified file.
 */

Sound newSound(string filename);

/*
 * Function: freeSound
 * Usage: freeSound(sound);
 * ------------------------
 * Frees the memory associated with the sound.
 */

void freeSound(Sound sound);

/*
 * Function: playSound
 * Usage: playSound(sound);
 * ------------------------
 * Starts playing the sound.  This call returns immediately without waiting
 * for the sound to finish.
 */

void playSound(Sound sound);

#endif
