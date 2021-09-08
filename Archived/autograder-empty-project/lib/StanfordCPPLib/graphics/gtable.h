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

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

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

#define INTERNAL_INCLUDE 1
#include "grid.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#define INTERNAL_INCLUDE 1
#include "gobjects.h"
#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#undef INTERNAL_INCLUDE

class _Internal_QTableWidget;

/**
 * A GTable represents a graphical editable 2D table, like a mediocre facsimile
 * of an Excel spreadsheet.
 *
 * After creating a GTable, you can listen for table events to be notified
 * when the user types a new value into a table cell by calling setTableListener.
 *
 * An editable table has a semi-complex editing model where the user can begin
 * modifying a cell by highlighting it and typing, which replaces the existing
 * value, or by double-clicking it, which edits the existing value.
 * You can also press F2 on a cell to edit it, equivalent to a double-click.
 * During editing, you can press Esc to cancel editing, or Tab or Enter to
 * complete editing and move to the next cell.
 *
 * All row/column indexes in this class are 0-based.
 */
class GTable : public GInteractor {
public:
    /**
     * Styles of column header labels that can be shown.
     * The "Excel" style is to use column names A-Z, then AA-AZ, BA-BZ, ...,
     * ZA-ZZ, then AAA, AAB, and so on.
     * The "numeric" style is to use simple numbers like 1, 2, 3, ...
     * The "none" style means not to use any column headers at all.
     */
    enum ColumnHeaderStyle {
        COLUMN_HEADER_NONE,     // headers will not show
        COLUMN_HEADER_EXCEL,    // A, B, ..., Z, AA, AB, ...
        COLUMN_HEADER_NUMERIC   // 1, 2, 3, ...
    };

    /**
     * Constructs a new table with the given dimensions and (optional) size.
     * If x, y, width, or height are omitted, they are set automatically by
     * the layout manager of the GWindow into which the table is placed.
     * This is often what you want.
     * @throw ErrorException if the number of rows, columns, width, or height is negative.
     */
    GTable(int rows = 0, int columns = 0, double width = 0, double height = 0,
            QWidget* parent = nullptr);

    virtual ~GTable() Q_DECL_OVERRIDE;

    /**
     * Changes widths of all columns to be perfectly large enough
     * to fit their contents.
     */
    virtual void autofitColumnWidths();

    /**
     * Sets all cells in the table to store an empty string value.
     */
    virtual void clear();

    /**
     * Sets the given cell to store an empty string value.
     * @throw ErrorException if the given row/column index is out of bounds
     */
    virtual void clearCell(int row, int column);

    /**
     * Removes any per-cell/column/row formatting that has been applied to the table.
     */
    virtual void clearFormatting();

    /**
     * Removes any formatting that has been applied to the given cell.
     */
    virtual void clearCellFormatting(int row, int column);

    /**
     * Deselects any currently selected cell.
     * If no cell is selected, calling this has no effect.
     */
    virtual void clearSelection();

    /**
     * Sets every cell in the table to have the given value.
     */
    virtual void fill(const std::string& text);

    /**
     * Returns the text stored in the given cell.
     * @throw ErrorException if the given row or column are out of bounds
     */
    virtual std::string get(int row, int column) const;

    /**
     * Returns the column headers to use the given style.
     * Default is none, but can be set to Excel style or numeric instead.
     */
    virtual ColumnHeaderStyle getColumnHeaderStyle() const;

    /**
     * Returns the width of the given column index in pixels.
     * When a table is constructed, all columns initially have equal width.
     * @throw ErrorException if the given column index is out of bounds
     */
    virtual double getColumnWidth(int column) const;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the height of the given row index in pixels.
     * When a table is constructed, all rows initially have equal height.
     * @throw ErrorException if the given row index is out of bounds
     */
    virtual double getRowHeight(int row) const;

    /**
     * Returns the row and column of the cell that is currently selected.
     * Sets both row and column to -1 if no cell is currently selected.
     */
    virtual GridLocation getSelectedCell() const;

    /**
     * Returns the row and column of the cell that is currently selected
     * by filling the given reference parameters.
     * Sets both row and column to -1 if no cell is currently selected.
     */
    virtual void getSelectedCell(int& row, int& column) const;

    /**
     * Returns the text in the cell that is currently selected.
     * If no cell is currently selected, returns an empty string.
     */
    virtual std::string getSelectedCellValue() const;

    /**
     * Returns the column of the cell that is currently selected, or -1 if no cell
     * is currently selected.
     */
    virtual int getSelectedColumn() const;

    /**
     * Returns the row of the cell that is currently selected, or -1 if no cell
     * is currently selected.
     */
    virtual int getSelectedRow() const;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns true if a cell is currently selected.
     */
    virtual bool hasSelectedCell() const;

    /**
     * Returns the number of rows in the table.
     * Equivalent to numRows().
     */
    virtual int height() const;

