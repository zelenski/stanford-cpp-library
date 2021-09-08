/*
 * File: gtable.cpp
 * ----------------
 * This file implements the GTable.h interface.
 * See that file for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2019/02/02
 * - destructor now stops event processing
 * @version 2018/09/06
 * - added bounds-checking and require() calls
 * @version 2018/08/23
 * - renamed to gtable.cpp to replace Java version
 * @version 2018/07/21
 * - rich formatting on cell, row, column, table
 * - improved event handling
 * @version 2018/07/17
 * - initial version, based on gtable.h
 * @since 2018/07/17
 */

#include "gtable.h"
#include <cstring>
#include <iostream>
#include <iomanip>
#include <QBrush>
#include <QColor>
#include <QHeaderView>
#include <QLineEdit>
#include <sstream>
#include "gclipboard.h"
#include "gcolor.h"
#include "gevent.h"
#include "gfont.h"
#include "gthread.h"
#include "require.h"

GTable::TableStyle GTable::_defaultCellStyle = GTable::TableStyle::unset();

GTable::GTable(int rows, int columns, double width, double height, QWidget* parent)
        : _iqtableview(nullptr),
          _columnHeaderStyle(GTable::COLUMN_HEADER_NONE) {
    GThread::runOnQtGuiThread([this, rows, columns, parent]() {
        _iqtableview = new _Internal_QTableWidget(this, rows, columns, getInternalParent(parent));
        _iqtableview->setSelectionMode(QAbstractItemView::SingleSelection);
        _globalCellStyle = TableStyle::unset();
    });
    require::nonNegative2D(rows, columns, "GTable::constructor", "rows", "columns");
    require::nonNegative2D(width, height, "GTable::constructor", "width", "height");
    setVisible(false);   // all widgets are not shown until added to a window
}

GTable::~GTable() {
    // TODO: delete _iqtableview;
    _iqtableview->detach();
    _iqtableview = nullptr;
}

void GTable::applyStyleToCell(int row, int column, const TableStyle& style) {
    setCellAlignmentInternal(row, column, style.alignment);
    setCellBackgroundInternal(row, column, style.background);
    setCellFontInternal(row, column, style.font);
    setCellForegroundInternal(row, column, style.foreground);
}

void GTable::autofitColumnWidths() {
    GThread::runOnQtGuiThread([this]() {
        _iqtableview->resizeColumnsToContents();
    });
}

void GTable::checkColumn(const std::string& member, int column) const {
    require::inRange(column, 0, numCols(), "GTable::" + member, "column");
}

void GTable::checkIndex(const std::string& member, int row, int column) const {
    require::inRange2D(row, column, 0, 0, numRows(), numCols(), "GTable::" + member, "row", "column");
}

void GTable::checkRow(const std::string& member, int row) const {
    require::inRange(row, 0, numRows(), "GTable::" + member, "row");
}

void GTable::clear() {
    GThread::runOnQtGuiThread([this]() {
        _iqtableview->clear();
        // for some reason, clearing a table also wipes the Excel-style column headers
        updateColumnHeaders();
    });
}

void GTable::clearCell(int row, int column) {
    checkIndex("clearCell", row, column);
    set(row, column, /* text */ "");
}

void GTable::clearFormatting() {
    GThread::runOnQtGuiThread([this]() {
        // clear out all records of row, column, and global table styles
        _columnStyles.clear();
        _rowStyles.clear();
        _globalCellStyle = TableStyle::unset();

        // set the formatting on each cell
        for (int row = 0, nr = numRows(), nc = numCols(); row < nr; row++) {
            for (int col = 0; col < nc; col++) {
                clearCellFormatting(row, col);
            }
        }
    });
}

void GTable::clearCellFormatting(int row, int column) {
    checkIndex("clearCellFormatting", row, column);
    GThread::runOnQtGuiThread([this, row, column]() {
        ensureDefaultFormatting();
        TableStyle style = _defaultCellStyle;
        if (style.background >= 0) {
            setCellBackgroundInternal(row, column, style.background);
        }
        if (!style.font.empty()) {
            setCellFontInternal(row, column, style.font);
        }
        if (style.foreground >= 0) {
            setCellForegroundInternal(row, column, style.foreground);
        }
        if (style.alignment >= 0) {
            setCellAlignmentInternal(row, column, style.alignment);
        }
    });
}

