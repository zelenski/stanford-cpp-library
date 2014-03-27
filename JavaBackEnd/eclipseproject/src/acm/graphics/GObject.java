98package acm.graphics;

import acm.util.ErrorException;
import acm.util.JTFTools;
import java.awt.AWTEventMulticaster;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.AffineTransform;
import java.io.Serializable;
import java.lang.reflect.Method;

public abstract class GObject implements Cloneable, Serializable, GScalable {
	private static boolean antialiasing = true;
	
	public static void setAntiAliasing(boolean value) {
		antialiasing = value;
	}

	private GCompound compoundParent;
	private AffineTransform matrix;
	private Color objectColor;
	private double lineWidth;
	private double xc;
	private double yc;
	private boolean isVisible;
	private boolean mouseListenersEnabled;
	private transient MouseListener mouseListener;
	private transient MouseMotionListener mouseMotionListener;
	private transient ActionListener actionListener;
	private transient GContainer transientParent;
	static final long serialVersionUID = 21L;

	protected GObject() {
		this.matrix = null;
		this.lineWidth = 1.0D;
		this.isVisible = true;
		this.mouseListenersEnabled = false;
	}

	public void paint(Graphics paramGraphics) {
		if (this.isVisible) {
			Graphics2D localGraphics2D = createTransformedGraphics(paramGraphics);
			paint2d(localGraphics2D);
			localGraphics2D.dispose();
		}
	}

	protected abstract void paint2d(Graphics2D paramGraphics2D);

	public abstract GRectangle getBounds();

	public void setLocation(double paramDouble1, double paramDouble2) {
		this.xc = paramDouble1;
		this.yc = paramDouble2;
		repaint();
	}

	public final void setLocation(GPoint paramGPoint) {
		setLocation(paramGPoint.getX(), paramGPoint.getY());
	}

	public GPoint getLocation() {
		return new GPoint(this.xc, this.yc);
	}

	public double getX() {
		return this.xc;
	}

	public double getY() {
		return this.yc;
	}

	public void move(double paramDouble1, double paramDouble2) {
		setLocation(this.xc + paramDouble1, this.yc + paramDouble2);
	}

	public final void movePolar(double paramDouble1, double paramDouble2) {
		double d = paramDouble2 * 3.141592653589793D / 180.0D;
		move(paramDouble1 * Math.cos(d), -paramDouble1 * Math.sin(d));
	}

	public GDimension getSize() {
		GRectangle localGRectangle = getBounds();
		return new GDimension(localGRectangle.getWidth(), localGRectangle.getHeight());
	}

	public double getWidth() {
		return getBounds().getWidth();
	}

	public double getHeight() {
		return getBounds().getHeight();
	}

	public boolean contains(double paramDouble1, double paramDouble2) {
		return false;
	}

	public final boolean contains(GPoint paramGPoint) {
		return contains(paramGPoint.getX(), paramGPoint.getY());
	}

	public void sendToFront() {
		if (this.compoundParent != null) {
			this.compoundParent.sendToFront(this);
		} else if ((this.transientParent instanceof GCanvas)) {
			((GCanvas) this.transientParent).sendToFront(this);
		} else if (this.transientParent != null) {
			try {
				Class<?> localClass = this.transientParent.getClass();
				Class<?>[] arrayOfClass = { Class.forName("acm.graphics.GObject") };
				Object[] arrayOfObject = { this };
				Method localMethod = localClass.getMethod("sendToFront", arrayOfClass);
				if (localMethod != null) {
					localMethod.invoke(this.transientParent, arrayOfObject);
				}
			} catch (Exception localException) {
			}
		}
		if (this.mouseListenersEnabled) {
			updateEnabledList();
		}
	}

