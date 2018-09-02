/*
 * File: gchooser.h
 * ----------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gchooser.h to replace Java version
 * @version 2018/06/28
 * - initial version
 */

#ifndef _gchooser_h
#define _gchooser_h

#include <initializer_list>
#include <string>
#include <QComboBox>
#include <QSize>
#include <QWidget>
#include "ginteractor.h"
#include "vector.h"

class _Internal_QComboBox;

/*
 * ...
 */
class GChooser : public GInteractor {
public:
    GChooser(QWidget* parent = nullptr);
    GChooser(const std::initializer_list<std::string>& items, QWidget* parent = nullptr);
    GChooser(const Vector<std::string>& items, QWidget* parent = nullptr);
    virtual ~GChooser();

    virtual void addItem(const std::string& item);
    virtual void addItems(const std::initializer_list<std::string>& items);
    virtual void addItems(const Vector<std::string>& items);
    virtual void clearItems();
    virtual std::string getActionCommand() const Q_DECL_OVERRIDE;
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;
    virtual std::string getItem(int index) const;
    virtual int getSelectedIndex() const;
    virtual std::string getSelectedItem() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual bool isEditable() const;
    virtual void removeActionListener();
    virtual void setActionListener(GEventListener func);
    virtual void setActionListener(GEventListenerVoid func);
    virtual void setEditable(bool editable);
    virtual void setItem(int index, const std::string& item);
    virtual void setSelectedIndex(int index);
    virtual void setSelectedItem(const std::string& item);
    virtual int size() const;

private:
    Q_DISABLE_COPY(GChooser)

    _Internal_QComboBox* _iqcomboBox;

    void checkIndex(const std::string& member, int index, int min = 0, int max = -1) const;

    friend class _Internal_QComboBox;
};

// Internal class; not to be used by clients.
class _Internal_QComboBox : public QComboBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QComboBox(GChooser* gchooser, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleChange();

private:
    GChooser* _gchooser;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gchooser_h
