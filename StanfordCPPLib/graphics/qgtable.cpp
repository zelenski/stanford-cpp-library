/*
 * File: qgtable.cpp
 * -----------------
 * This file implements the qgtable.h interface.
 * See that file for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2018/07/17
 * - initial version, based on gtable.h
 * @since 2018/07/17
 */

#include "qgtable.h"
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "qgclipboard.h"
#include "qgevent.h"

_Internal_QTableWidget::_Internal_QTableWidget(QGTable* qgtable, int rows, int columns, QWidget* parent)
        : QTableWidget(rows, columns, parent),
          _qgtable(qgtable) {
    // empty
    // this->model()->dataChanged();
    connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(handleActivate(QModelIndex)));
    connect(this, SIGNAL(cellActivated(int, int)), this, SLOT(handleCellActivate(int, int)));
    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(handleCellChange(int, int)));
    // connect(this, SIGNAL(cellClicked(int, int)), this, SLOT(handleCellClick(int, int)));
    connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(handleCellDoubleClick(int, int)));
    // connect(this, SIGNAL(cellEntered(int, int)), this, SLOT(handleCellEnter(int, int)));
    // connect(this, SIGNAL(cellPressed(int, int)), this, SLOT(handleCellPress(int, int)));
    // connect(this->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)),
    //         this, SLOT(handleModelDataChange(const QModelIndex&, const QModelIndex&, const QVector<int>&)));
    connect(this, SIGNAL(entered(QModelIndex)), this, SLOT(handleEnter(QModelIndex)));
    connect(this, SIGNAL(itemActivated(QTableWidgetItem*)), this, SLOT(handleItemActivate(QTableWidgetItem*)));
    connect(this, SIGNAL(itemEntered(QTableWidgetItem*)), this, SLOT(handleItemEnter(QTableWidgetItem*)));
    // connect(this, SIGNAL(item
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(handleItemSelectionChange()));
    connect(this->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(handleSelectionChange(const QItemSelection&, const QItemSelection&)));
//    connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(handleClick(const QModelIndex&)));
//    connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(handleDoubleClick(const QModelIndex&)));
}

void _Internal_QTableWidget::fireTableEvent(QGEvent::EventType eventType, const std::string& eventName, int row, int col) {
    QGEvent tableEvent(
                /* class  */ QGEvent::TABLE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ _qgtable);
    if (row < 0 && col < 0) {
        tableEvent.setRowAndColumn(_qgtable->getSelectedRow(), _qgtable->getSelectedColumn());;
    } else {
        tableEvent.setRowAndColumn(row, col);
    }
    tableEvent.setActionCommand(_qgtable->getActionCommand());
    _qgtable->fireEvent(tableEvent);
}

bool _Internal_QTableWidget::isEditing() const {
    return _qgtable->hasSelectedCell() && state() == QAbstractItemView::EditingState;
}

#include <iostream>
using namespace std;

void _Internal_QTableWidget::closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint) {
    cout << "  DEBUG: closeEditor" << endl;
    QTableWidget::closeEditor(editor, hint);
}

void _Internal_QTableWidget::closePersistentEditor(const QModelIndex& index) {
    cout << "  DEBUG: closePersistentEditor" << endl;
    QAbstractItemView::closePersistentEditor(index);
}

void _Internal_QTableWidget::openPersistentEditor(const QModelIndex& index) {
    cout << "  DEBUG: openPersistentEditor" << endl;
    QAbstractItemView::openPersistentEditor(index);
}

void _Internal_QTableWidget::handleActivate(QModelIndex index) {
    cout << "  DEBUG: handleActivate " << index.row() << ", " << index.column() << endl;
}

void _Internal_QTableWidget::handleCellActivate(int row, int column) {
    cout << "  DEBUG: handleCellActivate " << row << ", " << column << endl;
}

void _Internal_QTableWidget::handleCellChange(int row, int column) {
    cout << "  DEBUG: handleCellChange " << row << ", " << column << endl;
    fireTableEvent(QGEvent::TABLE_UPDATED, "tableupdate", row, column);
}