    /**
     * Returns true if the given 0-based row/column index is within the bounds
     * of the table.  In other words, whether the index is between (0, 0)
     * and (numRows-1, numCols-1) inclusive.
     */
    virtual bool inTableBounds(int row, int column) const;

    /**
     * Returns whether cells of the table can be edited.
     * Defaults to true when a table is initially created.
     */
    virtual bool isEditable() const;

    /**
     * Returns the number of columns in the table.
     * Equivalent to width().
     */
    virtual int numCols() const;

    /**
     * Returns the number of rows in the table.
     * Equivalent to height().
     */
    virtual int numRows() const;

    /**
     * Removes the table listener from this button so that it will no longer
     * call it when events occur.
     */
    virtual void removeTableListener();

    /* @inherit */
    virtual void requestFocus() Q_DECL_OVERRIDE;

    /**
     * Modifies the table to have the given number of rows and columns.
     * Any existing data is retained, and any new cells are empty.
     * @throw ErrorException if numRows or numCols is negative
     */
    virtual void resize(int numRows, int numCols);

    /**
     * Returns whether row and column headers are shown in the table.
     * Initially false.
     */
    virtual bool rowColumnHeadersVisible() const;

    /**
     * Sets the given cell to become currently selected,
     * replacing any previous selection.
     * Note that the user can click a different cell to select it afterward.
     * To indicate that you do not want to select any cell, call clearSelection.
     * @throw ErrorException if the given row or column are out of bounds
     */
    virtual void select(int row, int column);

    /**
     * Modifies the value in the given cell to store the given text.
     * @throw ErrorException if the given row or column are out of bounds
     */
    virtual void set(int row, int column, const std::string& text);

    /**
     * Sets the background color that appears behind each cell.
     * See gcolor.h for more detail about colors.
     */
    virtual void setBackground(int rgb) Q_DECL_OVERRIDE;

    /**
     * Sets the background color that appears behind each cell.
     * See gcolor.h for more detail about colors.
     */
    virtual void setBackground(const std::string& color) Q_DECL_OVERRIDE;

    /**
     * Sets the horizontal alignment of the given cell.
     * The row/column is specified by a 0-based row/column index from the top/left of the table.
     * @throw ErrorException if the given row/column index is out of bounds
     */
    virtual void setCellAlignment(int row, int column, HorizontalAlignment alignment);

    /**
     * Sets the background color of the given cell to the given color.
     * The row/column is specified by a 0-based row/column index from the top/left of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given row/column index is out of bounds
     */
    virtual void setCellBackground(int row, int column, int color);

    /**
     * Sets the background color of the given cell to the given color.
     * The row/column is specified by a 0-based row/column index from the top/left of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given row/column index is out of bounds
     */
    virtual void setCellBackground(int row, int column, const std::string& color);

    /**
     * Sets the text font of the given cell to the given RGB color.
     * The row/column is specified by a 0-based row/column index from the top/left of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given row/column index is out of bounds
     */
    virtual void setCellFont(int row, int column, const std::string& font);

    /**
     * Sets the foreground/text color of the given cell to the given color.
     * The row/column is specified by a 0-based row/column index from the top/left of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given row/column index is out of bounds
     */
    virtual void setCellForeground(int row, int column, int color);

    /**
     * Sets the foreground/text color of the given cell to the given color.
     * The row/column is specified by a 0-based row/column index from the top/left of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given row/column index is out of bounds
     */
    virtual void setCellForeground(int row, int column, const std::string& color);

    /**
     * Sets the color used for the text of each cell.
     * Equivalent to setForeground.
     * See gcolor.h for more detail about colors.
     */
    virtual void setColor(int rgb) Q_DECL_OVERRIDE;

    /**
     * Sets the color used for the text of each cell.
     * Equivalent to setForeground.
     * See gcolor.h for more detail about colors.
     */
    virtual void setColor(const std::string& color) Q_DECL_OVERRIDE;

    /**
     * Sets the horizontal alignment of the given column.
     * The column is specified by a 0-based column index from the left of the table.
     * @throw ErrorException if the given column index is out of bounds
     */
    virtual void setColumnAlignment(int column, HorizontalAlignment alignment);

    /**
     * Sets the background color of the given column to the given color.
     * The column is specified by a 0-based column index from the left of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given column index is out of bounds
     */
    virtual void setColumnBackground(int column, int color);

    /**
     * Sets the background color of the given column to the given color.
     * The column is specified by a 0-based column index from the left of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given column index is out of bounds
     */
    virtual void setColumnBackground(int column, const std::string& color);

    /**
     * Sets the text font of the given column to the given RGB color.
     * The column is specified by a 0-based column index from the left of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given column index is out of bounds
     */
    virtual void setColumnFont(int column, const std::string& font);

    /**
     * Sets the foreground/text color of the given column to the given color.
     * The column is specified by a 0-based column index from the left of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given column index is out of bounds
     */
    virtual void setColumnForeground(int column, int color);

