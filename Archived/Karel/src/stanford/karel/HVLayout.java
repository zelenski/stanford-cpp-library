/*
 * This file implements a layout manager that simplifies the
 * task of creating component assemblies.  In general, clients
 * will create HPanels and VPanels, which use an HVLayout
 * of the appropriate direction.
 *
 * To use a stack panel, the first step is to create the
 * appropriate HPanel or VPanel.  For example, you can
 * create a horizontal assemblage like this:
 *
 *      HPanel hbox = new HPanel();
 *
 * From there, you simply add components to the panel in a
 * left-to-right order (top-to-bottom for VPanels).  The
 * call to add a component looks like this:
 *
 *      hbox.add(constraints, component);
 *
 * where constraints is a string consisting of a series of
 * options.  Each option has one of the following two forms:
 *
 *     /option
 *     /option:value
 *
 * The available options and their interpretations are:
 *
 * /stretch:xxx  The /stretch option indicates that this
 *               component should stretch to fill its grid cell
 *               if the enclosing panel stretches.  The available
 *               option values are both, horizontal, vertical,
 *               and none.  If the /stretch option does not
 *               appear (or /stretch:none is used), the component
 *               will maintain its size.  If /stretch appears
 *               without an option value, it is interpreted as
 *               /stretch:both.
 *
 * /fill         The /fill option is very much like stretch, but
 *               causes the component to stretch only in the
 *               panel direction.  Thus, /fill is the same as
 *               /stretch:horizontal for HPanels and the same as
 *               /stretch:vertical for VPanels.
 *
 * /anchor:xxx   The /anchor option applies to components that do
 *               not stretch and specifies how those components
 *               should be oriented with respect to the space
 *               available for that component.  The /anchor option
 *               in the first component determines the anchoring
 *               for the entire panel in the dimension of expansion;
 *               in the opposite dimension, each component is anchored
 *               individually.  The legal option values are the names
 *               of the compass points and center.  If no /anchor
 *               option appears, the component is centered.  If
 *               /anchor appears with no argument, it is taken to
 *               be /anchor:northwest.  The /anchor option values
 *               can also be used as complete options, so that
 *               /south is a shorthand for /anchor:south.
 *
 * /space:n      The /space:n option adds n pixels of space before
 *               the component (i.e., to the left of components in
 *               an HPanel and above components in a VPanel).
 *
 * /top:n        These options add a margin of n pixels in the
 * /bottom:n     indicated direction from the component.  You may
 * /left:n       specify as many of these options as you want; if
 * /right:n      no value appears for a particular direction, no
 *               extra space is allocated.
 *
 * /width:n      These options specify the preferred width and height
 * /height:n     of their component.
 *
 * The option string is case-insensitive.  Numeric values can easily
 * be inserted into the string using Java's concatenation operator.
 * For example, if you want to add myButton to the HPanel hbox and
 * specify that it be BUTTON_WIDTH pixels wide, you could write
 *
 *          hbox.add("/width:" + BUTTON_WIDTH, myButton);
 * - 2015/03/31: Changed to use Swing graphical components.
 */

package stanford.karel;

import java.awt.*;
import java.util.*;

class HVLayout implements LayoutManager {
	/* Constants */
	public static final int DEFAULT_SPACE = 5;
	public static final int HORIZONTAL = GridBagConstraints.HORIZONTAL;
	public static final int VERTICAL = GridBagConstraints.VERTICAL;

	/* Private constants */
	private static final int MINIMUM = 0;
	private static final int PREFERRED = 1;
	private static final int CENTER = GridBagConstraints.CENTER;
	private static final int NORTH = GridBagConstraints.NORTH;
	private static final int NORTHEAST = GridBagConstraints.NORTHEAST;
	private static final int EAST = GridBagConstraints.EAST;
	private static final int SOUTHEAST = GridBagConstraints.SOUTHEAST;
	private static final int SOUTH = GridBagConstraints.SOUTH;
	private static final int SOUTHWEST = GridBagConstraints.SOUTHWEST;
	private static final int WEST = GridBagConstraints.WEST;
	private static final int NORTHWEST = GridBagConstraints.NORTHWEST;
	private static final int NONE = GridBagConstraints.NONE;
	private static final int BOTH = GridBagConstraints.BOTH;

	/* Private variables */
	private int orientation;
	private HashMap<Component, OptionTable> constraintTable;

