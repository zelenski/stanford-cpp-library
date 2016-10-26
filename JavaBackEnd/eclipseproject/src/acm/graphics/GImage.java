/*
 * @version: 2016/05/05
 * - resynched with eroberts source
 * - sort methods in ABC order
 * @version: 2015/05/18
 * - added inBounds() method
 * @version: 2015/04/23
 * - added fit(w, h) method for scaling while maintaining aspect ratio.
 */

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

/**
 * The <code>GImage</code> class is a graphical object whose appearance is
 * defined by an image.
 */
public class GImage extends GObject implements GResizable, GScalable {
	/**
	 * The serialization code for this class.  This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */
	static final long serialVersionUID = 1L;

	/* Private instance variables */
	private Image myImage;
	private double myWidth;
	private double myHeight;
	private boolean sizeDetermined;

	/**
	 * Creates a new <code>GImage</code> object at the origin that displays
	 * the specified image.
	 *
	 * @param image The image to use as the contents of this <code>GImage</code>
	 * @usage GImage gimage = new GImage(image);
	 */
	public GImage(Image image) {
		this(image, 0, 0);
	}

	/**
	 * Creates a new <code>GImage</code> object by looking for an image with that
	 * name.  The search for an image by name consists of the following steps:
	 * <p>
	 * <p><ol>
	 * <li>Check to see if an image with that name has already been defined.  If
	 * so, return that image.<p>
	 * <p>
	 * <li>Check to see if there is a resource available with that name whose
	 * contents can be read as an <code>Image</code>.  If so, read the image
	 * from the resource file.<p>
	 * <p>
	 * <li>Load the image from a file with the specified name, relative to the
	 * application directory or the applet code base.
	 * </ol><p>
	 *
	 * @param name The name used to search for the contents of this image
	 * @usage GImage gimage = new GImage(name);
	 */
	public GImage(String name) {
		this(name, 0, 0);
	}

/* Constructor: GImage(array) */

	/**
	 * Creates a new <code>GImage</code> object at the origin.  The <code>array</code>
	 * parameter is a two-dimensional pixel array in which each pixel value consists
	 * of an integer that is subdivided into four eight-bit bytes, as follows:
	 * <p>
	 * <center>
	 * <i>alpha</i> <code>&lt;&lt;</code> 24  <code>|</code>
	 * <i>red</i>   <code>&lt;&lt;</code> 16  <code>|</code>
	 * <i>green</i> <code>&lt;&lt;</code>  8  <code>|</code>
	 * <i>blue</i>
	 * </center>
	 * <p>
	 * The <i>alpha</i> value indicates the transparency, and the other values
	 * are the red, green, and blue components of the color.
	 *
	 * @param array A two-dimensional pixel array
	 * @usage GImage gimage = new GImage(array);
	 */
	public GImage(int[][] array) {
		this(array, 0, 0);
	}

/* Constructor: GImage(image, x, y) */

	/**
	 * Creates a new <code>GImage</code> object at the specified coordinates.  The
	 * <code>image</code> parameter is used to initialize the appearance of the image.
	 *
	 * @param image The image to use as the contents of this <code>GImage</code>
	 * @param x     The x-coordinate of the upper left corner of the image
	 * @param y     The y-coordinate of the upper left corner of the image
	 * @usage GImage gimage = new GImage(image, x, y);
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
	 * @param name The name used to search for the contents of this image
	 * @param x    The x-coordinate of the upper left corner of the image
	 * @param y    The y-coordinate of the upper left corner of the image
	 * @usage GImage gimage = new GImage(image, x, y);
	 */
	public GImage(String name, double x, double y) {
		this(MediaTools.loadImage(name), x, y);
	}