void GTable::clearSelection() {
    GThread::runOnQtGuiThread([this]() {
        _iqtableview->clearSelection();
    });
}

void GTable::ensureColumnStyle(int column) {
    ensureDefaultFormatting();
    if (!_columnStyles.containsKey(column)) {
        _columnStyles[column] = TableStyle::unset();
    }
}

void GTable::ensureDefaultFormatting() const {
    GTable* thisHack = (GTable*) this;
    if (!_defaultCellStyle.isSet()) {
        QPalette palette = thisHack->_iqtableview->palette();
        _defaultCellStyle.background = palette.base().color().rgb() & 0x00ffffff;
        _defaultCellStyle.foreground = palette.text().color().rgb() & 0x00ffffff;
        _defaultCellStyle.font       = GFont::toFontString(thisHack->_iqtableview->font());
        _defaultCellStyle.alignment  = ALIGN_LEFT;   // TODO: ask cell for its alignment
    }
}

void GTable::ensureRowStyle(int row) {
    ensureDefaultFormatting();
    if (!_rowStyles.containsKey(row)) {
        _rowStyles[row] = TableStyle::unset();
    }
}

void GTable::fill(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        int nr = numRows();
        int nc = numCols();
        for (int r = 0; r < nr; r++) {
            for (int c = 0; c < nc; c++) {
                set(r, c, text);
            }
        }
    });
}

std::string GTable::get(int row, int column) const {
    checkIndex("get", row, column);
    return _iqtableview->model()->data(_iqtableview->model()->index(row, column)).toString().toStdString();
}

GTable::ColumnHeaderStyle GTable::getColumnHeaderStyle() const {
    return _columnHeaderStyle;
}

double GTable::getColumnWidth(int column) const {
    checkColumn("getColumnWidth", column);
    return _iqtableview->columnWidth(column);
}

_Internal_QWidget* GTable::getInternalWidget() const {
    return _iqtableview;
}

GTable::TableStyle GTable::getMergedStyleForCell(int row, int column) {
    // style precedence: cell > column > row > global
    ensureDefaultFormatting();
    TableStyle style = _defaultCellStyle;
    style.mergeWith(_globalCellStyle);
    if (_rowStyles.containsKey(row)) {
        style.mergeWith(_rowStyles[row]);
    }
    if (_columnStyles.containsKey(column)) {
        style.mergeWith(_columnStyles[column]);
    }
    return style;
}

double GTable::getRowHeight(int row) const {
    checkRow("getRowHeight", row);
    return _iqtableview->rowHeight(row);
}

GridLocation GTable::getSelectedCell() const {
    QModelIndexList list = _iqtableview->selectionModel()->selectedIndexes();
    if (list.empty()) {
        return GridLocation(-1, -1);
    } else {
        QModelIndex index = list.at(0);
        return GridLocation(index.row(), index.column());
    }
}

void GTable::getSelectedCell(int& row, int& column) const {
    GridLocation loc = getSelectedCell();
    row = loc.row;
    column = loc.col;
}

std::string GTable::getSelectedCellValue() const {
    if (hasSelectedCell()) {
        GridLocation loc = getSelectedCell();
        return get(loc.row, loc.col);
    } else {
        return "";
    }
}

int GTable::getSelectedColumn() const {
    return getSelectedCell().col;
}

int GTable::getSelectedRow() const {
    return getSelectedCell().row;
}

std::string GTable::getType() const {
    return "GTable";
}

QWidget* GTable::getWidget() const {
    return static_cast<QWidget*>(_iqtableview);
}

bool GTable::hasSelectedCell() const {
    GridLocation loc = getSelectedCell();
    return loc.row >= 0 && loc.col >= 0;
}

int GTable::height() const {
    return numRows();
}

bool GTable::inTableBounds(int row, int column) const {
    return 0 <= row && row < height() && 0 <= column && column < width();
}

bool GTable::isEditable() const {
    return (_iqtableview->editTriggers() & QAbstractItemView::NoEditTriggers) != 0;
}