	/*
	 * This method creates a new HVLayout object that grows along the axis
	 * specified by orientation, which must be HORIZONTAL or VERTICAL.
	 */
	public HVLayout(int orientation) {
		this.orientation = orientation;
		constraintTable = new HashMap<Component, OptionTable>();
	}

	/* LayoutManager interface */
	/*
	 * This method adds the specified component to the layout in the position
	 * specified by name.
	 */
	public void addLayoutComponent(String constraints, Component comp) {
		synchronized (comp.getTreeLock()) {
			constraintTable.put(comp,
					new OptionTable(constraints.toLowerCase()));
		}
	}

	/*
	 * This method removes the specified component from the layour.
	 */
	public void removeLayoutComponent(Component comp) {
		constraintTable.remove(comp);
	}

	/*
	 * This method calculates the preferred layout size for the components
	 * contained in the parent.
	 */
	public Dimension preferredLayoutSize(Container parent) {
		synchronized (parent.getTreeLock()) {
			return getContainerSize(parent, PREFERRED);
		}
	}

	/*
	 * This method calculates the minimum layout size for the components
	 * contained in the parent.
	 */
	public Dimension minimumLayoutSize(Container parent) {
		synchronized (parent.getTreeLock()) {
			return getContainerSize(parent, MINIMUM);
		}
	}

	/*
	 * This method performs the actual layout operations, resizing and
	 * repositioning the components in the specified parent.
	 */
	public void layoutContainer(Container parent) {
		synchronized (parent.getTreeLock()) {
			int nComponents = parent.getComponentCount();
			Dimension psize = parent.getSize();
			Dimension tsize = preferredLayoutSize(parent);
			int nStretch = getStretchCount(parent);
			int extra = getExtraSpace(psize, tsize, nStretch);
			Point origin = new Point(0, 0);
			if (nStretch == 0)
				origin = getInitialOrigin(parent, psize, tsize);
			for (int i = 0; i < nComponents; i++) {
				Component comp = parent.getComponent(i);
				OptionTable options = constraintTable.get(comp);
				Dimension csize = getLayoutSize(comp, options, PREFERRED);
				Dimension lsize = applyStretching(getStretchOption(options),
						csize, psize, extra);
				Dimension vsize = applyStretching(BOTH, csize, psize, extra);
				Rectangle bounds = getLayoutBounds(options, lsize, vsize,
						origin);
				comp.setBounds(bounds);
				if (orientation == HORIZONTAL) {
					origin.x += lsize.width;
				} else {
					origin.y += lsize.height;
				}
			}
		}
	}

	/*
	 * This method unifies the calculation of the minimum and preferred sizes
	 * for the parent. The type parameter must be one of the constants MINIMUM
	 * and PREFERRED. This method assumes that the tree lock has been obtained
	 * by the caller.
	 */
	private Dimension getContainerSize(Container parent, int type) {
		Dimension result = new Dimension(0, 0);
		int nComponents = parent.getComponentCount();
		for (int i = 0; i < nComponents; i++) {
			Component comp = parent.getComponent(i);
			OptionTable options = constraintTable.get(comp);
			Dimension size = getLayoutSize(comp, options, type);
			if (orientation == HORIZONTAL) {
				result.width += size.width;
				result.height = Math.max(result.height, size.height);
			} else {
				result.width = Math.max(result.width, size.width);
				result.height += size.height;
			}
		}
		return result;
	}

	/*
	 * This method computes the minimum or preferred size of the specified
	 * component according to the constraint specifications for the layout and
	 * any size information set by the component class. The type parameter must
	 * be one of the constants MINIMUM and PREFERRED.
	 */
	private Dimension getLayoutSize(Component comp, OptionTable options,
			int type) {
		Dimension size = new Dimension(0, 0);
		;
		if (type == PREFERRED)
			size = new Dimension(comp.getPreferredSize());
		size.width = options.getIntOption("width", size.width);
		size.height = options.getIntOption("height", size.height);
		size = limitSize(size, comp);
		Insets insets = getInsetOption(options);
		size.width += insets.left + insets.right;
		size.height += insets.top + insets.bottom;
		return size;
	}

	/*
	 * This method returns a new Dimension value that is within the limits set
	 * by the minimum and maximum sizes for the component.
	 */
	private Dimension limitSize(Dimension size, Component comp) {
		Dimension minSize = comp.getMinimumSize();
		Dimension maxSize = comp.getMaximumSize();
		int width = Math
				.max(minSize.width, Math.min(size.width, maxSize.width));
		int height = Math.max(minSize.height,
				Math.min(size.height, maxSize.height));
		return new Dimension(width, height);
	}

