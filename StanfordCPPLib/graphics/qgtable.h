/*
 * File: qgtable.h
 * ---------------
 * This file exports the QGTable class for a graphical editable 2D table.
 *
 * @author Marty Stepp
 * @version 2018/07/17
 * - initial version, based on gtable.h
 * @since 2018/07/17
 */

#ifndef _qgtable_h
#define _qgtable_h

#include <string>
#include <QAbstractItemModel>
#include <QItemSelection>
#include <QWidget>
#include <QTableWidget>
#include "grid.h"
#include "qginteractor.h"
#include "qgobjects.h"
#include "qgtypes.h"

// forward declaration
class QGTable;

// Internal class; not to be used by clients.
class _Internal_QTableWidget : public QTableWidget, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QTableWidget(QGTable* qgtable, int rows, int columns, QWidget* parent = nullptr);
    virtual bool isEditing() const;
    virtual void closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint) Q_DECL_OVERRIDE;
    virtual void closePersistentEditor(const QModelIndex& index);
    virtual void openPersistentEditor(const QModelIndex& index);
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleActivate(QModelIndex index);
    void handleCellActivate(int row, int column);
    void handleCellChange(int row, int column);
    void handleCellClick(int row, int column);
    void handleCellDoubleClick(int row, int column);
    void handleCellEnter(int row, int column);
    void handleCellPress(int row, int column);
    void handleCurrentCellChange(int row, int column, int prevRow, int prevColumn);
    void handleEnter(QModelIndex index);
    void handleItemActivate(QTableWidgetItem* item);
    void handleItemEnter(QTableWidgetItem* item);
    void handleItemSelectionChange();
    void handleModelDataChange(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
    void handleSelectionChange(const QItemSelection& selected, const QItemSelection& deselected);

private:
    QGTable* _qgtable;

    void fireTableEvent(QGEvent::EventType eventType, const std::string& eventName, int row = -1, int col = -1);
};


/*
 * A QGTable is a thin wrapper around the Java Swing component JTable.
 * It represents a graphical editable 2D table, like a mediocre facsimile
 * of an Excel spreadsheet.
 * After creating a QGTable, you can listen for table events to be notified
 * when the user types a new value into a table cell:
 *
 * <pre>
 * while (true) {
 *     GEvent event = waitForEvent(TABLE_EVENT);
 *     QGTableEvent tableEvent(event);
 *     std::cout << "table event: " << tableEvent.toString() << std::endl;
 *     ...
 * }
 * </pre>
 *
 * All row/column indexes in this class are 0-based.
 */
class QGTable : public QGInteractor {
public:
    /*
     * The three supported kinds of cell alignment.
     * Used in the QGTable methods getHorizontalAlignment and setHorizontalAlignment.
     * Values match the corresponding constants from javax.swing.SwingConstants.
     */
    enum Alignment {
        CENTER = 0,
        LEFT = 2,
        RIGHT = 4
    };

    /*
     * Styles of column header labels that can be shown.
     */
    enum ColumnHeaderStyle {
        COLUMN_HEADER_NONE = 0,
        COLUMN_HEADER_EXCEL = 1,    // A, B, ..., Z, AA, AB, ...
        COLUMN_HEADER_NUMERIC = 2   // 1, 2, 3, ...
    };

    /*
     * Constructs a new table with the given dimensions and (optional) size.
     * If x, y, width, or height are omitted, they are set automatically by
     * the layout manager of the GWindow into which the table is placed.
     * This is often what you want.
     * Throws an error if the number of rows, columns, width, or height is negative.
     */
    QGTable(int rows = 0, int columns = 0,
            double width = 0, double height = 0,
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

    /*
     * Returns the horizontal alignment of the text in all cells in the table.
     * The alignment can be LEFT, CENTER, or RIGHT and is initially LEFT.
     * Currently a table's alignment is global and applies to all cells.
     */
    virtual Alignment getHorizontalAlignment() const;

    virtual _Internal_QWidget* getInternalWidget() const;

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

    virtual void removeTableEventHandler();

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
     * Member functions for per-cell formatting.
     * Throws an error if the given row/column index is out of bounds.
     */
    virtual void setCellAlignment(int row, int column, Alignment alignment);
    virtual void setCellBackground(int row, int column, int color);
    virtual void setCellBackground(int row, int column, const std::string& color);
    virtual void setCellFont(int row, int column, const std::string& font);
    virtual void setCellForeground(int row, int column, int color);
    virtual void setCellForeground(int row, int column, const std::string& color);

    /*
     * Member functions for per-column formatting.
     * Throws an error if the given row/column index is out of bounds.
     */
    virtual void setColumnAlignment(int column, Alignment alignment);
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
     * Sets the horizontal alignment of the text in all cells in the table.
     * The alignment can be LEFT, CENTER, or RIGHT and is initially LEFT.
     * Currently a table's alignment is global and applies to all cells.
     */
    virtual void setHorizontalAlignment(Alignment _alignment);

    /*
     * Member functions for per-row formatting.
     * Throws an error if the given row/column index is out of bounds.
     */
    virtual void setRowAlignment(int row, Alignment _alignment);
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
     * Sets the text in the cell that is currently selected.
     * If no cell is currently selected, does nothing.
     */
    virtual void setSelectedCellValue(const std::string& text);

    /*
     * Sets the given function to be called when events occur in this table.
     */
    virtual void setTableEventHandler(QGEventHandler func);
    virtual void setTableEventHandler(QGEventHandlerVoid func);

    /*
     * Returns the number of columns in the table.
     * Equivalent to numCols().
     */
    virtual int width() const;

private:
    // member variables
    _Internal_QTableWidget* _iqtableview;
    Alignment _alignment;
    bool _rowColHeadersVisible;
    ColumnHeaderStyle _columnHeaderStyle;

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

    friend class _Internal_QTableWidget;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgtable_h
