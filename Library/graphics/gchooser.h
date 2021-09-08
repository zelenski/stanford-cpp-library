/*
 * File: gchooser.h
 * ----------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - added key event support
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/09/04
 * - added GComboBox alias
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

#include "ginteractor.h"
#include "vector.h"

class _Internal_QComboBox;

/**
 * This interactor subclass represents a selectable drop-down list.
 * The GChooser constructor creates an empty chooser.
 * Once the chooser has been created, clients can use addItem to add the options.
 */
class GChooser : public GInteractor {
public:
    /**
     * Creates a chooser that initially contains no items.
     */
    GChooser(QWidget* parent = nullptr);

    /**
     * Creates a chooser that initially contains the given items.
     */
    GChooser(const std::initializer_list<std::string>& items, QWidget* parent = nullptr);

    /**
     * Creates a chooser that initially contains the given items.
     */
    GChooser(const Vector<std::string>& items, QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the chooser.
     */
    ~GChooser() override;

    /**
     * Adds a new item consisting of the specified string to the end of the list.
     */
    virtual void addItem(const std::string& item);

    /**
     * Adds each item from the given list to the end of the chooser's list.
     */
    virtual void addItems(const std::initializer_list<std::string>& items);

    /**
     * Adds each item from the given vector to the end of the chooser's list.
     */
    virtual void addItems(const Vector<std::string>& items);

    /**
     * Removes all items from the chooser.
     */
    virtual void clearItems();

    /* @inherit */
    std::string getActionCommand() const override;

    /* @inherit */
    _Internal_QWidget* getInternalWidget() const override;

    /**
     * Returns the item in the chooser at the given 0-based index.
     * @throw ErrorException if the index is out of range
     */
    virtual std::string getItem(int index) const;

    /**
     * Returns the number of items in the chooser.
     */
    virtual int getItemCount() const;

    /**
     * Returns which index is selected in the chooser.
     * If no item is selected, returns -1.
     * @return selected index, or -1 if no item is selected
     */
    virtual int getSelectedIndex() const;

    /**
     * Returns the currently selected item in the chooser,
     * or an empty string if no item is currently selected.
     */
    virtual std::string getSelectedItem() const;

    /* @inherit */
    std::string getType() const override;

    /* @inherit */
    QWidget* getWidget() const override;

    /**
     * Returns true if the chooser has an editable area for typing new items.
     * By default this is false unless setEditable(true) is called.
     */
    virtual bool isEditable() const;

    /**
     * Returns true if the chooser has no items.
     */
    virtual bool isEmpty() const;

    /**
     * Sets whether the chooser has an editable area for typing new items.
     * Initially false.
     */
    virtual void setEditable(bool editable);

    /**
     * Sets the item at the given index in the chooser to the given value.
     * @throw ErrorException if the index is out of range
     */
    virtual void setItem(int index, const std::string& item);

    /**
     * Sets the item at the given index in the chooser to be selected.
     * @throw ErrorException if the index is out of range
     */
    virtual void setSelectedIndex(int index);

    /**
     * Sets the given item in the chooser to be selected.
     * @throw ErrorException if the index is out of range
     */
    virtual void setSelectedItem(const std::string& item);

    /**
     * Returns the number of items in the chooser.
     */
    virtual int size() const;

protected:
    /**
     * @private
     */
    std::string getActionEventType() const override;

private:
    Q_DISABLE_COPY(GChooser)

    _Internal_QComboBox* _iqcomboBox;

    void checkIndex(const std::string& member, int index, int min = 0, int max = -1) const;

    friend class _Internal_QComboBox;
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QComboBox : public QComboBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QComboBox(GChooser* gchooser, QWidget* parent = nullptr);
    void detach() override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    QSize sizeHint() const override;

public slots:
    void handleChange();

private:
    GChooser* _gchooser;

    friend class GChooser;
};

// alias GComboBox for GChooser for name compatibility with Java and Qt
typedef GChooser GComboBox;

#endif // _gchooser_h
