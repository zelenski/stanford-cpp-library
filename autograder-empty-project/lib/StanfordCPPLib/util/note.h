/*
 * File: note.h
 * ------------
 * This file defines a class named Note that can play musical notes.
 *
 * @author Marty Stepp
 * @version 2016/09/26
 * - initial version
 * @since 2016/09/26
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _note_h
#define _note_h

#include <iostream>
#include <string>

/**
 * Constant for the minimum legal value that an octave can have.
 */
#define OCTAVE_MIN 1

/**
 * Constant for the maximum legal value that an octave can have.
 */
#define OCTAVE_MAX 10

/**
 * Should Note objects print a message to the console as they play?
 */
#define NOTE_DEBUG false

/**
 * Each Note object represents a musical note or rest.
 * A Note encapsulates a pitch (A-G), a duration in seconds, an octave,
 * an accidental (sharp, flat, or natural), and a flag of whether it is the
 * start/end of a repeated section or not.
 * A song or melody can be thought of as a list or array of Note objects.
 */
class Note {
public:
    /**
     * Represents a musical pitch from A-G or rest.
     */
    enum Pitch {A, B, C, D, E, F, G, R};

    /**
     * Represents a musical accidental: sharp, natural, or flat.
     */
    enum Accidental {SHARP, NATURAL, FLAT};

    /**
     * Constructs a Note with the information contained in the given line.
     * @param line a line of input data such as "0.2 C 4 NATURAL false" or "0.4 R false" for a rest
     * @throws an error if line is empty.
     * @throws an error if duration is negative or octave is not
     *         between OCTAVE_MIN and OCTAVE_MAX inclusive.
     */
    Note(std::string line);

    /**
     * Constructs a Note with the given information.
     * @param duration Note's duration in seconds.
     * @param pitch Note's pitch from A through G, or R for a rest.
     * @param octave Note's octave from OCTAVE_MIN through OCTAVE_MAX inclusive.
     * @param accidental Note's accidental from SHARP, FLAT, or NATURAL.
     * @param repeat true if this note starts/ends a repeated section.
     * @throws an error if any value passed is illegal.
     */
    Note(double duration = 0.0, Pitch pitch = R, int octave = OCTAVE_MIN, Accidental accidental = NATURAL, bool repeat = false);

    /**
     * Returns true if note2 has the same state as this Note object; otherwise false.
     * @param o the object to compare against
     */
    bool equals(const Note& note2) const;

    /**
     * Returns this Note's accidental value of SHARP, FLAT, or NATURAL.
     * The accidental value is meaningless for a rest; this method will
     * return NATURAL by default if called on a rest.
     * @return this Note's accidental value of SHARP, FLAT, or NATURAL.
     */
    Accidental getAccidental() const;

    /**
     * Returns this Note's duration in seconds.
     * @return this Note's duration in seconds.
     */
    double getDuration() const;

    /**
     * Returns this Note's octave.
     * The octave value is meaningless for a rest; this method will return
     * OCTAVE_MIN + 1 by default if called on a rest.
     * @return this Note's octave from OCTAVE_MIN to OCTAVE_MAX.
     */
    int getOctave() const;

    /**
     * Returns this Note's pitch value of A-G or R for a rest.
     * @return this Note's pitch value of A-G or R for a rest.
     */
    Pitch getPitch() const;

    /**
     * Returns true if this Note is the start or end of a repeated section.
     * @return true if this Note is the start or end of a repeated section,
     *         otherwise false.
     */
    bool isRepeat() const;

    /**
     * Returns true if this Note is a rest.  Equivalent to checking whether
     * this note's pitch is Pitch.R.  Provided for convenience.
     * @return true if this Note is a rest, otherwise false.
     */
    bool isRest() const;

    /**
     * Plays this note through the underlying audio system.
     * Also may print a message to the system console for debugging.
     * If the audio system is muted or paused, the note may not play.
     */
    void play() const;

    /**
     * Sets this Note's accidental value to be the given value: SHARP, FLAT, or NATURAL.
     * The accidental value is meaningless for a rest, but the Note object still
     * maintains an accidental value internally (initially NATURAL)
     * which is ignored.
     * @param accidental Note's accidental from SHARP, FLAT, or NATURAL.
     * @throws an error if the accidental value passed is illegal.
     */
    void setAccidental(Accidental accidental);

    /**
     * Sets this Note's duration in seconds to be the given value.
     * @param duration Note's duration in seconds.
     * @throws an error if duration is negative.
     */
    void setDuration(double duration);

    /**
     * Sets this Note's octave to be the given value.
     * The octave value is meaningless for a rest, but the Note object still
     * maintains an octave value internally (initially OCTAVE_MIN + 1)
     * which is ignored.
     * @param octave Note's octave from OCTAVE_MIN through OCTAVE_MAX inclusive.
     * @throws an error if octave is not between OCTAVE_MIN and OCTAVE_MAX inclusive.
     */
    void setOctave(int octave);

    /**
     * Sets this Note's pitch to be the given value.
     * @param pitch Note's pitch from A through G, or R for a rest.
     * @throws an error if pitch is invalid.
     */
    void setPitch(Pitch pitch);

    /**
     * Sets this Note's repeat flag to be the given value.
     * @param repeat true to indicate that this note is the start/end of a
     *               repeated section, or false if not.
     */
    void setRepeat(bool repeat);

    /**
     * Returns a string representation of this note.
     * @return A string such as "0.4 C 5 NATURAL false".
     */
    std::string toString() const;

private:
    // member variables (a.k.a. instance variables; fields)
    double duration;
    Pitch pitch;
    int octave;
    Accidental accidental;
    bool repeat;
};

/**
 * Compares the given two Note objects for equality.
 */
bool operator ==(const Note& note1, const Note& note2);

/**
 * Compares the given two Note objects for inequality.
 */
bool operator !=(const Note& note1, const Note& note2);

std::ostream& operator <<(std::ostream& out, const Note& note);

/**
 * Returns a unique integer hash code for the given Note object.
 */
int hashCode(const Note& note);

#endif // _note_h
