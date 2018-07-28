/*
 * File: qgtable.cpp
 * -----------------
 * This file implements the qgtable.h interface.
 * See that file for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2018/07/21
 * - rich formatting on cell, row, column, table
 * - improved event handling
 * @version 2018/07/17
 * - initial version, based on gtable.h
 * @since 2018/07/17
 */

#include "qgtable.h"
#include <cstring>
#include <iostream>
#include <iomanip>
#include <QBrush>
#include <QColor>
#include <QHeaderView>
#include <QLineEdit>
#include <sstream>
#include "qgclipboard.h"
#include "qgcolor.h"
#include "qgevent.h"
#include "qgfont.h"
#include "strlib.h"

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


_Internal_QTableWidget::_Internal_QTableWidget(QGTable* qgtable, int rows, int columns, QWidget* parent)
        : QTableWidget(rows, columns, parent),
          _qgtable(qgtable),
          _delegate(nullptr) {
    _delegate = new _Internal_QItemDelegate();
    setItemDelegate(_delegate);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(handleCellChange(int, int)));
    connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(handleCellDoubleClick(int, int)));
    connect(this->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(handleSelectionChange(const QItemSelection&, const QItemSelection&)));
}

void _Internal_QTableWidget::fireTableEvent(QGEvent::EventType eventType, const std::string& eventName, int row, int col) {
    QGEvent tableEvent(
                /* class  */ QGEvent::TABLE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ _qgtable);
    if (row < 0 && col < 0) {
        tableEvent.setRowAndColumn(_qgtable->getSelectedRow(), _qgtable->getSelectedColumn());
    } else {
        tableEvent.setRowAndColumn(row, col);
    }
    tableEvent.setActionCommand(_qgtable->getActionCommand());
    _qgtable->fireEvent(tableEvent);
}

bool _Internal_QTableWidget::edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event) {
    bool result = QAbstractItemView::edit(index, trigger, event);   // call super
    if (result) {
        bool isEdit = _lastKeyPressed == 0 || _lastKeyPressed == Qt::Key_F2 || _lastKeyPressed == Qt::Key_Tab;
        _lastKeyPressed = 0;
        if (isEdit) {
            fireTableEvent(QGEvent::TABLE_EDIT_BEGIN, "tableeditbegin", index.row(), index.column());
        } else {
            fireTableEvent(QGEvent::TABLE_REPLACE_BEGIN, "tablereplacebegin", index.row(), index.column());
        }
    }
    return result;
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
    // TODO: doesn't this fire even if the edit is committed?
    fireTableEvent(QGEvent::TABLE_EDIT_CANCEL, "tableeditcancel");
}

void _Internal_QTableWidget::handleCellChange(int row, int column) {
    fireTableEvent(QGEvent::TABLE_UPDATED, "tableupdate", row, column);
}

void _Internal_QTableWidget::handleCellDoubleClick(int /*row*/, int /*column*/) {
    _lastKeyPressed = Qt::Key_F2;   // pretend we pressed F2
    // edit/replace begin event will be fired by edit()
    // fireTableEvent(QGEvent::TABLE_EDIT_BEGIN, "tableeditbegin", row, column);
}

void _Internal_QTableWidget::handleSelectionChange(const QItemSelection& selected, const QItemSelection& /*deselected*/) {
    QItemSelectionRange range;
    if (!selected.empty()) {
        range = selected.at(0);
        QPersistentModelIndex index = range.topLeft();
        fireTableEvent(QGEvent::TABLE_SELECTED, "tableselect", index.row(), index.column());
    }
}

