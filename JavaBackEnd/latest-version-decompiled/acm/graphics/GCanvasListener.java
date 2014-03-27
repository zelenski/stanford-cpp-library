// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GCanvas.java

package acm.graphics;

import java.awt.event.*;

// Referenced classes of package acm.graphics:
//            GCanvas

class GCanvasListener
    implements MouseListener, MouseMotionListener
{

    public GCanvasListener(GCanvas gcanvas)
    {
        gCanvas = gcanvas;
    }

    public void mouseClicked(MouseEvent mouseevent)
    {
        gCanvas.dispatchMouseEvent(mouseevent);
    }

    public void mousePressed(MouseEvent mouseevent)
    {
        gCanvas.requestFocus();
        gCanvas.dispatchMouseEvent(mouseevent);
    }

    public void mouseReleased(MouseEvent mouseevent)
    {
        gCanvas.dispatchMouseEvent(mouseevent);
    }

    public void mouseEntered(MouseEvent mouseevent)
    {
        gCanvas.dispatchMouseEvent(mouseevent);
    }

    public void mouseExited(MouseEvent mouseevent)
    {
        gCanvas.dispatchMouseEvent(mouseevent);
    }

    public void mouseDragged(MouseEvent mouseevent)
    {
        gCanvas.dispatchMouseEvent(mouseevent);
    }

    public void mouseMoved(MouseEvent mouseevent)
    {
        gCanvas.dispatchMouseEvent(mouseevent);
    }

    private GCanvas gCanvas;
}