	public void sendToBack() {
		if (this.compoundParent != null) {
			this.compoundParent.sendToBack(this);
		} else if ((this.transientParent instanceof GCanvas)) {
			((GCanvas) this.transientParent).sendToBack(this);
		} else if (this.transientParent != null) {
			try {
				Class<?> localClass = this.transientParent.getClass();
				Class<?>[] arrayOfClass = { Class.forName("acm.graphics.GObject") };
				Object[] arrayOfObject = { this };
				Method localMethod = localClass.getMethod("sendToBack", arrayOfClass);
				if (localMethod != null) {
					localMethod.invoke(this.transientParent, arrayOfObject);
				}
			} catch (Exception localException) {
			}
		}
		if (this.mouseListenersEnabled) {
			updateEnabledList();
		}
	}

	public void sendForward() {
		if (this.compoundParent != null) {
			this.compoundParent.sendForward(this);
		} else if ((this.transientParent instanceof GCanvas)) {
			((GCanvas) this.transientParent).sendForward(this);
		} else if (this.transientParent != null) {
			try {
				Class<?> localClass = this.transientParent.getClass();
				Class<?>[] arrayOfClass = { Class.forName("acm.graphics.GObject") };
				Object[] arrayOfObject = { this };
				Method localMethod = localClass.getMethod("sendForward", arrayOfClass);
				if (localMethod != null) {
					localMethod.invoke(this.transientParent, arrayOfObject);
				}
			} catch (Exception localException) {
			}
		}
		if (this.mouseListenersEnabled) {
			updateEnabledList();
		}
	}

	public void sendBackward() {
		if (this.compoundParent != null) {
			this.compoundParent.sendBackward(this);
		} else if ((this.transientParent instanceof GCanvas)) {
			((GCanvas) this.transientParent).sendBackward(this);
		} else if (this.transientParent != null) {
			try {
				Class<?> localClass = this.transientParent.getClass();
				Class<?>[] arrayOfClass = { Class.forName("acm.graphics.GObject") };
				Object[] arrayOfObject = { this };
				Method localMethod = localClass.getMethod("sendBackward", arrayOfClass);
				if (localMethod != null) {
					localMethod.invoke(this.transientParent, arrayOfObject);
				}
			} catch (Exception localException) {
			}
		}
		if (this.mouseListenersEnabled) {
			updateEnabledList();
		}
	}

	public void setColor(Color paramColor) {
		this.objectColor = paramColor;
		repaint();
	}

	public Color getColor() {
		GObject localGObject = this;
		while (localGObject.objectColor == null) {
			GContainer localGContainer = localGObject.getParent();
			if ((localGContainer instanceof GObject)) {
				localGObject = (GObject) localGContainer;
			} else {
				if ((localGContainer instanceof Component)) {
					return ((Component) localGContainer).getForeground();
				}
				return Color.BLACK;
			}
		}
		return localGObject.objectColor;
	}

	public void setLineWidth(double paramDouble) {
		this.lineWidth = paramDouble;
		repaint();
	}

	public double getLineWidth() {
		return this.lineWidth;
	}

	public void rotate(double paramDouble) {
		if (this.matrix == null) {
			this.matrix = new AffineTransform();
		}
		this.matrix.rotate(-GMath.toRadians(paramDouble));
		repaint();
	}

	public void scale(double paramDouble1, double paramDouble2) {
		if (this.matrix == null) {
			this.matrix = new AffineTransform();
		}
		this.matrix.scale(paramDouble1, paramDouble2);
		repaint();
	}

	public final void scale(double paramDouble) {
		scale(paramDouble, paramDouble);
	}

	public void setVisible(boolean paramBoolean) {
		this.isVisible = paramBoolean;
		repaint();
	}

	public boolean isVisible() {
		return this.isVisible;
	}

	public String toString() {
		String str = getClass().getName();
		if (str.startsWith("acm.graphics.")) {
			str = str.substring("acm.graphics.".length());
		}
		return str + "[" + paramString() + "]";
	}

	public GContainer getParent() {
		return this.compoundParent != null ? this.compoundParent : this.transientParent;
	}

	public void pause(double paramDouble) {
		JTFTools.pause(paramDouble);
	}

	public void addMouseListener(MouseListener paramMouseListener) {
		this.mouseListener = AWTEventMulticaster.add(this.mouseListener, paramMouseListener);
		this.mouseListenersEnabled = true;
		updateEnabledList();
	}

