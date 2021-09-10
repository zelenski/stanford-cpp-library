/*
 * File: gconsolewindow.cpp
 * ------------------------
 * This file implements the gconsolewindow.h interface.
 *
 * @author Marty Stepp
 * @version 2019/04/25
 * - added hasInputScript
 * @version 2019/04/16
 * - bug fix for wrong text color on Mac dark mode
 * @version 2019/04/10
 * - toolbar support with icons from icon strip image
 * @version 2019/04/09
 * - bug fix for premature input script / compare output popup
 * - changed default Mac font to Courier New from Menlo
 * @version 2018/12/27
 * - bug fix for endless waitForEvent queued events caused by printing text
 *   to console (bug reported by Keith Schwarz)
 * @version 2018/10/11
 * - bug fixes for shutdown flag, input script hotkeys (e.g. Ctrl+1)
 * @version 2018/10/04
 * - changed default line wrap to true
 * @version 2018/09/27
 * - bug fix for printing strings with line breaks (remove \r, favor \n)
 * @version 2018/09/23
 * - added getFont
 * - bug fix for loading input scripts
 * - bug fix for default font on Mac
 * @version 2018/09/18
 * - window size/location fixes
 * @version 2018/09/17
 * - fixes for monospaced font on Mac OS X
 * @version 2018/08/23
 * - initial version, separated out from console.cpp
 */

#include "gconsolewindow.h"
#include <cstdio>
#include <QAction>
#include <QTextDocumentFragment>
#include "error.h"
#include "exceptions.h"
#include "filelib.h"
#include "gclipboard.h"
#include "gcolor.h"
#include "gcolorchooser.h"
#include "gfilechooser.h"
#include "gfont.h"
#include "gfontchooser.h"
#include "goptionpane.h"
#include "gthread.h"
#include "os.h"
#include "qtgui.h"
#include "private/static.h"
#include "private/version.h"

void setConsolePropertiesQt();

/*static*/ const bool GConsoleWindow::ALLOW_RICH_INPUT_EDITING = true;
/*static*/ const double GConsoleWindow::DEFAULT_WIDTH = 900;
/*static*/ const double GConsoleWindow::DEFAULT_HEIGHT = 550;
/*static*/ const double GConsoleWindow::DEFAULT_X = 10;
/*static*/ const double GConsoleWindow::DEFAULT_Y = 40;
/*static*/ const std::string GConsoleWindow::CONFIG_FILE_NAME = "spl-jar-settings.txt";
/*static*/ const std::string GConsoleWindow::DEFAULT_FONT_FAMILY = "Monospace";
/*static*/ const std::string GConsoleWindow::DEFAULT_FONT_WEIGHT = "";
/*static*/ const int GConsoleWindow::DEFAULT_FONT_SIZE = 12;
/*static*/ const int GConsoleWindow::MIN_FONT_SIZE = 4;
/*static*/ const int GConsoleWindow::MAX_FONT_SIZE = 255;
/*static*/ const std::string GConsoleWindow::DEFAULT_ERROR_COLOR = "#cc0000";
/*static*/ const std::string GConsoleWindow::DEFAULT_ERROR_COLOR_DARK_MODE = "#f47862";
/*static*/ const std::string GConsoleWindow::DEFAULT_USER_INPUT_COLOR = "#0000cc";
/*static*/ const std::string GConsoleWindow::DEFAULT_USER_INPUT_COLOR_DARK_MODE = "#2c90e5";
/*static*/ GConsoleWindow* GConsoleWindow::_instance = nullptr;
/*static*/ bool GConsoleWindow::_consoleEnabled = false;

/*static*/ bool GConsoleWindow::consoleEnabled() {
    return _consoleEnabled;
}

/*static*/ std::string GConsoleWindow::getDefaultFont() {
    if (OS::isMac()) {
        // for some reason, using "Monospace" doesn't work for me on Mac testing
        return "Courier New-"
                + std::to_string(DEFAULT_FONT_SIZE + 1)
                + (DEFAULT_FONT_WEIGHT.empty() ? "" : ("-" + DEFAULT_FONT_WEIGHT));
    } else {
        return DEFAULT_FONT_FAMILY
                + "-" + std::to_string(DEFAULT_FONT_SIZE)
                + (DEFAULT_FONT_WEIGHT.empty() ? "" : ("-" + DEFAULT_FONT_WEIGHT));
    }
}