int GTable::numCols() const {
    return _iqtableview->model()->columnCount();
}

int GTable::numRows() const {
    return _iqtableview->model()->rowCount();
}

void GTable::removeTableListener() {
    removeEventListeners({"table",
                          "tableupdate",
                          "tableselect",
                          "tableeditbegin",
                          "tablereplacebegin",
                          "tablecut",
                          "tablecopy",
                          "tablepaste"});
}

void GTable::requestFocus() {
    bool wasEditing = _iqtableview->isEditing();
    GInteractor::requestFocus();
    if (!wasEditing && hasSelectedCell()) {
        GThread::runOnQtGuiThread([this]() {
            GridLocation loc = getSelectedCell();
            _iqtableview->closePersistentEditor(_iqtableview->item(loc.row, loc.col));
        });
    }
}

void GTable::resize(int newNumRows, int newNumCols) {
    require::nonNegative2D(newNumRows, newNumCols, "GTable::resize", "rows", "columns");
    GThread::runOnQtGuiThread([this, newNumRows, newNumCols]() {
        int oldNumRows = numRows();
        int oldNumCols = numCols();
        _iqtableview->setRowCount(newNumRows);
        _iqtableview->setColumnCount(newNumCols);

        // make sure proper headers showing on each column
        if (newNumCols > oldNumCols) {
            updateColumnHeaders();
        }

        // apply appropriate styles to newly added cells
        if (newNumRows > oldNumRows || newNumCols > oldNumCols) {
            for (int row = 0; row < newNumRows; row++) {
                for (int col = 0; col < newNumCols; col++) {
                    if (row >= oldNumRows || col >= oldNumCols) {
                        // figure out appropriate style (row, col, global, etc.) and apply it
                        TableStyle style = getMergedStyleForCell(row, col);
                        applyStyleToCell(row, col, style);
                    }
                }
            }
        }
    });
}

bool GTable::rowColumnHeadersVisible() const {
    return _iqtableview->horizontalHeader()->isVisible()
            && _iqtableview->verticalHeader()->isVisible();
}

void GTable::select(int row, int column) {
    checkIndex("select", row, column);
    GThread::runOnQtGuiThread([this, row, column]() {
        QModelIndex index = _iqtableview->model()->index(row, column);
        _iqtableview->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    });
}

void GTable::set(int row, int column, const std::string& text) {
    checkIndex("set", row, column);
    GThread::runOnQtGuiThread([this, row, column, text]() {
        QModelIndex index = _iqtableview->model()->index(row, column);
        _iqtableview->model()->setData(index, QVariant(text.c_str()));
    });
}

void GTable::setBackground(int rgb) {
    // GInteractor::setBackground(rgb);
    // (don't call super; that will set the BG of the headers and everything)

    GThread::runOnQtGuiThread([this, rgb]() {
        // save this background color in the global cell style (for later cells on resize() etc)
        ensureDefaultFormatting();
        _globalCellStyle.background = rgb;

        // remove background colors from any row/column styles because they are
        // now overridden by this global background color style
        TableStyle unset = TableStyle::unset();
        for (int row : _rowStyles) {
            _rowStyles[row].background = unset.background;
        }
        for (int col : _columnStyles) {
            _columnStyles[col].background = unset.background;
        }

        // set each cell's background color
        for (int row = 0, nr = numRows(); row < nr; row++) {
            for (int col = 0, nc = numCols(); col < nc; col++) {
                setCellBackgroundInternal(row, col, rgb);
            }
        }
    });
}

void GTable::setBackground(const std::string& color) {
    setBackground(GColor::convertColorToRGB(color));
}

void GTable::setCellAlignment(int row, int column, HorizontalAlignment alignment) {
    checkIndex("setCellAlignment", row, column);
    GThread::runOnQtGuiThread([this, row, column, alignment]() {
        setCellAlignmentInternal(row, column, alignment);   // do the actual work
    });
}

void GTable::setCellAlignmentInternal(int row, int column, HorizontalAlignment alignment) {
    Qt::Alignment align = Qt::AlignVCenter | toQtAlignment(alignment);
    _iqtableview->model()->setData(_iqtableview->model()->index(row, column),
                                   QVariant(align), Qt::TextAlignmentRole);
}