	public void removeMouseListener(MouseListener paramMouseListener) {
		this.mouseListener = AWTEventMulticaster.remove(this.mouseListener, paramMouseListener);
	}

	public void addMouseMotionListener(MouseMotionListener paramMouseMotionListener) {
		this.mouseMotionListener = AWTEventMulticaster.add(this.mouseMotionListener,
				paramMouseMotionListener);

		this.mouseListenersEnabled = true;
		updateEnabledList();
	}

	public void removeMouseMotionListener(MouseMotionListener paramMouseMotionListener) {
		this.mouseMotionListener = AWTEventMulticaster.remove(this.mouseMotionListener,
				paramMouseMotionListener);
	}

	public void addActionListener(ActionListener paramActionListener) {
		this.actionListener = AWTEventMulticaster.add(this.actionListener, paramActionListener);
	}

	public void removeActionListener(ActionListener paramActionListener) {
		this.actionListener = AWTEventMulticaster.remove(this.actionListener, paramActionListener);
	}

	public void fireActionEvent(String paramString) {
		fireActionEvent(new ActionEvent(this, 1001, paramString));
	}

	public void fireActionEvent(ActionEvent paramActionEvent) {
		if (this.actionListener != null) {
			this.actionListener.actionPerformed(paramActionEvent);
		}
	}

	public Object clone() {
		try {
			GObject localGObject = (GObject) super.clone();
			localGObject.compoundParent = null;
			localGObject.transientParent = null;
			localGObject.mouseListener = null;
			localGObject.mouseMotionListener = null;
			localGObject.actionListener = null;
			if (localGObject.matrix != null) {
				localGObject.matrix = ((AffineTransform) this.matrix.clone());
			}
			return localGObject;
		} catch (Exception localException) {
			throw new ErrorException("Impossible exception");
		}
	}

	public void setParent(GContainer paramGContainer) {
		if ((paramGContainer instanceof GCompound)) {
			this.compoundParent = ((GCompound) paramGContainer);
		} else {
			this.transientParent = paramGContainer;
		}
	}

	protected void fireMouseListeners(MouseEvent paramMouseEvent) {
		switch (paramMouseEvent.getID()) {
		case 501:
			if (this.mouseListener != null) {
				this.mouseListener.mousePressed(paramMouseEvent);
			}
			break;
		case 502:
			if (this.mouseListener != null) {
				this.mouseListener.mouseReleased(paramMouseEvent);
			}
			break;
		case 500:
			if (this.mouseListener != null) {
				this.mouseListener.mouseClicked(paramMouseEvent);
			}
			break;
		case 505:
			if (this.mouseListener != null) {
				this.mouseListener.mouseExited(paramMouseEvent);
			}
			break;
		case 504:
			if (this.mouseListener != null) {
				this.mouseListener.mouseEntered(paramMouseEvent);
			}
			break;
		case 503:
			if (this.mouseMotionListener != null) {
				this.mouseMotionListener.mouseMoved(paramMouseEvent);
			}
			break;
		case 506:
			if (this.mouseMotionListener != null) {
				this.mouseMotionListener.mouseDragged(paramMouseEvent);
			}
			break;
		}
	}

	protected boolean areMouseListenersEnabled() {
		return this.mouseListenersEnabled;
	}

	protected void start() {
		start(null);
	}

	protected void start(String[] paramArrayOfString) {
		try {
			Class<?> localClass1 = Class.forName("acm.program.GraphicsProgram");
			Class<?> localClass2 = Class.forName("acm.graphics.GObject");
			Class<?>[] arrayOfClass = { localClass2, paramArrayOfString.getClass() };
			Object[] arrayOfObject = { this, paramArrayOfString };
			Method localMethod = localClass1.getMethod("startGraphicsProgram", arrayOfClass);

			localMethod.invoke(null, arrayOfObject);
		} catch (Exception localException) {
			throw new ErrorException(localException);
		}
	}

	protected Color getObjectColor() {
		return this.objectColor;
	}

	protected AffineTransform getMatrix() {
		return this.matrix;
	}

