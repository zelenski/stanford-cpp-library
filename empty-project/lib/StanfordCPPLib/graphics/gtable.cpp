/*
 * File: gtable.cpp
 * ----------------
 * This file implements the gtable.h interface.
 * See that file for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2015/12/01
 * - added setEventEnabled to turn on/off table update/selection events
 * - added isEditable, setEditable
 * @since 2015/11/07
 */

#include "gtable.h"
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "gevents.h"
#include "gwindow.h"
#include "private/platform.h"

GTable::GTable(int rows, int columns, double x, double y, double width, double height)
        : rows(rows),
          columns(columns),
          m_x(x),
          m_y(y),
          m_width(width),
          m_height(height),
          font("*-*-*"),
          alignment(Alignment::LEFT),
          editable(true) {
    checkDimensions("constructor", rows, columns);
    checkSize("constructor", width, height);
    stanfordcpplib::getPlatform()->gtable_constructor(this, rows, columns, x, y, width, height);
}

GRectangle GTable::getBounds() const {
    return GRectangle(m_x, m_y, m_width, m_height);
}

std::string GTable::getType() const {
    return "GTable";
}

std::string GTable::toString() const {
    std::ostringstream out;
    out << "GTable(" << rows << "x" << columns << ")";
    return out.str();
}

void GTable::clear() {
    stanfordcpplib::getPlatform()->gtable_clear(this);
    // clearSelection();
}

void GTable::clearSelection() {
    stanfordcpplib::getPlatform()->gtable_select(this, -1, -1);
}

std::string GTable::get(int row, int column) const {
    checkIndex("get", row, column);
    return stanfordcpplib::getPlatform()->gtable_get(this, row, column);
}

double GTable::getColumnWidth(int column) const {
    checkIndex("getColumnWidth", /* row */ 0, column);
    return stanfordcpplib::getPlatform()->gtable_getColumnWidth(this, column);
}

std::string GTable::getFont() const {
    return font;
}

GTable::Alignment GTable::getHorizontalAlignment() const {
    return alignment;
}

void GTable::getSelectedCell(int& row, int& column) const {
    stanfordcpplib::getPlatform()->gtable_getSelection(this, row, column);
}

int GTable::getSelectedColumn() const {
    int row, column;
    stanfordcpplib::getPlatform()->gtable_getSelection(this, row, column);
    return column;
}

int GTable::getSelectedRow() const {
    int row, column;
    stanfordcpplib::getPlatform()->gtable_getSelection(this, row, column);
    return row;
}

int GTable::height() const {
    return rows;
}

bool GTable::inBounds(int row, int column) const {
    return 0 <= row && row < rows
            && 0 <= column && column < columns;
}

bool GTable::isEditable() const {
    return editable;
}

int GTable::numCols() const {
    return columns;
}

int GTable::numRows() const {
    return rows;
}

void GTable::resize(int numRows, int numCols) {
    checkDimensions("resize", numRows, numCols);
    this->rows = numRows;
    this->columns = numCols;
    stanfordcpplib::getPlatform()->gtable_resize(this, numRows, numCols);
}

void GTable::select(int row, int column) {
    stanfordcpplib::getPlatform()->gtable_select(this, row, column);
}

void GTable::set(int row, int column, const std::string& text) {
    checkIndex("set", row, column);
    stanfordcpplib::getPlatform()->gtable_set(this, row, column, text);
}

void GTable::setColumnWidth(int column, double width) {
    checkIndex("setColumnWidth", /* row */ 0, column);
    if (width < 0) {
        error("GTable::setColumnWidth: width cannot be negative");
    }
    stanfordcpplib::getPlatform()->gtable_setColumnWidth(this, column, (int) width);
}

void GTable::setEditable(bool editable) {
    this->editable = editable;
    stanfordcpplib::getPlatform()->gtable_setEditable(this, editable);
}

void GTable::setEventEnabled(int type, bool enabled) {
    if (type != TABLE_SELECTED && type != TABLE_UPDATED) {
        error("GTable::setEventEnabled: invalid event type");
    }
    stanfordcpplib::getPlatform()->gtable_setEventEnabled(this, type, enabled);
}

void GTable::setFont(const std::string& font) {
    this->font = font;
    stanfordcpplib::getPlatform()->gtable_setFont(this, font);
}

void GTable::setHorizontalAlignment(GTable::Alignment alignment) {
    std::string alignmentStr;
    if (alignment == Alignment::LEFT) {
        alignmentStr = "LEFT";
    } else if (alignment == Alignment::CENTER) {
        alignmentStr = "CENTER";
    } else if (alignment == Alignment::RIGHT) {
        alignmentStr = "RIGHT";
    }
    this->alignment = alignment;
    stanfordcpplib::getPlatform()->gtable_setHorizontalAlignment(this, alignmentStr);
}

int GTable::width() const {
    return columns;
}

void GTable::checkDimensions(const std::string& member, int numRows, int numCols) const {
    if (numRows < 0 || numCols < 0) {
        std::ostringstream out;
        out << "GTable::" << member << ": numRows/numCols cannot be negative";
        error(out.str());
    }
}

void GTable::checkIndex(const std::string& member, int row, int column) const {
    if (!inBounds(row, column)) {
        std::ostringstream out;
        out << "GTable::" << member
            << ": (row=" << row
            << ", col=" << column
            << ") is out of valid range of (0, 0) through ("
            << rows << ", " << columns << ")";
        error(out.str());
    }
}

void GTable::checkSize(const std::string& member, double width, double height) const {
    if (width < 0 || height < 0) {
        std::ostringstream out;
        out << "GTable::" << member << ": width/height cannot be negative";
        error(out.str());
    }
}
