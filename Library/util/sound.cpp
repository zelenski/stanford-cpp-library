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
/*static*/ Sound::Status Sound::_currentStatus = Sound::NONE;

/*static*/ void Sound::initialize() {
    if (!_qmediaPlayer) {
        GThread::runOnQtGuiThread([]() {
            if (!_qmediaPlayer) {
                _qmediaPlayer = new QMediaPlayer;
                _qmediaPlayer->setAudioOutput(new QAudioOutput);
                _currentStatus = NONE;
                QObject::connect(_qmediaPlayer, &QMediaPlayer::mediaStatusChanged, Sound::statusChanged);
                QObject::connect( _qmediaPlayer, &QMediaPlayer::errorOccurred, Sound::errorOccurred);
            }
        });
    }
}

/* static */ void Sound::playSource(const QUrl &qurl) {
    _currentStatus = NONE;
    GThread::runOnQtGuiThreadAsync([qurl]() {
         _qmediaPlayer->setSource(qurl);
         _qmediaPlayer->play();
     });
    if (_currentStatus == ERROR && !GThread::iAmRunningOnTheQtGuiThread()) {
        // must error on student thread, not QtGui
        error("Sound::play() error " + _qmediaPlayer->errorString().toStdString() + " (" + _qmediaPlayer->source().toString().toStdString() + ")");
    }
}

/*static*/ void Sound::playFile(const std::string& filename) {
    initialize();
    std::string absPath = getAbsolutePath(filename);
    if (!fileExists(absPath)) {
        error("Sound::playFile() file not found (" + filename + ")");
    }
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
            _currentStatus = NONE;
            break;
        case QMediaPlayer::LoadingMedia:
        case QMediaPlayer::LoadedMedia:
        case QMediaPlayer::StalledMedia:
        case QMediaPlayer::BufferingMedia:
        case QMediaPlayer::BufferedMedia:
            _currentStatus = BUSY;
            break;
        case QMediaPlayer::EndOfMedia:
            _currentStatus = COMPLETE;
            break;
        case QMediaPlayer::InvalidMedia:
            _currentStatus = ERROR;
            break;
    }
}

/* static */ void Sound::errorOccurred(QMediaPlayer::Error, const QString &) {
   _currentStatus = ERROR;
}