void GTable::setCellBackground(int row, int column, int rgb) {
    checkIndex("setCellBackground", row, column);
    GThread::runOnQtGuiThread([this, row, column, rgb]() {
        setCellBackgroundInternal(row, column, rgb);   // do the actual work
    });
}

void GTable::setCellBackground(int row, int column, const std::string& color) {
    setCellBackground(row, column, GColor::convertColorToRGB(color));
}

void GTable::setCellBackgroundInternal(int row, int column, int rgb) {
    _iqtableview->item(row, column)->setBackground(QBrush(QColor(rgb)));
}

void GTable::setCellFont(int row, int column, const std::string& font) {
    checkIndex("setCellFont", row, column);
    GThread::runOnQtGuiThread([this, row, column, font]() {
        setCellFontInternal(row, column, font);   // do the actual work
    });
}

void GTable::setCellFontInternal(int row, int column, const std::string& font) {
    _iqtableview->item(row, column)->setFont(GFont::toQFont(_iqtableview->font(), font));
}

void GTable::setCellForeground(int row, int column, int rgb) {
    checkIndex("setCellForeground", row, column);
    GThread::runOnQtGuiThread([this, row, column, rgb]() {
        setCellForegroundInternal(row, column, rgb);   // do the actual work
    });
}

void GTable::setCellForegroundInternal(int row, int column, int rgb) {
    ensureDefaultFormatting();
    _iqtableview->item(row, column)->setForeground(QBrush(QColor(rgb)));
}

void GTable::setCellForeground(int row, int column, const std::string& color) {
    checkIndex("setCellForeground", row, column);
    setCellForeground(row, column, GColor::convertColorToRGB(color));
}

void GTable::setColor(int rgb) {
    setForeground(rgb);
}

void GTable::setColor(const std::string& color) {
    setForeground(color);
}

void GTable::setColumnAlignment(int column, HorizontalAlignment alignment) {
    checkColumn("setColumnAlignment", column);

    GThread::runOnQtGuiThread([this, column, alignment]() {
        // save this alignment in the column style (for later cells on resize() etc)
        ensureColumnStyle(column);
        _columnStyles[column].alignment = alignment;

        // set each cell's alignment in that column
        for (int row = 0, nr = numRows(); row < nr; row++) {
            setCellAlignmentInternal(row, column, alignment);
        }
    });
}

void GTable::setColumnBackground(int column, int rgb) {
    checkColumn("setColumnBackground", column);

    GThread::runOnQtGuiThread([this, column, rgb]() {
        // save this background color in the column style (for later cells on resize() etc)
        ensureColumnStyle(column);
        _columnStyles[column].background = rgb;

        // set each cell's background color in that column
        for (int row = 0, nr = numRows(); row < nr; row++) {
            setCellBackgroundInternal(row, column, rgb);
        }
    });
}

void GTable::setColumnBackground(int column, const std::string& color) {
    setColumnBackground(column, GColor::convertColorToRGB(color));
}

void GTable::setColumnFont(int column, const std::string& font) {
    checkColumn("setColumnFont", column);

    GThread::runOnQtGuiThread([this, column, font]() {
        // save this font in the column style (for later cells on resize() etc)
        ensureColumnStyle(column);
        _columnStyles[column].font = font;

        // set each cell's font in that column
        for (int row = 0, nr = numRows(); row < nr; row++) {
            setCellFontInternal(row, column, font);
        }
    });
}

void GTable::setColumnForeground(int column, int rgb) {
    checkColumn("setColumnForeground", column);

    GThread::runOnQtGuiThread([this, column, rgb]() {
        // save this foreground color in the column style (for later cells on resize() etc)
        ensureColumnStyle(column);
        _columnStyles[column].foreground = rgb;

        // set each cell's foreground color in that column
        for (int row = 0, nr = numRows(); row < nr; row++) {
            setCellForegroundInternal(row, column, rgb);
        }
    });
}

void GTable::setColumnForeground(int column, const std::string& color) {
    setColumnForeground(column, GColor::convertColorToRGB(color));
}

