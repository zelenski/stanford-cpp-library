/*
 * File: inputpanel.h
 * ------------------
 * This file contains declarations of functions that can pop up an
 * "input panel" of easy clickable buttons on the screen that map to
 * various console (cin) inputs to be inserted.
 * It is meant to be used by autograders to provide a simple way for
 * the grader to put input into the student's running program without
 * having to actually type said input. #lazy!
 * 
 * @author Marty Stepp
 * @version 2014/10/14
 * @since 2014/10/14
 */

#ifndef _inputpanel_h
#define _inputpanel_h

#include <string>

namespace inputpanel {
/*
 * Adds/removes an input window button for auto-inserting autograder inputs.
 */
void addInputButton(const std::string& text, const std::string& input);
void removeInputButton(const std::string& text);

/*
 * Adds/removes an input category containing related autograder input buttons.
 */
void addInputCategory(const std::string& name);
void removeInputCategory(const std::string& name);

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
void load(std::string xmlFilename);
bool isLoaded();

/*
 * Causes the input panel to become shown or hidden from the screen.
 */
void setVisible(bool value = true);
} // namespace inputpanel

#endif // _inputpanel_h
