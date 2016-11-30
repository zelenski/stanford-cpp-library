/*
 * File: gtable.cpp
 * ----------------
 * This file implements the gtable.h interface.
 * See that file for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2016/11/26
 * - added autofitColumnWidths
 * - added per-cell/column/row formatting: background/foreground color, font
 * @version 2016/11/18
 * - added column header methods
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
          editable(true),
          rowColHeadersVisible(false),
          columnHeaderStyle(GTable::COLUMN_HEADER_NONE) {
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

void GTable::autofitColumnWidths() {
    stanfordcpplib::getPlatform()->gtable_autofitColumnWidths(this);
}

void GTable::clear() {
    stanfordcpplib::getPlatform()->gtable_clear(this);
}

void GTable::clearFormatting() {
    stanfordcpplib::getPlatform()->gtable_clearFormatting(this);
}

void GTable::clearSelection() {
    stanfordcpplib::getPlatform()->gtable_select(this, -1, -1);
}

std::string GTable::get(int row, int column) const {
    checkIndex("get", row, column);
    return stanfordcpplib::getPlatform()->gtable_get(this, row, column);
}

GTable::ColumnHeaderStyle GTable::getColumnHeaderStyle() const {
    return columnHeaderStyle;
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

void GTable::setCellAlignment(int row, int column, Alignment alignment) {
    checkIndex("setCellAlignment", row, column);
    stanfordcpplib::getPlatform()->gtable_setCellAlignment(this, row, column, alignment);
}

void GTable::setCellBackground(int row, int column, int color) {
    checkIndex("setCellBackground", row, column);
    std::string colorStr = convertRGBToColor(color);
    stanfordcpplib::getPlatform()->gtable_setCellBackground(this, row, column, colorStr);
}

void GTable::setCellBackground(int row, int column, const std::string& color) {
    checkIndex("setCellBackground", row, column);
    stanfordcpplib::getPlatform()->gtable_setCellBackground(this, row, column, color);
}

void GTable::setCellFont(int row, int column, const std::string& font) {
    checkIndex("setCellFont", row, column);
    stanfordcpplib::getPlatform()->gtable_setCellFont(this, row, column, font);
}

void GTable::setCellForeground(int row, int column, int color) {
    checkIndex("setCellForeground", row, column);
    std::string colorStr = convertRGBToColor(color);
    stanfordcpplib::getPlatform()->gtable_setCellForeground(this, row, column, colorStr);
}

void GTable::setCellForeground(int row, int column, const std::string& color) {
    checkIndex("setCellForeground", row, column);
    stanfordcpplib::getPlatform()->gtable_setCellForeground(this, row, column, color);
}

void GTable::setColumnAlignment(int column, Alignment alignment) {
    checkIndex("setColumnAlignment", /* row */ 0, column);
    stanfordcpplib::getPlatform()->gtable_setColumnAlignment(this, column, alignment);
}

void GTable::setColumnBackground(int column, int color) {
    checkIndex("setColumnBackground", /* row */ 0, column);
    std::string colorStr = convertRGBToColor(color);
    stanfordcpplib::getPlatform()->gtable_setColumnBackground(this, column, colorStr);
}

void GTable::setColumnBackground(int column, const std::string& color) {
    checkIndex("setColumnBackground", /* row */ 0, column);
    stanfordcpplib::getPlatform()->gtable_setColumnBackground(this, column, color);
}

void GTable::setColumnFont(int column, const std::string& font) {
    checkIndex("setColumnFont", /* row */ 0, column);
    stanfordcpplib::getPlatform()->gtable_setColumnFont(this, column, font);
}

void GTable::setColumnForeground(int column, int color) {
    checkIndex("setColumnForeground", /* row */ 0, column);
    std::string colorStr = convertRGBToColor(color);
    stanfordcpplib::getPlatform()->gtable_setColumnForeground(this, column, colorStr);
}

void GTable::setColumnForeground(int column, const std::string& color) {
    checkIndex("setColumnForeground", /* row */ 0, column);
    stanfordcpplib::getPlatform()->gtable_setColumnForeground(this, column, color);
}

void GTable::setColumnHeaderStyle(GTable::ColumnHeaderStyle style) {
    stanfordcpplib::getPlatform()->gtable_setColumnHeaderStyle(this, style);
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

void GTable::setEditorValue(int row, int column, const std::string& text) {
    stanfordcpplib::getPlatform()->gtable_setEditorValue(this, row, column, text);
}

void GTable::setEventEnabled(int type, bool enabled) {
    if (type != TABLE_SELECTED && type != TABLE_UPDATED
            && type != TABLE_EDIT_BEGIN && type != TABLE_REPLACE_BEGIN) {
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

void GTable::setRowAlignment(int row, Alignment alignment) {
    checkIndex("setRowAlignment", row, /* column */ 0);
    stanfordcpplib::getPlatform()->gtable_setRowAlignment(this, row, alignment);
}

void GTable::setRowBackground(int row, int color) {
    checkIndex("setRowBackground", row, /* column */ 0);
    std::string colorStr = convertRGBToColor(color);
    stanfordcpplib::getPlatform()->gtable_setRowBackground(this, row, colorStr);
}

void GTable::setRowBackground(int row, const std::string& color) {
    checkIndex("setRowBackground", row, /* column */ 0);
    stanfordcpplib::getPlatform()->gtable_setRowBackground(this, row, color);
}

void GTable::setRowFont(int row, const std::string& font) {
    checkIndex("setRowFont", row, /* column */ 0);
    stanfordcpplib::getPlatform()->gtable_setRowFont(this, row, font);
}

void GTable::setRowForeground(int row, int color) {
    checkIndex("setRowForeground", row, /* column */ 0);
    std::string colorStr = convertRGBToColor(color);
    stanfordcpplib::getPlatform()->gtable_setRowForeground(this, row, colorStr);
}

void GTable::setRowForeground(int row, const std::string& color) {
    checkIndex("setRowForeground", row, /* column */ 0);
    stanfordcpplib::getPlatform()->gtable_setRowForeground(this, row, color);
}

void GTable::setRowColumnHeadersVisible(bool visible) {
    rowColHeadersVisible = visible;
    stanfordcpplib::getPlatform()->gtable_setRowColumnHeadersVisible(this, visible);
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