void GTable::setColumnHeaderStyle(GTable::ColumnHeaderStyle style) {
    GThread::runOnQtGuiThread([this, style]() {
        _columnHeaderStyle = style;
        if (style == GTable::COLUMN_HEADER_NONE) {
            // no headers
            setRowColumnHeadersVisible(false);
            return;
        } else {
            // build list of column names to display
            updateColumnHeaders();
            _iqtableview->horizontalHeader()->setVisible(true);
            _iqtableview->verticalHeader()->setVisible(true);
        }
    });
}

void GTable::setColumnWidth(int column, double width) {
    checkColumn("setColumnWidth", column);
    if (width < 0) {
        error("GTable::setColumnWidth: width cannot be negative");
    }
    GThread::runOnQtGuiThread([this, column, width]() {
        _iqtableview->setColumnWidth(column, (int) width);
    });
}

void GTable::setEditable(bool editable) {
    GThread::runOnQtGuiThread([this, editable]() {
        if (editable) {
            _iqtableview->setEditTriggers(
                        QAbstractItemView::CurrentChanged
                        | QAbstractItemView::DoubleClicked
                        | QAbstractItemView::EditKeyPressed
                        | QAbstractItemView::AnyKeyPressed);
        } else {
            _iqtableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }
    });
}

void GTable::setEditorValue(int row, int column, const std::string& text) {
    checkIndex("setEditorValue", row, column);
    GThread::runOnQtGuiThread([this, text]() {
        _Internal_QItemDelegate* delegate = _iqtableview->getItemDelegate();
        if (delegate != nullptr) {
            QWidget* editor = delegate->getEditor();
            if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor)) {
                lineEdit->setText(QString::fromStdString(text));
            }
        }
    });
}

void GTable::setFont(const QFont& font) {
    setFont(GFont::toFontString(font));
}

void GTable::setFont(const std::string& font) {
    GInteractor::setFont(font);   // call super

    GThread::runOnQtGuiThread([this, font]() {
        // save this font in the global cell style (for later cells on resize() etc)
        ensureDefaultFormatting();
        _globalCellStyle.font = font;

        // remove fonts from any row/column styles because they are
        // now overridden by this global font style
        TableStyle unset = TableStyle::unset();
        for (int row : _rowStyles) {
            _rowStyles[row].font = unset.font;
        }
        for (int col : _columnStyles) {
            _columnStyles[col].font = unset.font;
        }

        // set each cell's foreground color
        for (int row = 0, nr = numRows(); row < nr; row++) {
            for (int col = 0, nc = numCols(); col < nc; col++) {
                setCellFontInternal(row, col, font);
            }
        }
    });
}

void GTable::setForeground(int rgb) {
    GThread::runOnQtGuiThread([this, rgb]() {
        // GInteractor::setForeground(rgb);
        // (don't call super; that will set the FG of the scrollbars and everything)

        // save this foreground color in the global cell style (for later cells on resize() etc)
        ensureDefaultFormatting();
        _globalCellStyle.foreground = rgb;

        // remove foreground colors from any row/column styles because they are
        // now overridden by this global foreground color style
        TableStyle unset = TableStyle::unset();
        for (int row : _rowStyles) {
            _rowStyles[row].foreground = unset.foreground;
        }
        for (int col : _columnStyles) {
            _columnStyles[col].foreground = unset.foreground;
        }

        // set each cell's foreground color
        for (int row = 0, nr = numRows(); row < nr; row++) {
            for (int col = 0, nc = numCols(); col < nc; col++) {
                setCellForegroundInternal(row, col, rgb);
            }
        }
    });
}

void GTable::setForeground(const std::string& color) {
    setForeground(GColor::convertColorToRGB(color));
}

