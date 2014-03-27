// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Program.java

package acm.program;

import acm.util.JTFTools;
import java.io.PrintStream;
import java.lang.reflect.*;

// Referenced classes of package acm.program:
//            Program

class AppletStarter
    implements Runnable
{

    public AppletStarter(Program program)
    {
        myProgram = program;
    }

    public void start()
    {
        try
        {
            mainThread = new Thread(this);
            mainThread.start();
            if(JTFTools.testDebugOption("startup"))
                System.out.println("Starting main thread using Thread package");
        }
        catch(SecurityException securityexception)
        {
            if(JTFTools.testDebugOption("startup"))
                System.out.println((new StringBuilder()).append("Starting main thread using Executor because ").append(securityexception).toString());
            forkUsingExecutor();
        }
    }

    public void stop()
    {
        try
        {
            if(executor == null)
            {
                Class class1 = Class.forName("java.lang.Thread");
                Method method1 = class1.getMethod("stop", new Class[0]);
                method1.invoke(mainThread, new Object[0]);
            } else
            {
                Method method = executor.getClass().getMethod("shutdownNow", new Class[0]);
                method.invoke(executor, new Object[0]);
            }
        }
        catch(Exception exception) { }
    }

    public void run()
    {
        myProgram.startRun();
    }

    public Thread getMainThread()
    {
        return mainThread;
    }

    private void forkUsingExecutor()
    {
        try
        {
            Class class1 = Class.forName("java.util.concurrent.ScheduledExecutor");
            Class aclass[] = {
                Integer.TYPE
            };
            Object aobj[] = {
                new Integer(1)
            };
            Constructor constructor = class1.getConstructor(aclass);
            executor = constructor.newInstance(aobj);
            Class class2 = Class.forName("java.util.concurrent.TimeUnit");
            Field field = class2.getField("SECONDS");
            Object obj = field.get(null);
            Class aclass1[] = {
                Class.forName("java.lang.Runnable"), Long.TYPE, Class.forName("java.util.concurrent.TimeUnit")
            };
            Object aobj1[] = {
                this, new Long(0L), obj
            };
            Method method = executor.getClass().getMethod("schedule", aclass1);
            method.invoke(executor, aobj1);
        }
        catch(Exception exception)
        {
            if(JTFTools.testDebugOption("startup"))
                System.out.println((new StringBuilder()).append("Executor failed because ").append(exception).toString());
            executor = null;
            mainThread = Thread.currentThread();
            myProgram.startRun();
        }
    }

    private Program myProgram;
    private Thread mainThread;
    private Object executor;
}