	/*
	 * This method returns the Insets object specified by the options table,
	 * which comes from the /top, /bottom, /left, /right, and /space options.
	 */
	private Insets getInsetOption(OptionTable options) {
		Insets insets = new Insets(0, 0, 0, 0);
		if (options.isSpecified("space")) {
			switch (orientation) {
			case HORIZONTAL:
				insets.left = options.getIntOption("space");
				break;
			case VERTICAL:
				insets.top = options.getIntOption("space");
				break;
			}
		}
		if (options.isSpecified("left")) {
			insets.left = options.getIntOption("left");
		}
		if (options.isSpecified("right")) {
			insets.right = options.getIntOption("right");
		}
		if (options.isSpecified("top")) {
			insets.top = options.getIntOption("top");
		}
		if (options.isSpecified("bottom")) {
			insets.bottom = options.getIntOption("bottom");
		}
		return insets;
	}

	/*
	 * This method returns the stretching style specified by the options table,
	 * which comes from the /stretch and /fill options.
	 */
	private int getStretchOption(OptionTable options) {
		if (options.isSpecified("fill"))
			return orientation;
		if (options.isSpecified("stretch")) {
			String value = options.getOption("stretch", "both");
			if (value.equals("none"))
				return NONE;
			if (value.equals("horizontal"))
				return HORIZONTAL;
			if (value.equals("vertical"))
				return VERTICAL;
			if (value.equals("both"))
				return BOTH;
		}
		return NONE;
	}

	/*
	 * This method returns the anchoring style specified by the options table,
	 * which comes from the /anchor option, if specified, or from any of the
	 * anchoring constants.
	 */
	private int getAnchorOption(OptionTable options) {
		int anchor = CENTER;
		if (options.isSpecified("anchor")) {
			String value = options.getOption("anchor", "northwest");
			if (value.equals("center")) {
				anchor = CENTER;
			} else if (value.equals("north")) {
				anchor = NORTH;
			} else if (value.equals("northeast") || value.equals("ne")) {
				anchor = NORTHEAST;
			} else if (value.equals("east")) {
				anchor = EAST;
			} else if (value.equals("southeast") || value.equals("se")) {
				anchor = SOUTHEAST;
			} else if (value.equals("south")) {
				anchor = SOUTH;
			} else if (value.equals("southwest") || value.equals("sw")) {
				anchor = SOUTHWEST;
			} else if (value.equals("west")) {
				anchor = WEST;
			} else if (value.equals("northwest") || value.equals("nw")) {
				anchor = NORTHWEST;
			}
		} else {
			if (options.isSpecified("center")) {
				anchor = CENTER;
			} else if (options.isSpecified("north")) {
				anchor = NORTH;
			} else if (options.isSpecified("northeast")
					|| options.isSpecified("ne")) {
				anchor = NORTHEAST;
			} else if (options.isSpecified("east")) {
				anchor = EAST;
			} else if (options.isSpecified("southeast")
					|| options.isSpecified("se")) {
				anchor = SOUTHEAST;
			} else if (options.isSpecified("south")) {
				anchor = SOUTH;
			} else if (options.isSpecified("southwest")
					|| options.isSpecified("sw")) {
				anchor = SOUTHWEST;
			} else if (options.isSpecified("west")) {
				anchor = WEST;
			} else if (options.isSpecified("northwest")
					|| options.isSpecified("nw")) {
				anchor = NORTHWEST;
			}
		}
		return anchor;
	}

	/*
	 * This method counts the number of stretchable components in the parent,
	 * considering only the axis along which new components are added.
	 */
	private int getStretchCount(Container parent) {
		int nComponents = parent.getComponentCount();
		int nStretch = 0;
		for (int i = 0; i < nComponents; i++) {
			Component comp = parent.getComponent(i);
			OptionTable options = constraintTable.get(comp);
			int stretch = getStretchOption(options);
			if (orientation == HORIZONTAL) {
				if (stretch == HORIZONTAL || stretch == BOTH)
					nStretch++;
			} else {
				if (stretch == VERTICAL || stretch == BOTH)
					nStretch++;
			}
		}
		return nStretch;
	}

