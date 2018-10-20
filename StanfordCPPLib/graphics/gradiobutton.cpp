/*
 * File: gradiobutton.cpp
 * ----------------------
 *
 * @author Marty Stepp
 * @version 2018/10/06
 * - added toggle()
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gradiobutton.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gradiobutton.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

Map<std::string, QButtonGroup*> GRadioButton::_buttonGroups;

GRadioButton::GRadioButton(const std::string& text, const std::string& group, bool checked, QWidget* parent) {
    GThread::runOnQtGuiThread([this, text, group, checked, parent]() {
        _iqradioButton = new _Internal_QRadioButton(this, checked, getInternalParent(parent));
        QButtonGroup* buttonGroup = getButtonGroup(group);
        buttonGroup->addButton(_iqradioButton);
    });
    setText(text);
    setVisible(false);   // all widgets are not shown until added to a window
}

GRadioButton::~GRadioButton() {
    // TODO: delete _iqradioButton;
    _iqradioButton = nullptr;
}

std::string GRadioButton::getActionCommand() const {
    if (_actionCommand.empty()) {
        return getText();
    } else {
        return _actionCommand;
    }
}

_Internal_QWidget* GRadioButton::getInternalWidget() const {
    return _iqradioButton;
}

std::string GRadioButton::getText() const {
    return _iqradioButton->text().toStdString();
}

std::string GRadioButton::getType() const {
    return "GRadioButton";
}

QWidget* GRadioButton::getWidget() const {
    return static_cast<QWidget*>(_iqradioButton);
}

bool GRadioButton::isChecked() const {
    return _iqradioButton->isChecked();
}

bool GRadioButton::isSelected() const {
    return _iqradioButton->isChecked();
}

void GRadioButton::removeActionListener() {
    removeEventListener("change");
}

void GRadioButton::removeDoubleClickListener() {
    removeEventListener("doubleclick");
}

void GRadioButton::setActionListener(GEventListener func) {
    setEventListener("change", func);
}

void GRadioButton::setActionListener(GEventListenerVoid func) {
    setEventListener("change", func);
}

void GRadioButton::setDoubleClickListener(GEventListener func) {
    setEventListener("doubleclick", func);
}

void GRadioButton::setDoubleClickListener(GEventListenerVoid func) {
    setEventListener("doubleclick", func);
}

void GRadioButton::setChecked(bool checked) {
    GThread::runOnQtGuiThread([this, checked]() {
        _iqradioButton->setChecked(checked);
    });
}

void GRadioButton::setSelected(bool selected) {
    setChecked(selected);
}

void GRadioButton::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqradioButton->setText(QString::fromStdString(text));
    });
}

void GRadioButton::toggle() {
    setChecked(!isChecked());
}

/* static */ QButtonGroup* GRadioButton::getButtonGroup(const std::string& group) {
    if (!_buttonGroups.containsKey(group)) {
        GThread::runOnQtGuiThread([group]() {
            _buttonGroups.put(group, new QButtonGroup());
        });
    }
    return _buttonGroups[group];
}


_Internal_QRadioButton::_Internal_QRadioButton(GRadioButton* gradioButton, bool checked, QWidget* parent)
        : QRadioButton(parent),
          _gradioButton(gradioButton) {
    require::nonNull(gradioButton, "_Internal_QRadioButton::constructor");
    setObjectName(QString::fromStdString("_Internal_QRadioButton_" + std::to_string(gradioButton->getID())));
    setChecked(checked);
    // We handle the clicked signal rather than toggled because, in a radio button group,
    // the toggled signal will fire twice: once for the radio button clicked, and once
    // for the other button that was unchecked.
    connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

void _Internal_QRadioButton::handleClick() {
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gradioButton);
    changeEvent.setActionCommand(_gradioButton->getActionCommand());
    _gradioButton->fireEvent(changeEvent);
}

void _Internal_QRadioButton::mouseDoubleClickEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QRadioButton::mouseDoubleClickEvent");
    QWidget::mouseDoubleClickEvent(event);   // call super
    emit doubleClicked();
    if (!_gradioButton->isAcceptingEvent("doubleclick")) return;
    GEvent mouseEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ MOUSE_DOUBLE_CLICKED,
                /* name   */ "doubleclick",
                /* source */ _gradioButton);
    mouseEvent.setActionCommand(_gradioButton->getActionCommand());
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    _gradioButton->fireEvent(mouseEvent);
}

QSize _Internal_QRadioButton::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QRadioButton::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gradiobutton.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES
