/*
 * File: gtable.h
 * --------------
 * This file exports the GTable class for a graphical editable 2D table.
 *
 * @author Marty Stepp
 * @version 2015/12/01
 * - added setEventEnabled to turn on/off table update/selection events
 * - added isEditable, setEditable
 * @since 2015/11/07
 */

#ifndef _gtable_h
#define _gtable_h

#include "grid.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "gtypes.h"

/*
 * A GTable is a thin wrapper around the Java Swing component JTable.
 * It represents a graphical editable 2D table, like a mediocre facsimile
 * of an Excel spreadsheet.
 * After creating a GTable, you can listen for table events to be notified
 * when the user types a new value into a table cell:
 * 
 * <pre>
 * while (true) {
 *     GEvent event = waitForEvent(TABLE_EVENT);
 *     GTableEvent tableEvent(event);
 *     std::cout << "table event: " << tableEvent.toString() << std::endl;
 *     ...
 * }
 * </pre>
 * 
 * All row/column indexes in this class are 0-based.
 */
class GTable : public GInteractor {
public:
    /*
     * Constants for showConfirmDialog types, taken from Java's JOptionPane.
     */
    enum Alignment {
        /*
         * The three supported kinds of cell alignment.
         * Used in the GTable methods getHorizontalAlignment and setHorizontalAlignment.
         * Values match the corresponding constants from javax.swing.SwingConstants.
         */
        CENTER = 0,
        LEFT = 2,
        RIGHT = 4
    };
    
    /*
     * Constructs a new table with the given dimensions and (optional) size.
     * If x, y, width, or height are omitted, they are set automatically by
     * the layout manager of the GWindow into which the table is placed.
     * This is often what you want.
     * Throws an error if the number of rows, columns, width, or height is negative.
     */
    GTable(int rows = 0, int columns = 0,
           double x = 0, double y = 0,
           double width = 0, double height = 0);
    
    /* virtual methods from GInteractor */
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toString() const;

    /* unique GTable behavior */
    
    /*
     * Sets all cells in the table to store an empty string value.
     */
    void clear();
    
    /*
     * Deselects any currently selected cell.
     * If no cell is selected, calling this has no effect.
     */
    void clearSelection();
    
    /*
     * Returns the text stored in the given cell.
     * Throws an error if the given row or column are out of bounds.
     */
    std::string get(int row, int column) const;
    
    /*
     * Returns the width of the given column index in pixels.
     * When a table is constructed, all columns initially have equal width.
     * Throws an error if the given column index is out of bounds.
     */
    double getColumnWidth(int column) const;
    
    /*
     * Returns the font used in the cells of this table.
     * If the font has never been explicitly set, returns "*-*-*".
     * See also: GWindow documentation on font strings.
     */
    std::string getFont() const;
    
    /*
     * Returns the horizontal alignment of the text in all cells in the table.
     * The alignment can be LEFT, CENTER, or RIGHT and is initially LEFT.
     * Currently a table's alignment is global and applies to all cells.
     */
    Alignment getHorizontalAlignment() const;
    
    /*
     * Returns the row and column of the cell that is currently selected
     * by filling the given reference parameters.
     * Sets both row and column to -1 if no cell is currently selected.
     */
    void getSelectedCell(int& row, int& column) const;
    
    /*
     * Returns the row of the cell that is currently selected, or -1 if no cell
     * is currently selected.
     */
    int getSelectedColumn() const;
    
    /*
     * Returns the row of the cell that is currently selected, or -1 if no cell
     * is currently selected.
     */
    int getSelectedRow() const;
    
    /*
     * Returns the number of rows in the table.
     * Equivalent to numRows().
     */
    int height() const;
    
    /*
     * Returns true if the given 0-based row/column index is within the bounds
     * of the table.  In other words, whether the index is between (0, 0)
     * and (numRows-1, numCols-1) inclusive.
     */
    bool inBounds(int row, int column) const;
    
    /*
     * Returns whether cells of the table can be edited.
     * Defaults to true when a table is initially created.
     */
    bool isEditable() const;
    
    /*
     * Returns the number of columns in the table.
     * Equivalent to width().
     */
    int numCols() const;
    
    /*
     * Returns the number of rows in the table.
     * Equivalent to height().
     */
    int numRows() const;
    
    /*
     * Modifies the table to have the given number of rows and columns.
     * Any existing data is retained, and any new cells are empty.
     * Throws an error if numRows or numCols is negative.
     */
    void resize(int numRows, int numCols);
    
    /*
     * Sets the given cell to become currently selected,
     * replacing any previous selection.
     * Note that the user can click a different cell to select it afterward.
     * If the given row/column index is out of bounds, no cell is selected
     * and any currently selected cell is deselected.
     */
    void select(int row, int column);
    
    /*
     * Modifies the value in the given cell to store the given text.
     * Throws an error if the given row or column are out of bounds.
     */
    void set(int row, int column, const std::string& text);
    
    /*
     * Sets the given column index to have the given width in pixels.
     * Throws an error if the given column index is out of bounds
     * or if the width is negative.
     */
    void setColumnWidth(int column, double width);
    
    /*
     * Sets whether cells of the table can be edited.
     */
    void setEditable(bool editable);
    
    /*
     * Sets whether the given kind of event should be enabled on tables.
     * Must be a table event type such as TABLE_SELECTED or TABLE_UPDATED.
     */
    void setEventEnabled(int type, bool enabled = true);
    
    /*
     * Sets the cells of this table to display their text in the given font.
     * See also: GWindow documentation on font strings.
     */
    void setFont(const std::string& font);
    
    /*
     * Sets the horizontal alignment of the text in all cells in the table.
     * The alignment can be LEFT, CENTER, or RIGHT and is initially LEFT.
     * Currently a table's alignment is global and applies to all cells.
     */
    void setHorizontalAlignment(Alignment alignment);
    
    /*
     * Returns the number of columns in the table.
     * Equivalent to numCols().
     */
    int width() const;

private:
    // member variables
    int rows;
    int columns;
    double m_x;
    double m_y;
    double m_width;
    double m_height;
    std::string font;
    Alignment alignment;
    bool editable;

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
};

#endif