void _Internal_QTableWidget::handleCellClick(int row, int column) {
    cout << "  DEBUG: handleCellClick " << row << ", " << column << endl;
    // TODO
//    QGEvent tableEvent(
//                /* class  */ QGEvent::TABLE_EVENT,
//                /* type   */ QGEvent::TABLE_EDIT_BEGIN,
//                /* name   */ "table",
//                /* source */ _qgtable);
//    tableEvent.setActionCommand(_qgtable->getActionCommand());
//    _qgtable->fireEvent(tableEvent);
}

void _Internal_QTableWidget::handleCellDoubleClick(int row, int column) {
    cout << "  DEBUG: handleCellDoubleClick " << row << ", " << column << endl;
    // TODO
//    QGEvent tableEvent(
//                /* class  */ QGEvent::TABLE_EVENT,
//                /* type   */ QGEvent::TABLE_EDIT_BEGIN,
//                /* name   */ "table",
//                /* source */ _qgtable);
//    tableEvent.setActionCommand(_qgtable->getActionCommand());
//    _qgtable->fireEvent(tableEvent);
}

void _Internal_QTableWidget::handleCellEnter(int row, int column) {
    cout << "  DEBUG: handleCellEnter " << row << ", " << column << endl;
}

void _Internal_QTableWidget::handleCellPress(int row, int column) {
    cout << "  DEBUG: handleCellPress " << row << ", " << column << endl;
}

void _Internal_QTableWidget::handleCurrentCellChange(int row, int column, int prevRow, int prevColumn) {
    cout << "  DEBUG: handleCurrentCellChange " << prevRow << "," << prevColumn << " -> " << row << "," << column << endl;
}

void _Internal_QTableWidget::handleEnter(QModelIndex index) {
    cout << "  DEBUG: handleEnter " << index.row() << "," << index.column() << endl;
}

void _Internal_QTableWidget::handleItemEnter(QTableWidgetItem* item) {
    cout << "handleItemEnter " << item->row() << "," << item->column() << endl;
}

void _Internal_QTableWidget::handleItemActivate(QTableWidgetItem* item) {
    cout << "handleItemActivate " << item->row() << "," << item->column() << endl;
}

void _Internal_QTableWidget::handleItemSelectionChange() {
    cout << "handleItemSelectionChange" << endl;
}

#include "vector.h"

void _Internal_QTableWidget::handleModelDataChange(const QModelIndex& topLeft,
                                                   const QModelIndex& bottomRight,
                                                   const QVector<int>& roles) {
    cout << "  DEBUG: handleModelDataChange TL=" << topLeft.row() << "," << topLeft.column()
         << ", BR=" << bottomRight.row() << "," << bottomRight.column() << ", roles=" << Vector<int>(roles.toStdVector()) << endl;
    fireTableEvent(QGEvent::TABLE_UPDATED, "tableupdate");
}

void _Internal_QTableWidget::handleSelectionChange(const QItemSelection& selected, const QItemSelection& deselected) {
    QItemSelectionRange range;
    if (!selected.empty()) {
        range = selected.at(0);
        QPersistentModelIndex index = range.topLeft();
        cout << "DEBUG: handleSelectionChange sel=" << index.row() << "," << index.column() << endl;;
        fireTableEvent(QGEvent::TABLE_SELECTED, "tableselect");
    }
    if (!deselected.empty()) {
        QItemSelectionRange range = deselected.at(0);
        QPersistentModelIndex index = range.topLeft();
        cout << "  DEBUG: handleSelectionChange desel=" << index.row() << "," << index.column() << endl;;
    }
}

