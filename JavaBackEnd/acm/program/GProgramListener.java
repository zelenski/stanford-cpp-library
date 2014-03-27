// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GraphicsProgram.java

package acm.program;

import acm.graphics.GPoint;
import acm.util.ErrorException;
import java.awt.event.*;
import java.lang.reflect.Method;

// Referenced classes of package acm.program:
//            GraphicsProgram

class GProgramListener
    implements MouseListener, MouseMotionListener
{

    public GProgramListener(GraphicsProgram graphicsprogram)
    {
        myProgram = graphicsprogram;
        try
        {
            Class class1 = graphicsprogram.getClass();
            Class aclass[] = {
                Class.forName("acm.graphics.GPoint")
            };
            try
            {
                mousePressedHook = class1.getMethod("mousePressed", aclass);
            }
            catch(NoSuchMethodException nosuchmethodexception) { }
            try
            {
                mouseReleasedHook = class1.getMethod("mouseReleased", aclass);
            }
            catch(NoSuchMethodException nosuchmethodexception1) { }
            try
            {
                mouseClickedHook = class1.getMethod("mouseClicked", aclass);
            }
            catch(NoSuchMethodException nosuchmethodexception2) { }
            try
            {
                mouseMovedHook = class1.getMethod("mouseMoved", aclass);
            }
            catch(NoSuchMethodException nosuchmethodexception3) { }
            try
            {
                mouseDraggedHook = class1.getMethod("mouseDragged", aclass);
            }
            catch(NoSuchMethodException nosuchmethodexception4) { }
        }
        catch(Exception exception)
        {
            throw new ErrorException(exception);
        }
    }

    public boolean needsMouseMotionListeners()
    {
        return mouseMovedHook != null || mouseDraggedHook != null;
    }

    public void mouseClicked(MouseEvent mouseevent)
    {
        if(mouseClickedHook != null)
        {
            Object aobj[] = {
                new GPoint(mouseevent.getX(), mouseevent.getY())
            };
            try
            {
                mouseClickedHook.invoke(myProgram, aobj);
            }
            catch(Exception exception)
            {
                throw new ErrorException(exception);
            }
        }
        signalClickOccurred();
    }

    public void mousePressed(MouseEvent mouseevent)
    {
        if(mousePressedHook != null)
        {
            Object aobj[] = {
                new GPoint(mouseevent.getX(), mouseevent.getY())
            };
            try
            {
                mousePressedHook.invoke(myProgram, aobj);
            }
            catch(Exception exception)
            {
                throw new ErrorException(exception);
            }
        }
    }

    public void mouseReleased(MouseEvent mouseevent)
    {
        if(mouseReleasedHook != null)
        {
            Object aobj[] = {
                new GPoint(mouseevent.getX(), mouseevent.getY())
            };
            try
            {
                mouseReleasedHook.invoke(myProgram, aobj);
            }
            catch(Exception exception)
            {
                throw new ErrorException(exception);
            }
        }
    }

    public void mouseEntered(MouseEvent mouseevent)
    {
    }

    public void mouseExited(MouseEvent mouseevent)
    {
    }

    public void mouseMoved(MouseEvent mouseevent)
    {
        if(mouseMovedHook != null)
        {
            Object aobj[] = {
                new GPoint(mouseevent.getX(), mouseevent.getY())
            };
            try
            {
                mouseMovedHook.invoke(myProgram, aobj);
            }
            catch(Exception exception)
            {
                throw new ErrorException(exception);
            }
        }
    }

    public void mouseDragged(MouseEvent mouseevent)
    {
        if(mouseDraggedHook != null)
        {
            Object aobj[] = {
                new GPoint(mouseevent.getX(), mouseevent.getY())
            };
            try
            {
                mouseDraggedHook.invoke(myProgram, aobj);
            }
            catch(Exception exception)
            {
                throw new ErrorException(exception);
            }
        }
    }

    public synchronized void waitForClick()
    {
        clickFlag = false;
        while(!clickFlag) 
            try
            {
                wait();
            }
            catch(InterruptedException interruptedexception) { }
    }

    private synchronized void signalClickOccurred()
    {
        clickFlag = true;
        notifyAll();
    }

    private GraphicsProgram myProgram;
    private Method mousePressedHook;
    private Method mouseReleasedHook;
    private Method mouseClickedHook;
    private Method mouseMovedHook;
    private Method mouseDraggedHook;
    private boolean clickFlag;
}