	protected Graphics2D createTransformedGraphics(Graphics paramGraphics) {
		Graphics2D localGraphics2D = (Graphics2D) paramGraphics.create();
		Color localColor = getObjectColor();
		if (localColor != null) {
			localGraphics2D.setColor(localColor);
		}
		localGraphics2D.translate(getX(), getY());
		localGraphics2D.setStroke(new BasicStroke((float) this.lineWidth));
		if (this.matrix != null) {
			localGraphics2D.transform(this.matrix);
		}
		if (antialiasing) {
			localGraphics2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
					RenderingHints.VALUE_ANTIALIAS_ON);
		} else {
			localGraphics2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
					RenderingHints.VALUE_ANTIALIAS_OFF);
			localGraphics2D.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
					RenderingHints.VALUE_TEXT_ANTIALIAS_OFF);
		}
		localGraphics2D.setRenderingHint(RenderingHints.KEY_RENDERING,
				RenderingHints.VALUE_RENDER_SPEED);

		return localGraphics2D;
	}

	protected String paramString() {
		String str = "";
		Object localObject;
		if ((this instanceof GResizable)) {
			localObject = getBounds();
			str = str + "bounds=(" + ((GRectangle) localObject).getX() + ", "
					+ ((GRectangle) localObject).getY() + ", "
					+ ((GRectangle) localObject).getWidth() + ", "
					+ ((GRectangle) localObject).getHeight() + ")";
		} else {
			localObject = getLocation();
			str = str + "location=(" + ((GPoint) localObject).getX() + ", "
					+ ((GPoint) localObject).getY() + ")";
		}
		if (this.objectColor != null) {
			str = str + ", color=" + colorName(this.objectColor);
		}
		if ((this instanceof GFillable)) {
			str = str + ", filled=" + ((GFillable) this).isFilled();
			localObject = ((GFillable) this).getFillColor();
			if ((localObject != null) && (localObject != this.objectColor)) {
				str = str + ", fillColor=" + colorName((Color) localObject);
			}
		}
		return str;
	}
	
	protected static String colorName(Color paramColor) {
		if (paramColor.equals(Color.BLACK)) {
			return "BLACK";
		}
		if (paramColor.equals(Color.BLUE)) {
			return "BLUE";
		}
		if (paramColor.equals(Color.CYAN)) {
			return "CYAN";
		}
		if (paramColor.equals(Color.DARK_GRAY)) {
			return "DARK_GRAY";
		}
		if (paramColor.equals(Color.GRAY)) {
			return "GRAY";
		}
		if (paramColor.equals(Color.GREEN)) {
			return "GREEN";
		}
		if (paramColor.equals(Color.LIGHT_GRAY)) {
			return "LIGHT_GRAY";
		}
		if (paramColor.equals(Color.MAGENTA)) {
			return "MAGENTA";
		}
		if (paramColor.equals(Color.ORANGE)) {
			return "ORANGE";
		}
		if (paramColor.equals(Color.PINK)) {
			return "PINK";
		}
		if (paramColor.equals(Color.RED)) {
			return "RED";
		}
		if (paramColor.equals(Color.WHITE)) {
			return "WHITE";
		}
		if (paramColor.equals(Color.YELLOW)) {
			return "YELLOW";
		}
		int i = paramColor.getRGB() & 0xFFFFFF;
		return "0x" + Integer.toString(i, 16).toUpperCase();
	}

	protected Component getComponent() {
		GContainer localGContainer = getParent();
		while ((localGContainer instanceof GObject)) {
			localGContainer = ((GObject) localGContainer).getParent();
		}
		return (localGContainer instanceof Component) ? (Component) localGContainer : null;
	}

	protected void updateEnabledList() {
		Component localComponent = getComponent();
		if ((localComponent instanceof GCanvas)) {
			((GCanvas) localComponent).updateEnabledList();
		}
	}

	protected void repaint() {
		GContainer localGContainer = getParent();
		while ((localGContainer instanceof GObject)) {
			localGContainer = ((GObject) localGContainer).getParent();
		}
		if ((localGContainer instanceof GCanvas)) {
			((GCanvas) localGContainer).conditionalRepaint();
		}
	}
}
