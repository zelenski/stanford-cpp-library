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
class _Q_Internal_Chooser : public QComboBox {
    Q_OBJECT

public:
    _Q_Internal_Chooser(QGChooser* chooser, QWidget* parent = nullptr);

public slots:
    void handleChange();

private:
    QGChooser* _chooser;
};

/*
 * ...
 */
class QGChooser : public QGInteractor {
public:
    QGChooser(QWidget* parent = nullptr);
    QGChooser(const std::initializer_list<std::string>& items, QWidget* parent = nullptr);
    QGChooser(const Vector<std::string>& items, QWidget* parent = nullptr);

    void addItem(const std::string& item);
    void addItems(const std::initializer_list<std::string>& items);
    void addItems(const Vector<std::string>& items);
    void clearItems();
    std::string getItem(int index) const;
    int getSelectedIndex() const;
    std::string getSelectedItem() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    bool isEditable() const;
    void setChangeHandler(std::function<void()> func);
    void setEditable(bool editable);
    void setItem(int index, const std::string& item);
    void setSelectedIndex(int index);
    void setSelectedItem(const std::string& item);
    int size() const;

private:
    _Q_Internal_Chooser _chooser;

    void checkIndex(const std::string& member, int index, int min = 0, int max = -1) const;

    friend class _Q_Internal_Chooser;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgchooser_h
