/*
 * File: gtextarea.h
 * -----------------
 * This file exports the GTextArea class, which represents a multi-line
 * area that displays plain text.
 *
 * @author Jeff Lutgen
 * @version 2017/03/16
 * - improved comments and changed them from doxygen style to StanfordCPPLib style
 * @version 2016/10/12
 * - taken from https://github.com/jlutgen/stanford-whittier-cpplib/
 * - Thanks, Jeff!
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#ifndef _gtextarea_h
#define _gtextarea_h

#include "ginteractors.h"
#include "gobjects.h"
#include "gtypes.h"

/* Class: GTextArea
 * ----------------
 * This interactor subclass represents a multi-line
 * area that displays plain text.
 *
 * Scrollbars are added to the text area automatically if the
 * text is too large to fit in the displayed region of the
 * text area.
 */
class GTextArea : public GInteractor {
public:
    GTextArea(double width, double height);
    /*
     * Constructor: GTextArea
     * Usage: GTextArea textArea;
     *        GTextArea textArea(width, height);
     *        GTextArea textArea(x, y, width, height);
     * -----------------------------------------------
     * Constructs an text area with the specified width, height,
     * and initial location. If no location is passed, the default
     * of (0, 0) is used.
     *
     * The text area is editable by default.
     */
    GTextArea(double x, double y, double width, double height);

    /* Prototypes for the virtual methods */
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toString() const;

    /* unique GTextArea behavior */

    /*
     * Method: GTextArea
     * Usage: textarea.setFont("Monospaced-9");
     * ----------------------------------------
     * Sets the font used by this text area.
     */
    void setFont(std::string font);

    /*
     * Method: getText
     * Usage: string text = textarea.getText();
     * ----------------------------------------
     * Returns the text currently displayed by this text area.
     */
    std::string getText() const;

    /*
     * Method: setText
     * Usage: textarea.setText(text);
     * ------------------------------
     * Sets the text displayed by this text area.
     */
    void setText(std::string text);

    /*
     * Method: setEditable
     * Usage: textarea.setEditable(isEditable);
     * ----------------------------------------
     * Sets whether this text area is editable.
     */
    void setEditable(bool isEditable);

private:
    double width;
    double height;
};

#endif