void _Internal_QTableWidget::keyPressEvent(QKeyEvent* event) {
    _lastKeyPressed = event->key();
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
    if (wasEditing || !_qgtable->hasSelectedCell()) {
        QTableWidget::keyPressEvent(event);   // call super
        return;
    }

    bool nowEditing = isEditing();

    if (QGClipboard::isCut(event)) {
        // keyboard "cut" command; remove data from cell into clipboard
        GridLocation loc = _qgtable->getSelectedCell();
        std::string cellValue = _qgtable->get(loc.row, loc.col);
        QGClipboard::set(cellValue);
        _qgtable->clearCell(loc.row, loc.col);
        QTableWidget::keyPressEvent(event);   // call super
        fireTableEvent(QGEvent::TABLE_CUT, "tablecut");
        return;
    }

    if (QGClipboard::isCopy(event)) {
        // keyboard "copy" command; copy data from cell into clipboard
        std::string cellValue = _qgtable->getSelectedCellValue();
        QGClipboard::set(cellValue);
        QTableWidget::keyPressEvent(event);   // call super
        fireTableEvent(QGEvent::TABLE_COPY, "tablecopy");
        return;
    }

    if (QGClipboard::isPaste(event)) {
        // keyboard "paste" command; copy data from clipboard into cell
        std::string cellValue = QGClipboard::get();
        _qgtable->setSelectedCellValue(cellValue);
        QTableWidget::keyPressEvent(event);   // call super
        fireTableEvent(QGEvent::TABLE_PASTE, "tablepaste");
        return;
    }

    // if cell went from non-editing state to editing state, edit has begun
    if (nowEditing) {
        if (event->key() == Qt::Key_F2) {
            // F2 key begins editing existing value for a cell
            // edit_begin will be fired by edit() method
            // fireTableEvent(QGEvent::TABLE_EDIT_BEGIN, "tableeditbegin");
        } else if (event->key() == Qt::Key_Tab) {
            // Tab key jumps to edit the neighboring cell
        } else {
            // any other text starts replacing the value with a new value
            // replace_begin will be fired by edit() method
            // fireTableEvent(QGEvent::TABLE_REPLACE_BEGIN, "tablereplacebegin");
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


QGTable::TableStyle QGTable::_defaultCellStyle = QGTable::TableStyle::unset();


QGTable::QGTable(int rows, int columns, double width, double height, QWidget* parent)
        : _iqtableview(nullptr),
          _columnHeaderStyle(QGTable::COLUMN_HEADER_NONE) {
    _iqtableview = new _Internal_QTableWidget(this, rows, columns, getInternalParent(parent));
    _iqtableview->setSelectionMode(QAbstractItemView::SingleSelection);
    _globalCellStyle = TableStyle::unset();
    checkDimensions("constructor", rows, columns);
    checkSize("constructor", width, height);
}

void QGTable::applyStyleToCell(int row, int column, const TableStyle& style) {
    setCellAlignmentInternal(row, column, style.alignment);
    setCellBackgroundInternal(row, column, style.background);
    setCellFontInternal(row, column, style.font);
    setCellForegroundInternal(row, column, style.foreground);
}

void QGTable::autofitColumnWidths() {
    _iqtableview->resizeColumnsToContents();
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

void QGTable::clear() {
    _iqtableview->clear();
    // for some reason, clearing a table also wipes the Excel-style column headers
    updateColumnHeaders();
}

void QGTable::clearCell(int row, int column) {
    checkIndex("clearCell", row, column);
    set(row, column, /* text */ "");
}

void QGTable::clearFormatting() {
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
}

void QGTable::clearCellFormatting(int row, int column) {
    checkIndex("clearCellFormatting", row, column);
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
}

void QGTable::clearSelection() {
    _iqtableview->clearSelection();
}

void QGTable::ensureColumnStyle(int column) {
    ensureDefaultFormatting();
    if (!_columnStyles.containsKey(column)) {
        _columnStyles[column] = TableStyle::unset();
    }
}

void QGTable::ensureDefaultFormatting() const {
    QGTable* thisHack = (QGTable*) this;
    if (!_defaultCellStyle.isSet()) {
        QPalette palette = thisHack->_iqtableview->palette();
        _defaultCellStyle.background = palette.base().color().rgb() & 0x00ffffff;
        _defaultCellStyle.foreground = palette.text().color().rgb() & 0x00ffffff;
        _defaultCellStyle.font       = QGFont::toFontString(thisHack->_iqtableview->font());
        _defaultCellStyle.alignment  = qgenum::ALIGN_LEFT;   // TODO: ask cell for its alignment
    }
}

void QGTable::ensureRowStyle(int row) {
    ensureDefaultFormatting();
    if (!_rowStyles.containsKey(row)) {
        _rowStyles[row] = TableStyle::unset();
    }
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

_Internal_QWidget* QGTable::getInternalWidget() const {
    return _iqtableview;
}

QGTable::TableStyle QGTable::getMergedStyleForCell(int row, int column) {
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

double QGTable::getRowHeight(int row) const {
    checkIndex("getRowHeight", row, /* column */ 0);
    return _iqtableview->rowHeight(row);
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

void QGTable::removeTableEventListener() {
    removeEventListeners({"table",
                          "tableupdate",
                          "tableselect",
                          "tableeditbegin",
                          "tablereplacebegin",
                          "tablecut",
                          "tablecopy",
                          "tablepaste"});
}

void QGTable::requestFocus() {
    bool wasEditing = _iqtableview->isEditing();
    QGInteractor::requestFocus();
    if (!wasEditing && hasSelectedCell()) {
        GridLocation loc = getSelectedCell();
        _iqtableview->closePersistentEditor(_iqtableview->item(loc.row, loc.col));
    }
}

void QGTable::resize(int newNumRows, int newNumCols) {
    checkDimensions("resize", newNumRows, newNumCols);
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
}

bool QGTable::rowColumnHeadersVisible() const {
    return _iqtableview->horizontalHeader()->isVisible()
            && _iqtableview->verticalHeader()->isVisible();
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

void QGTable::setBackground(int rgb) {
    // QGInteractor::setBackground(rgb);
    // (don't call super; that will set the BG of the headers and everything)

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
}

void QGTable::setBackground(const std::string& color) {
    setBackground(QGColor::convertColorToRGB(color));
}

void QGTable::setCellAlignment(int row, int column, qgenum::HorizontalAlignment alignment) {
    checkIndex("setCellAlignment", row, column);
    setCellAlignmentInternal(row, column, alignment);   // do the actual work
}

void QGTable::setCellAlignmentInternal(int row, int column, qgenum::HorizontalAlignment alignment) {
    Qt::Alignment align = Qt::AlignVCenter | qgenum::toQtAlignment(alignment);
    _iqtableview->model()->setData(_iqtableview->model()->index(row, column),
                                   QVariant(align), Qt::TextAlignmentRole);
}

void QGTable::setCellBackground(int row, int column, int rgb) {
    checkIndex("setCellBackground", row, column);
    setCellBackgroundInternal(row, column, rgb);   // do the actual work
}

void QGTable::setCellBackground(int row, int column, const std::string& color) {
    setCellBackground(row, column, QGColor::convertColorToRGB(color));
}

void QGTable::setCellBackgroundInternal(int row, int column, int rgb) {
    _iqtableview->item(row, column)->setBackground(QBrush(QColor(rgb)));
}

void QGTable::setCellFont(int row, int column, const std::string& font) {
    checkIndex("setCellFont", row, column);
    setCellFontInternal(row, column, font);   // do the actual work
}

void QGTable::setCellFontInternal(int row, int column, const std::string& font) {
    _iqtableview->item(row, column)->setFont(QGFont::toQFont(_iqtableview->font(), font));
}

void QGTable::setCellForeground(int row, int column, int rgb) {
    checkIndex("setCellForeground", row, column);
    setCellForegroundInternal(row, column, rgb);   // do the actual work
}

void QGTable::setCellForegroundInternal(int row, int column, int rgb) {
    _iqtableview->item(row, column)->setForeground(QBrush(QColor(rgb)));
}

void QGTable::setCellForeground(int row, int column, const std::string& color) {
    checkIndex("setCellForeground", row, column);
    ensureDefaultFormatting();
    setCellForeground(row, column, QGColor::convertColorToRGB(color));
}

void QGTable::setColor(int rgb) {
    setForeground(rgb);
}

void QGTable::setColor(const std::string& color) {
    setForeground(color);
}

void QGTable::setColumnAlignment(int column, qgenum::HorizontalAlignment alignment) {
    checkIndex("setColumnAlignment", /* row */ 0, column);

    // save this alignment in the column style (for later cells on resize() etc)
    ensureColumnStyle(column);
    _columnStyles[column].alignment = alignment;

    // set each cell's alignment in that column
    for (int row = 0, nr = numRows(); row < nr; row++) {
        setCellAlignmentInternal(row, column, alignment);
    }
}

void QGTable::setColumnBackground(int column, int rgb) {
    checkIndex("setColumnBackground", /* row */ 0, column);

    // save this background color in the column style (for later cells on resize() etc)
    ensureColumnStyle(column);
    _columnStyles[column].background = rgb;

    // set each cell's background color in that column
    for (int row = 0, nr = numRows(); row < nr; row++) {
        setCellBackgroundInternal(row, column, rgb);
    }
}

void QGTable::setColumnBackground(int column, const std::string& color) {
    setColumnBackground(column, QGColor::convertColorToRGB(color));
}

void QGTable::setColumnFont(int column, const std::string& font) {
    checkIndex("setColumnFont", /* row */ 0, column);

    // save this font in the column style (for later cells on resize() etc)
    ensureColumnStyle(column);
    _columnStyles[column].font = font;

    // set each cell's font in that column
    for (int row = 0, nr = numRows(); row < nr; row++) {
        setCellFontInternal(row, column, font);
    }
}

void QGTable::setColumnForeground(int column, int rgb) {
    checkIndex("setColumnForeground", /* row */ 0, column);

    // save this foreground color in the column style (for later cells on resize() etc)
    ensureColumnStyle(column);
    _columnStyles[column].foreground = rgb;

    // set each cell's foreground color in that column
    for (int row = 0, nr = numRows(); row < nr; row++) {
        setCellForegroundInternal(row, column, rgb);
    }
}

void QGTable::setColumnForeground(int column, const std::string& color) {
    setColumnForeground(column, QGColor::convertColorToRGB(color));
}

void QGTable::setColumnHeaderStyle(QGTable::ColumnHeaderStyle style) {
    _columnHeaderStyle = style;
    if (style == QGTable::COLUMN_HEADER_NONE) {
        // no headers
        setRowColumnHeadersVisible(false);
        return;
    } else {
        // build list of column names to display
        updateColumnHeaders();
        _iqtableview->horizontalHeader()->setVisible(true);
        _iqtableview->verticalHeader()->setVisible(true);
    }
}

void QGTable::setColumnWidth(int column, double width) {
    checkIndex("setColumnWidth", /* row */ 0, column);
    if (width < 0) {
        error("QGTable::setColumnWidth: width cannot be negative");
    }
    _iqtableview->setColumnWidth(column, (int) width);
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

void QGTable::setEditorValue(int row, int column, const std::string& text) {
    checkIndex("setEditorValue", row, column);
    _Internal_QItemDelegate* delegate = _iqtableview->getItemDelegate();
    if (delegate != nullptr) {
        QWidget* editor = delegate->getEditor();
        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor)) {
            lineEdit->setText(QString::fromStdString(text));
        }
    }
}

void QGTable::setFont(const std::string& font) {
    QGInteractor::setFont(font);   // call super

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
}

void QGTable::setForeground(int rgb) {
    // QGInteractor::setForeground(rgb);
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
}

void QGTable::setForeground(const std::string& color) {
    setForeground(QGColor::convertColorToRGB(color));
}

void QGTable::setHorizontalAlignment(qgenum::HorizontalAlignment alignment) {
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
}

void QGTable::setRowAlignment(int row, qgenum::HorizontalAlignment alignment) {
    checkIndex("setRowAlignment", row, /* column */ 0);

    // save this alignment in the row style (for later cells on resize() etc)
    ensureRowStyle(row);
    _rowStyles[row].alignment = alignment;

    // set each cell's alignment in that row
    for (int col = 0, nc = numCols(); col < nc; col++) {
        setCellAlignmentInternal(row, col, alignment);
    }
}

void QGTable::setRowBackground(int row, int rgb) {
    checkIndex("setRowBackground", row, /* column */ 0);

    // save this background color in the row style (for later cells on resize() etc)
    ensureRowStyle(row);
    _rowStyles[row].background = rgb;

    // set each cell's background color in that row
    for (int col = 0, nc = numCols(); col < nc; col++) {
        setCellBackgroundInternal(row, col, rgb);
    }
}

void QGTable::setRowBackground(int row, const std::string& color) {
    setRowBackground(row, QGColor::convertColorToRGB(color));
}

void QGTable::setRowFont(int row, const std::string& font) {
    checkIndex("setRowFont", row, /* column */ 0);

    // save this font in the row style (for later cells on resize() etc)
    ensureRowStyle(row);
    _rowStyles[row].font = font;

    // set each cell's font in that row
    for (int col = 0, nc = numCols(); col < nc; col++) {
        setCellFontInternal(row, col, font);
    }
}

void QGTable::setRowForeground(int row, int rgb) {
    checkIndex("setRowForeground", row, /* column */ 0);

    // save this foreground color in the row style (for later cells on resize() etc)
    ensureRowStyle(row);
    _rowStyles[row].foreground = rgb;

    // set each cell's foreground color in that row
    for (int col = 0, nc = numCols(); col < nc; col++) {
        setCellForegroundInternal(row, col, rgb);
    }
}

void QGTable::setRowForeground(int row, const std::string& color) {
    checkIndex("setRowForeground", row, /* column */ 0);
    setRowForeground(row, QGColor::convertColorToRGB(color));
}

void QGTable::setRowColumnHeadersVisible(bool visible) {
    _iqtableview->horizontalHeader()->setVisible(visible);
    _iqtableview->verticalHeader()->setVisible(visible);
}

void QGTable::setRowHeight(int row, double height) {
    checkIndex("setRowHeight", row, /* column */ 0);
    if (height < 0) {
        error("QGTable::setRowHeight: height cannot be negative");
    }
    _iqtableview->setRowHeight(row, (int) height);
}

void QGTable::setSelectedCellValue(const std::string& text) {
    if (hasSelectedCell()) {
        GridLocation loc = getSelectedCell();
        set(loc.row, loc.col, text);
    }
}

void QGTable::setTableListener(QGEventListener func) {
    setEventListeners({"table",
                       "tableupdate",
                       "tableselect",
                       "tableeditbegin",
                       "tablereplacebegin",
                       "tablecut",
                       "tablecopy",
                       "tablepaste"}, func);
}

void QGTable::setTableListener(QGEventListenerVoid func) {
    setEventListeners({"table",
                       "tableupdate",
                       "tableselect",
                       "tableeditbegin",
                       "tablereplacebegin",
                       "tablecut",
                       "tablecopy",
                       "tablepaste"}, func);
}

std::string QGTable::toExcelColumnName(int col) {
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

void QGTable::updateColumnHeaders() {
    if (_columnHeaderStyle == QGTable::COLUMN_HEADER_NONE) {
        return;
    }

    // Qt wants me to put the headers into a string list, and apparently
    // you add things to the list using << like it was a ostream (bleh)
    QStringList columnHeaders;
    for (int col = 0, nc = numCols(); col < nc; col++) {
        if (_columnHeaderStyle == QGTable::COLUMN_HEADER_EXCEL) {
            columnHeaders << QString::fromStdString(toExcelColumnName(col));
        } else {
            // style == QGTable::COLUMN_HEADER_NUMERIC
            columnHeaders << QString::fromStdString(integerToString(col));
        }
    }
    _iqtableview->setHorizontalHeaderLabels(columnHeaders);
}

int QGTable::width() const {
    return numCols();
}
