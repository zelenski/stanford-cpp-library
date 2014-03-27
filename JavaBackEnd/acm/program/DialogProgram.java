// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   DialogProgram.java

package acm.program;


// Referenced classes of package acm.program:
//            Program

public abstract class DialogProgram extends Program
{

    public DialogProgram()
    {
        acm.io.IODialog iodialog = getDialog();
        setInputModel(iodialog);
        setOutputModel(iodialog);
    }

    public void run()
    {
    }

    public void init()
    {
    }
}
