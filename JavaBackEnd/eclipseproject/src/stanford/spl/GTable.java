/*
 * @author Marty Stepp
 * @version 2016/11/29
 * - changed arrow key behavior when editing a cell (won't exit)
 * @version 2016/11/26
 * - added autofitColumnWidths
 * - added per-cell formatting: set Cell/Column/Row Alignment/Background/Font/Foreground
 * - added GTableListener support
 * @version 2016/11/24
 * - added cut/copy/paste functionality
 * @version 2016/11/20
 * - added row and column header cells in numeric and Excel styles
 * @version 2016/11/16
 * - added table cell edit begin event
 * @version 2015/12/01
 * - added setEventEnabled / isEventEnabled / ListSelectionListener
 * - added isEditable, setEditable, JTableModel
 * @version 2015/11/11
 * - added select, getSelectedRow/Column, inBounds
 * @since 2015/11/07
 */

package stanford.spl;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.event.*;
import javax.swing.plaf.UIResource;
import javax.swing.table.*;
import stanford.cs106.gui.*;

public class GTable extends GInteractor {
	private static final long serialVersionUID = 1L;
	
	// default width of a column in pixels
	private static final int COLUMN_WIDTH_DEFAULT = 90;
	
	// max/min widths during column autofit process
	private static final int COLUMN_WIDTH_MIN = 30;
	private static final int COLUMN_WIDTH_MAX = 500;
	private static final int COLUMN_WIDTH_PADDING = 5;
	
	// these MUST match the constants in C++ lib gevents.h
	// NOTE: if you add event types, you may want to enable them below in the eventTypesEnabled set
	public static final int TABLE_EVENT	= 0x400;
	public static final int TABLE_UPDATED  = TABLE_EVENT + 1;
	public static final int TABLE_SELECTED = TABLE_EVENT + 2;
	public static final int TABLE_EDIT_BEGIN = TABLE_EVENT + 3;
	public static final int TABLE_REPLACE_BEGIN = TABLE_EVENT + 4;
	public static final int TABLE_CUT = TABLE_EVENT + 5;
	public static final int TABLE_COPY = TABLE_EVENT + 6;
	public static final int TABLE_PASTE = TABLE_EVENT + 7;
	
	// these MUST match the enum constants in C++ lib gtable.h
	public static final int COLUMN_HEADER_NONE = 0;
	public static final int COLUMN_HEADER_EXCEL = 1;
	public static final int COLUMN_HEADER_NUMERIC = 2;
	
	// set true to exit cell editor when L/R arrow key pressed (default false)
	private static /* final */ boolean EXIT_CELL_EDITOR_ON_ARROW_KEY = false;
	
	/**
	 * Settings for Java look-and-feel related to tables.
	 */
	public static void setSystemLookAndFeelProperties() {
		UIManager.put("Table.disabled", false);
		UIManager.put("Table.gridColor", new Color(204, 207, 213));
		UIManager.put("Table.intercellSpacing", new Dimension(1, 1));
		UIManager.put("Table.showGrid", true);
	}
	
	// private JScrollPane scrollPane;
	private JTable table;
	private GTableModel model;
	private JavaBackEnd jbe;
	private Set<Integer> eventTypesEnabled = new HashSet<Integer>(Arrays.asList(
			TABLE_COPY,
			TABLE_CUT,
			TABLE_EDIT_BEGIN,
			TABLE_PASTE,
			TABLE_REPLACE_BEGIN,
			TABLE_SELECTED,
			TABLE_UPDATED
	));
	private GTableCellEditor cellEditor;
	private boolean eventsEnabled = true;
	private boolean editable = true;
	private boolean rowColumnHeadersVisible = false;
	private int columnHeaderStyle = COLUMN_HEADER_NONE;
	private int previouslySelectedRow = -1;
	private int previouslySelectedColumn = -1;
	private Font tableFont;
	private int horizontalAlignment = SwingConstants.LEFT;
	private CellFormattingInfo[][] cellFormats;
	private CellFormattingInfo[] rowFormats;
	private CellFormattingInfo[] colFormats;
	
	// default formatting properties on a table cell
	private CellFormattingInfo DEFAULT_CELL_FORMAT;
	
	private Set<GTableListener> listeners = new LinkedHashSet<GTableListener>();
	
	// TODO: row/col header formats

	public GTable() {
		this(0, 0);
	}
	
	public GTable(int numRows, int numCols) {
		super(new JScrollPane(new JTable(numRows, numCols)));
		JScrollPane scrollPane = (JScrollPane) super.getInteractor();
		table = GuiUtils.getDescendent(scrollPane, JTable.class);
		
		checkDimensions("constructor", numRows, numCols);
		model = new GTableModel(numRows, numCols);
		table.setModel(model);
		table.setBorder(BorderFactory.createLineBorder(Color.GRAY, /* thickness */ 1));
		// table.setFillsViewportHeight(true);
		table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		table.setRowSelectionAllowed(false);
		table.setColumnSelectionAllowed(false);
		table.setCellSelectionEnabled(true);
		table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
		// table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
		
		GTableCellRenderer tableCellRenderer = new GTableCellRenderer();
		table.setDefaultRenderer(Character.class, tableCellRenderer);
		table.setDefaultRenderer(Character.TYPE, tableCellRenderer);
		table.setDefaultRenderer(Double.class, tableCellRenderer);
		table.setDefaultRenderer(Double.TYPE, tableCellRenderer);
		table.setDefaultRenderer(Integer.class, tableCellRenderer);
		table.setDefaultRenderer(Integer.TYPE, tableCellRenderer);
		table.setDefaultRenderer(Number.class, tableCellRenderer);
		table.setDefaultRenderer(Object.class, tableCellRenderer);
		table.setDefaultRenderer(String.class, tableCellRenderer);
		tableFont = table.getFont();
		
		cellFormats = new CellFormattingInfo[numRows][numCols];
		rowFormats = new CellFormattingInfo[numRows];
		colFormats = new CellFormattingInfo[numCols];
		
		TableModelListener tmListener = new GTableModelListener();
		model = (GTableModel) table.getModel();
		model.addTableModelListener(tmListener);
		
		ListSelectionListener lslListener = new GTableListSelectionListener();
		table.getSelectionModel().addListSelectionListener(lslListener);
		table.getColumnModel().getSelectionModel().addListSelectionListener(lslListener);
		
		table.addKeyListener(new GTableKeyListener());
		
		// make cut/copy/paste work (whyyyy do I have to write this myself?!)
		ActionListener alistener = new GTableActionListener();
		table.registerKeyboardAction(alistener, "Copy", KeyStroke.getKeyStroke(KeyEvent.VK_C, ActionEvent.CTRL_MASK, /* onRelease */ false), JComponent.WHEN_FOCUSED);
		table.registerKeyboardAction(alistener, "Copy", KeyStroke.getKeyStroke(KeyEvent.VK_C, ActionEvent.META_MASK, /* onRelease */ false), JComponent.WHEN_FOCUSED);
		table.registerKeyboardAction(alistener, "Copy", KeyStroke.getKeyStroke(KeyEvent.VK_INSERT, ActionEvent.CTRL_MASK, /* onRelease */ false), JComponent.WHEN_FOCUSED);
		table.registerKeyboardAction(alistener, "Cut", KeyStroke.getKeyStroke(KeyEvent.VK_X, ActionEvent.CTRL_MASK, /* onRelease */ false), JComponent.WHEN_FOCUSED);
		table.registerKeyboardAction(alistener, "Cut", KeyStroke.getKeyStroke(KeyEvent.VK_X, ActionEvent.META_MASK, /* onRelease */ false), JComponent.WHEN_FOCUSED);
		table.registerKeyboardAction(alistener, "Cut", KeyStroke.getKeyStroke(KeyEvent.VK_DELETE, ActionEvent.SHIFT_MASK, /* onRelease */ false), JComponent.WHEN_FOCUSED);
		table.registerKeyboardAction(alistener, "Paste", KeyStroke.getKeyStroke(KeyEvent.VK_V, ActionEvent.CTRL_MASK, /* onRelease */ false), JComponent.WHEN_FOCUSED);
		table.registerKeyboardAction(alistener, "Paste", KeyStroke.getKeyStroke(KeyEvent.VK_V, ActionEvent.META_MASK, /* onRelease */ false), JComponent.WHEN_FOCUSED);
		table.registerKeyboardAction(alistener, "Paste", KeyStroke.getKeyStroke(KeyEvent.VK_INSERT, ActionEvent.SHIFT_MASK, /* onRelease */ false), JComponent.WHEN_FOCUSED);
		
		setDefaultColumnWidth(COLUMN_WIDTH_DEFAULT);
		
		cellEditor = new GTableCellEditor();
		// table.setDefaultEditor(Boolean.TYPE, cellEditor);
		// table.setDefaultEditor(Boolean.class, cellEditor);
		table.setDefaultEditor(Character.TYPE, cellEditor);
		table.setDefaultEditor(Character.class, cellEditor);
		table.setDefaultEditor(Double.TYPE, cellEditor);
		table.setDefaultEditor(Double.class, cellEditor);
		table.setDefaultEditor(Integer.TYPE, cellEditor);
		table.setDefaultEditor(Integer.class, cellEditor);
		table.setDefaultEditor(Number.class, cellEditor);
		table.setDefaultEditor(Object.class, cellEditor);
		table.setDefaultEditor(String.class, cellEditor);
		
		CellEditorListener ceListener = new GTableCellEditorListener();
		table.getCellEditor(0, 0).addCellEditorListener(ceListener);
	}
	