/*static*/ GConsoleWindow* GConsoleWindow::instance() {
    if (!_instance) {
        // initialize Qt system and Qt Console window
        GThread::runOnQtGuiThread([]() {
            if (!_instance) {
                QtGui::instance()->initializeQt();
                _instance = new GConsoleWindow();
                setConsolePropertiesQt();
            }
        });
    }
    return _instance;
}

/*static*/ bool GConsoleWindow::isInitialized() {
    return _instance != nullptr;
}

/*static*/ void GConsoleWindow::setConsoleEnabled(bool enabled) {
    _consoleEnabled = enabled;
}

GConsoleWindow::GConsoleWindow()
        : GWindow(/* visible */ false),
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
          _userInputColor(""),
          _inputBuffer(""),
          _lastSaveFileName(""),
          _cinout_new_buf(nullptr),
          _cerr_new_buf(nullptr),
          _cin_old_buf(nullptr),
          _cout_old_buf(nullptr),
          _cerr_old_buf(nullptr) {
    _initMenuBar();
    _initWidgets();
    _initStreams();
    loadConfiguration();
}

void GConsoleWindow::_initMenuBar() {
    addToolbar();

    // File menu
    addMenu("&File");
    addMenuItem("File", "&Save", QPixmap(":/save"),
                [this]() { this->save(); })
                ->setShortcut(QKeySequence::Save);

    addMenuItem("File", "Save &As...", QPixmap(":/save_as"),
                [this]() { this->saveAs(); })
                ->setShortcut(QKeySequence::SaveAs);
    addMenuSeparator("File");

    addMenuItem("File", "&Print", QPixmap(":/print"),
                [this]() { this->showPrintDialog(); })
                ->setShortcut(QKeySequence::Print);
    setMenuItemEnabled("File", "Print", false);
    addMenuSeparator("File");

    addMenuItem("File", "&Quit", QPixmap(":/quit"),
                [this]() { this->close(); /* TODO: exit app */ })
                ->setShortcut(QKeySequence::Quit);

    // Edit menu
    addMenu("&Edit");
    addMenuItem("Edit", "Cu&t", QPixmap(":/cut"),
                [this]() { this->clipboardCut(); })
                ->setShortcut(QKeySequence::Cut);
    //   no cut operation available for Console, do not add to toolbar

    addMenuItem("Edit", "&Copy", QPixmap(":/copy"),
                [this]() { this->clipboardCopy(); })
                ->setShortcut(QKeySequence::Copy);
    addToolbarItem("Copy", QPixmap(":/copy"),
                       [this]() { this->clipboardCopy(); });

    addMenuItem("Edit", "&Paste", QPixmap(":/paste"),
                [this]() { this->clipboardPaste(); })
                ->setShortcut(QKeySequence::Paste);
    addToolbarItem("Paste", QPixmap(":/paste"),
                       [this]() { this->clipboardPaste(); });

    addMenuItem("Edit", "Select &All", QPixmap(":/select_all"),
                [this]() { this->selectAll(); })
                ->setShortcut(QKeySequence::SelectAll);

    addMenuItem("Edit", "C&lear Console", QPixmap(":/clear_console"),
                [this]() { this->clearConsole(); })
                ->setShortcut(QKeySequence(QString::fromStdString("Ctrl+L")));
    addToolbarItem("Clear Console", QPixmap(":/clear_console"),
                       [this]() { this->clearConsole(); });
    addToolbarSeparator();

    // Options menu
    addMenu("&Options");
    addMenuItem("Options", "&Font...", QPixmap(":/font"),
                [this]() { this->showFontDialog(); });
    addToolbarItem("Font...", QPixmap(":/font"),
                       [this]() { this->showFontDialog(); });

    addMenuItem("Options", "&Background Color...", QPixmap(":/background_color"),
                [this]() { this->showColorDialog(/* background */ true); });
    addToolbarItem("Background Color...", QPixmap(":/background_color"),
                       [this]() { this->showColorDialog(/* background */ true); });

    addMenuItem("Options", "&Text Color...", QPixmap(":/text_color"),
                [this]() { this->showColorDialog(/* background */ false); });
    addToolbarItem("Text Color...", QPixmap(":/text_color"),
                       [this]() { this->showColorDialog(/* background */ false); });
    addToolbarSeparator();

    // Help menu
    addMenu("&Help");
    addMenuItem("Help", "&About...", QPixmap(":/about"),
                [this]() { this->showAboutDialog(); })
                ->setShortcut(QKeySequence::HelpContents);
    addToolbarItem("About...", QPixmap(":/about"),
                       [this]() { this->showAboutDialog(); });

}

