/*
 * File: gtable.h
 * --------------
 * This file exports the GTable class for a graphical editable 2D table.
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gtable.h to replace Java version
 * @version 2018/07/17
 * - initial version, based on gtable.h
 * @since 2018/07/17
 */

#ifndef _gtable_h
#define _gtable_h

#include <string>
#include <QAbstractItemModel>
#include <QBrush>
#include <QFont>
#include <QItemSelection>
#include <QStyledItemDelegate>
#include <QWidget>
#include <QTableWidget>
#include "grid.h"
#include "map.h"
#include "ginteractor.h"
#include "gobjects.h"
#include "gtypes.h"

// forward declaration
class GTable;

// Internal class; not to be used by clients.
class _Internal_QItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    _Internal_QItemDelegate(QObject* parent = nullptr);
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void destroyEditor(QWidget* editor, const QModelIndex& index) const;
    virtual QWidget* getEditor() const;

private:
    QWidget* _editor;
};


// Internal class; not to be used by clients.
class _Internal_QTableWidget : public QTableWidget, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QTableWidget(GTable* gtable, int rows, int columns, QWidget* parent = nullptr);
    virtual bool edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event) Q_DECL_OVERRIDE;
    virtual QWidget* getEditor() const;
    virtual _Internal_QItemDelegate* getItemDelegate() const;
    virtual bool isEditing() const;
    virtual void closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleCellChange(int row, int column);
    void handleCellDoubleClick(int row, int column);
    void handleSelectionChange(const QItemSelection& selected, const QItemSelection& deselected);

private:
    GTable* _gtable;
    _Internal_QItemDelegate* _delegate;
    int _lastKeyPressed;

    void fireTableEvent(EventType eventType, const std::string& eventName, int row = -1, int col = -1);
};


/*
 * A QGTable represents a graphical editable 2D table, like a mediocre facsimile
 * of an Excel spreadsheet.
 * After creating a QGTable, you can listen for table events to be notified
 * when the user types a new value into a table cell by calling setTableListener.
 *
 * All row/column indexes in this class are 0-based.
 */
class GTable : public GInteractor {
public:
    /*
     * Styles of column header labels that can be shown.
     */
    enum ColumnHeaderStyle {
        COLUMN_HEADER_NONE,     // headers will not show
        COLUMN_HEADER_EXCEL,    // A, B, ..., Z, AA, AB, ...
        COLUMN_HEADER_NUMERIC   // 1, 2, 3, ...
    };

    /*
     * Constructs a new table with the given dimensions and (optional) size.
     * If x, y, width, or height are omitted, they are set automatically by
     * the layout manager of the GWindow into which the table is placed.
     * This is often what you want.
     * Throws an error if the number of rows, columns, width, or height is negative.
     */
    GTable(int rows = 0, int columns = 0, double width = 0, double height = 0,
            QWidget* parent = nullptr);

    /*
     * Changes widths of all columns to be perfectly large enough
     * to fit their contents.
     */
    virtual void autofitColumnWidths();

    /*
     * Sets all cells in the table to store an empty string value.
     */
    virtual void clear();

    /*
     * Sets the given cell to store an empty string value.
     * Throws an error if the given row/column index is out of bounds.
     */
    virtual void clearCell(int row, int column);

    /*
     * Removes any per-cell/column/row formatting that has been applied to the table.
     */
    virtual void clearFormatting();

    /*
     * Removes any formatting that has been applied to the given cell.
     */
    virtual void clearCellFormatting(int row, int column);

    /*
     * Deselects any currently selected cell.
     * If no cell is selected, calling this has no effect.
     */
    virtual void clearSelection();

    /*
     * Sets every cell in the table to have the given value.
     */
    virtual void fill(const std::string& text);

    /*
     * Returns the text stored in the given cell.
     * Throws an error if the given row or column are out of bounds.
     */
    virtual std::string get(int row, int column) const;

    /*
     * Returns the column headers to use the given style.
     * Default is none, but can be set to Excel style or numeric instead.
     */
    virtual ColumnHeaderStyle getColumnHeaderStyle() const;

    /*
     * Returns the width of the given column index in pixels.
     * When a table is constructed, all columns initially have equal width.
     * Throws an error if the given column index is out of bounds.
     */
    virtual double getColumnWidth(int column) const;

    virtual _Internal_QWidget* getInternalWidget() const;

    /*
     * Returns the height of the given row index in pixels.
     * When a table is constructed, all rows initially have equal width.
     * Throws an error if the given row index is out of bounds.
     */
    virtual double getRowHeight(int row) const;

    /*
     * Returns the row and column of the cell that is currently selected
     * by filling the given reference parameters.
     * Sets both row and column to -1 if no cell is currently selected.
     */
    virtual GridLocation getSelectedCell() const;
    virtual void getSelectedCell(int& row, int& column) const;

