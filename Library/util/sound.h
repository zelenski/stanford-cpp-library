/*
 * File: sound.h
 * -------------
 * This file defines functions to play a sound.
 *
 */

#ifndef _sound_h
#define _sound_h

#include <string>
#include <QMediaPlayer>
#include <QUrl>

/**
 * Play a sound. The sound resource can be read either from
 * a local file or a URL.
 *
 * Sample uses:
 *
 *<pre>
 *    Sound::playFile("all-right-now.mp3");
 *    Sound::playURL("https://download.samplelib.com/wav/sample-3s.wav");
 *</pre>
 */
class Sound {
public:

    /**
     * Play sound read from file.
     */
    static void playFile(const std::string& filename);

    /**
     * Play sound read from url.
     */
    static void playURL(const std::string& url);


private:
    static QMediaPlayer* _qmediaPlayer;

    enum Status {
        NONE = 0,
        BUSY = 1,
        COMPLETE = 2,
        ERROR = 3,
    };
    static Status _playbackStatus;

    static void initialize();
    static void statusChanged(QMediaPlayer::MediaStatus update);
    static void errorOccurred(QMediaPlayer::Error err, const QString &errorString);
    static void playSource(const QUrl &qurl);
    static bool waitComplete();
};

#endif // _sound_h
