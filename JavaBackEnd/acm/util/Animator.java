// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Animator.java

package acm.util;

import java.applet.Applet;
import java.awt.Component;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import javax.swing.JScrollBar;
import javax.swing.JSlider;

// Referenced classes of package acm.util:
//            ErrorException, JTFTools, SpeedBarListener

public class Animator extends Thread
{

    public Animator()
    {
        animatorState = 0;
        currentDepth = 0;
        callDepth = 0;
        delayCount = 0;
        animatorSpeed = 0.5D;
        initAnimator();
    }

    public Animator(ThreadGroup threadgroup)
    {
        super(threadgroup, (Runnable)null);
        animatorState = 0;
        currentDepth = 0;
        callDepth = 0;
        delayCount = 0;
        animatorSpeed = 0.5D;
        initAnimator();
    }

    public Animator(Runnable runnable)
    {
        super(runnable);
        animatorState = 0;
        currentDepth = 0;
        callDepth = 0;
        delayCount = 0;
        animatorSpeed = 0.5D;
        initAnimator();
    }

    public Animator(ThreadGroup threadgroup, Runnable runnable)
    {
        super(threadgroup, runnable);
        animatorState = 0;
        currentDepth = 0;
        callDepth = 0;
        delayCount = 0;
        animatorSpeed = 0.5D;
        initAnimator();
    }

    public int getAnimatorState()
    {
        return animatorState;
    }

    public void pause(double d)
    {
        if(animatorState == 7)
            terminate();
        JTFTools.pause(d);
    }

    public void startAction()
    {
        start(1);
    }

    public void stopAction()
    {
        switch(animatorState)
        {
        case 1: // '\001'
        case 2: // '\002'
        case 3: // '\003'
            animatorState = 4;
            break;
        }
    }

    public void stepAction()
    {
        start(2);
    }

    public void callAction()
    {
        callDepth = currentDepth;
        start(3);
    }

    public boolean buttonAction(String s)
    {
        if(s.equals("Start"))
            startAction();
        else
        if(s.equals("Stop"))
            stopAction();
        else
        if(s.equals("Step"))
            stepAction();
        else
        if(s.equals("Call"))
            callAction();
        else
            return false;
        return true;
    }

    public void setSpeed(double d)
    {
        animatorSpeed = d;
        if(speedBar instanceof JSlider)
        {
            JSlider jslider = (JSlider)speedBar;
            int i = jslider.getMinimum();
            int k = jslider.getMaximum();
            jslider.setValue((int)Math.round((double)i + d * (double)(k - i)));
        } else
        if(speedBar instanceof JScrollBar)
        {
            JScrollBar jscrollbar = (JScrollBar)speedBar;
            int j = jscrollbar.getMinimum();
            int l = jscrollbar.getMaximum();
            jscrollbar.setValue((int)Math.round((double)j + d * (double)(l - j)));
        }
    }

    public double getSpeed()
    {
        return animatorSpeed;
    }

    public void trace()
    {
        trace(0);
    }

    public void trace(int i)
    {
        if(Thread.currentThread() != this)
            throw new ErrorException("trace() can be called only by the animator thread itself");
        currentDepth = i;
        switch(animatorState)
        {
        case 5: // '\005'
        case 6: // '\006'
        default:
            break;

        case 1: // '\001'
            delay();
            break;

        case 2: // '\002'
        case 4: // '\004'
            breakpoint();
            break;

        case 3: // '\003'
            if(callDepth < currentDepth)
                delay();
            else
                breakpoint();
            break;

        case 7: // '\007'
            terminate();
            break;
        }
    }

    public void breakpoint()
    {
        if(Thread.currentThread() != this)
        {
            throw new ErrorException("breakpoint() can be called only by the animator thread itself");
        } else
        {
            animatorState = 5;
            breakHook();
            suspendAnimator();
            return;
        }
    }

