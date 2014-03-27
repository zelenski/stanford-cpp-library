// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GImage.java

package acm.graphics;

import acm.util.ErrorException;
import acm.util.MediaTools;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;
import java.io.File;

// Referenced classes of package acm.graphics:
//            GObject, GDimension, GRectangle, GResizable, 
//            GMath

public class GImage extends GObject
    implements GResizable
{

    public GImage(Image image1)
    {
        this(image1, 0.0D, 0.0D);
    }

    public GImage(String s)
    {
        this(s, 0.0D, 0.0D);
    }

    public GImage(int ai[][])
    {
        this(ai, 0.0D, 0.0D);
    }

    public GImage(Image image1, double d, double d1)
    {
        setImage(image1);
        setLocation(d, d1);
    }

    public GImage(String s, double d, double d1)
    {
        this(MediaTools.loadImage(s), d, d1);
    }

    public GImage(int ai[][], double d, double d1)
    {
        this(MediaTools.createImage(ai), d, d1);
    }

    public void setImage(Image image1)
    {
        image = MediaTools.loadImage(image1);
        sizeDetermined = false;
        determineSize();
        repaint();
    }

    public void setImage(String s)
    {
        setImage(MediaTools.loadImage(s));
    }

    public Image getImage()
    {
        return image;
    }

    public void saveImage(String s)
    {
        MediaTools.saveImage(image, s);
    }

    public void saveImage(File file)
    {
        MediaTools.saveImage(image, file);
    }

    protected void paint2d(Graphics2D graphics2d)
    {
        java.awt.Component component = getComponent();
        if(component == null)
            component = MediaTools.getImageObserver();
        if(image != null && component != null)
        {
            determineSize();
            java.awt.Color color = getObjectColor();
            if(color == null)
                graphics2d.drawImage(image, 0, 0, GMath.round(width), GMath.round(height), component);
            else
                graphics2d.drawImage(image, 0, 0, GMath.round(width), GMath.round(height), color, component);
        }
    }

    public void setSize(double d, double d1)
    {
        if(getMatrix() != null)
        {
            throw new ErrorException("setSize: Object has been transformed");
        } else
        {
            width = d;
            height = d1;
            repaint();
            return;
        }
    }

    public final void setSize(GDimension gdimension)
    {
        setSize(gdimension.getWidth(), gdimension.getHeight());
    }

    public GDimension getSize()
    {
        return new GDimension(width, height);
    }

    public void setBounds(double d, double d1, double d2, double d3)
    {
        if(getMatrix() != null)
        {
            throw new ErrorException("setBounds: Object has been transformed");
        } else
        {
            width = d2;
            height = d3;
            setLocation(d, d1);
            return;
        }
    }

    public final void setBounds(GRectangle grectangle)
    {
        if(getMatrix() != null)
        {
            throw new ErrorException("setBounds: Object has been transformed");
        } else
        {
            setBounds(grectangle.getX(), grectangle.getY(), grectangle.getWidth(), grectangle.getHeight());
            return;
        }
    }

    public GRectangle getBounds()
    {
        determineSize();
        Object obj = new java.awt.geom.Rectangle2D.Double(0.0D, 0.0D, width, height);
        AffineTransform affinetransform = getMatrix();
        if(affinetransform != null)
            obj = affinetransform.createTransformedShape(((Shape) (obj)));
        java.awt.Rectangle rectangle = ((Shape) (obj)).getBounds();
        return new GRectangle(getX() + rectangle.getX(), getY() + rectangle.getY(), rectangle.getWidth(), rectangle.getHeight());
    }

    public boolean contains(double d, double d1)
    {
        determineSize();
        Object obj = new java.awt.geom.Rectangle2D.Double(0.0D, 0.0D, width, height);
        AffineTransform affinetransform = getMatrix();
        if(affinetransform != null)
            obj = affinetransform.createTransformedShape(((Shape) (obj)));
        return ((Shape) (obj)).contains(d - getX(), d1 - getY());
    }

    public int[][] getPixelArray()
    {
        return MediaTools.getPixelArray(image);
    }

    public static int getAlpha(int i)
    {
        return i >> 24 & 0xff;
    }

    public static int getRed(int i)
    {
        return i >> 16 & 0xff;
    }

    public static int getGreen(int i)
    {
        return i >> 8 & 0xff;
    }

    public static int getBlue(int i)
    {
        return i & 0xff;
    }

    public static int createRGBPixel(int i, int j, int k)
    {
        return createRGBPixel(i, j, k, 255);
    }

    public static int createRGBPixel(int i, int j, int k, int l)
    {
        return l << 24 | i << 16 | j << 8 | k;
    }

    private void determineSize()
    {
        if(sizeDetermined)
            return;
        java.awt.Component component = getComponent();
        if(component == null)
            component = MediaTools.getImageObserver();
        width = image.getWidth(component);
        height = image.getHeight(component);
        sizeDetermined = true;
    }

    private Image image;
    private double width;
    private double height;
    private boolean sizeDetermined;
    static final long serialVersionUID = 21L;
}