void _Internal_QTableWidget::keyPressEvent(QKeyEvent* event) {
    bool wasEditing = isEditing();
    if (!wasEditing && event->key() == Qt::Key_Delete) {
        // clear data from selected cell
        if (_qgtable->hasSelectedCell()) {
            GridLocation loc = _qgtable->getSelectedCell();
            _qgtable->clearCell(loc.row, loc.col);
            return;
        }
    }

    // any other key
    QTableWidget::keyPressEvent(event);   // call super
    if (wasEditing || !_qgtable->hasSelectedCell()) {
        return;
    }

    bool nowEditing = isEditing();

    if (QGClipboard::isCut(event)) {
        // keyboard "cut" command; remove data from cell into clipboard
        GridLocation loc = _qgtable->getSelectedCell();
        std::string cellValue = _qgtable->get(loc.row, loc.col);
        QGClipboard::set(cellValue);
        _qgtable->clearCell(loc.row, loc.col);
        fireTableEvent(QGEvent::TABLE_CUT, "tablecut");
        return;
    }

    if (QGClipboard::isCopy(event)) {
        // keyboard "copy" command; copy data from cell into clipboard
        std::string cellValue = _qgtable->getSelectedCellValue();
        QGClipboard::set(cellValue);
        fireTableEvent(QGEvent::TABLE_COPY, "tablecopy");
        return;
    }

    if (QGClipboard::isPaste(event)) {
        // keyboard "paste" command; copy data from clipboard into cell
        std::string cellValue = QGClipboard::get();
        _qgtable->setSelectedCellValue(cellValue);
        fireTableEvent(QGEvent::TABLE_PASTE, "tablepaste");
        return;
    }

    // if cell went from non-editing state to editing state, edit has begun
    if (nowEditing) {
        if (event->key() == Qt::Key_F2) {
            // F2 key begins editing existing value for a cell
            fireTableEvent(QGEvent::TABLE_EDIT_BEGIN, "tableeditbegin");
        } else {
            // any other text starts replacing the value with a new value
            fireTableEvent(QGEvent::TABLE_REPLACE_BEGIN, "tablereplacebegin");
        }
    }
}

QSize _Internal_QTableWidget::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QTableWidget::sizeHint();
    }
}


QGTable::QGTable(int rows, int columns, double width, double height, QWidget* parent)
        : _iqtableview(nullptr),
          _alignment(Alignment::LEFT),
          _rowColHeadersVisible(false),
          _columnHeaderStyle(QGTable::COLUMN_HEADER_NONE) {
    _iqtableview = new _Internal_QTableWidget(this, rows, columns, getInternalParent(parent));
    _iqtableview->setSelectionMode(QAbstractItemView::SingleSelection);
    checkDimensions("constructor", rows, columns);
    checkSize("constructor", width, height);
}

void QGTable::autofitColumnWidths() {
    // TODO
    // stanfordcpplib::getPlatform()->qgtable_autofitColumnWidths(this);
}

void QGTable::clear() {
    fill("");
}

void QGTable::clearCell(int row, int column) {
    set(row, column, /* text */ "");
}

void QGTable::clearFormatting() {
    // TODO
    // stanfordcpplib::getPlatform()->qgtable_clearFormatting(this);
}

void QGTable::clearSelection() {
    _iqtableview->clearSelection();
}

void QGTable::fill(const std::string& text) {
    int nr = numRows();
    int nc = numCols();
    for (int r = 0; r < nr; r++) {
        for (int c = 0; c < nc; c++) {
            set(r, c, text);
        }
    }
}

std::string QGTable::get(int row, int column) const {
    checkIndex("get", row, column);
    return _iqtableview->model()->data(_iqtableview->model()->index(row, column)).toString().toStdString();
}

QGTable::ColumnHeaderStyle QGTable::getColumnHeaderStyle() const {
    return _columnHeaderStyle;
}

double QGTable::getColumnWidth(int column) const {
    checkIndex("getColumnWidth", /* row */ 0, column);
    return _iqtableview->columnWidth(column);
}

QGTable::Alignment QGTable::getHorizontalAlignment() const {
    return _alignment;
}

_Internal_QWidget* QGTable::getInternalWidget() const {
    return _iqtableview;
}

GridLocation QGTable::getSelectedCell() const {
    QModelIndexList list = _iqtableview->selectionModel()->selectedIndexes();
    if (list.empty()) {
        return GridLocation(-1, -1);
    } else {
        QModelIndex index = list.at(0);
        return GridLocation(index.row(), index.column());
    }
}

void QGTable::getSelectedCell(int& row, int& column) const {
    GridLocation loc = getSelectedCell();
    row = loc.row;
    column = loc.col;
}