    /*
     * Returns the text in the cell that is currently selected.
     * If no cell is currently selected, returns an empty string.
     */
    virtual std::string getSelectedCellValue() const;

    /*
     * Returns the column of the cell that is currently selected, or -1 if no cell
     * is currently selected.
     */
    virtual int getSelectedColumn() const;

    /*
     * Returns the row of the cell that is currently selected, or -1 if no cell
     * is currently selected.
     */
    virtual int getSelectedRow() const;

    /* virtual methods from GInteractor */
    virtual std::string getType() const;

    virtual QWidget* getWidget() const;

    /*
     * Returns true if a cell is currently selected.
     */
    virtual bool hasSelectedCell() const;

    /*
     * Returns the number of rows in the table.
     * Equivalent to numRows().
     */
    virtual int height() const;

    /*
     * Returns true if the given 0-based row/column index is within the bounds
     * of the table.  In other words, whether the index is between (0, 0)
     * and (numRows-1, numCols-1) inclusive.
     */
    virtual bool inBounds(int row, int column) const;

    /*
     * Returns whether cells of the table can be edited.
     * Defaults to true when a table is initially created.
     */
    virtual bool isEditable() const;

    /*
     * Returns the number of columns in the table.
     * Equivalent to width().
     */
    virtual int numCols() const;

    /*
     * Returns the number of rows in the table.
     * Equivalent to height().
     */
    virtual int numRows() const;

    virtual void removeTableEventListener();

    virtual void requestFocus() Q_DECL_OVERRIDE;

    /*
     * Modifies the table to have the given number of rows and columns.
     * Any existing data is retained, and any new cells are empty.
     * Throws an error if numRows or numCols is negative.
     */
    virtual void resize(int numRows, int numCols);

    /*
     * Returns whether row and column headers are shown in the table.
     * Initially false.
     */
    virtual bool rowColumnHeadersVisible() const;

    /*
     * Sets the given cell to become currently selected,
     * replacing any previous selection.
     * Note that the user can click a different cell to select it afterward.
     * If the given row/column index is out of bounds, no cell is selected
     * and any currently selected cell is deselected.
     */
    virtual void select(int row, int column);

    /*
     * Modifies the value in the given cell to store the given text.
     * Throws an error if the given row or column are out of bounds.
     */
    virtual void set(int row, int column, const std::string& text);

    /*
     * Sets the background color that appears behind each cell.
     */
    virtual void setBackground(int rgb) Q_DECL_OVERRIDE;
    virtual void setBackground(const std::string& color) Q_DECL_OVERRIDE;

    /*
     * Member functions for per-cell formatting.
     * Throws an error if the given row/column index is out of bounds.
     */
    virtual void setCellAlignment(int row, int column, HorizontalAlignment alignment);
    virtual void setCellBackground(int row, int column, int color);
    virtual void setCellBackground(int row, int column, const std::string& color);
    virtual void setCellFont(int row, int column, const std::string& font);
    virtual void setCellForeground(int row, int column, int color);
    virtual void setCellForeground(int row, int column, const std::string& color);

    /*
     * Sets the color used for the text of each cell.
     * Equivalent to setForeground.
     */
    virtual void setColor(int rgb) Q_DECL_OVERRIDE;
    virtual void setColor(const std::string& color) Q_DECL_OVERRIDE;

    /*
     * Member functions for per-column formatting.
     * Throws an error if the given row/column index is out of bounds.
     */
    virtual void setColumnAlignment(int column, HorizontalAlignment alignment);
    virtual void setColumnBackground(int column, int color);
    virtual void setColumnBackground(int column, const std::string& color);
    virtual void setColumnFont(int column, const std::string& font);
    virtual void setColumnForeground(int column, int color);
    virtual void setColumnForeground(int column, const std::string& color);

    /*
     * Sets the given column index to have the given width in pixels.
     * Throws an error if the given column index is out of bounds
     * or if the width is negative.
     */
    virtual void setColumnWidth(int column, double width);

    /*
     * Sets the column headers to use the given style.
     * Default is none, but can be set to Excel style or numeric instead.
     */
    virtual void setColumnHeaderStyle(ColumnHeaderStyle style);

    /*
     * Sets whether cells of the table can be edited.
     */
    virtual void setEditable(bool editable);

    /*
     * Modifies the value in the cell that is currently being edited to store the given text.
     * This does not modify the value in the table cell but merely the value in the editor widget.
     * Throws an error if the given row or column are out of bounds.
     */
    virtual void setEditorValue(int row, int column, const std::string& text);