void GTable::setHorizontalAlignment(HorizontalAlignment alignment) {
    GThread::runOnQtGuiThread([this, alignment]() {
        // save this alignment in the global cell style (for later cells on resize() etc)
        ensureDefaultFormatting();

        _globalCellStyle.alignment = alignment;

        // remove alignment from any row/column styles because they are
        // now overridden by this global alignment style
        TableStyle unset = TableStyle::unset();
        for (int row : _rowStyles) {
            _rowStyles[row].alignment = unset.alignment;
        }
        for (int col : _columnStyles) {
            _columnStyles[col].alignment = unset.alignment;
        }

        // set each cell's horizontal alignment
        for (int row = 0, nr = numRows(), nc = numCols(); row < nr; row++) {
            for (int col = 0; col < nc; col++) {
                setCellAlignmentInternal(row, col, alignment);
            }
        }
    });
}

void GTable::setRowAlignment(int row, HorizontalAlignment alignment) {
    checkRow("setRowAlignment", row);

    // save this alignment in the row style (for later cells on resize() etc)
    GThread::runOnQtGuiThread([this, row, alignment]() {
        ensureRowStyle(row);
        _rowStyles[row].alignment = alignment;

        // set each cell's alignment in that row
        for (int col = 0, nc = numCols(); col < nc; col++) {
            setCellAlignmentInternal(row, col, alignment);
        }
    });
}

void GTable::setRowBackground(int row, int rgb) {
    checkRow("setRowBackground", row);

    // save this background color in the row style (for later cells on resize() etc)
    GThread::runOnQtGuiThread([this, row, rgb]() {
        ensureRowStyle(row);
        _rowStyles[row].background = rgb;

        // set each cell's background color in that row
        for (int col = 0, nc = numCols(); col < nc; col++) {
            setCellBackgroundInternal(row, col, rgb);
        }
    });
}

void GTable::setRowBackground(int row, const std::string& color) {
    setRowBackground(row, GColor::convertColorToRGB(color));
}

void GTable::setRowFont(int row, const std::string& font) {
    checkRow("setRowFont", row);

    // save this font in the row style (for later cells on resize() etc)
    GThread::runOnQtGuiThread([this, row, font]() {
        ensureRowStyle(row);
        _rowStyles[row].font = font;

        // set each cell's font in that row
        for (int col = 0, nc = numCols(); col < nc; col++) {
            setCellFontInternal(row, col, font);
        }
    });
}

void GTable::setRowForeground(int row, int rgb) {
    checkRow("setRowForeground", row);

    // save this foreground color in the row style (for later cells on resize() etc)
    GThread::runOnQtGuiThread([this, row, rgb]() {
        ensureRowStyle(row);
        _rowStyles[row].foreground = rgb;

        // set each cell's foreground color in that row
        for (int col = 0, nc = numCols(); col < nc; col++) {
            setCellForegroundInternal(row, col, rgb);
        }
    });
}

void GTable::setRowForeground(int row, const std::string& color) {
    checkRow("setRowForeground", row);
    setRowForeground(row, GColor::convertColorToRGB(color));
}

void GTable::setRowColumnHeadersVisible(bool visible) {
    GThread::runOnQtGuiThread([this, visible]() {
        _iqtableview->horizontalHeader()->setVisible(visible);
        _iqtableview->verticalHeader()->setVisible(visible);
    });
}

void GTable::setRowHeight(int row, double height) {
    checkRow("setRowHeight", row);
    if (height < 0) {
        error("GTable::setRowHeight: height cannot be negative");
    }
    GThread::runOnQtGuiThread([this, row, height]() {
        _iqtableview->setRowHeight(row, (int) height);
    });
}

void GTable::setSelectedCellValue(const std::string& text) {
    if (hasSelectedCell()) {
        GridLocation loc = getSelectedCell();
        set(loc.row, loc.col, text);
    }
}

void GTable::setTableListener(GEventListener func) {
    setEventListeners({"table",
                       "tableupdate",
                       "tableselect",
                       "tableeditbegin",
                       "tablereplacebegin",
                       "tablecut",
                       "tablecopy",
                       "tablepaste"}, func);
}

void GTable::setTableListener(GEventListenerVoid func) {
    setEventListeners({"table",
                       "tableupdate",
                       "tableselect",
                       "tableeditbegin",
                       "tablereplacebegin",
                       "tablecut",
                       "tablecopy",
                       "tablepaste"}, func);
}

