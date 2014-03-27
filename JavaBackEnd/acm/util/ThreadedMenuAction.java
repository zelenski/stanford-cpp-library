// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   JTFTools.java

package acm.util;

import acm.io.IOConsole;
import acm.io.IOModel;
import acm.program.Program;
import java.io.*;

// Referenced classes of package acm.util:
//            ProgressBarDialog, ErrorException, JTFTools

class ThreadedMenuAction
    implements Runnable
{

    public ThreadedMenuAction(Program program, String s)
    {
        targetProgram = program;
        actionCommand = s;
    }

    public void run()
    {
        ProgressBarDialog progressbardialog = new ProgressBarDialog(targetProgram);
        try
        {
            if(actionCommand.equals("Export Applet"))
                JTFTools.exportApplet(targetProgram, progressbardialog.getProgressBar());
            else
            if(actionCommand.equals("Submit Project"))
                JTFTools.submitProject(targetProgram, progressbardialog.getProgressBar());
        }
        catch(Exception exception)
        {
            progressbardialog.setVisible(false);
            Object obj = targetProgram.getDialog();
            if(obj == null)
                obj = targetProgram.getConsole();
            if(obj == null)
                obj = IOConsole.SYSTEM_CONSOLE;
            String s = exception.getMessage();
            if(!(exception instanceof ErrorException))
            {
                s = (new StringBuilder()).append("").append(exception).toString();
                StringWriter stringwriter = new StringWriter();
                PrintWriter printwriter = new PrintWriter(stringwriter);
                exception.printStackTrace(printwriter);
                printwriter.close();
                try
                {
                    BufferedReader bufferedreader = new BufferedReader(new StringReader(stringwriter.toString()));
                    bufferedreader.readLine();
                    s = (new StringBuilder()).append(s).append(bufferedreader.readLine()).toString();
                }
                catch(IOException ioexception) { }
            }
            ((IOModel) (obj)).showErrorMessage(s);
        }
    }

    private Program targetProgram;
    private String actionCommand;
}