	/*
	 * This method returns the amount of extra space that must be added to each
	 * stretchable component along the main axis. The psize parameter gives the
	 * size of the parent, tsize is the total size of the components, and
	 * nStretch is the number of components that stretch.
	 */
	private int getExtraSpace(Dimension psize, Dimension tsize, int nStretch) {
		if (nStretch == 0) {
			return 0;
		} else {
			if (orientation == HORIZONTAL) {
				return (psize.width - tsize.width) / nStretch;
			} else {
				return (psize.height - tsize.height) / nStretch;
			}
		}
	}

	/*
	 * This method computes the dimensions of the layout box to account for the
	 * stretching of this component to fit into the available space.
	 */
	private Dimension applyStretching(int stretch, Dimension csize,
			Dimension psize, int extra) {
		int width = csize.width;
		int height = csize.height;
		if (stretch == HORIZONTAL || stretch == BOTH) {
			if (orientation == HORIZONTAL) {
				width += extra;
			} else {
				width = psize.width;
			}
		}
		if (stretch == VERTICAL || stretch == BOTH) {
			if (orientation == VERTICAL) {
				height += extra;
			} else {
				height = psize.height;
			}
		}
		return new Dimension(width, height);
	}

	/*
	 * This method returns the appropriate bounding box for the component that
	 * appears in the specified layout area beginning at the specified origin.
	 */
	private Rectangle getLayoutBounds(OptionTable options, Dimension lsize,
			Dimension vsize, Point origin) {
		Insets insets = getInsetOption(options);
		int anchor = getAnchorOption(options);
		int dx = insets.left;
		int dy = insets.top;
		int width = lsize.width - insets.left - insets.right;
		int height = lsize.height - insets.top - insets.bottom;
		if (orientation == HORIZONTAL) {
			switch (anchor) {
			case NORTH:
			case NORTHWEST:
			case NORTHEAST:
				dy = insets.top;
				break;
			case CENTER:
			case WEST:
			case EAST:
				dy = insets.top + (vsize.height - lsize.height) / 2;
				break;
			case SOUTH:
			case SOUTHWEST:
			case SOUTHEAST:
				dy = vsize.height - insets.bottom - lsize.height;
				break;
			}
		} else {
			switch (anchor) {
			case WEST:
			case NORTHWEST:
			case SOUTHWEST:
				dx = insets.left;
				break;
			case CENTER:
			case NORTH:
			case SOUTH:
				dx = insets.left + (vsize.width - lsize.width) / 2;
				break;
			case EAST:
			case NORTHEAST:
			case SOUTHEAST:
				dx = vsize.width - insets.right - lsize.width;
				break;
			}
		}
		return new Rectangle(origin.x + dx, origin.y + dy, width, height);
	}

	/*
	 * This method returns the initial origin at which the layout begins and is
	 * called only if no components are stretchable along the main axis. The
	 * anchor property of the first component is used to determine the
	 * alignment.
	 */
	private Point getInitialOrigin(Container parent, Dimension psize,
			Dimension tsize) {
		int x = 0;
		int y = 0;
		if (parent.getComponentCount() > 0) {
			OptionTable options = constraintTable.get(parent.getComponent(0));
			int anchor = getAnchorOption(options);
			if (orientation == HORIZONTAL) {
				switch (anchor) {
				case WEST:
				case NORTHWEST:
				case SOUTHWEST:
					x = 0;
					break;
				case CENTER:
				case NORTH:
				case SOUTH:
					x = (psize.width - tsize.width) / 2;
					break;
				case EAST:
				case NORTHEAST:
				case SOUTHEAST:
					x = psize.width - tsize.width;
					break;
				}
			} else {
				switch (anchor) {
				case NORTH:
				case NORTHWEST:
				case NORTHEAST:
					y = 0;
					break;
				case CENTER:
				case WEST:
				case EAST:
					y = (psize.height - tsize.height) / 2;
					break;
				case SOUTH:
				case SOUTHWEST:
				case SOUTHEAST:
					y = psize.height - tsize.height;
					break;
				}
			}
		}
		return new Point(x, y);
	}

	/*
	 * This method returns a readable name for the anchor constant.
	 */
	public String anchorName(int anchor) {
		switch (anchor) {
		case CENTER:
			return "CENTER";
		case NORTH:
			return "NORTH";
		case NORTHEAST:
			return "NORTHEAST";
		case EAST:
			return "EAST";
		case SOUTHEAST:
			return "SOUTHEAST";
		case SOUTH:
			return "SOUTH";
		case SOUTHWEST:
			return "SOUTHWEST";
		case WEST:
			return "WEST";
		case NORTHWEST:
			return "NORTHWEST";
		default:
			return "undefined";
		}
	}
}
