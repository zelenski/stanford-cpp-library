// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Animator.java

package acm.util;

import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.lang.reflect.Method;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

// Referenced classes of package acm.util:
//            ErrorException, Animator

class SpeedBarListener
    implements AdjustmentListener, ChangeListener
{

    SpeedBarListener()
    {
    }

    public static void register(Animator animator1, Object obj)
    {
        SpeedBarListener speedbarlistener = new SpeedBarListener();
        speedbarlistener.animator = animator1;
        speedbarlistener.speedBar = obj;
        Class class1 = obj.getClass();
        Method method = lookForMethod(class1, "addAdjustmentListener");
        if(method == null)
            method = lookForMethod(class1, "addChangeListener");
        if(method == null)
            method = lookForMethod(class1, "addChangeListener");
        try
        {
            speedbarlistener.getValue = class1.getMethod("getValue", new Class[0]);
            speedbarlistener.getMinimum = class1.getMethod("getMinimum", new Class[0]);
            speedbarlistener.getMaximum = class1.getMethod("getMaximum", new Class[0]);
            Object aobj[] = {
                speedbarlistener
            };
            method.invoke(obj, aobj);
        }
        catch(Exception exception)
        {
            throw new ErrorException("Illegal speed bar object");
        }
        speedbarlistener.setSpeed();
    }

    public void adjustmentValueChanged(AdjustmentEvent adjustmentevent)
    {
        setSpeed();
    }

    public void stateChanged(ChangeEvent changeevent)
    {
        setSpeed();
    }

    public void setSpeed()
    {
        try
        {
            int i = ((Integer)getMinimum.invoke(speedBar, new Object[0])).intValue();
            int j = ((Integer)getMaximum.invoke(speedBar, new Object[0])).intValue();
            int k = ((Integer)getValue.invoke(speedBar, new Object[0])).intValue();
            double d = (double)(k - i) / (double)(j - i);
            animator.setSpeed(d);
        }
        catch(Exception exception)
        {
            throw new ErrorException(exception);
        }
    }

    private static Method lookForMethod(Class class1, String s)
    {
        Method amethod[] = class1.getMethods();
        for(int i = 0; i < amethod.length; i++)
            if(s.equals(amethod[i].getName()))
                return amethod[i];

        return null;
    }

    private Animator animator;
    private Object speedBar;
    private Method getValue;
    private Method getMinimum;
    private Method getMaximum;
}