std::string GTable::toExcelColumnName(int col) {
    // convert column into a roughly base-26 Excel column name,
    // e.g. 0 -> "A", 1 -> "B", 26 -> "AA", ...
    std::string colStr;
    col = col + 1;   // 1-based
    while (col-- > 0) {
        colStr = charToString((char) ('A' + (col % 26))) + colStr;
        col /= 26;
    }
    return colStr;
}

void GTable::updateColumnHeaders() {
    GThread::runOnQtGuiThread([this]() {
        if (_columnHeaderStyle == GTable::COLUMN_HEADER_NONE) {
            return;
        }

        // Qt wants me to put the headers into a string list, and apparently
        // you add things to the list using << like it was a ostream (bleh)
        QStringList columnHeaders;
        for (int col = 0, nc = numCols(); col < nc; col++) {
            if (_columnHeaderStyle == GTable::COLUMN_HEADER_EXCEL) {
                columnHeaders << QString::fromStdString(toExcelColumnName(col));
            } else {
                // style == GTable::COLUMN_HEADER_NUMERIC
                columnHeaders << QString::fromStdString(std::to_string(col));
            }
        }
        _iqtableview->setHorizontalHeaderLabels(columnHeaders);
    });
}

int GTable::width() const {
    return numCols();
}


_Internal_QItemDelegate::_Internal_QItemDelegate(QObject* parent)
        : QStyledItemDelegate(parent),
          _editor(nullptr) {
    // empty
}

QWidget* _Internal_QItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
    _Internal_QItemDelegate* hack = (_Internal_QItemDelegate*) this;
    hack->_editor = editor;
    return editor;
}

void _Internal_QItemDelegate::destroyEditor(QWidget* editor, const QModelIndex& index) const {
    _Internal_QItemDelegate* hack = (_Internal_QItemDelegate*) this;
    hack->_editor = nullptr;
    QStyledItemDelegate::destroyEditor(editor, index);
}

QWidget* _Internal_QItemDelegate::getEditor() const {
    return _editor;
}


_Internal_QTableWidget::_Internal_QTableWidget(GTable* gtable, int rows, int columns, QWidget* parent)
        : QTableWidget(rows, columns, parent),
          _gtable(gtable),
          _delegate(nullptr) {
    require::nonNull(gtable, "_Internal_QTableWidget::constructor");
    setObjectName(QString::fromStdString("_Internal_QTableWidget_" + std::to_string(gtable->getID())));
    _delegate = new _Internal_QItemDelegate();
    setItemDelegate(_delegate);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(handleCellChange(int, int)));
    connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(handleCellDoubleClick(int, int)));
    connect(this->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(handleSelectionChange(const QItemSelection&, const QItemSelection&)));
}

void _Internal_QTableWidget::detach() {
    _gtable = nullptr;
}

bool _Internal_QTableWidget::edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event) {
    bool result = QAbstractItemView::edit(index, trigger, event);   // call super
    if (!_gtable) {
        return result;
    }
    if (result) {
        bool isEdit = _lastKeyPressed == 0 || _lastKeyPressed == Qt::Key_F2 || _lastKeyPressed == Qt::Key_Tab;
        _lastKeyPressed = 0;
        if (isEdit) {
            fireTableEvent(TABLE_EDIT_BEGIN, "tableeditbegin", index.row(), index.column());
        } else {
            fireTableEvent(TABLE_REPLACE_BEGIN, "tablereplacebegin", index.row(), index.column());
        }
    }
    return result;
}

void _Internal_QTableWidget::fireTableEvent(EventType eventType, const std::string& eventName, int row, int col) {
    if (!_gtable) {
        return;
    }
    GEvent tableEvent(
                /* class  */ TABLE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ _gtable);
    if (row < 0 && col < 0) {
        tableEvent.setRowAndColumn(_gtable->getSelectedRow(), _gtable->getSelectedColumn());
    } else {
        tableEvent.setRowAndColumn(row, col);
    }
    tableEvent.setActionCommand(_gtable->getActionCommand());
    _gtable->fireEvent(tableEvent);
}

QWidget* _Internal_QTableWidget::getEditor() const {
    return _delegate->getEditor();
}

_Internal_QItemDelegate* _Internal_QTableWidget::getItemDelegate() const {
    return _delegate;
}

