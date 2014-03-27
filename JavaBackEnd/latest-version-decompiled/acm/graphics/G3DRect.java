// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   G3DRect.java

package acm.graphics;

import java.awt.Graphics;
import java.awt.Graphics2D;

// Referenced classes of package acm.graphics:
//            GRect, GMath

public class G3DRect extends GRect
{

    public G3DRect(double d, double d1)
    {
        this(0.0D, 0.0D, d, d1, false);
    }

    public G3DRect(double d, double d1, double d2, double d3)
    {
        this(d, d1, d2, d3, false);
    }

    public G3DRect(double d, double d1, double d2, double d3, boolean flag)
    {
        super(d, d1, d2, d3);
        isRaised = flag;
    }

    public void paint(Graphics g)
    {
        if(!isVisible())
            return;
        Graphics2D graphics2d = createTransformedGraphics(g);
        if(isFilled())
        {
            graphics2d.setColor(getFillColor());
            graphics2d.fill3DRect(0, 0, GMath.round(getFrameWidth()), GMath.round(getFrameHeight()), isRaised);
            graphics2d.setColor(getColor());
        }
        graphics2d.draw3DRect(0, 0, GMath.round(getFrameWidth()), GMath.round(getFrameHeight()), isRaised);
        graphics2d.dispose();
    }

    public void setRaised(boolean flag)
    {
        isRaised = flag;
    }

    public boolean isRaised()
    {
        return isRaised;
    }

    private boolean isRaised;
    static final long serialVersionUID = 21L;
}
