/*
 * File: ginputpanel.h
 * -------------------
 * This file contains declarations of functions that can pop up an
 * "input panel" of easy clickable buttons on the screen that map to
 * various console (cin) inputs to be inserted.
 * It is meant to be used by autograders to provide a simple way for
 * the grader to put input into the student's running program without
 * having to actually type said input. #lazy!
 * 
 * @author Marty Stepp
 * @version 2018/08/28
 * - refactored from free functions to GInputPanel class
 * @version 2014/10/14
 * @since 2014/10/14
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _ginputpanel_h
#define _ginputpanel_h

#include <string>
#include <QObject>

/**
 * TODO: document
 * @private
 */
class GInputPanel {
public:
    static GInputPanel* instance();

    /*
     * Adds an input window button for auto-inserting autograder inputs.
     */
    void addButton(const std::string& text, const std::string& input);

    /*
     * Adds an input category containing related autograder input buttons.
     */
    void addCategory(const std::string& name);

    /*
     * Returns true if the input panel has been loaded already.
     */
    bool isLoaded() const;

    /*
     * Loads the input panel's state from a file of XML data in the following format:
     * <inputpanel assignment="CS 106B Game of Life">
     *   <category name="General">
     *     <button text="hi" input="hello there\n" />
     *     <button text="Yes" />
     *   </category>
     *   <category name="...">
     *     <button text="..." />
     *   </category>
     *   ...
     * </inputpanel>
     * The input panel will also immediately be shown on the screen.
     */
    void load(const std::string& xmlFilename);

    /*
     * Removes the given window input button if it is present.
     */
    void removeButton(const std::string& text);

    /*
     * Removes the given window input category if it is present.
     */
    void removeCategory(const std::string& name);

    /*
     * Causes the input panel to become shown or hidden from the screen.
     */
    void setVisible(bool value = true);

private:
    Q_DISABLE_COPY(GInputPanel)

    GInputPanel();   // prevent construction

    static GInputPanel* _instance;
    bool _loaded;
};

#endif // _ginputpanel_h
