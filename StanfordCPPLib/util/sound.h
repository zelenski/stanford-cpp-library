/*
 * File: sound.h
 * -------------
 * This file defines a class that represents a sound.
 *
 * TODO: reimplement using Qt/C++
 *
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _sound_h
#define _sound_h

#include <string>

/**
 * This class encapsulates a sound file.  The sound file is specified in the
 * constructor and must be a file in either the current directory or a
 * subdirectory named <code>sounds</code>.
 *
 * <p>The following code, for example, plays the sound file
 * <code>ringtone.wav</code>:
 *
 *<pre>
 *    Sound ringtone("ringtone.wav");
 *    ringtone.play();
 *</pre>
 */
class Sound {
public:
    /**
     * Creates a <code>Sound</code> object by reading in the contents of the
     * specified file.
     */
    Sound(std::string filename);

    /**
     * Frees the memory associated with the sound.
     */
    virtual ~Sound();

    /**
     * Starts playing the sound.
     * This call returns immediately without waiting for the sound to finish.
     */
    void play();
};

#endif // _sound_h
