/**
 * @file gtextarea.cpp
 * -------------------
 *
 * @brief
 * This file exports the GTextArea class.
 *
 * @author Jeff Lutgen
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

#include <string>
#include "gtextarea.h"
#include "private/platform.h"
#include "base64.h"

using namespace std;

GTextArea::GTextArea(double width, double height) {
    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
    stanfordcpplib::getPlatform()->gtextarea_create(this, width, height);
}

GTextArea::GTextArea(double x, double y, double width, double height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    stanfordcpplib::getPlatform()->gtextarea_create(this, width, height);
    setLocation(x, y);
}

void GTextArea::setText(std::string text) {
    stanfordcpplib::getPlatform()->gtextarea_setText(this, text);
}

std::string GTextArea::getText() const {
    string encoded = stanfordcpplib::getPlatform()->gtextarea_getText(this);
    return Base64::decode(encoded);
}

void GTextArea::setFont(std::string font) {
    stanfordcpplib::getPlatform()->gtextarea_setFont(this, font);
}

void GTextArea::setEditable(bool isEditable) {
    stanfordcpplib::getPlatform()->gtextarea_setEditable(this, isEditable);
}

GRectangle GTextArea::getBounds() const {
    return GRectangle(x, y, width, height);
}

string GTextArea::getType() const {
    return "GTextArea";
}

string GTextArea::toString() const {
    ostringstream oss;
    oss << "GTextArea(" << x << ", " << y << ", "
        << width << ", " << height << ")";
    return oss.str();
}