	public void addGTableListener(GTableListener listener) {
		this.listeners.add(listener);
	}
	
	public synchronized void autofitColumnWidths() {
		table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
		int numRows = table.getRowCount();
		int numCols = table.getColumnCount();
		for (int col = 0; col < numCols; col++) {
			TableColumn tableColumn = table.getColumnModel().getColumn(col);
			int preferredWidth = Math.max(COLUMN_WIDTH_MIN, tableColumn.getMinWidth());
			int maxWidth = Math.min(COLUMN_WIDTH_MAX, tableColumn.getMaxWidth());
			for (int row = 0; row < numRows; row++) {
				TableCellRenderer cellRenderer = table.getCellRenderer(row, col);
				Component comp = table.prepareRenderer(cellRenderer, row, col);
				int width = comp.getPreferredSize().width + COLUMN_WIDTH_PADDING + table.getIntercellSpacing().width;
				preferredWidth = Math.max(preferredWidth, width);
				if (preferredWidth >= maxWidth) {
					// we've exceeded the maximum width, no need to check other rows
					preferredWidth = maxWidth;
					break;
				}
			}

			tableColumn.setPreferredWidth(preferredWidth);
		}
		table.repaint();
	}
	
	public synchronized void clear() {
		eventsEnabled = false;
		for (int row = 0; row < table.getRowCount(); row++) {
			for (int col = 0; col < table.getColumnCount(); col++) {
				table.setValueAt("", row, col);
			}
		}
		previouslySelectedRow = -1;
		previouslySelectedColumn = -1;
		clearFormatting();
		eventsEnabled = true;
	}
	
	public synchronized void clearFormatting() {
		for (int row = 0; row < table.getRowCount(); row++) {
			for (int col = 0; col < table.getColumnCount(); col++) {
				cellFormats[row][col] = null;
			}
		}
		for (int row = 0; row < table.getRowCount(); row++) {
			rowFormats[row] = null;
		}
		for (int col = 0; col < table.getColumnCount(); col++) {
			colFormats[col] = null;
		}
		table.repaint();
	}
	
	public void clearGTableListeners() {
		this.listeners.clear();
	}
	
	public synchronized String get(int row, int column) {
		checkIndex("get", row, column);
		Object value = table.getValueAt(row, column);
		if (value == null) {
			value = "";
		}
		return String.valueOf(value);
	}
	
	public int getColumnHeaderStyle() {
		return columnHeaderStyle;
	}
	
	public synchronized int getColumnWidth(int col) {
		checkIndex("getColumnWidth", /* row (hack) */ 0, col);
		TableColumn column = table.getColumnModel().getColumn(col);
		return column.getWidth();
	}
	
	public synchronized Font getFont() {
		return table.getFont();
	}
	
//	@Override
//	public JTable getInteractor() {
//		return table;
//	}
	
	public synchronized int getSelectedColumn() {
		return table.getSelectedColumn();
	}
	
	public synchronized int getSelectedRow() {
		return table.getSelectedRow();
	}
	
	public boolean inBounds(int row, int column) {
		return row >= 0 && row < numRows() && column >= 0 && column < numCols();
	}
	
	public synchronized boolean isEventEnabled(int type) {
		return eventTypesEnabled.contains(type);
	}

	private void notifyBackEndOfEvent(int eventType) {
		int row = table.getSelectedRow();
		int col = table.getSelectedColumn();
		if (inBounds(row, col)) {
			notifyBackEndOfEvent(eventType, row, col);
		}
	}
	
