package stanford.karel;

import java.awt.Panel;
import java.awt.Scrollbar;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;

class KarelResizer extends Panel implements AdjustmentListener {
	private static final int MAX_WIDTH = 50;
	private static final int MAX_HEIGHT = 50;

	private ResizeCanvas resizeCanvas;
	private Scrollbar widthScrollbar;
	private Scrollbar heightScrollbar;

	public KarelResizer() {
		resizeCanvas = new ResizeCanvas();
		resizeCanvas.setDimension(10, 10);
		widthScrollbar = new Scrollbar(Scrollbar.HORIZONTAL);
		widthScrollbar.setValues(10 - 1, 1, 0, MAX_WIDTH);
		widthScrollbar.addAdjustmentListener(this);
		heightScrollbar = new Scrollbar(Scrollbar.VERTICAL);
		heightScrollbar.setValues(MAX_HEIGHT - 10, 1, 0, MAX_HEIGHT);
		heightScrollbar.addAdjustmentListener(this);
		setLayout(new ResizeLayout());
		add("canvas", resizeCanvas);
		add("hbar", widthScrollbar);
		add("vbar", heightScrollbar);
	}

	/* AdjustmentListener interface */
	public void adjustmentValueChanged(AdjustmentEvent event) {
		resizeCanvas.setDimension(getColumns(), getRows());
		resizeCanvas.repaint();
	}

	public int getColumns() {
		return widthScrollbar.getValue() + 1;
	}

	public int getRows() {
		return MAX_HEIGHT - heightScrollbar.getValue();
	}
}
