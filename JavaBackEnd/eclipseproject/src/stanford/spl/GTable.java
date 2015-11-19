package stanford.spl;

import java.awt.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;

/**
 * 
 * @author Marty Stepp
 * @version 2015/11/11
 * - added select, getSelectedRow/Column, inBounds
 * @since 2015/11/07
 */
public class GTable extends GInteractor implements TableModelListener {
	private static final long serialVersionUID = 1L;
	
	// private JScrollPane scrollPane;
	private JTable table;
	private DefaultTableModel model;
	private JavaBackEnd jbe;
	private boolean eventsEnabled = true;

	public GTable(int numRows, int numCols) {
		super(new JTable(numRows, numCols));
		checkDimensions("constructor", numRows, numCols);
		table = (JTable) super.getInteractor();
		table.setBorder(BorderFactory.createLineBorder(Color.GRAY, /* thickness */ 1));
		// scrollPane = new JScrollPane(table);
		// table.setFillsViewportHeight(true);
		table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		table.setRowSelectionAllowed(false);
		table.setColumnSelectionAllowed(false);
		table.setCellSelectionEnabled(false);
		table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
		model = (DefaultTableModel) table.getModel();
		model.addTableModelListener(this);
	}
	
	public void clear() {
		eventsEnabled = false;
		for (int row = 0; row < table.getRowCount(); row++) {
			for (int col = 0; col < table.getColumnCount(); col++) {
				table.setValueAt("", row, col);
			}
		}
		eventsEnabled = true;
	}
	
//	@Override
//	public JComponent getInteractor() {
//		return scrollPane;
//	}
	
	public String get(int row, int column) {
		checkIndex("get", row, column);
		Object value = table.getValueAt(row, column);
		if (value == null) {
			value = "";
		}
		return String.valueOf(value);
	}
	
	public int getColumnWidth(int col) {
		checkIndex("getColumnWidth", /* row (hack) */ 0, col);
		TableColumn column = table.getColumnModel().getColumn(col);
		return column.getWidth();
	}
	
	public Font getFont() {
		return table.getFont();
	}
	
	public int getSelectedColumn() {
		return table.getSelectedColumn();
	}
	
	public int getSelectedRow() {
		return table.getSelectedRow();
	}
	
	public boolean inBounds(int row, int column) {
		return row >= 0 && row < numRows() && column >= 0 && column < numCols();
	}
	
	public int numCols() {
		return table.getColumnCount();
	}
	
	public int numRows() {
		return table.getRowCount();
	}
	
	public void resize(int numRows, int numCols) {
		checkDimensions("resize", numRows, numCols);
		eventsEnabled = false;
		model.setRowCount(numRows);
		model.setColumnCount(numCols);
		eventsEnabled = true;
	}
	
	public void select(int row, int column) {
		if (inBounds(row, column)) {
			table.setRowSelectionInterval(row, row);
			table.setColumnSelectionInterval(column, column);
		} else {
			table.clearSelection();
		}
	}
	
	public void set(int row, int column, String value) {
		checkIndex("set", row, column);
		if (value == null) {
			value = "";
		}
		eventsEnabled = false;
		table.setValueAt(value, row, column);
		eventsEnabled = true;
	}
	
	public void setColumnWidth(int col, int width) {
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
	
	public void setColumnHeadersVisible(boolean visible) {
		if (visible) {
			Object[] columnHeaders = new Object[numCols()];
			for (int i = 0; i < numCols(); i++) {
				columnHeaders[i] = String.valueOf(i);
			}
			model.setColumnIdentifiers(columnHeaders);
		} else {
			// TODO: remove headers
		}
	}
	
	public void setFont(Font font) {
		table.setFont(font);
	}
	
	public void setHorizontalAlignment(int alignment) {
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
	
	public void setJavaBackEnd(JavaBackEnd jbe) {
		this.jbe = jbe;
	}
	
    // Required method of TableModelListener interface.
	// Notifies when data in a cell changes.
	public void tableChanged(TableModelEvent e) {
    	if (eventsEnabled) {
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
}