	private void notifyBackEndOfEvent(int eventType, int row, int col) {
		String eventStr = 
				  eventType == TABLE_COPY ? "tableCopy"
				: eventType == TABLE_CUT ? "tableCut"
				: eventType == TABLE_EDIT_BEGIN ? "tableEditBegin"
				: eventType == TABLE_PASTE ? "tablePaste"
				: eventType == TABLE_REPLACE_BEGIN ? "tableReplaceBegin"
				: eventType == TABLE_SELECTED ? "tableSelected"
				: eventType == TABLE_UPDATED ? "tableUpdated"
				: "unknown";
		String value = inBounds(row, col) ? get(row, col) : "";
		
		// notify all GTableListeners about this event
		for (GTableListener listener : this.listeners) {
			GTableEvent event = new GTableEvent(this, row, col, value);
			if (eventType == TABLE_COPY) {
				listener.tableCopy(event);
			} else if (eventType == TABLE_CUT) {
				listener.tableCut(event);
			} else if (eventType == TABLE_EDIT_BEGIN) {
				listener.tableEditBegin(event);
			} else if (eventType == TABLE_PASTE) {
				listener.tablePaste(event);
			} else if (eventType == TABLE_REPLACE_BEGIN) {
				listener.tableReplaceBegin(event);
			} else if (eventType == TABLE_SELECTED) {
				listener.tableSelected(event);
			} else if (eventType == TABLE_UPDATED) {
				listener.tableUpdated(event);
			}
		}
		
		// notify C++ program (if any) about this event
		if (jbe != null && eventsEnabled && eventTypesEnabled.contains(eventType)) {
			synchronized (this) {
				if (eventType == TABLE_UPDATED) {
					jbe.acknowledgeEvent("event:%s(\"%s\", %d, %d, %d, \"%s\")",
							eventStr,
							jbe.getSourceId(getInteractor()),
							(long) jbe.getEventTime(),
							row, col,
							SplPipeDecoder.encode(value));
				} else {
					jbe.acknowledgeEvent("event:%s(\"%s\", %d, %d, %d)",
							eventStr,
							jbe.getSourceId(getInteractor()),
							(long) jbe.getEventTime(),
							row, col);
				}
			}
		}
	}

	public int numCols() {
		return table.getColumnCount();
	}
	
	public int numRows() {
		return table.getRowCount();
	}
	
	public void removeGTableListener(GTableListener listener) {
		this.listeners.remove(listener);
	}
	
	@Override
	public void requestFocus() {
		super.requestFocus();
		table.requestFocus();
	}
	
	public synchronized void resize(int numRows, int numCols) {
		checkDimensions("resize", numRows, numCols);
		eventsEnabled = false;
		
		// copy over cell formatting information
		CellFormattingInfo[][] cellFormats2 = new CellFormattingInfo[numRows][numCols];
		for (int row = 0, maxRow = Math.min(numRows, model.getRowCount()); row < maxRow; row++) {
			for (int col = 0, maxCol = Math.min(numCols, model.getColumnCount()); col < maxCol; col++) {
				cellFormats2[row][col] = cellFormats[row][col];
			}
		}
		CellFormattingInfo[] rowFormats2 = new CellFormattingInfo[numRows];
		for (int row = 0, maxRow = Math.min(numRows, model.getRowCount()); row < maxRow; row++) {
			rowFormats2[row] = rowFormats[row];
		}
		CellFormattingInfo[] colFormats2 = new CellFormattingInfo[numCols];
		for (int col = 0, maxCol = Math.min(numCols, model.getColumnCount()); col < maxCol; col++) {
			colFormats2[col] = colFormats[col];
		}
		cellFormats = cellFormats2;
		rowFormats = rowFormats2;
		colFormats = colFormats2;
		
		// resize actual table model
		model.setRowCount(numRows);
		model.setColumnCount(numCols);
		eventsEnabled = true;
		
		previouslySelectedRow = -1;
		previouslySelectedColumn = -1;
	}
	
	public boolean rowColumnHeadersVisible() {
		return rowColumnHeadersVisible;
	}
	
	public synchronized void select(int row, int column) {
		if (inBounds(row, column)) {
			// previouslySelectedRow = getSelectedRow();
			// previouslySelectedColumn = getSelectedColumn();
			table.setRowSelectionInterval(row, row);
			table.setColumnSelectionInterval(column, column);
		} else {
			previouslySelectedRow = -1;
			previouslySelectedColumn = -1;
			table.clearSelection();
		}
	}
	
	public synchronized void set(int row, int column, String value) {
		set(row, column, value, /* notifyBackEnd */ true);
	}
	
	public synchronized void set(int row, int column, String value, boolean notifyBackEnd) {
		checkIndex("set", row, column);
		if (value == null) {
			value = "";
		}
		eventsEnabled = false;
		table.setValueAt(value, row, column);
		eventsEnabled = true;
		if (notifyBackEnd) {
			notifyBackEndOfEvent(TABLE_UPDATED, row, column);
		}
	}
	
	public synchronized void setBackground(Color color) {
		table.setBackground(color);
		if (DEFAULT_CELL_FORMAT != null) {
			DEFAULT_CELL_FORMAT.backgroundColor = color;
		}
		table.repaint();
	}
	
	public synchronized void setCellAlignment(int row, int column, int alignment) {
		if (!inBounds(row, column)) {
			return;
		}
		if (cellFormats[row][column] == null) {
			cellFormats[row][column] = new CellFormattingInfo();
		}
		cellFormats[row][column].alignment = alignment;
		table.repaint();
	}
	
	public synchronized void setCellBackground(int row, int column, Color color) {
		if (!inBounds(row, column)) {
			return;
		}
		if (cellFormats[row][column] == null) {
			cellFormats[row][column] = new CellFormattingInfo();
		}
		cellFormats[row][column].backgroundColor = color;
		table.repaint();
	}
	
	public synchronized void setCellColor(int row, int column, Color color) {
		if (!inBounds(row, column)) {
			return;
		}
		if (cellFormats[row][column] == null) {
			cellFormats[row][column] = new CellFormattingInfo();
		}
		cellFormats[row][column].foregroundColor = color;
		table.repaint();
	}
	
	public synchronized void setCellFont(int row, int column, Font font) {
		if (!inBounds(row, column)) {
			return;
		}
		if (cellFormats[row][column] == null) {
			cellFormats[row][column] = new CellFormattingInfo();
		}
		cellFormats[row][column].font = font;
		table.repaint();
	}
	
	public synchronized void setCellForeground(int row, int column, Color color) {
		setCellColor(row, column, color);
	}
	
	public synchronized void setColor(Color color) {
		table.setForeground(color);
		table.repaint();
	}
	
	public synchronized void setColumnAlignment(int column, int alignment) {
		if (!inBounds(/* row */ 0, column)) {
			return;
		}
		if (colFormats[column] == null) {
			colFormats[column] = new CellFormattingInfo();
		}
		colFormats[column].alignment = alignment;
		table.repaint();
	}
	
	public synchronized void setColumnBackground(int column, Color color) {
		if (!inBounds(/* row */ 0, column)) {
			return;
		}
		if (colFormats[column] == null) {
			colFormats[column] = new CellFormattingInfo();
		}
		colFormats[column].backgroundColor = color;
		table.repaint();
	}
	
	public synchronized void setColumnColor(int column, Color color) {
		setColumnForeground(column, color);
	}
	
	public synchronized void setColumnFont(int column, Font font) {
		if (!inBounds(/* row */ 0, column)) {
			return;
		}
		if (colFormats[column] == null) {
			colFormats[column] = new CellFormattingInfo();
		}
		colFormats[column].font = font;
		table.repaint();
	}
	
	public synchronized void setColumnForeground(int column, Color color) {
		if (!inBounds(/* row */ 0, column)) {
			return;
		}
		if (colFormats[column] == null) {
			colFormats[column] = new CellFormattingInfo();
		}
		colFormats[column].foregroundColor = color;
		table.repaint();
	}
	
