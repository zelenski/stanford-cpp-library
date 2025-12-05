/*
 * File: sound.cpp
 * ---------------
 * Implementation of the Sound class.
 * 
 * 2024/9 juliez: Restored functionality to play sound file on updated QtMultimedia.
 */

#include "sound.h"
#include "filelib.h"
#include "gthread.h"
#include <QAudioOutput>

/*static*/ QMediaPlayer* Sound::_qmediaPlayer = nullptr;
/*static*/ Sound::Status Sound::_playbackStatus = Sound::NONE;

/*static*/ void Sound::initialize() {
    if (!_qmediaPlayer) {
        GThread::runOnQtGuiThreadAsync([]() {
            if (!_qmediaPlayer) {
               _qmediaPlayer = new QMediaPlayer;
                _qmediaPlayer->setAudioOutput(new QAudioOutput);
                QObject::connect(_qmediaPlayer, &QMediaPlayer::mediaStatusChanged, Sound::statusChanged);
                QObject::connect( _qmediaPlayer, &QMediaPlayer::errorOccurred, Sound::errorOccurred);
            }
        });
    }
}

/* static */ bool Sound::waitComplete() {
    // cannot wait on this thread if it is GUI thread (i.e. deadlock)
    if (GThread::iAmRunningOnTheQtGuiThread()) {
        return false;
    }
    while (_playbackStatus == BUSY) GThread::msleep(100);
    return _playbackStatus == ERROR;
}

/* static */ void Sound::playSource(const QUrl &qurl) {
    _playbackStatus = BUSY;
    GThread::runOnQtGuiThread([qurl]() {
        _qmediaPlayer->setSource(qurl);
        _qmediaPlayer->play(); // dispatch is async, play will  execute on Qt thread
     });
    bool hadError = waitComplete();
    if (hadError) {
        error("Sound::play() error " + _qmediaPlayer->errorString().toStdString() + " (" + _qmediaPlayer->source().toString().toStdString() + ")");
    }
}

/*static*/ void Sound::playFile(const std::string& filename) {
    initialize();
    std::string absPath = getAbsolutePath(filename);
    if (!fileExists(absPath)) {
        error("Sound::playFile() file not found (" + filename + ")");
    }
    QUrl qurl = QUrl::fromLocalFile(QString::fromStdString(absPath));
    playSource(qurl);
}

/*static*/ void Sound::playURL(const std::string& url) {
    initialize();
    QUrl qurl = QUrl(QString::fromStdString(url));
    if (!qurl.isValid()) {
        error("Sound::playURL() url not valid (" + url + ")");
    }
    playSource(qurl);
}

/*static*/ void Sound::statusChanged(QMediaPlayer::MediaStatus update) {
    switch (update) {
        case QMediaPlayer::NoMedia:
            _playbackStatus = NONE;
            break;
        case QMediaPlayer::LoadingMedia:
        case QMediaPlayer::LoadedMedia:
        case QMediaPlayer::StalledMedia:
        case QMediaPlayer::BufferingMedia:
        case QMediaPlayer::BufferedMedia:
            _playbackStatus = BUSY;
            break;
        case QMediaPlayer::EndOfMedia:
            _playbackStatus = COMPLETE;
            break;
        case QMediaPlayer::InvalidMedia:
            _playbackStatus = ERROR;
            break;
    }
}

/* static */ void Sound::errorOccurred(QMediaPlayer::Error, const QString &) {
   _playbackStatus = ERROR;
}