void GConsoleWindow::_initStreams() {
    // buffer C-style stderr
    static char stderrBuf[BUFSIZ + 10] = {'\0'};
    std::ios::sync_with_stdio(false);
    setbuf(stderr, stderrBuf);

    // redirect cin/cout/cerr
    _cinout_new_buf = new stanfordcpplib::qtgui::ConsoleStreambufQt();
    _cerr_new_buf = new stanfordcpplib::qtgui::ConsoleStreambufQt(/* isStderr */ true);
    _cin_old_buf = std::cin.rdbuf(_cinout_new_buf);
    _cout_old_buf = std::cout.rdbuf(_cinout_new_buf);
    _cerr_old_buf = std::cerr.rdbuf(_cerr_new_buf);
}

void GConsoleWindow::_initWidgets() {
    _textArea = new GTextArea();
    _outputColor = _textArea->getColor();

    // BUGFIX: use OS defaults for BG/FG colors (helps w/ Mac dark mode)
    _textArea->setBackground(GWindow::getDefaultInteractorBackgroundColorInt());
    _textArea->setColor(GWindow::getDefaultInteractorTextColorInt());
    _textArea->setContextMenuEnabled(false);
    _textArea->setLineWrap(true);
    _textArea->setFont(getDefaultFont());
    QTextEdit* rawTextEdit = static_cast<QTextEdit*>(_textArea->getWidget());
    rawTextEdit->setTabChangesFocus(false);
    _textArea->setKeyListener([this](GEvent event) {
        if (event.getEventType() == KEY_PRESSED) {
            this->processKeyPress(event);
        } else if (event.getEventType() == KEY_RELEASED
                   || event.getEventType() == KEY_TYPED) {
            event.ignore();
        }
    });
    _textArea->setMouseListener([](GEvent event) {
        // snuff out mouse-based operations:
        // - popping up context menu by right-clicking
        // - Linux-style copy/paste operations using selection plus middle-click
        if (event.getButton() > 1
                || event.getEventType() == MOUSE_RELEASED) {
            event.ignore();
        }
    });
    addToRegion(_textArea, "Center");

    // tell window to shut down when it is closed
    setWindowListener([this](GEvent event) {
        if (event.getEventType() == WINDOW_CLOSING) {
            shutdown("Close");
        }
    });

    setTitle(QCoreApplication::applicationName().toStdString() + " Console");
    setCloseOperation(GWindow::CLOSE_HIDE);
    setLocation(DEFAULT_X, DEFAULT_Y);
    setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setVisible(true);
}


GConsoleWindow::~GConsoleWindow() {
    // TODO: delete?
    _cinout_new_buf = nullptr;
    _cerr_new_buf = nullptr;
    _cin_old_buf = nullptr;
    _cout_old_buf = nullptr;
    _cerr_old_buf = nullptr;
}


void GConsoleWindow::clearConsole() {
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

void GConsoleWindow::clipboardCopy() {
    std::string selectedText = _textArea->getSelectedText();
    if (!selectedText.empty()) {
        GClipboard::set(selectedText);
    }
}

void GConsoleWindow::clipboardCut() {
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
            GClipboard::set(selectedText);
        }
    }
}