	/**
	 * Creates a new <code>GImage</code> object at the specified coordinates.
	 * The <code>array</code> parameter is a two-dimensional pixel array in
	 * which each pixel value consists of an integer that is subdivided into
	 * four eight-bit bytes, as follows:
	 * <p>
	 * <center>
	 * <i>alpha</i> <code>&lt;&lt;</code> 24  <code>|</code>
	 * <i>red</i>   <code>&lt;&lt;</code> 16  <code>|</code>
	 * <i>green</i> <code>&lt;&lt;</code>  8  <code>|</code>
	 * <i>blue</i>
	 * </center>
	 * <p>
	 * The <i>alpha</i> value indicates the transparency, and the other values
	 * are the red, green, and blue components of the color.
	 *
	 * @param array A two-dimensional pixel array
	 * @param x     The x-coordinate of the upper left corner of the image
	 * @param y     The y-coordinate of the upper left corner of the image
	 * @usage GImage gimage = new GImage(array, x, y);
	 */
	public GImage(int[][] array, double x, double y) {
		this(MediaTools.createImage(array), x, y);
	}

	/**
	 * Creates an opaque pixel value with the color components given by
	 * <code>red</code>, <code>green</code>, and <code>blue</code>.
	 *
	 * @param red   The red component of the pixel (0 to 255)
	 * @param green The green component of the pixel (0 to 255)
	 * @param blue  The blue component of the pixel (0 to 255)
	 * @return An opaque pixel value containing these components
	 * @usage int pixel = GImage.createRGBPixel(red, green, blue);
	 */
	public static int createRGBPixel(int red, int green, int blue) {
		return createRGBPixel(red, green, blue, 0xFF);
	}

	/**
	 * Creates a pixel value with the color components given by
	 * <code>red</code>, <code>green</code>, and <code>blue</code>
	 * and the transparency value <code>alpha</code>.
	 *
	 * @param red   The red component of the pixel (0 to 255)
	 * @param green The green component of the pixel (0 to 255)
	 * @param blue  The blue component of the pixel (0 to 255)
	 * @param alpha The transparency value of the pixel (0 to 255)
	 * @return A pixel value containing these components
	 * @usage int pixel = GImage.createRGBPixel(red, green, blue);
	 */
	public static int createRGBPixel(int red, int green, int blue, int alpha) {
		return (alpha << 24) | (red << 16) | (green << 8) | blue;
	}

	/**
	 * Computes the size of the image.
	 */
	private void determineSize() {
		if (sizeDetermined) {
			return;
		}
		Component component = getComponent();
		if (component == null) {
			component = MediaTools.getImageObserver();
		}
		myWidth = myImage.getWidth(component);
		myHeight = myImage.getHeight(component);
		sizeDetermined = true;
	}

	/**
	 * Resizes the image to the largest size that fits within the given width/height
	 * while maintaining the image's "aspect ratio" (not stretching the image's
	 * relative width/height).
	 *
	 * @param width  maximum width, in pixels
	 * @param height maximum height, in pixels
	 */
	public void fit(double width, double height) {
		double aspectRatio = myWidth / myHeight;
		double widthRatio = myWidth / width;
		double heightRatio = myHeight / height;
		if (widthRatio > heightRatio) {
			// width is the constrained dimension
			myWidth = width;
			myHeight = width / aspectRatio;
		} else {
			// height is the constrained dimension
			myHeight = height;
			myWidth = height * aspectRatio;
		}
	}

	/**
	 * Returns the alpha component from an RGB value.
	 *
	 * @param pixel An <code>int</code> containing a pixel value as alpha/red/green/blue.
	 * @return The alpha component of the pixel
	 * @usage int alpha = GImage.getAlpha(pixel);
	 */
	public static int getAlpha(int pixel) {
		return (pixel >> 24) & 0xFF;
	}

	/**
	 * Returns an AWT <code>Rectangle</code> that specifies the bounds of this object.
	 *
	 * @return A <code>Rectangle</code> that specifies the bounds of this object
	 * @usage Rectangle r = grect.getAWTBounds();
	 */
	protected Rectangle getAWTBounds() {
		determineSize();
		return new Rectangle(GMath.round(getX()), GMath.round(getY()), GMath.round(myWidth), GMath.round(myHeight));
	}