std::string QGTable::getSelectedCellValue() const {
    if (hasSelectedCell()) {
        GridLocation loc = getSelectedCell();
        return get(loc.row, loc.col);
    } else {
        return "";
    }
}

int QGTable::getSelectedColumn() const {
    return getSelectedCell().col;
}

int QGTable::getSelectedRow() const {
    return getSelectedCell().row;
}

std::string QGTable::getType() const {
    return "QGTable";
}

QWidget* QGTable::getWidget() const {
    return static_cast<QWidget*>(_iqtableview);
}

bool QGTable::hasSelectedCell() const {
    GridLocation loc = getSelectedCell();
    return loc.row >= 0 && loc.col >= 0;
}

int QGTable::height() const {
    return numRows();
}

bool QGTable::inBounds(int row, int column) const {
    return 0 <= row && row < height() && 0 <= column && column < width();
}

bool QGTable::isEditable() const {
    return (_iqtableview->editTriggers() & QAbstractItemView::NoEditTriggers) != 0;
}

int QGTable::numCols() const {
    return _iqtableview->model()->columnCount();
}

int QGTable::numRows() const {
    return _iqtableview->model()->rowCount();
}

void QGTable::removeTableEventHandler() {
    removeEventHandlers({"table",
                         "tableupdate",
                         "tableselect",
                         "tableeditbegin",
                         "tablereplacebegin",
                         "tablecut",
                         "tablecopy",
                         "tablepaste"});
}

void QGTable::resize(int numRows, int numCols) {
    checkDimensions("resize", numRows, numCols);
    // TODO
}

bool QGTable::rowColumnHeadersVisible() const {
    // TODO
    return true;
}

void QGTable::select(int row, int column) {
    QModelIndex index = _iqtableview->model()->index(row, column);
    _iqtableview->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
}

void QGTable::set(int row, int column, const std::string& text) {
    checkIndex("set", row, column);
    QModelIndex index = _iqtableview->model()->index(row, column);
    _iqtableview->model()->setData(index, QVariant(text.c_str()));
}

void QGTable::setCellAlignment(int row, int column, Alignment /* alignment */) {
    checkIndex("setCellAlignment", row, column);
    // TODO
}

void QGTable::setCellBackground(int row, int column, int /* color */) {
    checkIndex("setCellBackground", row, column);
    // TODO
    // std::string colorStr = convertRGBToColor(color);
}

void QGTable::setCellBackground(int row, int column, const std::string& /* color */) {
    checkIndex("setCellBackground", row, column);
    // TODO
}

void QGTable::setCellFont(int row, int column, const std::string& /* font */) {
    checkIndex("setCellFont", row, column);
    // TODO
}

void QGTable::setCellForeground(int row, int column, int /* color */) {
    checkIndex("setCellForeground", row, column);
    // TODO
    // std::string colorStr = convertRGBToColor(color);
}

void QGTable::setCellForeground(int row, int column, const std::string& /* color */) {
    checkIndex("setCellForeground", row, column);
    // TODO
}

void QGTable::setColumnAlignment(int column, Alignment /* alignment */) {
    checkIndex("setColumnAlignment", /* row */ 0, column);
    // TODO
}

void QGTable::setColumnBackground(int column, int /* color */) {
    checkIndex("setColumnBackground", /* row */ 0, column);
    // TODO
    // std::string colorStr = convertRGBToColor(color);
}

void QGTable::setColumnBackground(int column, const std::string& /* color */) {
    checkIndex("setColumnBackground", /* row */ 0, column);
    // TODO
}

void QGTable::setColumnFont(int column, const std::string& /* font */) {
    checkIndex("setColumnFont", /* row */ 0, column);
    // TODO
}

void QGTable::setColumnForeground(int column, int /* color */) {
    checkIndex("setColumnForeground", /* row */ 0, column);
    // TODO
    // std::string colorStr = convertRGBToColor(color);
}

void QGTable::setColumnForeground(int column, const std::string& /* color */) {
    checkIndex("setColumnForeground", /* row */ 0, column);
    // TODO
}

void QGTable::setColumnHeaderStyle(QGTable::ColumnHeaderStyle /* style */) {
    // TODO
}

