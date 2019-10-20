/*
 * @(#)GImage.java   1.99.1 08/12/08
 */

// ************************************************************************
// * Copyright (c) 2008 by the Association for Computing Machinery        *
// *                                                                      *
// * The Java Task Force seeks to impose few restrictions on the use of   *
// * these packages so that users have as much freedom as possible to     *
// * use this software in constructive ways and can make the benefits of  *
// * that work available to others.  In view of the legal complexities    *
// * of software development, however, it is essential for the ACM to     *
// * maintain its copyright to guard against attempts by others to        *
// * claim ownership rights.  The full text of the JTF Software License   *
// * is available at the following URL:                                   *
// *                                                                      *
// *          http://www.acm.org/jtf/jtf-software-license.pdf             *
// *                                                                      *
// ************************************************************************

// REVISION HISTORY
//
// -- V2.0 --
// Bug fix 21-Dec-06 (ESR, JTFBug 2006-003)
//   1. Invalidated sizeDetermined in setImage.
//   2. Added repaint call in setImage.
//
// Feature enhancement 11-Jan-07 (ESR)
//   1. Added constructor that takes a pixel array.
//   2. Added a method getPixelArray to return the array.
//   3. Added static methods to extract and combine pixel components.
//
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.
//
// Feature enhancement 25-Nov-08 (ESR)
//   1. Added support for saving image files.

package acm.graphics;

import acm.util.*;
import java.awt.*;
import java.io.File;

/* Class: GImage */
/**
 * The <code>GImage</code> class is a graphical object whose appearance is
 * defined by an image.
 */