bool _Internal_QTableWidget::isEditing() const {
    return getEditor() != nullptr;
}

void _Internal_QTableWidget::closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint) {
    QTableWidget::closeEditor(editor, hint);
    if (!_gtable) {
        return;
    }
    // TODO: doesn't this fire even if the edit is committed?
    fireTableEvent(TABLE_EDIT_CANCEL, "tableeditcancel");
}

void _Internal_QTableWidget::handleCellChange(int row, int column) {
    if (!_gtable) {
        return;
    }
    fireTableEvent(TABLE_UPDATED, "tableupdate", row, column);
}

void _Internal_QTableWidget::handleCellDoubleClick(int /*row*/, int /*column*/) {
    if (!_gtable) {
        return;
    }
    _lastKeyPressed = Qt::Key_F2;   // pretend we pressed F2
    // edit/replace begin event will be fired by edit()
    // fireTableEvent(GEvent::TABLE_EDIT_BEGIN, "tableeditbegin", row, column);
}

void _Internal_QTableWidget::handleSelectionChange(const QItemSelection& selected, const QItemSelection& /*deselected*/) {
    if (!_gtable) {
        return;
    }
    QItemSelectionRange range;
    if (!selected.empty()) {
        range = selected.at(0);
        QPersistentModelIndex index = range.topLeft();
        fireTableEvent(TABLE_SELECTED, "tableselect", index.row(), index.column());
    }
}

void _Internal_QTableWidget::keyPressEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QTableWidget::keyPressEvent", "event");
    if (!_gtable) {
        QTableWidget::keyPressEvent(event);   // call super
        return;
    }
    _lastKeyPressed = event->key();
    bool wasEditing = isEditing();
    if (!wasEditing && event->key() == Qt::Key_Delete) {
        // clear data from selected cell
        if (_gtable->hasSelectedCell()) {
            GridLocation loc = _gtable->getSelectedCell();
            _gtable->clearCell(loc.row, loc.col);
            return;
        }
    }

    // any other key
    if (wasEditing || !_gtable->hasSelectedCell()) {
        QTableWidget::keyPressEvent(event);   // call super
        return;
    }

    bool nowEditing = isEditing();

    if (GClipboard::isCut(event)) {
        // keyboard "cut" command; remove data from cell into clipboard
        GridLocation loc = _gtable->getSelectedCell();
        std::string cellValue = _gtable->get(loc.row, loc.col);
        GClipboard::set(cellValue);
        _gtable->clearCell(loc.row, loc.col);
        QTableWidget::keyPressEvent(event);   // call super
        fireTableEvent(TABLE_CUT, "tablecut");
        return;
    }

    if (GClipboard::isCopy(event)) {
        // keyboard "copy" command; copy data from cell into clipboard
        std::string cellValue = _gtable->getSelectedCellValue();
        GClipboard::set(cellValue);
        QTableWidget::keyPressEvent(event);   // call super
        fireTableEvent(TABLE_COPY, "tablecopy");
        return;
    }

    if (GClipboard::isPaste(event)) {
        // keyboard "paste" command; copy data from clipboard into cell
        std::string cellValue = GClipboard::get();
        _gtable->setSelectedCellValue(cellValue);
        QTableWidget::keyPressEvent(event);   // call super
        fireTableEvent(TABLE_PASTE, "tablepaste");
        return;
    }

    // if cell went from non-editing state to editing state, edit has begun
    if (nowEditing) {
        if (event->key() == Qt::Key_F2) {
            // F2 key begins editing existing value for a cell
            // edit_begin will be fired by edit() method
            // fireTableEvent(GEvent::TABLE_EDIT_BEGIN, "tableeditbegin");
        } else if (event->key() == Qt::Key_Tab) {
            // Tab key jumps to edit the neighboring cell
        } else {
            // any other text starts replacing the value with a new value
            // replace_begin will be fired by edit() method
            // fireTableEvent(GEvent::TABLE_REPLACE_BEGIN, "tablereplacebegin");
        }
    }
    QTableWidget::keyPressEvent(event);   // call super
}

QSize _Internal_QTableWidget::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QTableWidget::sizeHint();
    }
}