	public synchronized void setColumnHeaderStyle(int style) {
		columnHeaderStyle = style;
		if (rowColumnHeadersVisible) {
			if (style == COLUMN_HEADER_NONE) {
				setColumnHeadersVisible(false);
			}
		}
	}
	
	public synchronized void setColumnHeadersVisible(boolean visible) {
		int numCols = numCols();
		if (visible) {
			if (columnHeaderStyle == COLUMN_HEADER_NONE) {
				columnHeaderStyle = COLUMN_HEADER_NUMERIC;
			}
			
			Object[] columnHeaders = new Object[numCols];
			for (int i = 0; i < numCols; i++) {
				columnHeaders[i] = columnHeaderStyle == COLUMN_HEADER_NUMERIC
						? String.valueOf(i + 1) : toExcelStyleColumnName(i);
			}
			model.setColumnIdentifiers(columnHeaders);
			
			Font font = table.getTableHeader().getFont();
			table.getTableHeader().setFont(font.deriveFont(Font.BOLD));
		} else {
			Object[] columnHeaders = new Object[numCols];
			for (int i = 0; i < numCols; i++) {
				columnHeaders[i] = "";
			}
			model.setColumnIdentifiers(columnHeaders);
		}
	}
	
	public synchronized void setColumnWidth(int col, int width) {
		checkIndex("setColumnWidth", /* row (hack) */ 0, col);
		if (width < 0) {
			throw new IllegalArgumentException("GTable.setColumnWidth: width cannot be negative");
		}
		TableColumn column = table.getColumnModel().getColumn(col);
		column.setPreferredWidth(width);
		column.setWidth(width);
		table.validate();
		table.repaint();
	}
	
	public synchronized void setDefaultColumnWidth(int width) {
		if (width < 0) {
			throw new IllegalArgumentException("GTable.setDefaultColumnWidth: width cannot be negative");
		}
		int numCols = table.getColumnCount();
		for (int col = 0; col < numCols; col++) {
			TableColumn column = table.getColumnModel().getColumn(col);
			if (column != null) {
				column.setPreferredWidth(width);
				column.setWidth(width);
			}
		}
		table.validate();
		table.repaint();
	}
	
	public synchronized void setEditable(boolean editable) {
		this.editable = editable;
	}
	
	public synchronized void setEditable(int row, int column, boolean editable) {
		checkIndex("setEditable", row, column);
		this.editable = editable;
	}
	
	public synchronized void setEditorValue(int row, int column, String value) {
		checkIndex("setEditorValue", row, column);
		if (cellEditor == null || !cellEditor.isEditing()) {
			return;
		}
		
		if (value == null) {
			value = "";
		}
		eventsEnabled = false;
		cellEditor.setEditorValue(value);
		eventsEnabled = true;
	}
	
	public synchronized void setEventEnabled(int eventType, boolean enabled) {
		if (eventType != TABLE_COPY
				&& eventType != TABLE_CUT
				&& eventType != TABLE_PASTE
				&& eventType != TABLE_EDIT_BEGIN
				&& eventType != TABLE_REPLACE_BEGIN
				&& eventType != TABLE_SELECTED
				&& eventType != TABLE_UPDATED) {
			throw new IllegalArgumentException("GTable.setEventEnabled: bad event type: 0x" + Integer.toHexString(eventType));
		}
		if (enabled) {
			eventTypesEnabled.add(eventType);
		} else {
			eventTypesEnabled.remove(eventType);
		}
	}
	
	public synchronized void setFont(Font font) {
		table.setFont(font);
		if (DEFAULT_CELL_FORMAT != null) {
			DEFAULT_CELL_FORMAT.font = font;
		}
	}
	
	public synchronized void setForeground(Color color) {
		table.setForeground(color);
		if (DEFAULT_CELL_FORMAT != null) {
			DEFAULT_CELL_FORMAT.foregroundColor = color;
		}
		table.repaint();
	}
	
	public synchronized void setHorizontalAlignment(int alignment) {
		this.horizontalAlignment = alignment;
		for (int row = 0; row < numRows(); row++) {
			for (int col = 0; col < numCols(); col++) {
				DefaultTableCellRenderer renderer = (DefaultTableCellRenderer) table.getCellRenderer(row, col);
				renderer.setHorizontalAlignment(alignment);
			}
		}
//		DefaultTableCellRenderer renderer = (DefaultTableCellRenderer) table.getCellRenderer(0, 0);
//		renderer.setHorizontalAlignment(alignment);
//		table.validate();
		if (DEFAULT_CELL_FORMAT != null) {
			DEFAULT_CELL_FORMAT.alignment = alignment;
		}
		table.repaint();
	}
	
	public synchronized void setJavaBackEnd(JavaBackEnd jbe) {
		this.jbe = jbe;
	}
	
	public synchronized void setRowAlignment(int row, int alignment) {
		if (!inBounds(row, /* column */ 0)) {
			return;
		}
		if (rowFormats[row] == null) {
			rowFormats[row] = new CellFormattingInfo();
		}
		rowFormats[row].alignment = alignment;
		table.repaint();
	}
	
	public synchronized void setRowBackground(int row, Color color) {
		if (!inBounds(row, /* column */ 0)) {
			return;
		}
		if (rowFormats[row] == null) {
			rowFormats[row] = new CellFormattingInfo();
		}
		rowFormats[row].backgroundColor = color;
		table.repaint();
	}
	
	public synchronized void setRowColor(int row, Color color) {
		setRowForeground(row, color);
	}
	
	public synchronized void setRowColumnHeadersVisible(boolean visible) {
		this.rowColumnHeadersVisible = visible;
		setColumnHeadersVisible(visible);
		setRowHeadersVisible(visible);
	}
	
	public synchronized void setRowFont(int row, Font font) {
		if (!inBounds(row, /* column */ 0)) {
			return;
		}
		if (rowFormats[row] == null) {
			rowFormats[row] = new CellFormattingInfo();
		}
		rowFormats[row].font = font;
		table.repaint();
	}
	
	public synchronized void setRowForeground(int row, Color color) {
		if (!inBounds(row, /* column */ 0)) {
			return;
		}
		if (rowFormats[row] == null) {
			rowFormats[row] = new CellFormattingInfo();
		}
		rowFormats[row].foregroundColor = color;
		table.repaint();
	}
	
	public synchronized void setRowHeadersVisible(boolean visible) {
		if (visible) {
			GTableRowUtilities.addNumberColumn(table, /* startingNumber */ 1, /* isRowSelectable */ false);
		} else {
			// TODO
		}
	}
	
	/**
	 * Returns an Excel-style column name, such as "A" for 0, "B" for 1, ..., "Z", "AA", "AB", etc.
	 * @param columnNumber 0-based column number
	 */
	public static String toExcelStyleColumnName(int columnNumber) {
		columnNumber++;   // 1-based
		String result = "";
		while (columnNumber > 0) {
			int lastDigit = (columnNumber - 1) % 26;
			columnNumber = (columnNumber - lastDigit) / 26;
			result = (char) ('A' + lastDigit) + result;
		}
		return result;
	}
	
