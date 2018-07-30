/*
 * File: qgui.cpp
 * --------------
 *
 * @version 2018/07/03
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qgui.h"
#include <QEvent>
#include <QThread>
#include "error.h"
#include "exceptions.h"
#include "qgwindow.h"
#include "strlib.h"

#ifdef _WIN32
#  include <direct.h>   // for chdir
#else // _WIN32
#  include <unistd.h>   // for chdir
#endif // _WIN32

QGuiEventQueue* QGuiEventQueue::_instance = nullptr;

QGuiEventQueue::QGuiEventQueue() {
    // empty
}

QGThunk QGuiEventQueue::dequeue() {
    _queueMutex.lockForWrite();
    QGThunk thunk = _functionQueue.dequeue();
    _queueMutex.unlock();
    return thunk;
}

QGuiEventQueue* QGuiEventQueue::instance() {
    if (!_instance) {
        _instance = new QGuiEventQueue();
    }
    return _instance;
}

bool QGuiEventQueue::isEmpty() const {
    return _functionQueue.isEmpty();
}

QGThunk QGuiEventQueue::peek() {
    _queueMutex.lockForRead();
    QGThunk thunk = _functionQueue.peek();
    _queueMutex.unlock();
    return thunk;
}

void QGuiEventQueue::runOnQtGuiThreadAsync(QGThunk thunk) {
    _queueMutex.lockForWrite();
    _functionQueue.add(thunk);
    _queueMutex.unlock();
    emit mySignal();
}

void QGuiEventQueue::runOnQtGuiThreadSync(QGThunk thunk) {
    _queueMutex.lockForWrite();
    _functionQueue.add(thunk);
    _queueMutex.unlock();
    emit mySignal();

    // TODO: "empty" is not quite right condition
    while (true) {
        _queueMutex.lockForRead();
        bool empty = _functionQueue.isEmpty();
        _queueMutex.unlock();
        if (empty) {
            break;
        } else {
            QGThread::sleep(50);
        }
    }
}

#endif // SPL_QT_GUI