public class GImage extends GObject implements GResizable, GScalable {

/* Constructor: GImage(image) */
/**
 * Creates a new <code>GImage</code> object at the origin that displays
 * the specified image.
 *
 * @usage GImage gimage = new GImage(image);
 * @param image The image to use as the contents of this <code>GImage</code>
 */
	public GImage(Image image) {
		this(image, 0, 0);
	}

/* Constructor: GImage(name) */
/**
 * Creates a new <code>GImage</code> object by looking for an image with that
 * name.  The search for an image by name consists of the following steps:
 *
 * <p><ol>
 * <li>Check to see if an image with that name has already been defined.  If
 *     so, return that image.<p>
 *
 * <li>Check to see if there is a resource available with that name whose
 *     contents can be read as an <code>Image</code>.  If so, read the image
 *     from the resource file.<p>
 *
 * <li>Load the image from a file with the specified name, relative to the
 *     application directory or the applet code base.
 * </ol><p>
 *
 * @usage GImage gimage = new GImage(name);
 * @param name The name used to search for the contents of this image
 */
	public GImage(String name) {
		this(name, 0, 0);
	}

/* Constructor: GImage(array) */
/**
 * Creates a new <code>GImage</code> object at the origin.  The <code>array</code>
 * parameter is a two-dimensional pixel array in which each pixel value consists
 * of an integer that is subdivided into four eight-bit bytes, as follows:
 *
 * <center>
 *    <i>alpha</i> <code>&lt;&lt;</code> 24  <code>|</code>
 *    <i>red</i>   <code>&lt;&lt;</code> 16  <code>|</code>
 *    <i>green</i> <code>&lt;&lt;</code>  8  <code>|</code>
 *    <i>blue</i>
 * </center>
 *
 * The <i>alpha</i> value indicates the transparency, and the other values
 * are the red, green, and blue components of the color.
 *
 * @usage GImage gimage = new GImage(array);
 * @param array A two-dimensional pixel array
 */
	public GImage(int[][] array) {
		this(array, 0, 0);
	}

/* Constructor: GImage(image, x, y) */
/**
 * Creates a new <code>GImage</code> object at the specified coordinates.  The
 * <code>image</code> parameter is used to initialize the appearance of the image.
 *
 * @usage GImage gimage = new GImage(image, x, y);
 * @param image The image to use as the contents of this <code>GImage</code>
 * @param x The x-coordinate of the upper left corner of the image
 * @param y The y-coordinate of the upper left corner of the image
 */
	public GImage(Image image, double x, double y) {
		setImage(image);
		setLocation(x, y);
	}

/* Constructor: GImage(name, x, y) */
/**
 * Creates a new <code>GImage</code> object at the specified coordinates.  The
 * <code>name</code> parameter is used to identify an image to display, as
 * described in the single-argument version of the
 * <a href="#GImage(String)"><code>GImage</code></a> constructor.
 *
 * @usage GImage gimage = new GImage(image, x, y);
 * @param name The name used to search for the contents of this image
 * @param x The x-coordinate of the upper left corner of the image
 * @param y The y-coordinate of the upper left corner of the image
 */
	public GImage(String name, double x, double y) {
		this(MediaTools.loadImage(name), x, y);
	}

/* Constructor: GImage(array, x, y) */
/**
 * Creates a new <code>GImage</code> object at the specified coordinates.
 * The <code>array</code> parameter is a two-dimensional pixel array in
 * which each pixel value consists of an integer that is subdivided into
 * four eight-bit bytes, as follows:
 *
 * <center>
 *    <i>alpha</i> <code>&lt;&lt;</code> 24  <code>|</code>
 *    <i>red</i>   <code>&lt;&lt;</code> 16  <code>|</code>
 *    <i>green</i> <code>&lt;&lt;</code>  8  <code>|</code>
 *    <i>blue</i>
 * </center>
 *
 * The <i>alpha</i> value indicates the transparency, and the other values
 * are the red, green, and blue components of the color.
 *
 * @usage GImage gimage = new GImage(array, x, y);
 * @param array A two-dimensional pixel array
 * @param x The x-coordinate of the upper left corner of the image
 * @param y The y-coordinate of the upper left corner of the image
 */
	public GImage(int[][] array, double x, double y) {
		this(MediaTools.createImage(array), x, y);
	}

/* Method: setImage(image) */
/**
 * Resets the image used by this <code>GImage</code> object to the new image
 * specified as an argument.  Calling <code>setImage</code> automatically changes
 * the size of the image to be equal to that of the image data.
 *
 * @usage gimage.setImage(image);
 * @param image The image to use as the contents of this <code>GImage</code>
 */
	public void setImage(Image image) {
		myImage = MediaTools.loadImage(image);
		sizeDetermined = false;
		determineSize();
		repaint();
	}

/* Method: setImage(name) */
/**
 * Resets the image used by this <code>GImage</code> object to the one identified
 * by the argument <code>name</code>, which is processed exactly as described
 * in the constructors.  Calling <code>setImage</code> automatically changes
 * the size of the image to be equal to that of the image data.
 *
 * @usage gimage.setImage(name);
 * @param name The name used to search for the contents of this image
 */
	public void setImage(String name) {
		setImage(MediaTools.loadImage(name));
	}

/* Method: getImage() */
/**
 * Returns the image stored inside this <code>GImage</code>.
 *
 * @usage Image image = gimage.getImage();
 * @return The <code>Image</code> object stored inside this <code>GImage</code>
 */
	public Image getImage() {
		return myImage;
	}

/* Method: saveImage(filename) */
/**
 * Saves the image to a file with the specified filename.  The data format
 * for the image file is determined by the suffix of the filename.  If the
 * suffix of the file is not recognized as a supported image type, calling
 * this method generates an error.
 *
 * @usage gimage.saveImage(filename);
 * @param filename The name of the file to which the image is saved
 */
	public void saveImage(String filename) {
		MediaTools.saveImage(myImage, filename);
	}

/* Method: saveImage(file) */
/**
 * Saves the image to the specified file.  The data format for the
 * image file is determined by the suffix of the filename.  If the
 * suffix of the file is not recognized as a supported image type,
 * calling this method generates an error.
 *
 * @usage gimage.saveImage(file);
 * @param file The <code>File</code> to which the image is saved
 */
	public void saveImage(File file) {
		MediaTools.saveImage(myImage, file);
	}

/* Method: paint(g) */
/**
 * Implements the <code>paint</code> operation for this graphical object.  This method
 * is not called directly by clients.
 * @noshow
 */
	public void paint(Graphics g) {
		Component imageObserver = getComponent();
		if (imageObserver == null) imageObserver = MediaTools.getImageObserver();
		if (myImage != null && imageObserver != null) {
			Rectangle r = getAWTBounds();
			Color color = getObjectColor();
			if (color == null) {
				g.drawImage(myImage, r.x, r.y, r.width, r.height, imageObserver);
			} else {
				g.drawImage(myImage, r.x, r.y, r.width, r.height, color, imageObserver);
			}
		}
	}

/* Method: setSize(width, height) */
/**
 * Changes the size of this object to the specified width and height.
 *
 * @usage gimage.setSize(width, height);
 * @param width The new width of the object
 * @param height The new height of the object
 */
	public void setSize(double width, double height) {
		myWidth = width;
		myHeight = height;
		repaint();
	}

/* Method: setSize(size) */
/**
 * Changes the size of this object to the specified <code>GDimension</code>.
 *
 * @usage gimage.setSize(size);
 * @param size A <code>GDimension</code> object specifying the size
 * @noshow
 */
	public final void setSize(GDimension size) {
		setSize(size.getWidth(), size.getHeight());
	}

/* Method: getSize() */
/**
 * Returns the size of this object as a <code>GDimension</code>.
 *
 * @usage GDimension size = gimage.getSize();
 * @return The size of this object
 */
	public GDimension getSize() {
		return new GDimension(myWidth, myHeight);
	}

/* Method: setBounds(x, y, width, height) */
/**
 * Changes the bounds of this object to the specified values.
 *
 * @usage gimage.setBounds(x, y, width, height);
 * @param x The new x-coordinate for the object
 * @param y The new y-coordinate for the object
 * @param width The new width of the object
 * @param height The new height of the object
 */
	public void setBounds(double x, double y, double width, double height) {
		myWidth = width;
		myHeight = height;
		setLocation(x, y);
	}

/* Method: setBounds(bounds) */
/**
 * Changes the bounds of this object to the values from the specified
 * <code>GRectangle</code>.
 *
 * @usage gimage.setBounds(bounds);
 * @param bounds A <code>GRectangle</code> specifying the new bounds
 */
	public final void setBounds(GRectangle bounds) {
		setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
	}

/* Method: getBounds() */
/**
 * Returns the bounding box of this object.
 *
 * @usage GRectangle bounds = gimage.getBounds();
 * @return The bounding box for this object
 */
	public GRectangle getBounds() {
		determineSize();
		return new GRectangle(getX(), getY(), myWidth, myHeight);
	}

/* Method: scale(sx, sy) */
/**
 * Scales the object on the screen by the scale factors <code>sx</code> and <code>sy</code>.
 *
 * @usage gobj.scale(sx, sy);
 * @param sx The factor used to scale all coordinates in the x direction
 * @param sy The factor used to scale all coordinates in the y direction
 */
	public void scale(double sx, double sy) {
		myWidth *= sx;
		myHeight *= sy;
		repaint();
	}

/* Method: scale(sf) */
/**
 * Scales the object on the screen by the scale factor <code>sf</code>, which applies
 * in both dimensions.
 *
 * @usage gobj.scale(sf);
 * @param sf The factor used to scale all coordinates in both dimensions
 */
	public final void scale(double sf) {
		scale(sf, sf);
	}

/* Method: getPixelArray() */
/**
 * Returns a two-dimensional array of pixel values from the stored image.
 *
 * @usage int[][] array = gimage.getPixelArray();
 * @return A two-dimensional array of pixel values from the stored image
 */
	public int[][] getPixelArray() {
		return MediaTools.getPixelArray(myImage);
	}

/* Static method: getAlpha(pixel) */
/**
 * Returns the alpha component from an RGB value.
 *
 * @usage int alpha = GImage.getAlpha(pixel);
 * @param pixel An <code>int</code> containing a pixel value as alpha/red/green/blue.
 * @return The alpha component of the pixel
 */
	public static int getAlpha(int pixel) {
		return (pixel >> 24) & 0xFF;
	}

/* Static method: getRed(pixel) */
/**
 * Returns the red component from an RGB value.
 *
 * @usage int red = GImage.getRed(pixel);
 * @param pixel An <code>int</code> containing a pixel value as alpha/red/green/blue.
 * @return The red component of the pixel
 */
	public static int getRed(int pixel) {
		return (pixel >> 16) & 0xFF;
	}

/* Static method: getGreen(pixel) */
/**
 * Returns the green component from an RGB value.
 *
 * @usage int green = GImage.getGreen(pixel);
 * @param pixel An <code>int</code> containing a pixel value as alpha/red/green/blue.
 * @return The green component of the pixel
 */
	public static int getGreen(int pixel) {
		return (pixel >> 8) & 0xFF;
	}

/* Static method: getBlue(pixel) */
/**
 * Returns the blue component from an RGB value.
 *
 * @usage int blue = GImage.getBlue(pixel);
 * @param pixel An <code>int</code> containing a pixel value as alpha/red/green/blue.
 * @return The blue component of the pixel
 */
	public static int getBlue(int pixel) {
		return pixel & 0xFF;
	}

/* Static method: createRGBPixel(red, green, blue) */
/**
 * Creates an opaque pixel value with the color components given by
 * <code>red</code>, <code>green</code>, and <code>blue</code>.
 *
 * @usage int pixel = GImage.createRGBPixel(red, green, blue);
 * @param red The red component of the pixel (0 to 255)
 * @param green The green component of the pixel (0 to 255)
 * @param blue The blue component of the pixel (0 to 255)
 * @return An opaque pixel value containing these components
 */
	public static int createRGBPixel(int red, int green, int blue) {
		return createRGBPixel(red, green, blue, 0xFF);
	}

/* Static method: createRGBPixel(red, green, blue, alpha) */
/**
 * Creates a pixel value with the color components given by
 * <code>red</code>, <code>green</code>, and <code>blue</code>
 * and the transparency value <code>alpha</code>.
 *
 * @usage int pixel = GImage.createRGBPixel(red, green, blue);
 * @param red The red component of the pixel (0 to 255)
 * @param green The green component of the pixel (0 to 255)
 * @param blue The blue component of the pixel (0 to 255)
 * @param alpha The transparency value of the pixel (0 to 255)
 * @return A pixel value containing these components
 */
	public static int createRGBPixel(int red, int green, int blue, int alpha) {
		return (alpha << 24) | (red << 16) | (green << 8) | blue;
	}

/* Inherited method: setLocation(x, y) */
/**
 * @inherited GObject#void setLocation(double x, double y)
 * Sets the location of this object to the point (<code>x</code>, <code>y</code>).
 */

/* Inherited method: setLocation(pt) */
/**
 * @inherited GObject#void setLocation(GPoint pt)
 * Sets the location of this object to the specified point.
 */

/* Inherited method: getLocation() */
/**
 * @inherited GObject#GPoint getLocation()
 * Returns the location of this object as a <code>GPoint</code>.
 */

/* Inherited method: getX() */
/**
 * @inherited GObject#double getX()
 * Returns the x-coordinate of the object.
 */

/* Inherited method: getY() */
/**
 * @inherited GObject#double getY()
 * Returns the y-coordinate of the object.
 */

/* Inherited method: getWidth() */
/**
 * @inherited GObject#double getWidth()
 * Returns the width of this object as a double-precision value, which
 * is defined to be the width of the bounding box.
 */

/* Inherited method: getHeight() */
/**
 * @inherited GObject#double getHeight()
 * Returns the height of this object as a double-precision value, which
 * is defined to be the height of the bounding box.
 */

/* Inherited method: move(dx, dy) */
/**
 * @inherited GObject#void move(double dx, double dy)
 * Moves the object on the screen using the displacements <code>dx</code> and <code>dy</code>.
 */

/* Inherited method: movePolar(r, theta) */
/**
 * @inherited GObject#void movePolar(double r, double theta)
 * Moves the object using displacements given in polar coordinates.
 */

/* Inherited method: contains(x, y) */
/**
 * @inherited GObject#boolean contains(double x, double y)
 * Checks to see whether a point is inside the object.
 */

/* Inherited method: contains(pt) */
/**
 * @inherited GObject#boolean contains(GPoint pt)
 * Checks to see whether a point is inside the object.
 */

/* Inherited method: sendToFront() */
/**
 * @inherited GObject#void sendToFront()
 * Moves this object to the front of the display in the <i>z</i> dimension.
 */

/* Inherited method: sendToBack() */
/**
 * @inherited GObject#void sendToBack()
 * Moves this object to the back of the display in the <i>z</i> dimension.
 */

/* Inherited method: sendForward() */
/**
 * @inherited GObject#void sendForward()
 * Moves this object one step toward the front in the <i>z</i> dimension.
 */

/* Inherited method: sendBackward() */
/**
 * @inherited GObject#void sendBackward()
 * Moves this object one step toward the back in the <i>z</i> dimension.
 */

/* Inherited method: setVisible(visible) */
/**
 * @inherited GObject#void setVisible(boolean visible)
 * Sets whether this object is visible.
 */

/* Inherited method: isVisible() */
/**
 * @inherited GObject#boolean isVisible()
 * Checks to see whether this object is visible.
 */

/* Inherited method: addMouseListener(listener) */
/**
 * @inherited GObject#void addMouseListener(MouseListener listener)
 * Adds a mouse listener to this graphical object.
 */

/* Inherited method: removeMouseListener(listener) */
/**
 * @inherited GObject#void removeMouseListener(MouseListener listener)
 * Removes a mouse listener from this graphical object.
 */

/* Inherited method: addMouseMotionListener(listener) */
/**
 * @inherited GObject#void addMouseMotionListener(MouseMotionListener listener)
 * Adds a mouse motion listener to this graphical object.
 */

/* Inherited method: removeMouseMotionListener(listener) */
/**
 * @inherited GObject#void removeMouseMotionListener(MouseMotionListener listener)
 * Removes a mouse motion listener from this graphical object.
 */

/* Protected method: getAWTBounds() */
/**
 * Returns an AWT <code>Rectangle</code> that specifies the bounds of this object.
 *
 * @usage Rectangle r = grect.getAWTBounds();
 * @return A <code>Rectangle</code> that specifies the bounds of this object
 */
	protected Rectangle getAWTBounds() {
		determineSize();
		return new Rectangle(GMath.round(getX()), GMath.round(getY()), GMath.round(myWidth), GMath.round(myHeight));
	}

/* Private method: determineSize() */
/**
 * Computes the size of the image.
 */
	private void determineSize() {
		if (sizeDetermined) return;
		Component component = getComponent();
		if (component == null) component = MediaTools.getImageObserver();
		myWidth = myImage.getWidth(component);
		myHeight = myImage.getHeight(component);
		sizeDetermined = true;
	}

/* Private instance variables */
	private Image myImage;
	private double myWidth;
	private double myHeight;
	private boolean sizeDetermined;

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}
