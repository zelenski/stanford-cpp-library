package stanford.spl;

import java.awt.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;

/**
 * 
 * @author Marty Stepp
 * @version 2015/12/01
 * - added setEventEnabled / isEventEnabled / ListSelectionListener
 * - added isEditable, setEditable, JTableModel
 * @version 2015/11/11
 * - added select, getSelectedRow/Column, inBounds
 * @since 2015/11/07
 */
public class GTable extends GInteractor implements ListSelectionListener, TableModelListener {
	private static final long serialVersionUID = 1L;
	
	// these MUST match the constants in C++ lib gevents.h
	public static final int TABLE_EVENT    = 0x400;
	public static final int TABLE_UPDATED  = TABLE_EVENT + 1;
	public static final int TABLE_SELECTED = TABLE_EVENT + 2;
	
	// private JScrollPane scrollPane;
	private JTable table;
	private JTableModel model;
	private JavaBackEnd jbe;
	private Set<Integer> eventTypesEnabled = new HashSet<Integer>(Arrays.asList(TABLE_SELECTED, TABLE_UPDATED));
	private boolean eventsEnabled = true;
	private boolean editable = true;
	private int previouslySelectedRow = -1;
	private int previouslySelectedColumn = -1;

	public GTable(int numRows, int numCols) {
		super(new JTable(numRows, numCols));
		checkDimensions("constructor", numRows, numCols);
		table = (JTable) super.getInteractor();
		model = new JTableModel(numRows, numCols);
		table.setModel(model);
		table.setBorder(BorderFactory.createLineBorder(Color.GRAY, /* thickness */ 1));
		// scrollPane = new JScrollPane(table);
		// table.setFillsViewportHeight(true);
		table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		table.setRowSelectionAllowed(false);
		table.setColumnSelectionAllowed(false);
		table.setCellSelectionEnabled(true);
		table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
		model = (JTableModel) table.getModel();
		model.addTableModelListener(this);
		table.getSelectionModel().addListSelectionListener(this);
		table.getColumnModel().getSelectionModel().addListSelectionListener(this);
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
		eventsEnabled = true;
	}
	
//	@Override
//	public JComponent getInteractor() {
//		return scrollPane;
//	}
	
	public synchronized String get(int row, int column) {
		checkIndex("get", row, column);
		Object value = table.getValueAt(row, column);
		if (value == null) {
			value = "";
		}
		return String.valueOf(value);
	}
	
	public synchronized int getColumnWidth(int col) {
		checkIndex("getColumnWidth", /* row (hack) */ 0, col);
		TableColumn column = table.getColumnModel().getColumn(col);
		return column.getWidth();
	}
	
	public synchronized Font getFont() {
		return table.getFont();
	}
	
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
	
	public int numCols() {
		return table.getColumnCount();
	}
	
	public int numRows() {
		return table.getRowCount();
	}
	
	public synchronized void resize(int numRows, int numCols) {
		checkDimensions("resize", numRows, numCols);
		eventsEnabled = false;
		model.setRowCount(numRows);
		model.setColumnCount(numCols);
		eventsEnabled = true;
		
		previouslySelectedRow = -1;
		previouslySelectedColumn = -1;
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
		checkIndex("set", row, column);
		if (value == null) {
			value = "";
		}
		eventsEnabled = false;
		table.setValueAt(value, row, column);
		eventsEnabled = true;
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
	
	public synchronized void setColumnHeadersVisible(boolean visible) {
		if (visible) {
			Object[] columnHeaders = new Object[numCols()];
			for (int i = 0; i < numCols(); i++) {
				columnHeaders[i] = String.valueOf(i);
			}
			model.setColumnIdentifiers(columnHeaders);
		} else {
			Object[] columnHeaders = new Object[numCols()];
			for (int i = 0; i < numCols(); i++) {
				columnHeaders[i] = "";
			}
			model.setColumnIdentifiers(columnHeaders);
		}
	}
	
	public synchronized void setEditable(boolean editable) {
		this.editable = editable;
	}
	
	public synchronized void setEventEnabled(int eventType, boolean enabled) {
		if (eventType != TABLE_SELECTED && eventType != TABLE_UPDATED) {
			throw new IllegalArgumentException("GTable.setEventEnabled: bad event type: " + eventType);
		}
		if (enabled) {
			eventTypesEnabled.add(eventType);
		} else {
			eventTypesEnabled.remove(eventType);
		}
	}
	
	public synchronized void setFont(Font font) {
		table.setFont(font);
	}
	
	public synchronized void setHorizontalAlignment(int alignment) {
//		for (int row = 0; row < numRows(); row++) {
//			for (int col = 0; col < numCols(); col++) {
//				DefaultTableCellRenderer renderer = (DefaultTableCellRenderer) table.getCellRenderer(row, col);
//				renderer.setHorizontalAlignment(alignment);
//			}
//		}
		DefaultTableCellRenderer renderer = (DefaultTableCellRenderer) table.getCellRenderer(0, 0);
		renderer.setHorizontalAlignment(alignment);
		table.validate();
		table.repaint();
	}
	
	public synchronized void setJavaBackEnd(JavaBackEnd jbe) {
		this.jbe = jbe;
	}
	
    // Required method of TableModelListener interface.
	// Notifies when data in a cell changes.
	public void tableChanged(TableModelEvent e) {
    	synchronized (this) {
			if (eventsEnabled && eventTypesEnabled.contains(TABLE_UPDATED)) {
				int row = e.getFirstRow();
		    	int col = e.getColumn();
		    	String value = get(row, col);
		    	jbe.acknowledgeEvent("event:tableUpdated(\"%s\", %d, %d, %d, \"%s\")",
		    			jbe.getSourceId(this.getInteractor()),
						(long) jbe.getEventTime(),
		    			row,
		    			col,
		    			SplPipeDecoder.encode(value));
	    	}
    	}
    }

    // Required method of ListSelectionListener interface.
	// Notifies when selected cell changes.
	public void valueChanged(ListSelectionEvent e) {
		if (!e.getValueIsAdjusting()) {
			int row = table.getSelectedRow();
			int col = table.getSelectedColumn();
			if (eventsEnabled && (row != previouslySelectedRow || col != previouslySelectedColumn)) {
		    	synchronized (this) {
			    	if (eventsEnabled && eventTypesEnabled.contains(TABLE_SELECTED)) {
			    		jbe.acknowledgeEvent("event:tableSelected(\"%s\", %d, %d, %d)",
				    			jbe.getSourceId(this.getInteractor()),
								(long) jbe.getEventTime(),
				    			row,
				    			col);
			    	}
		    	}
				previouslySelectedRow = row;
				previouslySelectedColumn = col;
			}
		}
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
	
	/**
	 * Small class to represent the data model of a JTable.
	 * Basically only needed for setEditable / isEditable.
	 */
	private class JTableModel extends DefaultTableModel {
		public JTableModel(int rows, int cols) {
			super(rows, cols);
		}
		
		public boolean isCellEditable(int row, int col) {
			return editable;
		}
	}
}