    public void delay()
    {
        boolean flag = true;
        double d = 0.0D;
        if(animatorSpeed < 0.25D)
            d = 1000D + (animatorSpeed / 0.25D) * -800D;
        else
        if(animatorSpeed < 0.90000000000000002D)
        {
            d = 200D + Math.sqrt((animatorSpeed - 0.25D) / 0.65000000000000002D) * -200D;
        } else
        {
            switch((int)(animatorSpeed * 99.989999999999995D - 90D))
            {
            case 0: // '\0'
                flag = true;
                break;

            case 1: // '\001'
                flag = delayCount % 10 != 0;
                break;

            case 2: // '\002'
                flag = delayCount % 7 != 0;
                break;

            case 3: // '\003'
                flag = delayCount % 5 != 0;
                break;

            case 4: // '\004'
                flag = delayCount % 3 != 0;
                break;

            case 5: // '\005'
                flag = delayCount % 2 == 0;
                break;

            case 6: // '\006'
                flag = delayCount % 3 == 0;
                break;

            case 7: // '\007'
                flag = delayCount % 4 == 0;
                break;

            case 8: // '\b'
                flag = delayCount % 6 == 0;
                break;

            case 9: // '\t'
                flag = false;
                break;
            }
            delayCount = (delayCount + 1) % 420;
        }
        if(flag)
        {
            delayHook();
            JTFTools.pause(d);
        }
    }

    public void registerSpeedBar(JSlider jslider)
    {
        SpeedBarListener.register(this, jslider);
        speedBar = jslider;
    }

    public void registerSpeedBar(JScrollBar jscrollbar)
    {
        SpeedBarListener.register(this, jscrollbar);
        speedBar = jscrollbar;
    }

    public Component getSpeedBar()
    {
        return speedBar;
    }

    public void requestTermination()
    {
        animatorState = 7;
    }

    public void checkForTermination()
    {
        if(animatorState == 7)
            terminate();
        else
            yield();
    }

    public static void shutdown(Applet applet)
    {
        try
        {
            Class class1 = Class.forName("java.lang.Thread");
            Method method = class1.getMethod("stop", new Class[0]);
            Object aobj[] = new Object[0];
            ArrayList arraylist = (ArrayList)animatorTable.get(applet);
            if(arraylist != null)
            {
                animatorTable.remove(applet);
                int i = arraylist.size();
                for(int j = 0; j < i; j++)
                {
                    Thread thread = (Thread)arraylist.get(j);
                    method.invoke(thread, aobj);
                }

            }
        }
        catch(Exception exception) { }
    }

    protected void delayHook()
    {
    }

    protected void breakHook()
    {
    }

    protected void resumeHook()
    {
    }

    protected void controllerHook()
    {
    }

    public void start()
    {
        start(1);
    }

    private void initAnimator()
    {
        Applet applet = JTFTools.getApplet();
        if(applet != null)
        {
            JTFTools.registerApplet(applet, this);
            ArrayList arraylist = (ArrayList)animatorTable.get(applet);
            if(arraylist == null)
            {
                arraylist = new ArrayList();
                animatorTable.put(applet, arraylist);
            }
            arraylist.add(this);
        }
    }

    private void start(int i)
    {
        switch(animatorState)
        {
        case 0: // '\0'
        case 6: // '\006'
            animatorState = i;
            resumeHook();
            controllerHook();
            super.start();
            break;

        case 5: // '\005'
            animatorState = i;
            resumeHook();
            controllerHook();
            resumeAnimator();
            break;
        }
    }

    private synchronized void suspendAnimator()
    {
        resumed = false;
        while(!resumed) 
            try
            {
                wait();
            }
            catch(InterruptedException interruptedexception) { }
    }

    private synchronized void resumeAnimator()
    {
        resumed = true;
        notifyAll();
    }

    private void terminate()
    {
        animatorState = 6;
        if(Thread.currentThread() == this)
            throw new ThreadDeath();
        else
            throw new ErrorException("Illegal call to terminate");
    }

    public static final int INITIAL = 0;
    public static final int RUNNING = 1;
    public static final int STEPPING = 2;
    public static final int CALLING = 3;
    public static final int STOPPING = 4;
    public static final int STOPPED = 5;
    public static final int FINISHED = 6;
    public static final int TERMINATING = 7;
    private static final double SLOW_DELAY = 1000D;
    private static final double CLIP_DELAY = 200D;
    private static final double FAST_DELAY = 0D;
    private static HashMap animatorTable = new HashMap();
    private int animatorState;
    private int currentDepth;
    private int callDepth;
    private int delayCount;
    private double animatorSpeed;
    private Component speedBar;
    private boolean resumed;

}