	private void checkDimensions(String member, int numRows, int numCols) {
		if (numRows < 0 || numCols < 0) {
			throw new IllegalArgumentException("GTable." + member + ": dimensions " + numRows + "x" + numCols
					+ " cannot be negative");
		}
	}
	
	private void checkIndex(String member, int row, int column) {
		if (!inBounds(row, column)) {
			throw new IndexOutOfBoundsException("GTable." + member + ": index R" + row + "C" + column
					+ " is out of bounds of 0,0 - " + (numRows() - 1) + "," + (numCols() - 1));
		}
	}
	
	private void clipboardCopy() {
		int row = table.getSelectedRow();
		int col = table.getSelectedColumn();
		if (!inBounds(row, col)) {
			return;
		}
		
		String value = get(row, col);
		if (!value.isEmpty()) {
			ClipboardUtils.copy(value);
			notifyBackEndOfEvent(TABLE_COPY, row, col);
		}
	}
	
	private void clipboardCut() {
		int row = table.getSelectedRow();
		int col = table.getSelectedColumn();
		if (!inBounds(row, col)) {
			return;
		}
		
		String value = get(row, col);
		if (!value.isEmpty()) {
			ClipboardUtils.copy(value);
			set(row, col, "");   // only change from clipboardCopy(); notifies back-end
			notifyBackEndOfEvent(TABLE_CUT, row, col);
		}
	}
	
	private void clipboardPaste() {
		int row = table.getSelectedRow();
		int col = table.getSelectedColumn();
		if (!inBounds(row, col)) {
			return;
		}
		
		if (!ClipboardUtils.isEmpty()) {
			String clip = ClipboardUtils.get();
			set(row, col, clip);   // notifies back-end
			notifyBackEndOfEvent(TABLE_PASTE, row, col);
		}
	}
	
//	private class ActionEditReplace extends AbstractAction {
//		public void actionPerformed(ActionEvent event) {
//			int row = table.getSelectedRow();
//			int col = table.getSelectedColumn();
//			if (inBounds(row, col)) {
//				table.editCellAt(row, col);
//				table.getCellEditor(row, col);
//			}
//		}
//	}
	
	/* Represents all of the various formats that can be set on a cell (colors, fonts, alignment). */
	private class CellFormattingInfo {
		private int alignment = -1;
		private Font font;
		private Color foregroundColor;
		private Color backgroundColor;
		
		public void apply(DefaultTableCellRenderer renderer, Component comp) {
			if (comp == null) {
				return;
			}
			if (alignment >= 0) {
				if (comp instanceof JComponent) {
					JComponent jcomp = (JComponent) comp;
					if (alignment == SwingConstants.LEFT) {
						jcomp.setAlignmentX(0.0f);
					} else if (alignment == SwingConstants.CENTER) {
						jcomp.setAlignmentX(0.5f);
					} else if (alignment == SwingConstants.RIGHT) {
						jcomp.setAlignmentX(1.0f);
					}
				}
				
				if (renderer != null) {
					renderer.setHorizontalAlignment(alignment);
					renderer.setVerticalAlignment(SwingConstants.TOP);
				}
			}
			if (font != null) {
				comp.setFont(font);
			}
			if (foregroundColor != null) {
				comp.setForeground(foregroundColor);
			}
			if (backgroundColor != null) {
				comp.setBackground(backgroundColor);
			}
		}
	}
	