    /*
     * Sets the font used to display each cell's text.
     */
    virtual void setFont(const std::string& font) Q_DECL_OVERRIDE;

    /*
     * Sets the color used for the text of each cell.
     * Equivalent to setColor.
     */
    virtual void setForeground(int rgb) Q_DECL_OVERRIDE;
    virtual void setForeground(const std::string& color) Q_DECL_OVERRIDE;

    /*
     * Sets the horizontal alignment of the text in all cells in the table.
     * The alignment can be LEFT, CENTER, or RIGHT and is initially LEFT.
     * Currently a table's alignment is global and applies to all cells.
     */
    virtual void setHorizontalAlignment(HorizontalAlignment alignment);

    /*
     * Member functions for per-row formatting.
     * Throws an error if the given row/column index is out of bounds.
     */
    virtual void setRowAlignment(int row, HorizontalAlignment alignment);
    virtual void setRowBackground(int row, int color);
    virtual void setRowBackground(int row, const std::string& color);
    virtual void setRowFont(int row, const std::string& font);
    virtual void setRowForeground(int row, int color);
    virtual void setRowForeground(int row, const std::string& color);

    /*
     * Sets whether row and column headers should be shown in the table.
     * Initially false.
     */
    virtual void setRowColumnHeadersVisible(bool visible);

    /*
     * Sets the given row index to have the given height in pixels.
     * Throws an error if the given row index is out of bounds
     * or if the height is negative.
     */
    virtual void setRowHeight(int row, double width);

    /*
     * Sets the text in the cell that is currently selected.
     * If no cell is currently selected, does nothing.
     */
    virtual void setSelectedCellValue(const std::string& text);

    /*
     * Sets the given function to be called when events occur in this table.
     */
    virtual void setTableListener(GEventListener func);
    virtual void setTableListener(GEventListenerVoid func);

    /*
     * Returns the number of columns in the table.
     * Equivalent to numCols().
     */
    virtual int width() const;

private:
    // Represents cascading styles on a cell, row, column, or table.
    struct TableStyle {
        int background;
        int foreground;
        std::string font;
        HorizontalAlignment alignment;
        // TODO: borders?

        TableStyle() {
            background = 0;
            foreground = 0;
            font = "";
            alignment = ALIGN_LEFT;
        }

        bool isSet() const {
            return background >= 0
                    && foreground >= 0
                    && !font.empty()
                    && alignment >= 0;
        }

        void mergeWith(const TableStyle& other) {
            if (other.background >= 0) {
                background = other.background;
            }
            if (other.foreground >= 0) {
                foreground = other.foreground;
            }
            if (!other.font.empty()) {
                font = other.font;
            }
            if (other.alignment >= 0) {
                alignment = other.alignment;
            }
        }

        TableStyle mergedWith(const TableStyle& other) {
            TableStyle copy = *this;
            copy.mergeWith(other);
            return copy;
        }

        static TableStyle unset() {
            TableStyle style;
            style.background = -1;
            style.foreground = -1;
            style.font = "";
            style.alignment = (HorizontalAlignment) -1;
            return style;
        }
    };

    // static variables for default formatting:
    // background/foreground colors
    // font
    // alignment
    static TableStyle _defaultCellStyle;

    // member variables
    _Internal_QTableWidget* _iqtableview;
    ColumnHeaderStyle _columnHeaderStyle;

    // styles on table, rows, columns, cells
    Map<int, TableStyle> _rowStyles;
    Map<int, TableStyle> _columnStyles;
    TableStyle _globalCellStyle;

    void applyStyleToCell(int row, int column, const TableStyle& style);

    /*
     * Throws an error if the given numRows/numCols values are negative.
     */
    void checkDimensions(const std::string& member, int numRows, int numCols) const;

    /*
     * Throws an error if the given row/column values are out of bounds.
     */
    void checkIndex(const std::string& member, int row, int column) const;

    /*
     * Throws an error if the given width/height values are out of bounds.
     */
    void checkSize(const std::string& member, double width, double height) const;

    void ensureColumnStyle(int column);
    void ensureDefaultFormatting() const;   // const hack
    void ensureGlobalCellStyle();
    void ensureRowStyle(int row);
    TableStyle getMergedStyleForCell(int row, int column);

    // Internal setters for cell formatting.
    virtual void setCellAlignmentInternal(int row, int column, HorizontalAlignment alignment);
    virtual void setCellBackgroundInternal(int row, int column, int color);
    virtual void setCellFontInternal(int row, int column, const std::string& font);
    virtual void setCellForegroundInternal(int row, int column, int color);

    static std::string toExcelColumnName(int col);
    // static GridLocation toRowColumn(const std::string& excelColumnName);

    void updateColumnHeaders();

    friend class _Internal_QTableWidget;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gtable_h
