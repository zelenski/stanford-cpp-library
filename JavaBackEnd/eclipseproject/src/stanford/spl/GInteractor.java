/*
 * @version 2016/10/23
 * - added requestFocus
 * @version 2016/10/16
 * - added get/setIcon
 * @version 2015/12/01
 * - added setBackground, setColor
 */

package stanford.spl;

import acm.graphics.*;
import acm.util.JTFTools;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public abstract class GInteractor extends GObject implements GResizable {
	private static final long serialVersionUID = 1L;
	
	private JComponent interactor;
	private String actionCommand;
	private String accelerator = "";

	public GInteractor(JComponent paramJComponent) {
		this.interactor = paramJComponent;
		this.actionCommand = "";
		Dimension localDimension = paramJComponent.getPreferredSize();
		paramJComponent.setSize(localDimension.width, localDimension.height);
	}
	
	public boolean setAccelerator(String accelerator) {
		final JComponent interactor = getInteractor();
		if (interactor == null) {
			return false;
		}
		
		if (accelerator.isEmpty()) {
			// remove accelerator
			interactor.getActionMap().remove("hotkey");
			return true;
		}
		
		// do a tiny bit of preprocessing on the accelerator
		// e.g. "Ctrl-x" => "control x"
		accelerator = accelerator.toUpperCase();
		accelerator = accelerator.replace("ALTGRAPH", "altGraph");
		accelerator = accelerator.replace("ALT", "alt");
		accelerator = accelerator.replace("CTRL", "control");
		accelerator = accelerator.replace("CONTROL", "control");
		accelerator = accelerator.replace("META", "meta");
		accelerator = accelerator.replace("SHIFT", "shift");
		accelerator = accelerator.replace("PRESSED", "pressed");
		accelerator = accelerator.replace("RELEASED", "released");
		accelerator = accelerator.replace("TYPED", "typed");
		accelerator = accelerator.replace("-", " ");
		
		if (!accelerator.contains(" ")) {
			// "y" -> "typed y"
			accelerator = "pressed " + accelerator;
		}
		
		// see also:
		// https://docs.oracle.com/javase/7/docs/api/javax/swing/KeyStroke.html#getKeyStroke(java.lang.String)
		
		KeyStroke keystroke = KeyStroke.getKeyStroke(accelerator);
		if (keystroke == null) {
			return false;
		}
		
		this.accelerator = accelerator;
		// interactor.getInputMap().put(keystroke, "hotkey");
		interactor.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(keystroke, "hotkey");
		
		interactor.getActionMap().put("hotkey", new AbstractAction() {
			public void actionPerformed(ActionEvent event) {
				if (interactor instanceof AbstractButton) {
					// delegate to existing action listener (likely the JavaBackEnd)
					AbstractButton button = (AbstractButton) interactor;
					for (ActionListener listener : button.getActionListeners()) {
						listener.actionPerformed(event);
					}
				}
			}
		});
		return true;
	}
	
	public String getAccelerator() {
		return accelerator;
	}
	
	public void setBackground(Color color) {
		this.interactor.setBackground(color);
		if (this.interactor instanceof JButton) {
			// workaround for buttons not showing background on some platforms
			this.interactor.setOpaque(true);
		}
	}
	
	// JL: SwingUtilities.invokeLater
	@Override
	public void setColor(Color color) {
		super.setColor(color);
		setForeground(color);
	}
	
	public void setFont(Font font) {
		
	}
	
	// e.g. "Arial-Bold-16"
	public void setFont(String fontString) {
		Font font = null;
		JComponent interactor = getInteractor();
		if (interactor == null) {
			return;
		}
		
		Font oldFont = interactor.getFont();
		// font = JTFTools.decodeFont(fontString, oldFont);
		font = JTFTools.decodeFont(fontString);
		if (font == null) {
			font = JTFTools.decodeFont(fontString, oldFont);
		}
		if (font != null) {
			interactor.setFont(font);
		}
	}
	
	public Font getFont() {
		Font font = null;
		JComponent interactor = getInteractor();
		if (interactor != null) {
			font = interactor.getFont();
		}
		return font;
	}
	
	public String getFontString() {
		Font font = getFont();
		if (font == null) {
			return "";
		} else {
			int style = font.getStyle();
			String fontString = font.getFamily() + "-";
			if (style == Font.PLAIN) {
				fontString += "plain";
			} else {
				if ((style & Font.BOLD) != 0) {
					fontString += "bold";
				}
				if ((style & Font.ITALIC) != 0) {
					fontString += "italic";
				}
			}
			fontString += "-" + font.getSize();
			return fontString;
		}
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
	
	public Icon getIcon() {
		JComponent interactor = getInteractor();
		if (interactor == null) {
			return null;
		} else if (interactor instanceof JButton) {
			return ((JButton) interactor).getIcon();
		} else if (interactor instanceof JCheckBox) {
			return ((JCheckBox) interactor).getIcon();
		} else if (interactor instanceof JLabel) {
			return ((JLabel) interactor).getIcon();
		} else if (interactor instanceof JRadioButton) {
			return ((JRadioButton) interactor).getIcon();
		} else {
			return null;
		}
	}
	
	public void setIcon(Icon icon) {
		JComponent interactor = getInteractor();
		if (interactor == null) {
			return;
		} else if (interactor instanceof JButton) {
			((JButton) interactor).setIcon(icon);
		} else if (interactor instanceof JCheckBox) {
			((JCheckBox) interactor).setIcon(icon);
		} else if (interactor instanceof JLabel) {
			((JLabel) interactor).setIcon(icon);
		} else if (interactor instanceof JRadioButton) {
			((JRadioButton) interactor).setIcon(icon);
		} else {
			throw new IllegalArgumentException("interactor type does not support setIcon: " + getClass().getSimpleName());
		}
	}
	
	public void setIcon(String iconFileName) {
		setIcon(new ImageIcon(iconFileName));
	}

	public void setLocation(double paramDouble1, double paramDouble2) {
		super.setLocation(paramDouble1, paramDouble2);
		this.interactor.setLocation(GMath.round(paramDouble1), GMath.round(paramDouble2));
		this.interactor.repaint();
	}
	
	public boolean setMnemonic(char mnemonic) {
		return setMnemonic((int) mnemonic);
	}
	
	public boolean setMnemonic(int mnemonic) {
		JComponent interactor = getInteractor();
		if (interactor == null) {
			return false;
		}
		if (interactor instanceof AbstractButton) {
			((AbstractButton) interactor).setMnemonic(mnemonic);
			return true;
		} else {
			return false;
		}
	}
	
	public char getMnemonic() {
		JComponent interactor = getInteractor();
		if (interactor == null) {
			return '\0';
		}
		if (interactor instanceof AbstractButton) {
			return (char) ((AbstractButton) interactor).getMnemonic();
		} else {
			return '\0';
		}
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

	// JL: SwingUtilities.invokeLater
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
	
	public void setTooltip(String tooltipText) {
		JComponent interactor = this.getInteractor();
		if (interactor != null) {
			interactor.setToolTipText(tooltipText);
		}
	}

	public void repaint() {
	}

	public void paint2d(Graphics2D paramGraphics2D) {
	}

	protected void paintObject(Graphics paramGraphics) {
	}
	
	public void requestFocus() {
		JComponent interactor = getInteractor();
		if (interactor != null) {
			interactor.requestFocus();
		}
	}
}