	/**
	 * Returns the blue component from an RGB value.
	 *
	 * @param pixel An <code>int</code> containing a pixel value as alpha/red/green/blue.
	 * @return The blue component of the pixel
	 * @usage int blue = GImage.getBlue(pixel);
	 */
	public static int getBlue(int pixel) {
		return pixel & 0xFF;
	}

	/**
	 * Returns the bounding box of this object.
	 *
	 * @return The bounding box for this object
	 * @usage GRectangle bounds = gimage.getBounds();
	 */
	public GRectangle getBounds() {
		determineSize();
		return new GRectangle(getX(), getY(), myWidth, myHeight);
	}

	/**
	 * Returns the green component from an RGB value.
	 *
	 * @param pixel An <code>int</code> containing a pixel value as alpha/red/green/blue.
	 * @return The green component of the pixel
	 * @usage int green = GImage.getGreen(pixel);
	 */
	public static int getGreen(int pixel) {
		return (pixel >> 8) & 0xFF;
	}

	/**
	 * Returns the image stored inside this <code>GImage</code>.
	 *
	 * @return The <code>Image</code> object stored inside this <code>GImage</code>
	 * @usage Image image = gimage.getImage();
	 */
	public Image getImage() {
		return myImage;
	}

	/**
	 * Returns the red component from an RGB value.
	 *
	 * @param pixel An <code>int</code> containing a pixel value as alpha/red/green/blue.
	 * @return The red component of the pixel
	 * @usage int red = GImage.getRed(pixel);
	 */
	public static int getRed(int pixel) {
		return (pixel >> 16) & 0xFF;
	}

	/**
	 * Returns the size of this object as a <code>GDimension</code>.
	 *
	 * @return The size of this object
	 * @usage GDimension size = gimage.getSize();
	 */
	public GDimension getSize() {
		return new GDimension(myWidth, myHeight);
	}

	/**
	 * Returns whether the given x/y pixel position is within the bounds of the image,
	 * in other words, whether it is between (0, 0) and (w, h).
	 *
	 * @param x the pixel's x-coordinate
	 * @param y the pixel's y-coordinate
	 * @return true if the given x/y pixel position is between (0, 0) and (w, h)
	 */
	public boolean inBounds(double x, double y) {
		int xx = (int) x;
		int yy = (int) y;
		return xx >= 0 && xx < getWidth()
				&& yy >= 0 && yy < getHeight();
	}

	/**
	 * Implements the <code>paint</code> operation for this graphical object.  This method
	 * is not called directly by clients.
	 *
	 * @noshow
	 */
//	public void paint2d(Graphics g) {
//		Component imageObserver = getComponent();
//		if (imageObserver == null) {
//			imageObserver = MediaTools.getImageObserver();
//		}
//		if (myImage != null && imageObserver != null) {
//			Rectangle r = getAWTBounds();
//			Color color = getObjectColor();
//			if (color == null) {
//				g.drawImage(myImage, r.x, r.y, r.width, r.height, imageObserver);
//			} else {
//				g.drawImage(myImage, r.x, r.y, r.width, r.height, color, imageObserver);
//			}
//		}
//	}

	/**
	 * Implements the <code>paint2d</code> operation for this graphical object.  This method
	 * is not called directly by clients.
	 *
	 * @noshow
	 */
	protected void paint2d(Graphics2D graphics2d) {
		java.awt.Component component = getComponent();
		if (component == null) {
			component = MediaTools.getImageObserver();
		}
		if (myImage != null && component != null) {
			determineSize();
			java.awt.Color color = getObjectColor();
			if (color == null) {
				graphics2d.drawImage(myImage, 0, 0, GMath.round(myWidth), GMath.round(myHeight), component);
			} else {
				graphics2d.drawImage(myImage, 0, 0, GMath.round(myWidth), GMath.round(myHeight), color, component);
			}
		}
	}

