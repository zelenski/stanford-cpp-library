// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GLabel.java

package acm.graphics;

import acm.util.JTFTools;
import acm.util.MediaTools;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;

// Referenced classes of package acm.graphics:
//            GObject, GRectangle

public class GLabel extends GObject
{

    public GLabel(String s)
    {
        this(s, 0.0D, 0.0D);
    }

    public GLabel(String s, double d, double d1)
    {
        label = s;
        setFont(DEFAULT_FONT);
        setLocation(d, d1);
    }

    public void setFont(Font font)
    {
        labelFont = JTFTools.getStandardFont(font);
        repaint();
    }

    public void setFont(String s)
    {
        setFont(JTFTools.decodeFont(s, getFont()));
    }

    public Font getFont()
    {
        return labelFont;
    }

    public void setLabel(String s)
    {
        label = s;
        repaint();
    }

    public String getLabel()
    {
        return label;
    }

    protected void paint2d(Graphics2D graphics2d)
    {
        graphics2d.setFont(labelFont);
        graphics2d.drawString(label, 0, 0);
    }

    public double getAscent()
    {
        return (double)getFontMetrics().getAscent();
    }

    public double getDescent()
    {
        return (double)getFontMetrics().getDescent();
    }

    public FontMetrics getFontMetrics()
    {
        Component component = getComponent();
        if(component == null)
            component = DUMMY_COMPONENT;
        return component.getFontMetrics(labelFont);
    }

    public GRectangle getBounds()
    {
        FontMetrics fontmetrics = getFontMetrics();
        Object obj = new java.awt.geom.Rectangle2D.Double(0.0D, -fontmetrics.getAscent(), fontmetrics.stringWidth(label), fontmetrics.getHeight());
        AffineTransform affinetransform = getMatrix();
        if(affinetransform != null)
            obj = affinetransform.createTransformedShape(((Shape) (obj)));
        java.awt.Rectangle rectangle = ((Shape) (obj)).getBounds();
        return new GRectangle(getX() + rectangle.getX(), getY() + rectangle.getY(), rectangle.getWidth(), rectangle.getHeight());
    }

    public boolean contains(double d, double d1)
    {
        FontMetrics fontmetrics = getFontMetrics();
        Object obj = new java.awt.geom.Rectangle2D.Double(0.0D, -fontmetrics.getAscent(), fontmetrics.stringWidth(label), fontmetrics.getHeight());
        AffineTransform affinetransform = getMatrix();
        if(affinetransform != null)
            obj = affinetransform.createTransformedShape(((Shape) (obj)));
        return ((Shape) (obj)).contains(d - getX(), d1 - getY());
    }

    public String paramString()
    {
        return (new StringBuilder()).append(super.paramString()).append(", string=\"").append(label).append("\"").toString();
    }

    public static final Font DEFAULT_FONT = new Font("Default", 0, 12);
    private String label;
    private Font labelFont;
    private static final Component DUMMY_COMPONENT = MediaTools.getImageObserver();
    static final long serialVersionUID = 21L;

}
