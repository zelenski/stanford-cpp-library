/*
 * File: qconsole.cpp
 * ------------------
 * This file implements the qconsole.h interface.
 *
 * @version 2018/07/15
 * - initial version, based on io/console.cpp
 */

#define __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
#include "qconsole.h"
#undef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE

#include <cstdio>
#include "error.h"
#include "exceptions.h"
#include "qgclipboard.h"
#include "qgfont.h"
#include "private/static.h"
#include "private/version.h"

namespace stanfordcpplib {
namespace qtgui {

void setConsolePropertiesQt();


const bool QGConsoleWindow::QGConsoleWindow::ALLOW_RICH_INPUT_EDITING = false;
const double QGConsoleWindow::DEFAULT_WIDTH = 700;
const double QGConsoleWindow::DEFAULT_HEIGHT = 500;
const double QGConsoleWindow::DEFAULT_X = 10;
const double QGConsoleWindow::DEFAULT_Y = 10;
const std::string QGConsoleWindow::DEFAULT_WINDOW_TITLE = "Console";
const std::string QGConsoleWindow::DEFAULT_FONT_FAMILY = "Monospaced";
const int QGConsoleWindow::DEFAULT_FONT_SIZE = 14;
const int QGConsoleWindow::MIN_FONT_SIZE = 4;
const int QGConsoleWindow::MAX_FONT_SIZE = 128;
const std::string QGConsoleWindow::USER_INPUT_COLOR = "blue";
QGConsoleWindow* QGConsoleWindow::_instance = nullptr;
bool QGConsoleWindow::_consoleEnabled = false;

bool QGConsoleWindow::consoleEnabled() {
    return _consoleEnabled;
}

QGConsoleWindow* QGConsoleWindow::instance() {
    if (!_instance) {
        // initialize Qt system and Qt Console window
        QGui::instance()->runOnQtGuiThread([]() {
            QGui::instance()->initializeQt();
            _instance = new QGConsoleWindow();
            setConsolePropertiesQt();
        });
    }
    return _instance;
}

void QGConsoleWindow::setConsoleEnabled(bool enabled) {
    _consoleEnabled = enabled;
}

QGConsoleWindow::QGConsoleWindow()
        : QGWindow(),
          _clearEnabled(true),
          _echo(false),
          _locationSaved(false),
          _locked(false),
          _promptActive(false),
          _shutdown(false),
          _outputColor("black"),
          _errorColor("red"),
          _inputBuffer(""),
          _cinout_new_buf(nullptr),
          _cerr_new_buf(nullptr) {
    _textArea = new QGTextArea();
    _textArea->setColor("black");
    _textArea->setContextMenuEnabled(false);
    // _textArea->setEditable(false);
    _textArea->setLineWrap(false);
    _textArea->setFont(DEFAULT_FONT_FAMILY + "-" + integerToString(DEFAULT_FONT_SIZE));
    // _textArea->setRowsColumns(25, 70);
    QTextEdit* rawTextEdit = static_cast<QTextEdit*>(_textArea->getWidget());
    rawTextEdit->setTabChangesFocus(false);
    _textArea->setKeyListener([this](QGEvent event) {
        if (event.getEventType() == QGEvent::KEY_PRESSED) {
            this->processKeyPress(event);
        } else if (event.getEventType() == QGEvent::KEY_RELEASED
                   || event.getEventType() == QGEvent::KEY_TYPED) {
            event.ignore();
        }
    });
    _textArea->setMouseListener([this](QGEvent event) {
        // snuff out mouse-based operations:
        // - popping up context menu by right-clicking
        // - Linux-style copy/paste operations using selection plus middle-click
        if (event.getButton() > 1
                || event.getEventType() == QGEvent::MOUSE_RELEASED) {
            event.ignore();
        }
    });
    addToRegion(_textArea, "Center");
    setTitle(DEFAULT_WINDOW_TITLE);
    setCloseOperation(QGWindow::CLOSE_HIDE);
    setLocation(DEFAULT_X, DEFAULT_Y);
    setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setResizable(false);
    setVisible(true);

    // buffer C-style stderr
    static char stderrBuf[BUFSIZ + 10] = {'\0'};
    std::ios::sync_with_stdio(false);
    setbuf(stderr, stderrBuf);

    // redirect cin/cout/cerr
    _cinout_new_buf = new stanfordcpplib::qtgui::ConsoleStreambufQt();
    _cerr_new_buf = new stanfordcpplib::qtgui::ConsoleStreambufQt(/* isStderr */ true);
    std::cin.rdbuf(_cinout_new_buf);
    std::cout.rdbuf(_cinout_new_buf);
    std::cerr.rdbuf(_cerr_new_buf);
    // ::stanfordcpplib::ForwardingStreambuf* stderrStreamBuf = new ::stanfordcpplib::ForwardingStreambuf(*_cinout_new_buf, /* isStderr */ true);
    // stderrStreamBuf->pubsetbuf(cerrBuf, BUFSIZ + 10);
    // std::cerr.rdbuf(stderrStreamBuf);
    // std::nounitbuf(std::cerr);   // disable buffering after each token
}

QGConsoleWindow::~QGConsoleWindow() {
    // empty
}

void QGConsoleWindow::clearConsole() {
    if (_shutdown) {
        return;
    }
    std::string msg = "==================== (console cleared) ====================";
    if (_clearEnabled) {
        // print to standard console (not Stanford graphical console)
        printf("%s\n", msg.c_str());

        // clear the graphical console window
        _coutMutex.lock();
        _textArea->clearText();
        _coutMutex.unlock();
    } else {
        // don't actually clear the window, just display 'cleared' message on it
        println(msg);
    }
}

std::string QGConsoleWindow::getErrorColor() const {
    return _errorColor;
}

std::string QGConsoleWindow::getOutputColor() const {
    return _outputColor;
}

QTextFragment QGConsoleWindow::getUserInputFragment() const {
    QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
    QTextBlock block = textArea->document()->end().previous();
    while (block.isValid()) {
        QTextBlock::iterator it;
        for (it = block.begin(); !(it.atEnd()); ++it) {
            QTextFragment frag = it.fragment();
            if (frag.isValid()) {
                std::string fragText = frag.text().toStdString();

                // see if it is the given user input
                if (fragText == _inputBuffer) {
                    return frag;
                }
            }
        }
        block = block.previous();
    }

    // didn't find the fragment
    QTextFragment notFound;
    return notFound;
}

int QGConsoleWindow::getUserInputStart() const {
    QTextFragment frag = getUserInputFragment();
    return frag.isValid() ? frag.position() : -1;
}

int QGConsoleWindow::getUserInputEnd() const {
    QTextFragment frag = getUserInputFragment();
    return frag.isValid() ? frag.position() + frag.length() : -1;
}

bool QGConsoleWindow::isClearEnabled() const {
    return _clearEnabled;
}

bool QGConsoleWindow::isEcho() const {
    return _echo;
}

bool QGConsoleWindow::isLocationSaved() const {
    return _locationSaved;
}

bool QGConsoleWindow::isLocked() const {
    return _locked;
}

bool QGConsoleWindow::isCursorInUserInputArea() const {
    int cursorPosition = _textArea->getCursorPosition();
    return _promptActive
            && getUserInputStart() <= cursorPosition
            && cursorPosition <= getUserInputEnd();
}

void QGConsoleWindow::print(const std::string& str, bool isStdErr) {
    if (_shutdown) {
        return;
    }
    if (_echo) {
        fflush(isStdErr ? stdout : stderr);
        fflush(isStdErr ? stderr : stdout);
        fprintf(isStdErr ? stderr : stdout, "%s", str.c_str());
        if (str.find("\n") != std::string::npos) {
            fflush(isStdErr ? stderr : stdout);
            fflush(isStdErr ? stdout : stderr);
        }
    }
    QGui::instance()->runOnQtGuiThreadAsync([this, str, isStdErr]() {
        _coutMutex.lock();
        this->_textArea->appendFormattedText(str, isStdErr ? _errorColor : _outputColor);
        this->_textArea->moveCursorToEnd();
        this->_textArea->scrollToBottom();
        _coutMutex.unlock();
    });
}

void QGConsoleWindow::println(bool isStdErr) {
    print("\n", isStdErr);
}

void QGConsoleWindow::println(const std::string& str, bool isStdErr) {
    print(str + "\n", isStdErr);
}

void QGConsoleWindow::processKeyPress(QGEvent event) {
    // TODO: should this be done in a different thread?
    char key = event.getKeyChar();
    int keyCode = event.getKeyCode();

    if (event.isCtrlOrCommandKeyDown()) {
        if (keyCode == Qt::Key_Plus || keyCode == Qt::Key_Equal) {
            // increase font size
            QFont font = QGFont::toQFont(_textArea->getFont());
            if (font.pointSize() + 1 <= MAX_FONT_SIZE) {
                font.setPointSize(font.pointSize() + 1);
                _textArea->setFont(QGFont::toFontString(font));
            }
        } else if (keyCode == Qt::Key_Minus) {
            // decrease font size
            QFont font = QGFont::toQFont(_textArea->getFont());
            if (font.pointSize() - 1 >= MIN_FONT_SIZE) {
                font.setPointSize(font.pointSize() - 1);
                _textArea->setFont(QGFont::toFontString(font));
            }
        } else if (keyCode == Qt::Key_0) {
            // normalize font size
            _textArea->setFont(DEFAULT_FONT_FAMILY + "-" + integerToString(DEFAULT_FONT_SIZE));
        } else if (keyCode == Qt::Key_C) {
            processCopy();
        } else if (event.isCtrlKeyDown() && keyCode == Qt::Key_D) {
            processEof();
        } else if (keyCode == Qt::Key_V) {
            processPaste();
        }
    }

    if (_shutdown) {
        return;
    }

    if (event.isCtrlOrCommandKeyDown() || event.isAltKeyDown()) {
        // system hotkey; let the normal keyboard handler process this event
        event.ignore();
        return;
    }

    switch (keyCode) {
        case QGEvent::PAGE_UP_KEY:
        case QGEvent::PAGE_DOWN_KEY:
            // don't ignore event
            break;
        case QGEvent::BACKSPACE_KEY:
        case QGEvent::DELETE_KEY: {
            event.ignore();
            processBackspace(keyCode);
            break;
        }
        case QGEvent::HOME_KEY:
            if (ALLOW_RICH_INPUT_EDITING) {
                // move to start of input buffer
                if (_promptActive) {
                    event.ignore();
                    int start = getUserInputStart();
                    if (start >= 0) {
                        _textArea->setCursorPosition(start);
                    } else {
                        _textArea->moveCursorToEnd();
                    }
                }
            } else {
                event.ignore();
            }
            break;
        case QGEvent::END_KEY:
            if (ALLOW_RICH_INPUT_EDITING) {
                // move to end of input buffer
                if (_promptActive) {
                    event.ignore();
                    int end = getUserInputEnd();
                    if (end >= 0) {
                        _textArea->setCursorPosition(end);
                    } else {
                        _textArea->moveCursorToEnd();
                    }
                }
            } else {
                event.ignore();
            }
            break;
        case QGEvent::LEFT_ARROW_KEY:
        case QGEvent::RIGHT_ARROW_KEY:
            // TODO: allow fine-grained editing of user input value
            event.ignore();
            break;
        case QGEvent::UP_ARROW_KEY:
        case QGEvent::DOWN_ARROW_KEY:
            // TODO: command history
            event.ignore();
            break;
        case QGEvent::TAB_KEY:
        case QGEvent::CLEAR_KEY:
        case QGEvent::F1_KEY:
        case QGEvent::F2_KEY:
        case QGEvent::F3_KEY:
        case QGEvent::F4_KEY:
        case QGEvent::F5_KEY:
        case QGEvent::F6_KEY:
        case QGEvent::F7_KEY:
        case QGEvent::F8_KEY:
        case QGEvent::F9_KEY:
        case QGEvent::F10_KEY:
        case QGEvent::F11_KEY:
        case QGEvent::F12_KEY:
        case QGEvent::INSERT_KEY:
        case QGEvent::HELP_KEY: {
            // various control/modifier keys: do nothing / consume event
            event.ignore();
            break;
        }
        case QGEvent::SHIFT_KEY:
        case QGEvent::CTRL_KEY:
        case QGEvent::ALT_KEY:
        case QGEvent::PAUSE_KEY:
        case QGEvent::CAPS_LOCK_KEY:
        case QGEvent::ESCAPE_KEY:
        case QGEvent::NUM_LOCK_KEY:
        case QGEvent::SCROLL_LOCK_KEY:
        case QGEvent::PRINT_SCREEN_KEY:
        case QGEvent::META_KEY:
        case QGEvent::WINDOWS_KEY:
        case QGEvent::MENU_KEY: {
            // various other control/modifier keys: let OS have the event (don't call ignore())
            break;
        }
        case QGEvent::RETURN_KEY:
        case QGEvent::ENTER_KEY: {
            // \n end line
            event.ignore();
            processUserInputEnterKey();
            break;
        }
        default: {
            event.ignore();
            processUserInputKey(key);
            break;
        }
    }
}

void QGConsoleWindow::processBackspace(int key) {
    if (_shutdown || !_promptActive) {
        return;
    }

    // check whether it is a backspace or a delete
    bool isBackspace = key == QGEvent::BACKSPACE_KEY /* TODO: or computer is Mac */;

    _cinMutex.lockForWrite();
    if (!_inputBuffer.empty()) {
        // remove last char from onscreen text editor:
        // - find last blue area
        QTextFragment frag = getUserInputFragment();
        if (frag.isValid()) {
            // remove last char from onscreen document fragment
            QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
            QTextCursor cursor(textArea->textCursor());

            int oldCursorPosition = cursor.position();
            int indexToDelete = _inputBuffer.length() - 1;
            if (oldCursorPosition >= frag.position() || oldCursorPosition < frag.position() + frag.length()) {
                // cursor is inside the user input fragment;
                // figure out which character it's on so we can delete it
                indexToDelete = oldCursorPosition - frag.position() - (isBackspace ? 1 : 0);
                printf("  DEBUG: inside fragment, indexToDelete = %d\n", indexToDelete);
            } else {
                // cursor is outside of the user input fragment; move it there
                cursor.setPosition(frag.position() + frag.length());
                printf("  DEBUG: outside fragment, indexToDelete = %d\n", indexToDelete);
            }

            if (isBackspace) {
                cursor.deletePreviousChar();
            } else {
                cursor.deleteChar();   // Delete
            }

            // remove last char from internal input buffer
            _inputBuffer.erase(indexToDelete, 1);
        }
    }
    _cinMutex.unlock();
}

void QGConsoleWindow::processCopy() {
    std::string selectedText = _textArea->getSelectedText();
    if (!selectedText.empty()) {
        QGClipboard::set(selectedText);
    }
}

void QGConsoleWindow::processEof() {
    // only set EOF if input buffer is empty; this is the behavior on most *nix consoles
    if (_inputBuffer.empty()) {
        std::cin.setstate(std::ios_base::eofbit);
    }
}

void QGConsoleWindow::processPaste() {
    if (_shutdown) {
        return;
    }

    std::string clipboardText = QGClipboard::get();
    for (int i = 0; i < (int) clipboardText.length(); i++) {
        if (clipboardText[i] == '\r') {
            continue;
        } else if (clipboardText[i] == '\n') {
            processUserInputEnterKey();
        } else {
            processUserInputKey(clipboardText[i]);
        }
    }
}

void QGConsoleWindow::processUserInputEnterKey() {
    if (_shutdown) {
        return;
    }
    _cinMutex.lockForWrite();
    _cinQueueMutex.lockForWrite();
    _inputLines.enqueue(_inputBuffer);
    _cinQueueMutex.unlock();
    _inputBuffer = "";   // clear input buffer
    this->_textArea->appendFormattedText("\n", USER_INPUT_COLOR);
    _cinMutex.unlock();
}

void QGConsoleWindow::processUserInputKey(int key) {
    if (_shutdown) {
        return;
    }
    if (key != '\0' && isprint(key)) {
        // normal key: append to user input buffer
        _cinMutex.lockForWrite();

        std::string keyStr = charToString((char) key);

//        if (isCursorInUserInputArea()) {
//            QTextFragment frag = getUserInputFragment();
//            if (frag.isValid()) {
//                QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
//                QTextCursor cursor(textArea->textCursor());

//                int cursorPosition = cursor.position();
//                int indexToInsert = cursorPosition - frag.position();
//                if (indexToInsert == 0) {
//                    // special case for inserting at start of fragment.
//                    // example: fragment is "abcde", cursor at start, user types "x".
//                    // if we just insert the "x" in the document, it won't be part of
//                    // the same fragment and won't have the blue bold format.
//                    // So what we do is temporarily insert it after the first character,
//                    // then delete the first character, so that everything is inside
//                    // the formatted span.
//                    // "abcde"
//                    //  ^
//                    //   ^          move right by 1
//                    // "axabcde"    insert "xa" at index 1
//                    //     ^
//                    //  ^           move left by 3
//                    // "xabcde"     delete "a" from index 0
//                    //  ^
//                    //   ^          move right by 1
//                    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);             // move to index 1
//                    cursor.insertText(QString::fromStdString(keyStr + _inputBuffer.substr(0, 1)));   // insert new char + old first char
//                    // cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 2);              // delete old copy of first char
//                    // cursor.deleteChar();
//                    // cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);
//                } else {
//                    cursor.insertText(QString::fromStdString(keyStr));
//                }
//                _inputBuffer.insert(indexToInsert, keyStr);

//                printf("  DEBUG: after insert, buffer = \"%s\", frag = \"%s\"\n",
//                       _inputBuffer.c_str(),
//                       (frag.isValid() ? frag.text().toStdString().c_str() : ""));
//                fflush(stdout);
//            }
//        } else {
            // append to end of buffer/fragment
            _inputBuffer += keyStr;
            // display in blue highlighted text
            this->_textArea->appendFormattedText(keyStr, USER_INPUT_COLOR, "*-*-Bold");
//        }

        _cinMutex.unlock();
    }
}

std::string QGConsoleWindow::readLine() {
    // TODO: threads/locking
    // wait for a line to be available in queue
    std::string line;
    if (_shutdown) {
        return line;
    }

    this->_textArea->moveCursorToEnd();
    this->_textArea->scrollToBottom();

    while (!_shutdown && !std::cin.eof()) {
        _cinMutex.lockForWrite();
        _promptActive = true;
        _cinMutex.unlock();

        if (!_inputLines.isEmpty()) {
            _cinQueueMutex.lockForWrite();
            if (!_inputLines.isEmpty()) {
                line = _inputLines.dequeue();

                _cinMutex.lockForWrite();
                _promptActive = false;
                _cinMutex.unlock();

                _cinQueueMutex.unlock();
                break;
            }
            _cinQueueMutex.unlock();
        }
        QGui::getCurrentThread()->msleep(50);
    }

    if (_echo) {
        fprintf(stdout, "%s\n", line.c_str());
    }
    return line;
}

void QGConsoleWindow::setClearEnabled(bool clearEnabled) {
    if (_locked || _shutdown) {
        return;
    }
    _clearEnabled = clearEnabled;
}

void QGConsoleWindow::setConsoleSize(double width, double height) {
    // TODO: base on text area's preferred size / packing window
    // _textArea->setPreferredSize(width, height);
    // pack();
    setSize(width, height);
}

void QGConsoleWindow::setEcho(bool echo) {
    if (_locked || _shutdown) {
        return;
    }
    _echo = echo;
}

void QGConsoleWindow::setFont(const std::string& font) {
    if (_locked) {
        return;
    }
    QGWindow::setFont(font);
    _textArea->setFont(font);
}

void QGConsoleWindow::setLocationSaved(bool locationSaved) {
    if (_locked) {
        return;
    }
    _locationSaved = locationSaved;
}

void QGConsoleWindow::setLocked(bool locked) {
    _locked = locked;
}

void QGConsoleWindow::setErrorColor(const std::string& errorColor) {
    if (_locked) {
        return;
    }
    _errorColor = errorColor;
}

void QGConsoleWindow::setOutputColor(const std::string& outputColor) {
    if (_locked) {
        return;
    }
    _outputColor = outputColor;
}

void QGConsoleWindow::shutdown() {
    const std::string PROGRAM_COMPLETED_TITLE_SUFFIX = " [completed]";
    _shutdown = true;
    _textArea->setEditable(false);
    std::string title = getTitle();
    if (title.find(PROGRAM_COMPLETED_TITLE_SUFFIX) == std::string::npos) {
        setTitle(title + PROGRAM_COMPLETED_TITLE_SUFFIX);
    }
}


// private functions
void endLineConsoleQt(bool isStdErr) {
    QGConsoleWindow::instance()->println(isStdErr);
}

std::string getLineConsoleQt() {
    return QGConsoleWindow::instance()->readLine();
}

void putConsoleQt(const std::string& str, bool isStdErr) {
    QGConsoleWindow::instance()->print(str, isStdErr);
}


void clearConsole() {
    QGConsoleWindow::instance()->clearConsole();
}

bool getConsoleClearEnabled() {
    return QGConsoleWindow::instance()->isClearEnabled();
}

bool getConsoleEcho() {
    return QGConsoleWindow::instance()->isEcho();
}

bool getConsoleEnabled() {
#ifdef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    return false;
#else
    return QGConsoleWindow::consoleEnabled();
#endif
}

bool getConsoleExitProgramOnClose() {
    return QGConsoleWindow::instance()->getCloseOperation() == QGWindow::CLOSE_EXIT;
}

std::string getConsoleFont() {
    return QGConsoleWindow::instance()->getFont();
}

double getConsoleHeight() {
    return QGConsoleWindow::instance()->getHeight();
}

QGPoint getConsoleLocation() {
    return QGConsoleWindow::instance()->getLocation();
}

bool getConsoleLocationSaved() {
    return QGConsoleWindow::instance()->isLocationSaved();
}

bool getConsolePrintExceptions() {
    return exceptions::getTopLevelExceptionHandlerEnabled();
}

bool getConsoleSettingsLocked() {
    return QGConsoleWindow::instance()->isLocked();
}

QGDimension getConsoleSize() {
    return QGConsoleWindow::instance()->getSize();
}

double getConsoleWidth() {
    return QGConsoleWindow::instance()->getWidth();
}

QGConsoleWindow* getConsoleWindow() {
    return QGConsoleWindow::instance();
}

std::string getConsoleWindowTitle() {
    return QGConsoleWindow::instance()->getTitle();
}

void setConsoleClearEnabled(bool value) {
    if (QGConsoleWindow::instance()->isLocked()) { return; }
    QGConsoleWindow::instance()->setClearEnabled(value);
}

void setConsoleCloseOperation(QGWindow::CloseOperation op) {
    if (QGConsoleWindow::instance()->isLocked()) { return; }
    QGConsoleWindow::instance()->setCloseOperation(op);
}

void setConsoleEcho(bool echo) {
    if (QGConsoleWindow::instance()->isLocked()) { return; }
    QGConsoleWindow::instance()->setEcho(echo);
}

void setConsoleErrorColor(const std::string& color) {
    if (QGConsoleWindow::instance()->isLocked()) { return; }
    QGConsoleWindow::instance()->setErrorColor(color);
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    if (QGConsoleWindow::instance()->isLocked()) { return; }
    QGConsoleWindow::instance()->setExitOnClose(exitOnClose);
}

void setConsoleFont(const std::string& font) {
    if (QGConsoleWindow::instance()->isLocked()) { return; }
    QGConsoleWindow::instance()->setFont(font);
}

void setConsoleLocation(double x, double y) {
    if (QGConsoleWindow::instance()->isLocked()) { return; }
    QGConsoleWindow::instance()->setLocation(x, y);
}

void setConsoleLocationSaved(bool value) {
    QGConsoleWindow::instance()->setLocationSaved(value);
}

void setConsoleOutputColor(const std::string& color) {
    QGConsoleWindow::instance()->setOutputColor(color);
}

void setConsolePrintExceptions(bool printExceptions) {
    if (QGConsoleWindow::instance()->isLocked()) { return; }
    exceptions::setTopLevelExceptionHandlerEnabled(printExceptions);
}

void setConsoleSettingsLocked(bool value) {
    QGConsoleWindow::instance()->setLocked(value);
}

void setConsoleSize(double width, double height) {
    if (QGConsoleWindow::instance()->isLocked()) { return; }
    QGConsoleWindow::instance()->setConsoleSize(width, height);
}

void setConsoleWindowTitle(const std::string& title) {
    if (QGConsoleWindow::instance()->isLocked()) { return; }
    QGConsoleWindow::instance()->setTitle(title);
}

void shutdownConsole() {
    if (getConsoleEnabled() && !QGConsoleWindow::instance()->isLocked()) {
        QGConsoleWindow::instance()->shutdown();
    }
}

/*
 * Sets up console settings like window size, location, exit-on-close, etc.
 * based on compiler options set in the .pro file.
 */
void setConsolePropertiesQt() {
#if defined(SPL_CONSOLE_FONTSIZE)
    std::string fontStr = std::string("Monospaced-Bold-") + integerToString(SPL_CONSOLE_FONTSIZE);
    setConsoleFont(fontStr);
#endif

#if defined(SPL_CONSOLE_WIDTH) && defined(SPL_CONSOLE_HEIGHT)
    setConsoleSize(SPL_CONSOLE_WIDTH, SPL_CONSOLE_HEIGHT);
#endif

#if defined(SPL_CONSOLE_X) && defined(SPL_CONSOLE_Y)
    setConsoleLocation(SPL_CONSOLE_X, SPL_CONSOLE_Y);
#endif

#if defined(SPL_CONSOLE_ECHO)
    setConsoleEcho(true);
#endif

#if defined(SPL_CONSOLE_EXIT_ON_CLOSE)
    setConsoleExitProgramOnClose(true);
#endif

#if defined(SPL_CONSOLE_LOCATION_SAVED)
    setConsoleLocationSaved(true);
#endif

#if defined(SPL_VERIFY_PROJECT_VERSION)
    version::ensureProjectVersion();
#endif
}

// TODO: should this be in qgui?
void initializeStanfordCppLibraryQt() {
    // ensure that library is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

    // declaring this object ensures that std::cin, cout, cerr are initialized
    // properly before our lib tries to mess with them / redirect them
    static std::ios_base::Init ios_base_init;

#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    setConsolePrintExceptions(true);
#endif
}

void initializeQtGraphicalConsole() {
    // ensure that console is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

    initializeStanfordCppLibraryQt();

    QGConsoleWindow::instance();   // ensure that console window is ready

#ifdef _WIN32
//    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // _WIN32

    setConsolePropertiesQt();
}

// This one is at the bottom because it's not meant to be called by students.
void setConsoleEnabled(bool enabled) {
    QGConsoleWindow::setConsoleEnabled(enabled);
}

} // namespace qtgui

} // namespace stanfordcpplib