	/**
	 * Saves the image to the specified file.  The data format for the
	 * image file is determined by the suffix of the filename.  If the
	 * suffix of the file is not recognized as a supported image type,
	 * calling this method generates an error.
	 *
	 * @param file The <code>File</code> to which the image is saved
	 * @usage gimage.saveImage(file);
	 */
	public void saveImage(File file) {
		MediaTools.saveImage(myImage, file);
	}

	/**
	 * Saves the image to a file with the specified filename.  The data format
	 * for the image file is determined by the suffix of the filename.  If the
	 * suffix of the file is not recognized as a supported image type, calling
	 * this method generates an error.
	 *
	 * @param filename The name of the file to which the image is saved
	 * @usage gimage.saveImage(filename);
	 */
	public void saveImage(String filename) {
		MediaTools.saveImage(myImage, filename);
	}

	/**
	 * Changes the bounds of this object to the specified values.
	 *
	 * @param x      The new x-coordinate for the object
	 * @param y      The new y-coordinate for the object
	 * @param width  The new width of the object
	 * @param height The new height of the object
	 * @usage gimage.setBounds(x, y, width, height);
	 */
	public void setBounds(double x, double y, double width, double height) {
		myWidth = width;
		myHeight = height;
		setLocation(x, y);
	}

	/**
	 * Changes the bounds of this object to the values from the specified
	 * <code>GRectangle</code>.
	 *
	 * @param bounds A <code>GRectangle</code> specifying the new bounds
	 * @usage gimage.setBounds(bounds);
	 */
	public final void setBounds(GRectangle bounds) {
		setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
	}

	/**
	 * Resets the image used by this <code>GImage</code> object to the new image
	 * specified as an argument.  Calling <code>setImage</code> automatically changes
	 * the size of the image to be equal to that of the image data.
	 *
	 * @param image The image to use as the contents of this <code>GImage</code>
	 * @usage gimage.setImage(image);
	 */
	public void setImage(Image image) {
		myImage = MediaTools.loadImage(image);
		sizeDetermined = false;
		determineSize();
		repaint();
	}

	/**
	 * Resets the image used by this <code>GImage</code> object to the one identified
	 * by the argument <code>name</code>, which is processed exactly as described
	 * in the constructors.  Calling <code>setImage</code> automatically changes
	 * the size of the image to be equal to that of the image data.
	 *
	 * @param name The name used to search for the contents of this image
	 * @usage gimage.setImage(name);
	 */
	public void setImage(String name) {
		setImage(MediaTools.loadImage(name));
	}

	/**
	 * Changes the size of this object to the specified width and height.
	 *
	 * @param width  The new width of the object
	 * @param height The new height of the object
	 * @usage gimage.setSize(width, height);
	 */
	public void setSize(double width, double height) {
		myWidth = width;
		myHeight = height;
		repaint();
	}

	/**
	 * Changes the size of this object to the specified <code>GDimension</code>.
	 *
	 * @param size A <code>GDimension</code> object specifying the size
	 * @usage gimage.setSize(size);
	 * @noshow
	 */
	public final void setSize(GDimension size) {
		setSize(size.getWidth(), size.getHeight());
	}

	/**
	 * Scales the object on the screen by the scale factors <code>sx</code> and <code>sy</code>.
	 *
	 * @param sx The factor used to scale all coordinates in the x direction
	 * @param sy The factor used to scale all coordinates in the y direction
	 * @usage gobj.scale(sx, sy);
	 */
	public void scale(double sx, double sy) {
		myWidth *= sx;
		myHeight *= sy;
		repaint();
	}

	/**
	 * Returns a two-dimensional array of pixel values from the stored image.
	 *
	 * @return A two-dimensional array of pixel values from the stored image
	 * @usage int[][] array = gimage.getPixelArray();
	 */
	public int[][] getPixelArray() {
		return MediaTools.getPixelArray(myImage);
	}

	/**
	 * Sets the image to use the given two-dimensional array of pixel values.
	 *
	 * @usage gimage.setPixelArray(array);
	 */
	public void setPixelArray(int[][] pixels) {
		setImage(MediaTools.createImage(pixels));
	}
}
