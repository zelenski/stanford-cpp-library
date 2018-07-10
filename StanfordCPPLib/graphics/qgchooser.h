/*
 * File: qgchooser.h
 * -----------------
 *
 * @version 2018/06/28
 * - initial version
 */

#ifndef _qgchooser_h
#define _qgchooser_h

#include <initializer_list>
#include <string>
#include <QComboBox>
#include <QWidget>
#include "qginteractor.h"
#include "vector.h"

// forward declaration
class QGChooser;

// Internal class; not to be used by clients.
class _Internal_QComboBox : public QComboBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QComboBox(QGChooser* qgchooser, QWidget* parent = nullptr);

public slots:
    void handleChange();

private:
    QGChooser* _qgchooser;
};

/*
 * ...
 */
class QGChooser : public QGInteractor {
public:
    QGChooser(QWidget* parent = nullptr);
    QGChooser(const std::initializer_list<std::string>& items, QWidget* parent = nullptr);
    QGChooser(const Vector<std::string>& items, QWidget* parent = nullptr);
    virtual ~QGChooser();

    virtual void addItem(const std::string& item);
    virtual void addItems(const std::initializer_list<std::string>& items);
    virtual void addItems(const Vector<std::string>& items);
    virtual void clearItems();
    virtual std::string getItem(int index) const;
    virtual int getSelectedIndex() const;
    virtual std::string getSelectedItem() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual bool isEditable() const;
    virtual void removeChangeHandler();
    virtual void setChangeHandler(QGEventHandler func);
    virtual void setChangeHandler(QGEventHandlerVoid func);
    virtual void setEditable(bool editable);
    virtual void setItem(int index, const std::string& item);
    virtual void setSelectedIndex(int index);
    virtual void setSelectedItem(const std::string& item);
    virtual int size() const;

private:
    _Internal_QComboBox* _iqcomboBox;

    void checkIndex(const std::string& member, int index, int min = 0, int max = -1) const;

    friend class _Internal_QComboBox;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgchooser_h