	private class GTableActionListener implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			String cmd = event.getActionCommand().intern();
			if (cmd == "Copy") {
				clipboardCopy();
			} else if (cmd == "Cut") {
				clipboardCut();
			} else if (cmd == "Paste") {
				clipboardPaste();
			}
		}
	}
	
	private class GTableCellEditor extends DefaultCellEditor
			implements DocumentListener, FocusListener, KeyListener {
		private JTextField field;
		private boolean isEditing = false;
		private boolean isReplacing = false;
		private int lastKeyCode = 0;
		private String lastValue = "";

		public GTableCellEditor() {
			super(new JTextField());
			getComponent().setName("Table.editor");
			field = (JTextField) getComponent();
			field.setBorder(BorderFactory.createEmptyBorder(/* TLBR */ 1, 3, 1, 3));
			field.setFont(tableFont);
			if (DEFAULT_CELL_FORMAT != null) {
				DEFAULT_CELL_FORMAT.apply(/* renderer */ null, field);
			}
			field.addFocusListener(this);
			field.getDocument().addDocumentListener(this);
			field.addKeyListener(this);
		}

		@Override
		public boolean isCellEditable(EventObject e) {
			if (e instanceof KeyEvent) {
				return startWithKeyEvent((KeyEvent) e);
			}
			return super.isCellEditable(e);
		}

		private boolean startWithKeyEvent(KeyEvent e) {
			int row = getSelectedRow();
			int col = getSelectedColumn();
			String cellValue = inBounds(row, col) ? get(row, col) : "";
			
			if (e.isControlDown() || e.isMetaDown() || e.isAltDown()) {
				// don't swallow control keys for editing table contents
				return false;
			} else if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
				return false;
			} else if (e.getKeyCode() == KeyEvent.VK_DELETE || e.getKeyCode() == KeyEvent.VK_BACK_SPACE) {
				// empty out contents of currently selected cell; don't begin edit mode
				lastValue = "";
				set(row, col, "");   // notifies C++ backend that the cell's value changed
				return false;
			} else if (e.getKeyCode() == KeyEvent.VK_F2) {
				// standard key for editing an existing value
				isEditing = true;
				isReplacing = false;
				lastValue = cellValue;
				return true;
			} else if (!isEditing) {
				// typing a new value; overwrite old one
				lastValue = cellValue;
				set(row, col, "", /* notifyBackEnd */ false);
				isEditing = true;
				isReplacing = true;
				return true;
			}
			return true;
		}

		public void focusGained(FocusEvent e) {
			if (DEFAULT_CELL_FORMAT != null) {
				DEFAULT_CELL_FORMAT.apply(/* renderer */ null, field);
			}
			isEditing = true;
			
			// make sure we aren't selecting any characters and that the cursor is placed
			// at the end of the text so we can append (dumb Mac bug)
			field.setSelectionStart(field.getText().length());
			
			int eventType = isReplacing ? TABLE_REPLACE_BEGIN : TABLE_EDIT_BEGIN;
			notifyBackEndOfEvent(eventType);
		}

		public void focusLost(FocusEvent e) {
			stopCellEditing();
			if (isReplacing && lastKeyCode == KeyEvent.VK_ESCAPE) {
				int row = table.getSelectedRow();
				int col = table.getSelectedColumn();
				if (inBounds(row, col)) {
					set(row, col, lastValue, /* notifyBackEnd */ false);
				}
			}
			isEditing = false;
			isReplacing = false;
			lastKeyCode = '\0';
		}

		public boolean isEditing() {
			return isEditing;
		}

		public void setEditorValue(String value) {
			lastValue = value;
			field.setText(value);
		}

		public void insertUpdate(DocumentEvent e) {
			if (!field.hasFocus()) {
				field.requestFocus();
			}
		}

		public void removeUpdate(DocumentEvent e) {
			if (!field.hasFocus()) {
				field.requestFocus();
			}
		}

		public void changedUpdate(DocumentEvent e) {
			if (!field.hasFocus()) {
				field.requestFocus();
			}
		}

		public void keyTyped(KeyEvent e) {
			if (lastKeyCode == KeyEvent.VK_ESCAPE) {
				e.consume();
			}
		}

		public void keyPressed(KeyEvent e) {
			if (!e.isShiftDown()) {
				lastKeyCode = e.getKeyCode();
			}
			if (lastKeyCode == KeyEvent.VK_ESCAPE) {
				e.consume();
				// stopCellEditing();
				cancelCellEditing();
			} else if (lastKeyCode == KeyEvent.VK_DOWN) {
				if (EXIT_CELL_EDITOR_ON_ARROW_KEY) {
					// move down when down arrow pressed during edit
					int row = table.getSelectedRow();
					int col = table.getSelectedColumn();
					if (inBounds(row + 1, col)) {
						e.consume();
						stopCellEditing();
						select(row + 1, col);
					}
				} else {
					e.consume();
				}
			} else if (lastKeyCode == KeyEvent.VK_ENTER) {
				// move down and/or stop editing when Enter pressed during edit
				e.consume();
				stopCellEditing();
				int row = table.getSelectedRow();
				int col = table.getSelectedColumn();
				if (inBounds(row + 1, col)) {
					select(row + 1, col);
				}
			} else if (EXIT_CELL_EDITOR_ON_ARROW_KEY && lastKeyCode == KeyEvent.VK_LEFT) {
				// move to left when left arrow pressed during edit at start of text
				if (field.getSelectionStart() <= 0) {
					int row = table.getSelectedRow();
					int col = table.getSelectedColumn();
					if (inBounds(row, col - 1)) {
						e.consume();
						stopCellEditing();
						select(row, col - 1);
					}
				}
			} else if (EXIT_CELL_EDITOR_ON_ARROW_KEY && lastKeyCode == KeyEvent.VK_RIGHT) {
				// move to right when right arrow pressed during edit at end of text
				if (field.getSelectionStart() >= field.getText().length()) {
					int row = table.getSelectedRow();
					int col = table.getSelectedColumn();
					if (inBounds(row, col + 1)) {
						e.consume();
						stopCellEditing();
						select(row, col + 1);
					}
				}
			} else if (lastKeyCode == KeyEvent.VK_UP) {
				if (EXIT_CELL_EDITOR_ON_ARROW_KEY) {
					// move up when up arrow pressed during edit
					int row = table.getSelectedRow();
					int col = table.getSelectedColumn();
					if (inBounds(row - 1, col)) {
						e.consume();
						stopCellEditing();
						select(row - 1, col);
					}
				} else {
					e.consume();
				}
			}
		}

		public void keyReleased(KeyEvent e) {
			if (lastKeyCode == KeyEvent.VK_ESCAPE) {
				e.consume();
			}
		}
	}

	private class GTableCellEditorListener implements CellEditorListener {
		public void editingStopped(ChangeEvent e) {
			// System.out.println("DEBUG: editingStopped: " + e);
		}

		public void editingCanceled(ChangeEvent e) {
			// System.out.println("DEBUG: editingCanceled: " + e);
		}
	}
	
	private class GTableCellRenderer extends DefaultTableCellRenderer {
		private Color selectedCellBackground;
		
		@Override
		public Component getTableCellRendererComponent(JTable table, Object value,
				boolean isSelected, boolean hasFocus, int row, int column) {
			Component comp = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
			if (!inBounds(row, column)) {
				return comp;
			}
			
			if (selectedCellBackground == null) {
				selectedCellBackground = table.getSelectionBackground();
				if (selectedCellBackground == null || Color.WHITE.equals(selectedCellBackground)) {
					selectedCellBackground = Color.CYAN.brighter().brighter();
				}
			}
			
			// apply any formatting that has been set on the row or column or cell
			if (DEFAULT_CELL_FORMAT == null) {
				DEFAULT_CELL_FORMAT = new CellFormattingInfo();
				DEFAULT_CELL_FORMAT.alignment = GTable.this.horizontalAlignment;
				DEFAULT_CELL_FORMAT.backgroundColor = table.getBackground();
				DEFAULT_CELL_FORMAT.font = table.getFont();
				DEFAULT_CELL_FORMAT.foregroundColor = table.getForeground();
			} else {
				DEFAULT_CELL_FORMAT.apply(this, comp);
			}
			
			if (rowFormats[row] != null) {
				rowFormats[row].apply(this, comp);
			}
			if (colFormats[column] != null) {
				colFormats[column].apply(this, comp);
			}
			if (cellFormats[row][column] != null) {
				cellFormats[row][column].apply(this, comp);
			}
			
			// BUGFIX: make selected cell use selection background color (even if not in focus)
			int selectedRow = getSelectedRow();
			int selectedCol = getSelectedColumn();
			if (selectedRow == row && selectedCol == column) {
				Color bg = selectedCellBackground;
				if (!table.hasFocus()) {
					bg = bg.brighter();
					bg = new Color(bg.getRed(), bg.getGreen(), bg.getBlue(), /* alpha */ 200);
				}
				comp.setBackground(bg);
			}
			
			return comp;
		}
	}

	private class GTableKeyListener implements KeyListener {
		public void keyPressed(KeyEvent e) {
			if (jbe != null) {
				jbe.keyPressed(e);
			}
		}

		public void keyReleased(KeyEvent e) {
			if (jbe != null) {
				jbe.keyReleased(e);
			}
		}
		
		public void keyTyped(KeyEvent e) {
			if (jbe != null) {
				jbe.keyTyped(e);
			}
		}
	}
	
	private class GTableListSelectionListener implements ListSelectionListener {
		// Required method of ListSelectionListener interface.
		// Notifies when selected cell changes.
		public void valueChanged(ListSelectionEvent e) {
			if (!e.getValueIsAdjusting()) {
				int row = table.getSelectedRow();
				int col = table.getSelectedColumn();
				if (eventsEnabled && (row != previouslySelectedRow || col != previouslySelectedColumn)) {
					if (inBounds(row, col)) {
						notifyBackEndOfEvent(TABLE_SELECTED, row, col);
					}
					previouslySelectedRow = row;
					previouslySelectedColumn = col;
				}
			}
		}
	}
	
	/**
	 * Small class to represent the data model of a JTable.
	 * Basically only needed for setEditable / isEditable.
	 */
	private class GTableModel extends DefaultTableModel {
		public GTableModel(int rows, int cols) {
			super(rows, cols);
		}
		
		public boolean isCellEditable(int row, int col) {
			return editable;
		}
	}
	
	private class GTableModelListener implements TableModelListener {
		// Required method of TableModelListener interface.
		// Notifies when data in a cell changes.
		public void tableChanged(TableModelEvent e) {
			if (eventsEnabled) {
				int row = e.getFirstRow();
				int col = e.getColumn();
				if (inBounds(row, col)) {
					notifyBackEndOfEvent(TABLE_UPDATED, row, col);
				}
			}
		}
	}
	
	/**
	 * GTableRowUtilities. Utility for adding a row column to a JTable
	 * @author Oliver Watkins, modified by Marty Stepp
	 */
	private static class GTableRowUtilities {

		/**
		 * Adds a number column in the row header of the scrollpane, to match rows
		 * in the table. Assumes that table has already been added to a scollpane.
		 * If the table is not in a scrollpane nothing will happen.
		 * 
		 * @param userTable - Table to have column added to (if it is in a scrollpane)
		 * @param startingNumber - Number to start number column with, typically 0 or 1.
		 */
		public static void addNumberColumn(final JTable userTable, int startingNumber, boolean isRowSelectable) {
			Container parentContainer = userTable.getParent();
			JViewport viewport = null;
			JScrollPane scrollPane = null;
			if (parentContainer instanceof JViewport) {
				viewport = (JViewport) parentContainer;
				
				Container parentParentContainer = parentContainer.getParent();
				if (parentParentContainer instanceof JScrollPane) {
					scrollPane = (JScrollPane) parentParentContainer;
					
					// Make certain we are the viewPort's view and not, for
					// example, the rowHeaderView of the scrollPane -
					// an implementor of fixed columns might do this.
					viewport = scrollPane.getViewport();
					if (viewport == null || viewport.getView() != userTable) {
						return;
					}
				}
			}

			JTableHeader tableHeader = (JTableHeader) userTable.getTableHeader();
			if (scrollPane != null) {
				scrollPane.setColumnHeaderView(tableHeader);
			}
			final JTable rowHeadersTable = new JTable(new GTableRowUtilities().new RowHeadersTableModel(userTable.getModel().getRowCount(), startingNumber));
			Font font = rowHeadersTable.getTableHeader().getFont();
			rowHeadersTable.getTableHeader().setFont(font.deriveFont(Font.BOLD));
			
			// rowHeadersTable.getModel().addTableModelListener()
			userTable.getModel().addTableModelListener(new TableModelListener() {

				public void tableChanged(TableModelEvent e) {
					RowHeadersTableModel m = (RowHeadersTableModel) rowHeadersTable.getModel();

					if (userTable.getRowCount() != m.getRowCount()) {
						if (userTable.getRowCount() > m.getRowCount()) {

							int rowDiff = userTable.getRowCount() - m.getRowCount();

							for (int i = 0; i < rowDiff; i++) {
								m.addNumber();
							}
						} else if (userTable.getRowCount() < m.getRowCount()) {

							int rowDiff = m.getRowCount() - userTable.getRowCount();

							for (int i = 0; i < rowDiff; i++) {
								m.removeNumber();
							}
						}
						m.fireTableDataChanged();
					}
				}
			});

			// label used for rendering and for
			final JLabel label = new JLabel();

			if (scrollPane != null) {
				scrollPane.getVerticalScrollBar().addAdjustmentListener(new AdjustmentListener() {
					public void adjustmentValueChanged(AdjustmentEvent e) {
						int scrollBarValue = e.getValue();
	
						adjustColumnWidth(rowHeadersTable, label, scrollBarValue);
					}
				});
			}

			// this is where you set the width of the row headers
			rowHeadersTable.createDefaultColumnsFromModel();

			// make the rows look like headers
			rowHeadersTable.setBackground(rowHeadersTable.getTableHeader().getBackground());
			rowHeadersTable.setForeground(rowHeadersTable.getTableHeader().getForeground());
			rowHeadersTable.setFont(font.deriveFont(Font.BOLD));

			rowHeadersTable.setRowHeight(userTable.getRowHeight());

			rowHeadersTable.getTableHeader().setReorderingAllowed(false);
			/**
			 * If selectable then change the colouring in the renderer
			 */
			if (isRowSelectable) {
				// adding a renderer
				rowHeadersTable.getColumnModel().getColumn(0).setCellRenderer(new TableCellRenderer() {
					public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected,
							boolean hasFocus, int row, int column) {
						label.setText("" + value);

						if (isSelected) {
							label.setForeground(rowHeadersTable.getSelectionForeground());
							label.setBackground(rowHeadersTable.getSelectionBackground());
						} else {
							label.setBackground(rowHeadersTable.getBackground());
							label.setForeground(rowHeadersTable.getForeground());
						}
						return label;
					}
				});
			}
			rowHeadersTable.setRowSelectionAllowed(isRowSelectable);
			rowHeadersTable.setCellSelectionEnabled(isRowSelectable);
			rowHeadersTable.setFocusable(isRowSelectable);
			// rowHeadersTable.setDragEnabled(isRowSelectable);   // TODO: re-enable?
			
			if (scrollPane != null) {
				scrollPane.setRowHeaderView(rowHeadersTable);
	
				// set the row header name into the top left corner
				scrollPane.setCorner(JScrollPane.UPPER_LEFT_CORNER, rowHeadersTable.getTableHeader());
	
				Border border = scrollPane.getBorder();
				if (border == null || border instanceof UIResource) {
					scrollPane.setBorder(UIManager.getBorder("Table.scrollPaneBorder"));
				}
	
				final JScrollPane theScrollPane = scrollPane;   // Java is dumb about closures
				scrollPane.addComponentListener(new ComponentAdapter() {
					/**
					 * Whenever the component is resized need to re-adjust the
					 * column width if necessary. This method is also called
					 * when the screen is first layed out.
					 */
					public void componentResized(ComponentEvent e) {
						adjustColumnWidth(rowHeadersTable, label, theScrollPane.getVerticalScrollBar().getValue());
					}
				});
			}

			rowHeadersTable.setSelectionMode(userTable.getSelectionModel().getSelectionMode());

			new GTableRowUtilities().new TableListener(rowHeadersTable, userTable);
		}

		/**
		 * Table Model for the row number column. It just has one column (the numbers)
		 * 
		 * @author Oliver Watkins 
		 */
		private class RowHeadersTableModel extends AbstractTableModel {
			private ArrayList<Integer> numbersList = new ArrayList<Integer>();
			private int startNumber;
			
			/**
			 * Initalise model
			 * 
			 * @param maxNumber determined by JTable row size
			 * @param startingNumber usually zero or 1
			 */
			public RowHeadersTableModel(int maxNumber, int startingNumber) {
				// start at starting number and then go to row count (plus starting
				// number amount)
				this.startNumber = startingNumber;
				int j = 0;
				for (int i = startingNumber; i < maxNumber + startNumber; i++) {
					numbersList.add(new Integer(j + startNumber));
					j++;
				}
			}

			public int getRowCount() {
				if (numbersList != null)
					return numbersList.size();
				else
					return 0;
			}

			public int getMaxIntValue() {
				if (numbersList != null && numbersList.size() != 0) {
					Integer integer = (Integer) getValueAt(numbersList.size() - 1, 0);
					return integer.intValue();
				} else
					return 0;
			}

			public int getColumnCount() {
				return 1;
			}

			public String getColumnName(int columnIndex) {
				return "";
			}

			public Class<?> getColumnClass(int columnIndex) {
				return Integer.class;
			}

			public boolean isCellEditable(int rowIndex, int columnIndex) {
				return false;
			}

			public Object getValueAt(int rowIndex, int columnIndex) {
				return numbersList.get(rowIndex);
			}

			public void addNumber() {
				if (numbersList.size() != 0) {
					Integer maxNum = (Integer) numbersList.get(numbersList.size() - 1);

					numbersList.add(numbersList.size(), new Integer(maxNum.intValue() + 1));
				} else {
					numbersList.add(numbersList.size(), new Integer(startNumber));
				}
				this.fireTableDataChanged();
			}

			public void removeNumber() {
				numbersList.remove(numbersList.size() - 1);
			}

			public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
			}

			public void addTableModelListener(TableModelListener l) {
				super.addTableModelListener(l);
			}

			public void removeTableModelListener(TableModelListener l) {
				super.removeTableModelListener(l);
			}

		};

		/**
		 * Adjusts the column width of the row headers table containg the number
		 * column. The font metrics are extracted from the label of the row at the
		 * bottom of the viewport and used to determing the appropriate width.
		 * 
		 * The reason why this method is important, is that when the row number increases by an extra digit
		 * the column needs to get wider. It also needs to shrink when scrolling to smaller digit numbers.
		 * 
		 * @param rowHeadersTable - single column table in the row header
		 * @param label - label used to get font metrics
		 * @param scrollBarValue - int value for determing point of lowest row
		 */
		private static void adjustColumnWidth(final JTable rowHeadersTable, final JLabel label, int scrollBarValue) {
			label.setFont(rowHeadersTable.getFont());
			label.setOpaque(true);
			label.setHorizontalAlignment(JLabel.CENTER);

			int v = rowHeadersTable.getVisibleRect().height;

			int row = rowHeadersTable.rowAtPoint(new Point(0, v + scrollBarValue));

			Integer modelValue = null;
			if (row != -1) {
				modelValue = (Integer) rowHeadersTable.getModel().getValueAt(row, 0);
			} else {
				RowHeadersTableModel tm = (RowHeadersTableModel) rowHeadersTable.getModel();
				modelValue = new Integer(tm.getMaxIntValue());
			}

			label.setText("" + modelValue);
			FontMetrics fontMetrics = label.getFontMetrics(label.getFont());

			int widthFactor = 0;

			if (fontMetrics != null && label.getText() != null) {
				widthFactor = fontMetrics.stringWidth(label.getText());
				rowHeadersTable.setPreferredScrollableViewportSize(new Dimension(widthFactor + 8, 100));   // height is ignored
				rowHeadersTable.repaint();
			}
		}

		/**
		 * Listener that joins the two tables; the main table, and the single column row number table. When either
		 * are moved or selected, then the affect is passed on to the other table.
		 * 
		 * @author Oliver Watkins
		 */
		private class TableListener implements ListSelectionListener {
			private JTable rowHeadersTable;
			private JTable userTable;
			private JViewport userTableViewPort;
			private JViewport rowHeadersViewPort;

			public TableListener(JTable rowHeadersTable, JTable userTable) {
				this.userTable = userTable;
				this.rowHeadersTable = rowHeadersTable;

				Container p = userTable.getParent();
				userTableViewPort = (JViewport) p;

				Container p2 = rowHeadersTable.getParent();
				rowHeadersViewPort = (JViewport) p2;

				Point newPosition = userTableViewPort.getViewPosition();
				rowHeadersViewPort.setViewPosition(newPosition);

				// userTableViewPort.setViewPosition(newPosition);

				rowHeadersTable.getSelectionModel().addListSelectionListener(this);
				userTable.getSelectionModel().addListSelectionListener(this);

			}

			public void valueChanged(ListSelectionEvent e) {
				if (e.getSource() == userTable.getSelectionModel()) {
					rowHeadersTable.getSelectionModel().removeListSelectionListener(this);
					rowHeadersTable.getSelectionModel().clearSelection();

					int[] rows = userTable.getSelectedRows();

					for (int i = 0; i < rows.length; i++) {
						rowHeadersTable.getSelectionModel().addSelectionInterval(rows[i], rows[i]);

					}
					/* int[] iarr = */ rowHeadersTable.getSelectedRows();

					rowHeadersTable.getSelectionModel().addListSelectionListener(this);
				} else if (e.getSource() == rowHeadersTable.getSelectionModel()) {
					boolean isColumnSelectionAllowed = userTable.getColumnSelectionAllowed();
					boolean isRowSelectionAllowed = userTable.getRowSelectionAllowed();
					boolean isCellSelectionAllowed = userTable.getCellSelectionEnabled();

					userTable.getSelectionModel().removeListSelectionListener(this);
					userTable.getSelectionModel().clearSelection();

					int[] rows = rowHeadersTable.getSelectedRows();

					if (isRowSelectionAllowed && !isCellSelectionAllowed && !isColumnSelectionAllowed) {
						for (int i = 0; i < rows.length; i++) {
							userTable.addRowSelectionInterval(rows[i], rows[i]);
							userTableViewPort.setViewPosition(rowHeadersViewPort.getViewPosition());

						}
					} else {
						// looks cleaner
						userTableViewPort.setScrollMode(JViewport.BACKINGSTORE_SCROLL_MODE);

						for (int i = 0; i < rows.length; i++) {
							if (i == 0) {
								// need to create row first with change selection
								userTable.changeSelection(rows[i], 0, false, false);
								userTable.changeSelection(rows[i], userTable.getColumnCount(), false, true);

							} else {
								userTable.addRowSelectionInterval(rows[i], rows[i]);
							}
						}
					}
					// re-adding the listener to the user table
					userTable.getSelectionModel().addListSelectionListener(this);
				}
			}
		}
	}
	

	// small internal test driver for GTable class
	public static void main(String[] args) {
		GuiUtils.setSystemLookAndFeel();
		
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		GTable table = new GTable(5, 3);
		table.setEditable(true);
		table.setEventEnabled(TABLE_EDIT_BEGIN, true);
		// table.setColumnHeadersVisible(true);
//		frame.add(new JScrollPane(table.getInteractor()));
		table.setColumnHeaderStyle(COLUMN_HEADER_EXCEL);
		table.setColumnHeadersVisible(true);
		table.setRowHeadersVisible(true);
		frame.add(table.getInteractor());
		frame.pack();
		frame.setVisible(true);
	}
}