void QGTable::setColumnWidth(int column, double width) {
    checkIndex("setColumnWidth", /* row */ 0, column);
    if (width < 0) {
        error("QGTable::setColumnWidth: width cannot be negative");
    }
    // TODO
}

void QGTable::setEditable(bool editable) {
    if (editable) {
        _iqtableview->setEditTriggers(
                    QAbstractItemView::CurrentChanged
                    | QAbstractItemView::DoubleClicked
                    | QAbstractItemView::EditKeyPressed
                    | QAbstractItemView::AnyKeyPressed);
    } else {
        _iqtableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void QGTable::setEditorValue(int row, int column, const std::string& /*text*/) {
    checkIndex("setEditorValue", row, column);
    // TODO
}

void QGTable::setHorizontalAlignment(QGTable::Alignment alignment) {
    std::string alignmentStr;
    if (alignment == Alignment::LEFT) {
        alignmentStr = "LEFT";
    } else if (alignment == Alignment::CENTER) {
        alignmentStr = "CENTER";
    } else if (alignment == Alignment::RIGHT) {
        alignmentStr = "RIGHT";
    }
    this->_alignment = alignment;
    // TODO: align all cells?
}

void QGTable::setRowAlignment(int row, Alignment /* alignment */) {
    checkIndex("setRowAlignment", row, /* column */ 0);
    // TODO: align all cells in row?
}

void QGTable::setRowBackground(int row, int /*color*/) {
    checkIndex("setRowBackground", row, /* column */ 0);
    // TODO
    // std::string colorStr = convertRGBToColor(color);
}

void QGTable::setRowBackground(int row, const std::string& /*color*/) {
    checkIndex("setRowBackground", row, /* column */ 0);
    // TODO
}

void QGTable::setRowFont(int row, const std::string& /*font*/) {
    checkIndex("setRowFont", row, /* column */ 0);
    // TODO
}

void QGTable::setRowForeground(int row, int /*color*/) {
    checkIndex("setRowForeground", row, /* column */ 0);
    // TODO
    // std::string colorStr = convertRGBToColor(color);
}

void QGTable::setRowForeground(int row, const std::string& /*color*/) {
    checkIndex("setRowForeground", row, /* column */ 0);
    // TODO
}

void QGTable::setRowColumnHeadersVisible(bool visible) {
    _rowColHeadersVisible = visible;
    // TODO
}

void QGTable::setSelectedCellValue(const std::string& text) {
    if (hasSelectedCell()) {
        GridLocation loc = getSelectedCell();
        set(loc.row, loc.col, text);
    }
}

void QGTable::setTableEventHandler(QGEventHandler func) {
    setEventHandlers({"table",
                      "tableupdate",
                      "tableselect",
                      "tableeditbegin",
                      "tablereplacebegin",
                      "tablecut",
                      "tablecopy",
                      "tablepaste"}, func);
}

void QGTable::setTableEventHandler(QGEventHandlerVoid func) {
    setEventHandlers({"table",
                      "tableupdate",
                      "tableselect",
                      "tableeditbegin",
                      "tablereplacebegin",
                      "tablecut",
                      "tablecopy",
                      "tablepaste"}, func);
}


int QGTable::width() const {
    return numCols();
}

void QGTable::checkDimensions(const std::string& member, int numRows, int numCols) const {
    if (numRows < 0 || numCols < 0) {
        std::ostringstream out;
        out << "QGTable::" << member << ": numRows/numCols cannot be negative";
        error(out.str());
    }
}

void QGTable::checkIndex(const std::string& member, int row, int column) const {
    if (!inBounds(row, column)) {
        std::ostringstream out;
        out << "QGTable::" << member
            << ": (row=" << row
            << ", col=" << column
            << ") is out of valid range of (0, 0) through ("
            << numRows() << ", " << numCols() << ")";
        error(out.str());
    }
}

void QGTable::checkSize(const std::string& member, double width, double height) const {
    if (width < 0 || height < 0) {
        std::ostringstream out;
        out << "QGTable::" << member << ": width/height cannot be negative";
        error(out.str());
    }
}