    /**
     * Sets the foreground/text color of the given column to the given color.
     * The column is specified by a 0-based column index from the left of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given column index is out of bounds
     */
    virtual void setColumnForeground(int column, const std::string& color);

    /**
     * Sets the given column index to have the given width in pixels.
     * @throw ErrorException if the given column index is out of bounds
     *        or if the width is negative
     */
    virtual void setColumnWidth(int column, double width);

    /**
     * Sets the column headers to use the given style.
     * Default is none, but can be set to Excel style or numeric instead.
     */
    virtual void setColumnHeaderStyle(ColumnHeaderStyle style);

    /**
     * Sets whether cells of the table can be edited.
     * Initially true.
     */
    virtual void setEditable(bool editable);

    /**
     * Modifies the value in the cell that is currently being edited to store
     * the given text. This does not modify the value in the table cell but
     * merely the value in the editor widget.
     * @throw ErrorException if the given row or column are out of bounds
     */
    virtual void setEditorValue(int row, int column, const std::string& text);

    /**
     * Sets the font used to display each cell's text.
     */
    virtual void setFont(const QFont& font) Q_DECL_OVERRIDE;

    /**
     * Sets the font used to display each cell's text.
     * See gfont.h for more detail about fonts.
     */
    virtual void setFont(const std::string& font) Q_DECL_OVERRIDE;

    /**
     * Sets the color used for the text of each cell.
     * Equivalent to setColor.
     * See gcolor.h for more detail about colors.
     */
    virtual void setForeground(int rgb) Q_DECL_OVERRIDE;

    /**
     * Sets the color used for the text of each cell.
     * Equivalent to setColor.
     * See gcolor.h for more detail about colors.
     */
    virtual void setForeground(const std::string& color) Q_DECL_OVERRIDE;

    /**
     * Sets the horizontal alignment of the text in all cells in the table.
     * The alignment can be LEFT, CENTER, or RIGHT and is initially LEFT.
     */
    virtual void setHorizontalAlignment(HorizontalAlignment alignment);

    /**
     * Sets the horizontal alignment of the given row.
     * The row is specified by a 0-based row index from the top of the table.
     * @throw ErrorException if the given row index is out of bounds
     */
    virtual void setRowAlignment(int row, HorizontalAlignment alignment);

    /**
     * Sets the background color of the given row to the given RGB color.
     * The row is specified by a 0-based row index from the top of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given row index is out of bounds
     */
    virtual void setRowBackground(int row, int rgb);

    /**
     * Sets the background color of the given row to the given color.
     * The row is specified by a 0-based row index from the top of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given row index is out of bounds
     */
    virtual void setRowBackground(int row, const std::string& color);

    /**
     * Sets the text font of the given row to the given font.
     * The row is specified by a 0-based row index from the top of the table.
     * See gfont.h for more detail about fonts.
     * @throw ErrorException if the given row index is out of bounds
     */
    virtual void setRowFont(int row, const std::string& font);

    /**
     * Sets the foreground/text color of the given row to the given color.
     * The row is specified by a 0-based row index from the top of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given row index is out of bounds
     */
    virtual void setRowForeground(int row, int rgb);

    /**
     * Sets the foreground/text color of the given row to the given color.
     * The row is specified by a 0-based row index from the top of the table.
     * See gcolor.h for more detail about colors.
     * @throw ErrorException if the given row index is out of bounds
     */
    virtual void setRowForeground(int row, const std::string& color);

    /**
     * Sets whether row and column headers should be shown in the table.
     * Initially false.
     */
    virtual void setRowColumnHeadersVisible(bool visible);

    /**
     * Sets the given row index to have the given height in pixels.
     * @throw ErrorException if the given row index is out of bounds
     *        or if the height is negative
     */
    virtual void setRowHeight(int row, double width);

    /**
     * Sets the text in the cell that is currently selected.
     * If no cell is currently selected, does nothing.
     */
    virtual void setSelectedCellValue(const std::string& text);

    /**
     * Sets the given function to be called when events occur in this table.
     * Any existing table listener will be replaced.
     */
    virtual void setTableListener(GEventListener func);

    /**
     * Sets the given function to be called when events occur in this table.
     * Any existing table listener will be replaced.
     */
    virtual void setTableListener(GEventListenerVoid func);

    /**
     * Returns the number of columns in the table.
     * Equivalent to numCols().
     */
    virtual int width() const;

private:
    Q_DISABLE_COPY(GTable)

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
     * @throw ErrorException if the given row/column values are out of bounds.
     */
    void checkColumn(const std::string& member, int column) const;
    void checkIndex(const std::string& member, int row, int column) const;
    void checkRow(const std::string& member, int row) const;

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

/**
 * Internal class; not to be used by clients.
 * @private
 */
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


/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QTableWidget : public QTableWidget, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QTableWidget(GTable* gtable, int rows, int columns, QWidget* parent = nullptr);
    virtual void detach() Q_DECL_OVERRIDE;
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

    friend class GTable;
};

#endif // _gtable_h