void GConsoleWindow::clipboardPaste() {
    if (_shutdown) {
        return;
    }

    _textArea->clearSelection();
    if (!isCursorInUserInputArea()) {
        _textArea->moveCursorToEnd();
    }

    std::string clipboardText = GClipboard::get();
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

void GConsoleWindow::close() {
    shutdown("Close");
    GWindow::close();   // call super
}

std::string GConsoleWindow::getBackground() const {
    return _textArea->getBackground();
}

int GConsoleWindow::getBackgroundInt() const {
    return _textArea->getBackgroundInt();
}

std::string GConsoleWindow::getColor() const {
    return getOutputColor();
}

int GConsoleWindow::getColorInt() const {
    return GColor::convertColorToRGB(getOutputColor());
}

std::string GConsoleWindow::getErrorColor() const {
    if (!_errorColor.empty()) {
        return _errorColor;
    } else {
        return GWindow::isDarkMode() ? DEFAULT_ERROR_COLOR_DARK_MODE : DEFAULT_ERROR_COLOR;
    }
}

std::string GConsoleWindow::getFont() const {
    return _textArea->getFont();
}

std::string GConsoleWindow::getForeground() const {
    return getOutputColor();
}

int GConsoleWindow::getForegroundInt() const {
    return GColor::convertColorToRGB(getOutputColor());
}

std::string GConsoleWindow::getOutputColor() const {
    return _outputColor.empty() ? GWindow::getDefaultInteractorTextColor() : _outputColor;
}

std::string GConsoleWindow::getUserInputColor() const {
    if (!_userInputColor.empty()) {
        return _userInputColor;
    } else {
        return GWindow::isDarkMode() ? DEFAULT_USER_INPUT_COLOR_DARK_MODE : DEFAULT_USER_INPUT_COLOR;
    }
}

QTextFragment GConsoleWindow::getUserInputFragment() const {
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

int GConsoleWindow::getUserInputStart() const {
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

int GConsoleWindow::getUserInputEnd() const {
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

bool GConsoleWindow::isClearEnabled() const {
    return _clearEnabled;
}

bool GConsoleWindow::isCursorInUserInputArea() const {
    int cursorPosition = _textArea->getCursorPosition();
    int userInputStart = getUserInputStart();
    int userInputEnd   = getUserInputEnd();
    return _promptActive
            && userInputStart <= cursorPosition
            && cursorPosition <= userInputEnd;
}

bool GConsoleWindow::isEcho() const {
    return _echo;
}

bool GConsoleWindow::isLocationSaved() const {
    return _locationSaved;
}

bool GConsoleWindow::isLocked() const {
    return _locked;
}

bool GConsoleWindow::isSelectionInUserInputArea() const {
    int userInputStart = getUserInputStart();
    int userInputEnd   = getUserInputEnd();
    int selectionStart = _textArea->getSelectionStart();
    int selectionEnd = _textArea->getSelectionEnd();
    return userInputStart >= 0 && userInputEnd >= 0
            && selectionStart >= userInputStart
            && selectionEnd <= userInputEnd;
}

void GConsoleWindow::loadConfiguration() {
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



void GConsoleWindow::print(const std::string& str, bool isStdErr) {
    if (_echo) {
        fflush(isStdErr ? stdout : stderr);
        fflush(isStdErr ? stderr : stdout);
        fprintf(isStdErr ? stderr : stdout, "%s", str.c_str());
        if (str.find("\n") != std::string::npos) {
            fflush(isStdErr ? stderr : stdout);
            fflush(isStdErr ? stdout : stderr);
        }
    }

    // clean up line breaks (remove \r)
    std::string strToPrint = str;
    stringReplaceInPlace(strToPrint, "\r\n", "\n");
    stringReplaceInPlace(strToPrint, "\r", "\n");

    GThread::runOnQtGuiThread([this, strToPrint, isStdErr]() {
        _coutMutex.lock();
        _allOutputBuffer << strToPrint;
        if (!this->_textArea) {
            return;
        }
        this->_textArea->setEventsEnabled(false);
        this->_textArea->appendFormattedText(strToPrint, isStdErr ? getErrorColor() : getOutputColor());
        this->_textArea->moveCursorToEnd();
        this->_textArea->scrollToBottom();
        this->_textArea->setEventsEnabled(true);
        _coutMutex.unlock();
    });
}

void GConsoleWindow::println(bool isStdErr) {
    print("\n", isStdErr);
}

void GConsoleWindow::println(const std::string& str, bool isStdErr) {
    print(str + "\n", isStdErr);
}

void GConsoleWindow::processKeyPress(GEvent event) {
    char key = event.getKeyChar();
    int keyCode = event.getKeyCode();

    if (event.isCtrlOrCommandKeyDown()) {
        if (keyCode == Qt::Key_Plus || keyCode == Qt::Key_Equal) {
            // increase font size
            event.ignore();
            QFont font = GFont::toQFont(_textArea->getFont());
            if (font.pointSize() + 1 <= MAX_FONT_SIZE) {
                font.setPointSize(font.pointSize() + 1);
                setFont(GFont::toFontString(font));
            }
        } else if (keyCode == Qt::Key_Minus) {
            // decrease font size
            event.ignore();
            QFont font = GFont::toQFont(_textArea->getFont());
            if (font.pointSize() - 1 >= MIN_FONT_SIZE) {
                font.setPointSize(font.pointSize() - 1);
                setFont(GFont::toFontString(font));
            }
        } else if (keyCode == Qt::Key_Insert) {
            // Ctrl+Ins => Copy
            event.ignore();
            clipboardCopy();
        } else if (keyCode == Qt::Key_0) {
            // normalize font size
            event.ignore();
            setFont(DEFAULT_FONT_FAMILY + "-" + std::to_string(DEFAULT_FONT_SIZE));
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
        case GEvent::PAGE_UP_KEY:
        case GEvent::PAGE_DOWN_KEY:
            // don't ignore event
            break;
        case GEvent::BACKSPACE_KEY: {
            event.ignore();
            processBackspace(keyCode);
            break;
        }
        case GEvent::DELETE_KEY: {
            event.ignore();
            if (event.isShiftKeyDown()) {
                clipboardCut();   // Shift+Del => Cut
            } else {
                processBackspace(keyCode);
            }
            break;
        }
        case GEvent::INSERT_KEY: {
            event.ignore();
            if (event.isShiftKeyDown()) {
                clipboardPaste();   // Shift+Ins => Paste
            }
            break;
        }
        case GEvent::HOME_KEY:
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
        case GEvent::END_KEY:
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
        case GEvent::LEFT_ARROW_KEY: {
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
        case GEvent::RIGHT_ARROW_KEY:
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
        case GEvent::UP_ARROW_KEY:
            if (isCursorInUserInputArea()) {
                event.ignore();
                processCommandHistory(/* delta */ -1);
            }
            break;
        case GEvent::DOWN_ARROW_KEY:
            if (isCursorInUserInputArea()) {
                event.ignore();
                processCommandHistory(/* delta */ 1);
            }
            break;
        case GEvent::TAB_KEY:
            // TODO: tab completion?
        case GEvent::CLEAR_KEY:
            break;
        case GEvent::F1_KEY: {
            event.ignore();
            showAboutDialog();
            break;
        }
        case GEvent::F2_KEY:
        case GEvent::F3_KEY:
        case GEvent::F4_KEY:
        case GEvent::F5_KEY:
        case GEvent::F6_KEY:
        case GEvent::F7_KEY:
        case GEvent::F8_KEY:
        case GEvent::F9_KEY:
        case GEvent::F10_KEY:
        case GEvent::F11_KEY:
        case GEvent::F12_KEY:
        case GEvent::HELP_KEY: {
            // various control/modifier keys: do nothing / consume event
            event.ignore();
            break;
        }
        case GEvent::SHIFT_KEY:
        case GEvent::CTRL_KEY:
        case GEvent::ALT_KEY:
        case GEvent::PAUSE_KEY:
        case GEvent::CAPS_LOCK_KEY:
        case GEvent::ESCAPE_KEY:
        case GEvent::NUM_LOCK_KEY:
        case GEvent::SCROLL_LOCK_KEY:
        case GEvent::PRINT_SCREEN_KEY:
        case GEvent::META_KEY:
        case GEvent::WINDOWS_KEY:
        case GEvent::MENU_KEY: {
            // various other control/modifier keys: let OS have the event (don't call ignore())
            break;
        }
        case GEvent::RETURN_KEY:
        case GEvent::ENTER_KEY: {
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

void GConsoleWindow::processBackspace(int key) {
    if (_shutdown || !_promptActive) {
        return;
    }

    // check whether it is a backspace or a delete
    bool isBackspace = key == GEvent::BACKSPACE_KEY /* TODO: or computer is Mac */;

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

void GConsoleWindow::processCommandHistory(int delta) {
    if (_shutdown) {
        return;
    }
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

void GConsoleWindow::processEof() {
    if (_shutdown) {
        return;
    }
    // only set EOF if input buffer is empty; this is the behavior on most *nix consoles
    if (_inputBuffer.empty()) {
        std::cin.setstate(std::ios_base::eofbit);
    }
}

void GConsoleWindow::processUserInputEnterKey() {
    if (_shutdown) {
        return;
    }
    _cinMutex.lockForWrite();
    _cinQueueMutex.lockForWrite();
    _inputLines.enqueue(_inputBuffer);
    _inputCommandHistory.add(_inputBuffer);
    _commandHistoryIndex = _inputCommandHistory.size();
    _cinQueueMutex.unlock();
    _allOutputBuffer << _inputBuffer << std::endl;
    _inputBuffer = "";   // clear input buffer
    this->_textArea->appendFormattedText("\n", getUserInputColor());
    _cinMutex.unlock();
}

void GConsoleWindow::processUserInputKey(int key) {
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

                // BUGFIX: if there is any selected text, remove it first
                int fragStart = frag.position();
                int selectionStart = cursor.selectionStart() - fragStart;
                int selectionEnd = cursor.selectionEnd() - fragStart;
                if (selectionEnd > selectionStart
                        && selectionStart >= 0
                        && selectionEnd <= (int) _inputBuffer.length()) {
                    cursor.removeSelectedText();
                    _inputBuffer.erase(selectionStart, selectionEnd - selectionStart);
                }

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
            this->_textArea->appendFormattedText(keyStr, getUserInputColor(), "*-*-Bold");
        }

        _cinMutex.unlock();
    }
}

std::string GConsoleWindow::readLine() {
    // TODO: threads/locking
    // wait for a line to be available in queue
    std::string line;
    if (_shutdown) {
        return line;
    }

    this->_textArea->moveCursorToEnd();
    this->_textArea->scrollToBottom();
    this->toFront();   // move window to front on prompt for input
    this->_textArea->requestFocus();

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
            GThread::runOnQtGuiThreadAsync([this, line]() {
                _coutMutex.lock();
                _allOutputBuffer << line << std::endl;
                _textArea->appendFormattedText(line + "\n", getUserInputColor(), "*-*-Bold");
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

void GConsoleWindow::save() {
    saveAs(_lastSaveFileName);
}

void GConsoleWindow::saveAs(const std::string& filename) {
    std::string filenameToUse;
    if (filename.empty()) {
        filenameToUse = GFileChooser::showSaveDialog(
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

void GConsoleWindow::saveConfiguration(bool prompt) {
    if (prompt && !GOptionPane::showConfirmDialog(
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

void GConsoleWindow::selectAll() {
    _textArea->selectAll();
}

void GConsoleWindow::setBackground(int color) {
    GWindow::setBackground(color);   // call super
    _textArea->setBackground(color);
}

void GConsoleWindow::setBackground(const std::string& color) {
    GWindow::setBackground(color);   // call super
    _textArea->setBackground(color);
}

void GConsoleWindow::setClearEnabled(bool clearEnabled) {
    if (_locked || _shutdown) {
        return;
    }
    _clearEnabled = clearEnabled;
}

void GConsoleWindow::setConsoleSize(double width, double height) {
    // TODO: base on text area's preferred size / packing window
    // _textArea->setPreferredSize(width, height);
    // pack();
    setSize(width, height);
}

void GConsoleWindow::setColor(int color) {
    setOutputColor(color);
}

void GConsoleWindow::setColor(const std::string& color) {
    setOutputColor(color);
}

void GConsoleWindow::setEcho(bool echo) {
    if (_locked || _shutdown) {
        return;
    }
    _echo = echo;
}

void GConsoleWindow::setFont(const QFont& font) {
    GWindow::setFont(font);   // call super
    _textArea->setFont(font);
}

void GConsoleWindow::setFont(const std::string& font) {
    GWindow::setFont(font);   // call super
    _textArea->setFont(font);
}

void GConsoleWindow::setForeground(int color) {
    setOutputColor(color);
}

void GConsoleWindow::setForeground(const std::string& color) {
    setOutputColor(color);
}

void GConsoleWindow::setLocationSaved(bool locationSaved) {
    _locationSaved = locationSaved;
}

void GConsoleWindow::setLocked(bool locked) {
    _locked = locked;
}

void GConsoleWindow::setErrorColor(const std::string& errorColor) {
    _errorColor = errorColor;
}

void GConsoleWindow::setOutputColor(int rgb) {
    setOutputColor(GColor::convertRGBToColor(rgb));
}

void GConsoleWindow::setOutputColor(const std::string& outputColor) {
    _outputColor = outputColor;
    _textArea->setForeground(outputColor);
    if (_shutdown) {
        return;
    }

    // go through any past fragments and recolor them to this color

    // select all previous text and change its color
    // (BUG?: also changes user input text to be that color; desired?)
    QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
    QTextCursor cursor = textArea->textCursor();
    cursor.beginEditBlock();
    cursor.setPosition(0);
    QTextCharFormat format = cursor.charFormat();
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    format.setForeground(QBrush(GColor::toQColor(outputColor)));
    textArea->setTextCursor(cursor);
    cursor.setCharFormat(format);
    cursor.endEditBlock();
    _textArea->moveCursorToEnd();
}

void GConsoleWindow::setSize(double width, double height) {
    if (isHighDpiScalingEnabled() && isHighDensityScreen()) {
        double ratio = getScreenDpiScaleRatio();
        width = std::min(getScreenWidth(), width * ratio);
        height = std::min(getScreenHeight(), height * ratio);;
    }

    // call super
    GWindow::setSize(width, height);
}

void GConsoleWindow::setUserInput(const std::string& userInput) {
    if (_shutdown) {
        return;
    }
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

void GConsoleWindow::setUserInputColor(const std::string& userInputColor) {
    _userInputColor = userInputColor;
}

void GConsoleWindow::showAboutDialog() {
    // this text roughly matches that from old spl.jar message
    static const std::string ABOUT_MESSAGE = version::getLibraryInfoPanelMessage();
    GOptionPane::showMessageDialog(
                /* parent */   getWidget(),
                /* message */  ABOUT_MESSAGE,
                /* title */    "About Stanford C++ Library",
                /* type */     GOptionPane::MESSAGE_ABOUT);
}

void GConsoleWindow::showColorDialog(bool background) {
    std::string color = GColorChooser::showDialog(
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

void GConsoleWindow::showFontDialog() {
    std::string font = GFontChooser::showDialog(
                /* parent */ getWidget(),
                /* title  */ "",
                /* initialFont */ _textArea->getFont());
    if (!font.empty()) {
        _textArea->setFont(font);
        saveConfiguration();   // prompt to save configuration
    }
}

void GConsoleWindow::showPrintDialog() {
    // TODO
}

void GConsoleWindow::shutdown(const std::string& reason) {
    _shutdown = true;
    std::cout.flush();
    std::cerr.flush();

    // restore old cin, cout, cerr
    if (_cin_old_buf) {
        _coutMutex.lock();
        std::cin.rdbuf(_cin_old_buf);
        std::cout.rdbuf(_cout_old_buf);
        std::cerr.rdbuf(_cerr_old_buf);
        _cin_old_buf = nullptr;
        _cout_old_buf = nullptr;
        _cerr_old_buf = nullptr;
        std::cout.flush();
        std::cerr.flush();
        _coutMutex.unlock();
    }

    _textArea->setEditable(false);
    std::string title = getTitle();
    if (title.find(reason) == std::string::npos) {
        setTitle(title + " [" + reason + "]");
    }

    // TODO: disable some menu items
}

// global functions used by ConsoleStreambufQt

namespace stanfordcpplib {
namespace qtgui {

void endLineConsoleQt(bool isStderr) {
    GConsoleWindow::instance()->println(isStderr);
}

std::string getLineConsoleQt() {
    return GConsoleWindow::instance()->readLine();
}

void putConsoleQt(const std::string& str, bool isStderr) {
    GConsoleWindow::instance()->print(str, isStderr);
}

} // namespace qtgui
} // namespace stanfordcpplib
