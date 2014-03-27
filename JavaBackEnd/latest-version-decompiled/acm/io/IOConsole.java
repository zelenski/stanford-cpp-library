// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOConsole.java

package acm.io;

import acm.program.ProgramMenuBar;
import acm.util.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.*;
import java.util.Iterator;
import javax.swing.JMenuItem;

// Referenced classes of package acm.io:
//            ConsoleReader, ConsoleWriter, IODialog, StandardConsoleModel, 
//            SystemConsole, IOModel, ConsoleModel

public class IOConsole extends Container
    implements IOModel
{

    public IOConsole()
    {
        consoleModel = createConsoleModel();
        consoleModel.setConsole(this);
        setBackground(Color.WHITE);
        setInputColor(Color.BLUE);
        setInputStyle(1);
        setErrorColor(Color.RED);
        setErrorStyle(1);
        setFont(JTFTools.getStandardFont(DEFAULT_FONT));
        Component component = consoleModel.getConsolePane();
        if(component != null)
        {
            setLayout(new BorderLayout());
            add(component, "Center");
        }
        reader = null;
        writer = null;
        exceptionOnError = false;
    }

    public void clear()
    {
        consoleModel.clear();
    }

    public void print(String s)
    {
        getWriter().print(s);
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
        getWriter().println();
    }

    public void println(String s)
    {
        getWriter().println(s);
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
        consoleModel.print(s, 2);
        consoleModel.print("\n", 2);
    }

    public final String readLine()
    {
        return readLine(null);
    }

    public String readLine(String s)
    {
        if(s != null)
            print(s);
        consoleModel.requestFocus();
        String s1 = getReader().readLine();
        return s1;
        IOException ioexception;
        ioexception;
        throw new ErrorException(ioexception);
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
        Object obj = null;
_L2:
        String s2 = readLine(s);
        int k = Integer.parseInt(s2);
        if(k >= i && k <= j)
            return k;
        String s1;
        try
        {
            s1 = (new StringBuilder()).append("Value is outside the range [").append(i).append(":").append(j).append("]").toString();
        }
        catch(NumberFormatException numberformatexception)
        {
            s1 = "Illegal numeric format";
        }
        showErrorMessage(s1);
        if(s == null)
            s = "Retry: ";
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
        Object obj = null;
_L2:
        String s2 = readLine(s);
        double d2 = Double.valueOf(s2).doubleValue();
        if(d2 >= d && d2 <= d1)
            return d2;
        String s1;
        try
        {
            s1 = (new StringBuilder()).append("Value is outside the range [").append(d).append(":").append(d1).append("]").toString();
        }
        catch(NumberFormatException numberformatexception)
        {
            s1 = "Illegal numeric format";
        }
        showErrorMessage(s1);
        if(s == null)
            s = "Retry: ";
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
        do
        {
            do
            {
                String s3 = readLine(s);
                if(s3 == null)
                    throw new ErrorException("End of file encountered");
                if(s3.equalsIgnoreCase(s1))
                    return true;
                if(s3.equalsIgnoreCase(s2))
                    return false;
                if(exceptionOnError)
                    throw new ErrorException("Illegal boolean format");
                showErrorMessage("Illegal boolean format");
            } while(s != null);
            s = "Retry: ";
        } while(true);
    }

    public BufferedReader getReader()
    {
        if(reader == null)
            reader = new BufferedReader(new ConsoleReader(consoleModel));
        return reader;
    }

    public PrintWriter getWriter()
    {
        if(writer == null)
            writer = new PrintWriter(new ConsoleWriter(consoleModel));
        return writer;
    }

    public void setExceptionOnError(boolean flag)
    {
        exceptionOnError = flag;
    }

    public boolean getExceptionOnError()
    {
        return exceptionOnError;
    }

    public void setInputStyle(int i)
    {
        inputStyle = i;
        consoleModel.setInputStyle(i);
    }

    public int getInputStyle()
    {
        return inputStyle;
    }

    public void setInputColor(Color color)
    {
        inputColor = color;
        consoleModel.setInputColor(color);
    }

    public Color getInputColor()
    {
        return inputColor;
    }

    public void setErrorStyle(int i)
    {
        errorStyle = i;
        consoleModel.setErrorStyle(i);
    }

    public int getErrorStyle()
    {
        return errorStyle;
    }

    public void setErrorColor(Color color)
    {
        errorColor = color;
        consoleModel.setErrorColor(color);
    }

    public Color getErrorColor()
    {
        return errorColor;
    }

    public void setFont(String s)
    {
        setFont(JTFTools.decodeFont(s, getFont()));
    }

    public void setInputScript(BufferedReader bufferedreader)
    {
        consoleModel.setInputScript(bufferedreader);
    }

    public BufferedReader getInputScript()
    {
        return consoleModel.getInputScript();
    }

    public void cut()
    {
        consoleModel.cut();
    }

    public void copy()
    {
        consoleModel.copy();
    }

    public void paste()
    {
        consoleModel.paste();
    }

    public void selectAll()
    {
        consoleModel.selectAll();
    }

    public void save()
    {
        FileWriter filewriter = null;
_L2:
        if(filewriter != null)
            break; /* Loop/switch isn't completed */
        java.awt.Frame frame;
        if(file != null)
            break MISSING_BLOCK_LABEL_85;
        frame = JTFTools.getEnclosingFrame(this);
        if(frame == null)
            return;
        FileDialog filedialog;
        String s1;
        String s = System.getProperty("user.dir");
        filedialog = new FileDialog(frame, "Save Console As", 1);
        filedialog.setDirectory(s);
        filedialog.setVisible(true);
        s1 = filedialog.getFile();
        if(s1 == null)
            return;
        file = new File(filedialog.getDirectory(), s1);
        filewriter = new FileWriter(file);
        save(((Writer) (filewriter)));
        filewriter.close();
        Platform.setFileTypeAndCreator(file, "TEXT", "ttxt");
        continue; /* Loop/switch isn't completed */
        IOException ioexception;
        ioexception;
        IODialog iodialog = new IODialog(this);
        iodialog.showErrorMessage(ioexception.getMessage());
        if(true) goto _L2; else goto _L1
_L1:
    }

    public void saveAs()
    {
        file = null;
        save();
    }

    public void save(Writer writer1)
    {
        try
        {
            writer1.write(consoleModel.getText());
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public void printConsole()
    {
        java.awt.Frame frame = JTFTools.getEnclosingFrame(this);
        if(frame == null)
            return;
        PrintJob printjob = getToolkit().getPrintJob(frame, "Console", null);
        if(printjob == null)
        {
            return;
        } else
        {
            printConsole(printjob);
            printjob.end();
            return;
        }
    }

    public void printConsole(PrintJob printjob)
    {
        consoleModel.print(printjob);
    }

    public void script()
    {
        java.awt.Frame frame = JTFTools.getEnclosingFrame(this);
        FileDialog filedialog = new FileDialog(frame, "Input Script", 0);
        filedialog.setDirectory(System.getProperty("user.dir"));
        filedialog.setVisible(true);
        String s = filedialog.getDirectory();
        String s1 = filedialog.getFile();
        if(s1 != null)
            try
            {
                FileReader filereader = new FileReader(new File(new File(s), s1));
                setInputScript(new BufferedReader(filereader));
            }
            catch(IOException ioexception)
            {
                throw new ErrorException(ioexception);
            }
    }

    public void setMenuBar(ProgramMenuBar programmenubar)
    {
        menuBar = programmenubar;
        consoleModel.setMenuBar(programmenubar);
    }

    public ProgramMenuBar getMenuBar()
    {
        return menuBar;
    }

    public boolean menuAction(ActionEvent actionevent)
    {
        String s = actionevent.getActionCommand();
        if(s.equals("Cut"))
        {
            cut();
            return true;
        }
        if(s.equals("Copy"))
        {
            copy();
            return true;
        }
        if(s.equals("Paste"))
        {
            paste();
            return true;
        }
        if(s.equals("Select All"))
        {
            selectAll();
            return true;
        }
        if(s.equals("Save"))
        {
            save();
            return true;
        }
        if(s.equals("Save As"))
        {
            saveAs();
            return true;
        }
        if(s.equals("Script"))
        {
            script();
            return true;
        }
        if(s.equals("Print Console"))
        {
            printConsole();
            return true;
        } else
        {
            return false;
        }
    }

    public boolean isConsoleMenuItem(JMenuItem jmenuitem)
    {
        String s = jmenuitem.getActionCommand();
        if(s == null)
            return false;
        for(int i = 0; i < CONSOLE_MENU_ACTIONS.length; i++)
            if(s.equals(CONSOLE_MENU_ACTIONS[i]))
                return true;

        return false;
    }

    public void updateMenuBar(ProgramMenuBar programmenubar)
    {
        for(Iterator iterator = programmenubar.iterator(); iterator.hasNext();)
        {
            JMenuItem jmenuitem = (JMenuItem)iterator.next();
            if(isConsoleMenuItem(jmenuitem))
                jmenuitem.setEnabled(true);
            else
                jmenuitem.setEnabled(!programmenubar.isFocusedItem(jmenuitem));
        }

    }

    public void setBackground(Color color)
    {
        Component component = consoleModel.getTextPane();
        if(component != null)
            component.setBackground(color);
        super.setBackground(color);
    }

    public void setForeground(Color color)
    {
        Component component = consoleModel.getTextPane();
        if(component != null)
            component.setForeground(color);
        super.setForeground(color);
    }

    public void setFont(Font font)
    {
        font = JTFTools.getStandardFont(font);
        Component component = consoleModel.getTextPane();
        if(component != null)
            component.setFont(font);
        super.setFont(font);
    }

    public void requestFocus()
    {
        consoleModel.requestFocus();
    }

    public Dimension getPreferredSize()
    {
        return getMinimumSize();
    }

    public Dimension getMinimumSize()
    {
        return new Dimension(50, 40);
    }

    protected ConsoleModel createConsoleModel()
    {
        return new StandardConsoleModel();
    }

    public static final IOConsole SYSTEM_CONSOLE = new SystemConsole();
    protected static final Font DEFAULT_FONT = new Font("Monospaced", 0, 12);
    protected static final String LINE_SEPARATOR = System.getProperty("line.separator");
    protected static final int MINIMUM_CONSOLE_WIDTH = 50;
    protected static final int MINIMUM_CONSOLE_HEIGHT = 40;
    private static final String CONSOLE_MENU_ACTIONS[] = {
        "Save", "Save As", "Print Console", "Script", "Cut", "Copy", "Paste", "Select All"
    };
    private ConsoleModel consoleModel;
    private boolean exceptionOnError;
    private Color inputColor;
    private int inputStyle;
    private Color errorColor;
    private int errorStyle;
    private BufferedReader reader;
    private PrintWriter writer;
    private File file;
    private ProgramMenuBar menuBar;

}
