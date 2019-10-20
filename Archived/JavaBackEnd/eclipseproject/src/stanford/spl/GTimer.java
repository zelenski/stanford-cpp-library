package stanford.spl;

import javax.swing.Timer;

public class GTimer extends Timer {
	private String id;

	public GTimer(String addr, double ms) {
		super((int) Math.round(ms), /* action listener */ null);
		this.id = addr;
	}

	public String getId() {
		return this.id;
	}
}
