package stanford.spl;

public class GTableEvent {
	private GTable table;
	private int row;
	private int col;
	private String value;
	
	public GTableEvent(GTable table) {
		this(table, -1, -1, "");
	}
	
	public GTableEvent(GTable table, int row, int col) {
		this(table, row, col, "");
	}
	
	public GTableEvent(GTable table, int row, int col, String value) {
		this.table = table;
		this.row = row;
		this.col = col;
		this.value = value;
	}
	
	public int getColumn() {
		return col;
	}
	
	public int getRow() {
		return row;
	}

	public GTable getSource() {
		return table;
	}
	
	public String getValue() {
		return value;
	}
}
