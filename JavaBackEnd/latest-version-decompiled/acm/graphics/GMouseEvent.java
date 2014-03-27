// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GCanvas.java

package acm.graphics;

import java.awt.Component;
import java.awt.event.MouseEvent;

class GMouseEvent extends MouseEvent
{

    public GMouseEvent(Object obj, int i, MouseEvent mouseevent)
    {
        super(mouseevent.getComponent(), i, mouseevent.getWhen(), mouseevent.getModifiers(), mouseevent.getX(), mouseevent.getY(), mouseevent.getClickCount(), mouseevent.isPopupTrigger());
        effectiveSource = obj;
    }

    public Object getSource()
    {
        return effectiveSource;
    }

    public Component getComponent()
    {
        return (Component)super.getSource();
    }

    public String toString()
    {
        return (new StringBuilder()).append(getClass().getName()).append("[").append(paramString()).append("] on ").append(getSource()).toString();
    }

    private Object effectiveSource;
}
