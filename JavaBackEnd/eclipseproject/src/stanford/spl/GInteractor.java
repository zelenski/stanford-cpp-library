/*
 * @version 2015/12/01
 * - added setBackground, setColor
 */

package stanford.spl;

import acm.graphics.*;
import java.awt.*;
import javax.swing.*;

public abstract class GInteractor extends GObject implements GResizable {
	private static final long serialVersionUID = 1L;
	
	private JComponent interactor;
	private String actionCommand;

	public GInteractor(JComponent paramJComponent) {
		this.interactor = paramJComponent;
		this.actionCommand = "";
		Dimension localDimension = paramJComponent.getPreferredSize();
		paramJComponent.setSize(localDimension.width, localDimension.height);
	}
	
	public void setBackground(Color color) {
		this.interactor.setBackground(color);
		if (this.interactor instanceof JButton) {
			// workaround for buttons not showing background on some platforms
			this.interactor.setOpaque(true);
		}
	}
	
	@Override
	public void setColor(Color color) {
		super.setColor(color);
		setForeground(color);
	}
	
	public void setForeground(Color color) {
		this.interactor.setForeground(color);
	}
	
	public void setEnabled(boolean value) {
		this.interactor.setEnabled(value);
	}

	public boolean isEnabled() {
		return this.interactor.isEnabled();
	}

	public JComponent getInteractor() {
		return this.interactor;
	}

	public GRectangle getBounds() {
		Point localPoint = this.interactor.getLocation();
		Dimension localDimension = this.interactor.getPreferredSize();
		return new GRectangle(localPoint.x, localPoint.y, localDimension.width,
				localDimension.height);
	}

	public void setLocation(double paramDouble1, double paramDouble2) {
		super.setLocation(paramDouble1, paramDouble2);
		this.interactor.setLocation(GMath.round(paramDouble1), GMath.round(paramDouble2));
		this.interactor.repaint();
	}

	public void setVisible(boolean paramBoolean) {
		super.setVisible(paramBoolean);
		this.interactor.setVisible(paramBoolean);
		this.interactor.repaint();
	}

	public void setParent(GContainer paramGContainer) {
		if (paramGContainer == null) {
			java.awt.Container parent = this.interactor.getParent();
			if (parent != null) {
				parent.remove(this.interactor);
				parent.validate();
			}
		} else if ((paramGContainer instanceof TopCompound)) {
			TopCompound localTopCompound = (TopCompound) paramGContainer;
			JBECanvas localJBECanvas = localTopCompound.getCanvas();
			if (localJBECanvas != null) {
				localJBECanvas.add(this.interactor);
				localJBECanvas.validate();
			}
		}
	}

	public void setSize(double paramDouble1, double paramDouble2) {
		int i = GMath.round(paramDouble1);
		int j = GMath.round(paramDouble2);
		this.interactor.setPreferredSize(new Dimension(i, j));
		this.interactor.setSize(i, j);
		this.interactor.repaint();
	}

	public void setSize(GDimension paramGDimension) {
		setSize(paramGDimension.getWidth(), paramGDimension.getHeight());
	}

	public void setBounds(double paramDouble1, double paramDouble2, double paramDouble3,
			double paramDouble4) {
		setLocation(paramDouble1, paramDouble2);
		setSize(paramDouble3, paramDouble4);
	}

	public void setBounds(GRectangle paramGRectangle) {
		setBounds(paramGRectangle.getX(), paramGRectangle.getY(), paramGRectangle.getWidth(),
				paramGRectangle.getHeight());
	}

	public void setActionCommand(String paramString) {
		this.actionCommand = paramString;
	}

	public String getActionCommand() {
		return this.actionCommand;
	}

	public void repaint() {
	}

	public void paint2d(Graphics2D paramGraphics2D) {
	}

	protected void paintObject(Graphics paramGraphics) {
	}
}
