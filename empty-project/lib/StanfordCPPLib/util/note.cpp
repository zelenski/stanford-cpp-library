/*
 * File: note.cpp
 * --------------
 * This file implements the body of each member of the Note class.
 *
 * @author Marty Stepp
 * @version 2016/09/26
 * - initial version
 * @since 2016/09/26
 */

#include "note.h"
#include <sstream>
#include "error.h"
#include "hashcode.h"
#include "private/platform.h"

// line e.g. "1.5 G 5 NATURAL false"
Note::Note(std::string line) {
    std::istringstream input(line);

    // parse duration
    double dur;
    if (!(input >> dur)) {
        error("illegal/missing duration");
    }
    setDuration(dur);

    // parse pitch
    std::string pit;
    if (!(input >> pit)) {
        error("illegal/missing pitch");
    }
    if (pit == "A") {
        setPitch(A);
    } else if (pit == "B") {
        setPitch(B);
    } else if (pit == "C") {
        setPitch(C);
    } else if (pit == "D") {
        setPitch(D);
    } else if (pit == "E") {
        setPitch(E);
    } else if (pit == "F") {
        setPitch(F);
    } else if (pit == "G") {
        setPitch(G);
    } else if (pit == "R") {
        setPitch(R);
    } else {
        error("illegal pitch");
    }

    if (isRest()) {
        // rests always have fixed octave and accidental values
        setOctave(OCTAVE_MIN + 1);
        setAccidental(NATURAL);
    } else {
        // parse octave
        int oct;
        if (!(input >> oct)) {
            error("illegal/missing octave");
        }
        setOctave(oct);

        // parse accidental
        std::string acc;
        if (!(input >> acc)) {
            error("illegal/missing accidental");
        }
        if (acc == "SHARP") {
            setAccidental(SHARP);
        } else if (acc == "NATURAL") {
            setAccidental(NATURAL);
        } else if (acc == "FLAT") {
            setAccidental(FLAT);
        } else {
            error("illegal accidental");
        }
    }

    // parse repeat
    std::string repStr;
    if (!(input >> repStr)) {
        error("illegal/missing repeat");
    }
    if (repStr == "true") {
        setRepeat(true);
    } else if (repStr == "false") {
        setRepeat(false);
    } else {
        error("illegal repeat");
    }
}

Note::Note(double duration, Pitch pitch, int octave, Accidental accidental, bool repeat) {
    setDuration(duration);
    setPitch(pitch);
    setOctave(octave);
    setAccidental(accidental);
    setRepeat(repeat);
}

bool Note::equals(const Note& note2) const {
    return duration == note2.duration
            && pitch == note2.pitch
            && octave == note2.octave
            && accidental == note2.accidental
            && repeat == note2.repeat;
}

Note::Accidental Note::getAccidental() const {
    return accidental;
}

double Note::getDuration() const {
    return duration;
}

int Note::getOctave() const {
    return octave;
}

Note::Pitch Note::getPitch() const {
    return pitch;
}

bool Note::isRepeat() const {
    return repeat;
}

bool Note::isRest() const {
    return pitch == R;
}

void Note::play() const {
#ifdef NOTE_DEBUG
    if (NOTE_DEBUG) {
        printf("%s\n", toString().c_str());
    }
#endif // NOTE_DEBUG
    stanfordcpplib::getPlatform()->note_play(toString());
}

void Note::setAccidental(Note::Accidental accidental) {
    if (accidental != NATURAL && accidental != SHARP && accidental != FLAT) {
        error("ilegal accidental value");
    }
    if (isRest()) {
        this->accidental = NATURAL;
    } else {
        this->accidental = accidental;
    }
}

void Note::setDuration(double duration) {
    if (duration < 0.0) {
        error("ilegal negative duration");
    }
    this->duration = duration;
}

void Note::setOctave(int octave) {
    if (octave < OCTAVE_MIN || octave > OCTAVE_MAX) {
        error("illegal octave");
    }
    if (isRest()) {
        this->octave = OCTAVE_MIN + 1;
    } else {
        this->octave = octave;
    }
}

void Note::setPitch(Note::Pitch pitch) {
    if (pitch != A && pitch != B && pitch != C && pitch != D && pitch != E
            && pitch != F && pitch != G && pitch != R) {
        error("illegal pitch");
    }
    this->pitch = pitch;
}

void Note::setRepeat(bool repeat) {
    this->repeat = repeat;
}

std::string Note::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

bool operator ==(const Note& note1, const Note& note2) {
    return note1.equals(note2);
}

bool operator !=(const Note& note1, const Note& note2) {
    return !note1.equals(note2);
}

std::ostream& operator <<(std::ostream& out, const Note& note) {
    static std::string PITCH_STR[] {"A", "B", "C", "D", "E", "F", "G", "R"};
    static std::string ACCIDENTAL_STR[] {"SHARP", "NATURAL", "FLAT"};

    out << note.getDuration() << " " << PITCH_STR[note.getPitch()] << " ";
    if (!note.isRest()) {
        out << note.getOctave() << " " << ACCIDENTAL_STR[note.getAccidental()] << " ";
    }
    return out << std::boolalpha << note.isRepeat();
}

int hashCode(const Note& note) {
    int code = hashSeed();
    code = code * hashMultiplier() + hashCode(note.getDuration());
    code = code * hashMultiplier() + hashCode(note.getPitch());
    code = code * hashMultiplier() + hashCode(note.getOctave());
    code = code * hashMultiplier() + hashCode(note.getAccidental());
    code = code * hashMultiplier() + hashCode(note.isRepeat());
    return int(code & hashMask());
}
