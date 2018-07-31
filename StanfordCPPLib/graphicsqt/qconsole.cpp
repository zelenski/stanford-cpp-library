/*
 * File: qconsole.cpp
 * ------------------
 * This file implements the qconsole.h interface.
 *
 * @version 2018/07/15
 * - initial version, based on io/console.cpp
 */

#ifdef SPL_QT_GUI
#define __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
#include "qconsole.h"
#undef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE

#include <cstdio>
#include <QAction>
#include <QTextDocumentFragment>
#include "error.h"
#include "exceptions.h"
#include "filelib.h"
#include "qgclipboard.h"
#include "qgcolor.h"
#include "qgcolorchooser.h"
#include "qgfilechooser.h"
#include "qgfont.h"
#include "qgfontchooser.h"
#include "qgoptionpane.h"
#include "qgthread.h"
#include "private/static.h"
#include "private/version.h"

namespace stanfordcpplib {
namespace qtgui {

void setConsolePropertiesQt();


const bool QGConsoleWindow::QGConsoleWindow::ALLOW_RICH_INPUT_EDITING = true;
const double QGConsoleWindow::DEFAULT_WIDTH = 700;
const double QGConsoleWindow::DEFAULT_HEIGHT = 500;
const double QGConsoleWindow::DEFAULT_X = 10;
const double QGConsoleWindow::DEFAULT_Y = 40;
const std::string QGConsoleWindow::CONFIG_FILE_NAME = "spl-jar-settings.txt";
const std::string QGConsoleWindow::DEFAULT_WINDOW_TITLE = "Console";
const std::string QGConsoleWindow::DEFAULT_FONT_FAMILY = "Monospaced";
const int QGConsoleWindow::DEFAULT_FONT_SIZE = 12;
const int QGConsoleWindow::MIN_FONT_SIZE = 4;
const int QGConsoleWindow::MAX_FONT_SIZE = 255;
const std::string QGConsoleWindow::DEFAULT_BACKGROUND_COLOR = "white";
const std::string QGConsoleWindow::DEFAULT_ERROR_COLOR = "red";
const std::string QGConsoleWindow::DEFAULT_OUTPUT_COLOR = "black";
const std::string QGConsoleWindow::USER_INPUT_COLOR = "blue";
QGConsoleWindow* QGConsoleWindow::_instance = nullptr;
bool QGConsoleWindow::_consoleEnabled = false;

bool QGConsoleWindow::consoleEnabled() {
    return _consoleEnabled;
}

QGConsoleWindow* QGConsoleWindow::instance() {
    if (!_instance) {
        // initialize Qt system and Qt Console window
        QGThread::runOnQtGuiThread([]() {
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
          _textArea(nullptr),
          _clearEnabled(true),
          _echo(false),
          _locationSaved(false),
          _locked(false),
          _promptActive(false),
          _shutdown(false),
          _commandHistoryIndex(-1),
          _errorColor(""),
          _outputColor(""),
          _inputBuffer(""),
          _lastSaveFileName(""),
          _cinout_new_buf(nullptr),
          _cerr_new_buf(nullptr) {
    _initMenuBar();
    _initWidgets();
    _initStreams();
    loadConfiguration();
}

void QGConsoleWindow::_initMenuBar() {
    const std::string ICON_FOLDER = "icons/";

    // File menu
    addMenu("&File");
    addMenuItem("File", "&Save", ICON_FOLDER + "save.gif",
                [this]() { this->save(); })
                ->setShortcut(QKeySequence::Save);

    addMenuItem("File", "Save &As...", ICON_FOLDER + "save_as.gif",
                [this]() { this->saveAs(); })
                ->setShortcut(QKeySequence::SaveAs);
    addMenuSeparator("File");

    addMenuItem("File", "&Print", ICON_FOLDER + "print.gif",
                [this]() { this->showPrintDialog(); })
                ->setShortcut(QKeySequence::Print);
    setMenuItemEnabled("File", "Print", false);
    addMenuSeparator("File");

    addMenuItem("File", "&Load Input Script...", ICON_FOLDER + "script.gif",
                [this]() { this->showInputScriptDialog(); });

    addMenuItem("File", "&Compare Output...", ICON_FOLDER + "compare_output.gif",
                [this]() { this->compareOutput(); });
    setMenuItemEnabled("File", "Compare Output...", false);

    addMenuItem("File", "&Quit", ICON_FOLDER + "quit.gif",
                [this]() { this->close(); /* TODO: exit app */ })
                ->setShortcut(QKeySequence::Quit);

    // Edit menu
    addMenu("&Edit");
    addMenuItem("Edit", "Cu&t", ICON_FOLDER + "cut.gif",
                [this]() { this->clipboardCut(); })
                ->setShortcut(QKeySequence::Cut);

    addMenuItem("Edit", "&Copy", ICON_FOLDER + "copy.gif",
                [this]() { this->clipboardCopy(); })
                ->setShortcut(QKeySequence::Copy);

    addMenuItem("Edit", "&Paste", ICON_FOLDER + "paste.gif",
                [this]() { this->clipboardPaste(); })
                ->setShortcut(QKeySequence::Paste);

    addMenuItem("Edit", "Select &All", ICON_FOLDER + "select_all.gif",
                [this]() { this->selectAll(); })
                ->setShortcut(QKeySequence::SelectAll);

    addMenuItem("Edit", "C&lear Console", ICON_FOLDER + "clear_console.gif",
                [this]() { this->clearConsole(); })
                ->setShortcut(QKeySequence(QString::fromStdString("Ctrl+L")));

    // Options menu
    addMenu("&Options");
    addMenuItem("Options", "&Font...", ICON_FOLDER + "font.gif",
                [this]() { this->showFontDialog(); });

    addMenuItem("Options", "&Background Color...", ICON_FOLDER + "background_color.gif",
                [this]() { this->showColorDialog(/* background */ true); });

    addMenuItem("Options", "&Text Color...", ICON_FOLDER + "text_color.gif",
                [this]() { this->showColorDialog(/* background */ false); });

    // Help menu
    addMenu("&Help");
    addMenuItem("Help", "&About...", ICON_FOLDER + "about.gif",
                [this]() { this->showAboutDialog(); })
                ->setShortcut(QKeySequence::HelpContents);

    addMenuItem("Help", "&Check for Updates", ICON_FOLDER + "check_for_updates.gif",
                [this]() { this->checkForUpdates(); });
    setMenuItemEnabled("Help", "Check for Updates", false);
}

void QGConsoleWindow::_initStreams() {
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
}

void QGConsoleWindow::_initWidgets() {
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
}


QGConsoleWindow::~QGConsoleWindow() {
    // empty
}

void QGConsoleWindow::checkForUpdates() {
    // TODO
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

void QGConsoleWindow::clipboardCopy() {
    std::string selectedText = _textArea->getSelectedText();
    if (!selectedText.empty()) {
        QGClipboard::set(selectedText);
    }
}

void QGConsoleWindow::clipboardCut() {
    if (_shutdown || !_promptActive || !ALLOW_RICH_INPUT_EDITING) {
        return;
    }

    // if selection is entirely within the user input area, cut out of user input area
    int userInputStart = getUserInputStart();
    int userInputEnd   = getUserInputEnd();
    int selectionStart = _textArea->getSelectionStart();
    int selectionEnd = _textArea->getSelectionEnd();
    if (selectionEnd > selectionStart
            && selectionStart >= userInputStart
            && selectionEnd <= userInputEnd) {
        // selection is entirely user input! cut it!
        QTextFragment frag = getUserInputFragment();
        if (frag.isValid()) {
            std::string selectedText = _textArea->getSelectedText();
            QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
            QTextCursor cursor(textArea->textCursor());

            int indexStart = selectionStart - userInputStart;
            int selectionLength = _textArea->getSelectionLength();
            _cinMutex.lockForWrite();
            _inputBuffer.erase(indexStart, selectionLength);
            cursor.beginEditBlock();
            cursor.removeSelectedText();
            cursor.endEditBlock();
            textArea->setTextCursor(cursor);
            _cinMutex.unlock();
            QGClipboard::set(selectedText);
        }
    }
}

void QGConsoleWindow::clipboardPaste() {
    if (_shutdown) {
        return;
    }

    _textArea->clearSelection();
    if (!isCursorInUserInputArea()) {
        _textArea->moveCursorToEnd();
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

void QGConsoleWindow::close() {
    shutdown();
    QGWindow::close();   // call super
}

void QGConsoleWindow::compareOutput() {
    // TODO
}

std::string QGConsoleWindow::getBackground() const {
    return _textArea->getBackground();
}

int QGConsoleWindow::getBackgroundInt() const {
    return _textArea->getBackgroundInt();
}

std::string QGConsoleWindow::getColor() const {
    return getOutputColor();
}

int QGConsoleWindow::getColorInt() const {
    return QGColor::convertColorToRGB(getOutputColor());
}

std::string QGConsoleWindow::getErrorColor() const {
    return _errorColor.empty() ? DEFAULT_ERROR_COLOR : _errorColor;
}

std::string QGConsoleWindow::getForeground() const {
    return getOutputColor();
}

int QGConsoleWindow::getForegroundInt() const {
    return QGColor::convertColorToRGB(getOutputColor());
}

std::string QGConsoleWindow::getOutputColor() const {
    return _outputColor.empty() ? DEFAULT_OUTPUT_COLOR : _outputColor;
}

QTextFragment QGConsoleWindow::getUserInputFragment() const {
    if (!_inputBuffer.empty()) {
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
    }

    // didn't find the fragment; this will return an 'invalid' fragment
    QTextFragment notFound;
    return notFound;
}

int QGConsoleWindow::getUserInputStart() const {
    QTextFragment frag = getUserInputFragment();
    if (frag.isValid()) {
        return frag.position();
    } else if (_promptActive) {
        // at end of text
        return (int) _textArea->getText().length();
    } else {
        return -1;
    }
}

int QGConsoleWindow::getUserInputEnd() const {
    QTextFragment frag = getUserInputFragment();
    if (frag.isValid()) {
        return frag.position() + frag.length();
    } else if (_promptActive) {
        // at end of text
        return (int) _textArea->getText().length();
    } else {
        return -1;
    }
}

bool QGConsoleWindow::isClearEnabled() const {
    return _clearEnabled;
}

bool QGConsoleWindow::isCursorInUserInputArea() const {
    int cursorPosition = _textArea->getCursorPosition();
    int userInputStart = getUserInputStart();
    int userInputEnd   = getUserInputEnd();
    return _promptActive
            && userInputStart <= cursorPosition
            && cursorPosition <= userInputEnd;
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

bool QGConsoleWindow::isSelectionInUserInputArea() const {
    int userInputStart = getUserInputStart();
    int userInputEnd   = getUserInputEnd();
    int selectionStart = _textArea->getSelectionStart();
    int selectionEnd = _textArea->getSelectionEnd();
    return userInputStart >= 0 && userInputEnd >= 0
            && selectionStart >= userInputStart
            && selectionEnd <= userInputEnd;
}

void QGConsoleWindow::loadConfiguration() {
    std::string configFile = getTempDirectory() + "/" + CONFIG_FILE_NAME;
    if (fileExists(configFile)) {
        std::ifstream infile;
        infile.open(configFile.c_str());
        if (!infile) {
                return;
        }
        std::string line;
        while (getline(infile, line)) {
            line = trim(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }
            Vector<std::string> tokens = stringSplit(line, "=");
            if (tokens.size() < 2) {
                continue;
            }
            std::string key   = toLowerCase(tokens[0]);
            std::string value = tokens[1];
            if (key == "font") {
                setFont(value);
            } else if (key == "background") {
                setBackground(value);
            } else if (key == "foreground") {
                setForeground(value);
            }
        }
    }
}

void QGConsoleWindow::loadInputScript(const std::string& /*filename*/) {
    // TODO
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
    QGThread::runOnQtGuiThreadAsync([this, str, isStdErr]() {
        _coutMutex.lock();
        this->_textArea->appendFormattedText(str, isStdErr ? getErrorColor() : getOutputColor());
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
            event.ignore();
            QFont font = QGFont::toQFont(_textArea->getFont());
            if (font.pointSize() + 1 <= MAX_FONT_SIZE) {
                font.setPointSize(font.pointSize() + 1);
                _textArea->setFont(QGFont::toFontString(font));
            }
        } else if (keyCode == Qt::Key_Minus) {
            // decrease font size
            event.ignore();
            QFont font = QGFont::toQFont(_textArea->getFont());
            if (font.pointSize() - 1 >= MIN_FONT_SIZE) {
                font.setPointSize(font.pointSize() - 1);
                _textArea->setFont(QGFont::toFontString(font));
            }
        } else if (keyCode == Qt::Key_Insert) {
            // Ctrl+Ins => Copy
            event.ignore();
            clipboardCopy();
        } else if (keyCode == Qt::Key_0) {
            // normalize font size
            event.ignore();
            _textArea->setFont(DEFAULT_FONT_FAMILY + "-" + integerToString(DEFAULT_FONT_SIZE));
        } else if (keyCode == Qt::Key_C) {
            event.ignore();
            clipboardCopy();
        } else if (event.isCtrlKeyDown() && keyCode == Qt::Key_D) {
            event.ignore();
            processEof();
        } else if (keyCode == Qt::Key_L) {
            event.ignore();
            clearConsole();
        } else if (keyCode == Qt::Key_Q || keyCode == Qt::Key_W) {
            event.ignore();
            close();
        } else if (keyCode == Qt::Key_S) {
            event.ignore();
            if (event.isShiftKeyDown()) {
                saveAs();
            } else {
                save();
            }
        } else if (keyCode == Qt::Key_V) {
            event.ignore();
            clipboardPaste();
        } else if (keyCode == Qt::Key_X) {
            event.ignore();
            clipboardCut();
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
        case QGEvent::BACKSPACE_KEY: {
            event.ignore();
            processBackspace(keyCode);
            break;
        }
        case QGEvent::DELETE_KEY: {
            event.ignore();
            if (event.isShiftKeyDown()) {
                clipboardCut();   // Shift+Del => Cut
            } else {
                processBackspace(keyCode);
            }
            break;
        }
        case QGEvent::INSERT_KEY: {
            event.ignore();
            if (event.isShiftKeyDown()) {
                clipboardPaste();   // Shift+Ins => Paste
            }
            break;
        }
        case QGEvent::HOME_KEY:
            if (ALLOW_RICH_INPUT_EDITING) {
                // move to start of input buffer
                if (_promptActive) {
                    event.ignore();
                    int start = getUserInputStart();
                    if (start >= 0) {
                        _textArea->setCursorPosition(
                                start,
                                /* keepAnchor */ event.isShiftKeyDown() && isCursorInUserInputArea());
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
                        _textArea->setCursorPosition(
                                end,
                                /* keepAnchor */ event.isShiftKeyDown() && isCursorInUserInputArea());
                    } else {
                        _textArea->moveCursorToEnd();
                    }
                }
            } else {
                event.ignore();
            }
            break;
        case QGEvent::LEFT_ARROW_KEY: {
            // bound within user input area if a prompt is active
            if (ALLOW_RICH_INPUT_EDITING) {
                if (isCursorInUserInputArea()) {
                    int cursorPosition = _textArea->getCursorPosition();
                    int userInputStart = getUserInputStart();
                    if (cursorPosition <= userInputStart) {
                        event.ignore();
                        if (!event.isShiftKeyDown()) {
                            _textArea->clearSelection();
                        }
                    }
                }
            } else {
                event.ignore();
            }
            break;
        }
        case QGEvent::RIGHT_ARROW_KEY:
            // bound within user input area if a prompt is active
            if (ALLOW_RICH_INPUT_EDITING) {
                if (isCursorInUserInputArea()) {
                    int cursorPosition = _textArea->getCursorPosition();
                    int userInputEnd   = getUserInputEnd();
                    if (cursorPosition >= userInputEnd) {
                        event.ignore();
                        if (!event.isShiftKeyDown()) {
                            _textArea->clearSelection();
                        }
                    }
                }
            } else {
                event.ignore();
            }
            break;
        case QGEvent::UP_ARROW_KEY:
            if (isCursorInUserInputArea()) {
                event.ignore();
                processCommandHistory(/* delta */ -1);
            }
            break;
        case QGEvent::DOWN_ARROW_KEY:
            if (isCursorInUserInputArea()) {
                event.ignore();
                processCommandHistory(/* delta */ 1);
            }
            break;
        case QGEvent::TAB_KEY:
            // TODO: tab completion?
        case QGEvent::CLEAR_KEY:
            break;
        case QGEvent::F1_KEY: {
            event.ignore();
            showAboutDialog();
            break;
        }
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
            int indexToDelete = (int) _inputBuffer.length() - 1;
            int userInputIndexMin = frag.position();
            int userInputIndexMax = frag.position() + frag.length() - (isBackspace ? 0 : 1);

            if (oldCursorPosition >= userInputIndexMin && oldCursorPosition < userInputIndexMax) {
                // cursor is inside the user input fragment;
                // figure out which character it's on so we can delete it
                indexToDelete = oldCursorPosition - frag.position() - (isBackspace ? 1 : 0);
            } else {
                // cursor is outside of the user input fragment; move it there
                cursor.setPosition(frag.position() + frag.length());
            }

            if (indexToDelete >= 0 && indexToDelete < (int) _inputBuffer.length()) {
                if (isBackspace || indexToDelete == (int) _inputBuffer.length() - 1) {
                    cursor.deletePreviousChar();
                } else {
                    cursor.deleteChar();   // Delete
                }

                // remove last char from internal input buffer
                _inputBuffer.erase(indexToDelete, 1);
            }
        }
    }
    _cinMutex.unlock();
}

void QGConsoleWindow::processCommandHistory(int delta) {
    _cinMutex.lockForRead();
    std::string oldCommand = "";
    _commandHistoryIndex += delta;
    _commandHistoryIndex = std::max(-1, _commandHistoryIndex);
    _commandHistoryIndex = std::min(_commandHistoryIndex, _inputCommandHistory.size());
    if (0 <= _commandHistoryIndex && _commandHistoryIndex < _inputCommandHistory.size()) {
        oldCommand = _inputCommandHistory[_commandHistoryIndex];
    }
    _cinMutex.unlock();
    setUserInput(oldCommand);
}

void QGConsoleWindow::processEof() {
    // only set EOF if input buffer is empty; this is the behavior on most *nix consoles
    if (_inputBuffer.empty()) {
        std::cin.setstate(std::ios_base::eofbit);
    }
}

void QGConsoleWindow::processUserInputEnterKey() {
    if (_shutdown) {
        return;
    }
    _cinMutex.lockForWrite();
    _cinQueueMutex.lockForWrite();
    _inputLines.enqueue(_inputBuffer);
    _inputCommandHistory.add(_inputBuffer);
    _commandHistoryIndex = _inputCommandHistory.size();
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

        bool inserted = false;
        if (ALLOW_RICH_INPUT_EDITING && isCursorInUserInputArea()) {
            QTextFragment frag = getUserInputFragment();
            if (frag.isValid()) {
                QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
                QTextCursor cursor(textArea->textCursor());

                int cursorPosition = cursor.position();
                int indexToInsert = cursorPosition - frag.position();
                if (indexToInsert == 0) {
                    // special case for inserting at start of fragment.
                    // example: fragment is "abcde", cursor at start, user types "x".
                    // if we just insert the "x" in the document, it won't be part of
                    // the same fragment and won't have the blue bold format.
                    // So what we do is temporarily insert it after the first character,
                    // then delete the first character, so that everything is inside
                    // the formatted span.
                    // "abcde"
                    //  ^
                    //   ^          move right by 1
                    // "axabcde"    insert "xa" at index 1
                    //     ^
                    //   ^          move left by 2
                    // "xabcde"     delete previous character "a" from index 0
                    //  ^
                    //   ^          move right by 1
                    cursor.beginEditBlock();
                    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);             // move to index 1
                    cursor.insertText(QString::fromStdString(keyStr + _inputBuffer.substr(0, 1)));   // insert new char + old first char
                    cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 2);              // delete old copy of first char
                    cursor.deletePreviousChar();
                    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);             // move to index 1
                    cursor.endEditBlock();
                    textArea->setTextCursor(cursor);
                } else {
                    cursor.beginEditBlock();
                    cursor.insertText(QString::fromStdString(keyStr));
                    cursor.endEditBlock();
                    textArea->setTextCursor(cursor);
                }
                _inputBuffer.insert(indexToInsert, keyStr);
                inserted = true;
            }
        }

        if (!inserted) {
            // append to end of buffer/fragment
            _inputBuffer += keyStr;
            // display in blue highlighted text
            this->_textArea->appendFormattedText(keyStr, USER_INPUT_COLOR, "*-*-Bold");
        }

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

    _cinMutex.lockForWrite();
    _promptActive = true;
    _cinMutex.unlock();

    while (!_shutdown && !std::cin.eof()) {
        bool lineRead = false;
        if (!_inputScript.isEmpty()) {
            _cinQueueMutex.lockForWrite();
            line = _inputScript.dequeue();
            lineRead = true;
            _cinQueueMutex.unlock();

            // echo user input, as if the user had just typed it
            QGThread::runOnQtGuiThreadAsync([this, line]() {
                _coutMutex.lock();
                _textArea->appendFormattedText(line + "\n", USER_INPUT_COLOR, "*-*-Bold");
                _coutMutex.unlock();
            });
        }

        if (!_inputLines.isEmpty()) {
            _cinQueueMutex.lockForWrite();
            if (!_inputLines.isEmpty()) {
                line = _inputLines.dequeue();
                lineRead = true;
            }

            _cinQueueMutex.unlock();
        }

        if (lineRead) {
            break;
        } else {
            sleep(20);
        }
    }

    _cinMutex.lockForWrite();
    _promptActive = false;
    _cinMutex.unlock();
    this->_textArea->scrollToBottom();

    if (_echo) {
        fprintf(stdout, "%s\n", line.c_str());
    }
    return line;
}

void QGConsoleWindow::save() {
    saveAs(_lastSaveFileName);
}

void QGConsoleWindow::saveAs(const std::string& filename) {
    std::string filenameToUse;
    if (filename.empty()) {
        filenameToUse = QGFileChooser::showSaveDialog(
                    /* parent */ this->getWidget(),
                    /* title */ "",
                    getHead(_lastSaveFileName));
    } else {
        filenameToUse = filename;
    }
    if (filenameToUse.empty()) {
        return;
    }

    std::string consoleText = _textArea->getText();
    writeEntireFile(filenameToUse, consoleText);
    _lastSaveFileName = filenameToUse;
}

void QGConsoleWindow::saveConfiguration(bool prompt) {
    if (prompt && !QGOptionPane::showConfirmDialog(
            /* parent  */  getWidget(),
            /* message */  "Make this the default for future console windows?",
            /* title   */  "Save configuration?")) {
        return;
    }
    std::string configFile = getTempDirectory() + "/" + CONFIG_FILE_NAME;
    std::string configText = "# Stanford C++ library configuration file\n"
            "background=" + _textArea->getBackground() + "\n"
            "foreground=" + getOutputColor() + "\n"
            "font=" + _textArea->getFont() + "\n";
    writeEntireFile(configFile, configText);
}

void QGConsoleWindow::selectAll() {
    _textArea->selectAll();
}

void QGConsoleWindow::setBackground(int color) {
    QGWindow::setBackground(color);   // call super
    _textArea->setBackground(color);
}

void QGConsoleWindow::setBackground(const std::string& color) {
    QGWindow::setBackground(color);   // call super
    _textArea->setBackground(color);
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

void QGConsoleWindow::setColor(int color) {
    setOutputColor(color);
}

void QGConsoleWindow::setColor(const std::string& color) {
    setOutputColor(color);
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
    QGWindow::setFont(font);   // call super
    _textArea->setFont(font);
}

void QGConsoleWindow::setForeground(int color) {
    setOutputColor(color);
}

void QGConsoleWindow::setForeground(const std::string& color) {
    setOutputColor(color);
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

void QGConsoleWindow::setOutputColor(int rgb) {
    setOutputColor(QGColor::convertRGBToColor(rgb));
}

void QGConsoleWindow::setOutputColor(const std::string& outputColor) {
    if (_locked) {
        return;
    }
    _outputColor = outputColor;
    _textArea->setForeground(outputColor);

    // go through any past fragments and recolor them to this color

    // select all previous text and change its color
    // (BUG?: also changes user input text to be that color; desired?)
    QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
    QTextCursor cursor = textArea->textCursor();
    cursor.beginEditBlock();
    cursor.setPosition(0);
    QTextCharFormat format = cursor.charFormat();
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    format.setForeground(QBrush(QGColor::toQColor(outputColor)));
    textArea->setTextCursor(cursor);
    cursor.setCharFormat(format);
    cursor.endEditBlock();
    _textArea->moveCursorToEnd();
}

void QGConsoleWindow::setUserInput(const std::string& userInput) {
    _cinMutex.lockForWrite();
    QTextEdit* textArea = static_cast<QTextEdit*>(_textArea->getWidget());

    // delete any current user input
    QTextFragment frag = getUserInputFragment();
    if (frag.isValid()) {
        QTextCursor cursor = textArea->textCursor();
        cursor.beginEditBlock();
        cursor.setPosition(frag.position(), QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, frag.length());
        cursor.removeSelectedText();
        cursor.endEditBlock();
        textArea->setTextCursor(cursor);
    }
    _inputBuffer.clear();
    _cinMutex.unlock();

    // insert the given user input
    for (int i = 0; i < (int) userInput.length(); i++) {
        processUserInputKey(userInput[i]);
    }
}

void QGConsoleWindow::showAboutDialog() {
    // this text roughly matches that from old spl.jar message
    static const std::string ABOUT_MESSAGE =
            "<html><p>"
            "Stanford C++ Library version <b>" + version::getCppLibraryVersion() + "</b><br>\n"
            "<br>\n"
            "Libraries originally written by <b>Eric Roberts</b>,<br>\n"
            "with assistance from Julie Zelenski, Keith Schwarz, et al.<br>\n"
            "This version of the library is unofficially maintained by <b>Marty Stepp</b>.<br>\n"
            "<br>\n"
            "See <a href=\"" + version::getCppLibraryDocsUrl() + "\">" + version::getCppLibraryDocsUrl() + "</a> for documentation."
            "</p></html>";
    QGOptionPane::showMessageDialog(
                /* parent */   getWidget(),
                /* message */  ABOUT_MESSAGE,
                /* title */    "About Stanford C++ Library",
                /* type */     QGOptionPane::ABOUT);
}

void QGConsoleWindow::showColorDialog(bool background) {
    std::string color = QGColorChooser::showDialog(
                /* parent */   getWidget(),
                /* title */    "",
                /* initial */  background ? _textArea->getBackground() : _textArea->getForeground());
    if (!color.empty()) {
        if (background) {
            setBackground(color);
        } else {
            setOutputColor(color);
        }
        saveConfiguration();   // prompt to save configuration
    }
}

void QGConsoleWindow::showFontDialog() {
    std::string font = QGFontChooser::showDialog(
                /* parent */ getWidget(),
                /* title  */ "",
                /* initialFont */ _textArea->getFont());
    if (!font.empty()) {
        _textArea->setFont(font);
        saveConfiguration();   // prompt to save configuration
    }
}

void QGConsoleWindow::showInputScriptDialog() {
    std::string filename = QGFileChooser::showOpenDialog(
                /* parent */ getWidget(),
                /* title  */ "Select an input script file");
    if (!filename.empty() && fileExists(filename)) {
        std::ifstream infile;
        infile.open(filename.c_str());
        Vector<std::string> lines;
        readEntireFile(infile, lines);

        _cinQueueMutex.lockForWrite();
        _inputScript.clear();
        for (std::string line : lines) {
            _inputScript.enqueue(line);
        }
        _cinQueueMutex.unlock();
    }
}

void QGConsoleWindow::showPrintDialog() {
    // TODO
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

#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    setConsolePrintExceptions(true);
#endif

#if defined(SPL_VERIFY_PROJECT_VERSION)
    version::ensureProjectVersion();
#endif
}

void initializeQtGraphicalConsole() {
#ifndef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    // ensure that console is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

    // declaring this object ensures that std::cin, cout, cerr are initialized
    // properly before our lib tries to mess with them / redirect them
    static std::ios_base::Init ios_base_init;

    QGConsoleWindow::instance();   // ensure that console window is ready
    setConsolePropertiesQt();
#endif // __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
}

// This one is at the bottom because it's not meant to be called by students.
void setConsoleEnabled(bool enabled) {
    QGConsoleWindow::setConsoleEnabled(enabled);
}

} // namespace qtgui

} // namespace stanfordcpplib

#endif // SPL_QT_GUI
