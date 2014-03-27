// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IODialog.java

package acm.io;

import acm.util.*;
import java.awt.Component;
import java.lang.reflect.Constructor;
import javax.swing.JPanel;

// Referenced classes of package acm.io:
//            DialogModel, AWTDialogModel, IOModel, IOConsole

public class IODialog
    implements IOModel
{

    public IODialog()
    {
        this(null);
    }

    public IODialog(Component component)
    {
        myComponent = component;
        model = createModel();
        outputLine = "";
        exceptionOnError = false;
        allowCancel = false;
    }

    public void print(String s)
    {
        new StringBuilder();
        this;
        JVM INSTR dup_x1 ;
        outputLine;
        append();
        s;
        append();
        toString();
        outputLine;
    }

    public final void print(boolean flag)
    {
        print((new StringBuilder()).append("").append(flag).toString());
    }

    public final void print(char c)
    {
        print((new StringBuilder()).append("").append(c).toString());
    }

    public final void print(double d)
    {
        print((new StringBuilder()).append("").append(d).toString());
    }

    public final void print(float f)
    {
        print((new StringBuilder()).append("").append(f).toString());
    }

    public final void print(int i)
    {
        print((new StringBuilder()).append("").append(i).toString());
    }

    public final void print(long l)
    {
        print((new StringBuilder()).append("").append(l).toString());
    }

    public final void print(Object obj)
    {
        print((new StringBuilder()).append("").append(obj).toString());
    }

    public void println()
    {
        model.popupMessage(outputLine);
        outputLine = "";
    }

    public void println(String s)
    {
        print(s);
        println();
    }

    public final void println(boolean flag)
    {
        println((new StringBuilder()).append("").append(flag).toString());
    }

    public final void println(char c)
    {
        println((new StringBuilder()).append("").append(c).toString());
    }

    public final void println(double d)
    {
        println((new StringBuilder()).append("").append(d).toString());
    }

    public final void println(float f)
    {
        println((new StringBuilder()).append("").append(f).toString());
    }

    public final void println(int i)
    {
        println((new StringBuilder()).append("").append(i).toString());
    }

    public final void println(long l)
    {
        println((new StringBuilder()).append("").append(l).toString());
    }

    public final void println(Object obj)
    {
        println((new StringBuilder()).append("").append(obj).toString());
    }

    public void showErrorMessage(String s)
    {
        model.popupErrorMessage(s);
    }

    public final String readLine()
    {
        return readLine(null);
    }

    public String readLine(String s)
    {
        if(myConsole != null && myConsole.getInputScript() != null)
            return myConsole.readLine(s);
        s = s != null ? (new StringBuilder()).append(outputLine).append(s).toString() : outputLine;
        outputLine = "";
        String s1;
        while((s1 = model.popupLineInputDialog(s, allowCancel)) == null) 
            if(allowCancel)
                throw new CancelledException();
        return s1;
    }

    public final int readInt()
    {
        return readInt(null, 0x80000000, 0x7fffffff);
    }

    public final int readInt(int i, int j)
    {
        return readInt(null, i, j);
    }

    public final int readInt(String s)
    {
        return readInt(s, 0x80000000, 0x7fffffff);
    }

    public int readInt(String s, int i, int j)
    {
_L2:
        String s1 = readLine(s);
        int k;
        k = Integer.parseInt(s1);
        if(k < i || k > j)
            signalError((new StringBuilder()).append("Value is outside the range [").append(i).append(":").append(j).append("]").toString());
        return k;
        NumberFormatException numberformatexception;
        numberformatexception;
        signalError("Illegal integer format");
        if(true) goto _L2; else goto _L1
_L1:
    }

    public final double readDouble()
    {
        return readDouble(null, (-1.0D / 0.0D), (1.0D / 0.0D));
    }

    public final double readDouble(double d, double d1)
    {
        return readDouble(null, d, d1);
    }

    public final double readDouble(String s)
    {
        return readDouble(s, (-1.0D / 0.0D), (1.0D / 0.0D));
    }

    public double readDouble(String s, double d, double d1)
    {
_L2:
        String s1 = readLine(s);
        double d2;
        d2 = Double.valueOf(s1).doubleValue();
        if(d2 < d || d2 > d1)
            signalError((new StringBuilder()).append("Value is outside the range [").append(d).append(":").append(d1).append("]").toString());
        return d2;
        NumberFormatException numberformatexception;
        numberformatexception;
        signalError("Illegal numeric format");
        if(true) goto _L2; else goto _L1
_L1:
    }

    public final boolean readBoolean()
    {
        return readBoolean(null);
    }

    public final boolean readBoolean(String s)
    {
        return readBoolean(s, "true", "false");
    }

    public boolean readBoolean(String s, String s1, String s2)
    {
        if(myConsole != null && myConsole.getInputScript() != null)
            return myConsole.readBoolean(s, s1, s2);
        s = s != null ? (new StringBuilder()).append(outputLine).append(s).toString() : outputLine;
        outputLine = "";
        Boolean boolean1;
        while((boolean1 = model.popupBooleanInputDialog(s, s1, s2, allowCancel)) == null) 
            if(allowCancel)
                throw new CancelledException();
        return boolean1.booleanValue();
    }

    public void setExceptionOnError(boolean flag)
    {
        exceptionOnError = flag;
    }

    public boolean getExceptionOnError()
    {
        return exceptionOnError;
    }

    public void setAllowCancel(boolean flag)
    {
        allowCancel = flag;
    }

    public boolean getAllowCancel()
    {
        return allowCancel;
    }

    public void setAssociatedConsole(IOConsole ioconsole)
    {
        myConsole = ioconsole;
    }

    public IOConsole getAssociatedConsole()
    {
        return myConsole;
    }

    protected DialogModel createModel()
    {
        String s = (new JPanel()).getClass().getName();
        if(!s.startsWith("javax.swing.") || !Platform.isSwingAvailable())
            break MISSING_BLOCK_LABEL_92;
        Object aobj[];
        Constructor constructor;
        Class class1 = Class.forName("acm.io.SwingDialogModel");
        Class aclass[] = {
            Class.forName("java.awt.Component")
        };
        aobj = (new Object[] {
            myComponent
        });
        constructor = class1.getConstructor(aclass);
        return (DialogModel)constructor.newInstance(aobj);
        Exception exception;
        exception;
        return new AWTDialogModel(myComponent);
        return new AWTDialogModel(myComponent);
    }

    private void signalError(String s)
    {
        if(exceptionOnError)
        {
            throw new ErrorException(s);
        } else
        {
            model.popupErrorMessage(s);
            return;
        }
    }

    private boolean exceptionOnError;
    private boolean allowCancel;
    private DialogModel model;
    private Component myComponent;
    private IOConsole myConsole;
    private String outputLine;
}
