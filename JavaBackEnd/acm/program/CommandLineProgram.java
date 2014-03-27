// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   CommandLineProgram.java

package acm.program;

import acm.io.*;
import acm.util.ErrorException;
import acm.util.JTFTools;
import java.applet.AppletStub;
import java.awt.*;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.lang.reflect.Method;
import java.util.*;
import javax.swing.JPanel;

// Referenced classes of package acm.program:
//            CommandLineProgramLoader

public class CommandLineProgram
    implements IOModel, Runnable, MouseListener, MouseMotionListener, KeyListener, ActionListener
{

    protected CommandLineProgram()
    {
        parameterTable = null;
        finalizers = new ArrayList();
        myTitle = getClass().getName();
        myTitle = myTitle.substring(myTitle.lastIndexOf(".") + 1);
        setConsole(createConsole());
    }

    public static void checkIfHeadless(String s)
    {
        if(!JTFTools.testDebugOption("headless"))
            try
            {
                Class class1 = Class.forName("java.awt.GraphicsEnvironment");
                Method method = class1.getMethod("isHeadless", new Class[0]);
                if(!Boolean.TRUE.equals(method.invoke(null, new Object[0])))
                    return;
            }
            catch(Exception exception)
            {
                return;
            }
        try
        {
            CommandLineProgramLoader commandlineprogramloader = new CommandLineProgramLoader(s);
            Class class2 = commandlineprogramloader.loadClass(s);
            CommandLineProgram commandlineprogram = (CommandLineProgram)class2.newInstance();
            commandlineprogram.init();
            commandlineprogram.run();
            commandlineprogram.exit();
        }
        catch(Exception exception1)
        {
            throw new ErrorException(exception1);
        }
    }

    public void run()
    {
    }

    public void init()
    {
    }

    public void print(String s)
    {
        getOutputModel().print(s);
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
        getOutputModel().println();
    }

    public void println(String s)
    {
        getOutputModel().println(s);
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
        getOutputModel().showErrorMessage(s);
    }

    public final String readLine()
    {
        return readLine(null);
    }

    public String readLine(String s)
    {
        return getInputModel().readLine(s);
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
        return getInputModel().readInt(s, i, j);
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
        return getInputModel().readDouble(s, d, d1);
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
        return getInputModel().readBoolean(s, s1, s2);
    }

    public boolean isAppletMode()
    {
        return false;
    }

    public void setConsole(IOConsole ioconsole)
    {
        myConsole = ioconsole;
    }

    public IOConsole getConsole()
    {
        return myConsole;
    }

    public IODialog getDialog()
    {
        return null;
    }

    public IOModel getInputModel()
    {
        return getConsole();
    }

    public IOModel getOutputModel()
    {
        return getConsole();
    }

    public BufferedReader getReader()
    {
        return getConsole().getReader();
    }

    public PrintWriter getWriter()
    {
        return getConsole().getWriter();
    }

    public JPanel getRegionPanel(String s)
    {
        throw new ErrorException("No graphics environment");
    }

    public void add(Component component, String s, Object obj)
    {
        throw new ErrorException("No graphics environment");
    }

    public void addActionListeners()
    {
        throw new ErrorException("No graphics environment");
    }

    public void addActionListeners(ActionListener actionlistener)
    {
        throw new ErrorException("No graphics environment");
    }

    public void setTitle(String s)
    {
        myTitle = s;
    }

    public String getTitle()
    {
        return myTitle;
    }

    public void start(String as[])
    {
        if(parameterTable == null && as != null)
            parameterTable = createParameterTable(as);
        init();
        run();
    }

    public void exit()
    {
        int i = finalizers.size();
        for(int j = 0; j < i; j++)
        {
            Object obj = finalizers.get(j);
            try
            {
                Class class1 = obj.getClass();
                Method method = class1.getMethod("exit", new Class[0]);
                method.invoke(obj, new Object[0]);
            }
            catch(Exception exception)
            {
                throw new ErrorException(exception);
            }
        }

    }

    public void addExitHook(Object obj)
    {
        finalizers.add(obj);
    }

    public void mouseClicked(MouseEvent mouseevent)
    {
    }

    public void mousePressed(MouseEvent mouseevent)
    {
    }

    public void mouseReleased(MouseEvent mouseevent)
    {
    }

    public void mouseEntered(MouseEvent mouseevent)
    {
    }

    public void mouseExited(MouseEvent mouseevent)
    {
    }

    public void mouseMoved(MouseEvent mouseevent)
    {
    }

    public void mouseDragged(MouseEvent mouseevent)
    {
    }

    public void keyTyped(KeyEvent keyevent)
    {
    }

    public void keyPressed(KeyEvent keyevent)
    {
    }

    public void keyReleased(KeyEvent keyevent)
    {
    }

    public void actionPerformed(ActionEvent actionevent)
    {
    }

    protected IOConsole createConsole()
    {
        return IOConsole.SYSTEM_CONSOLE;
    }

    protected IODialog createDialogIO()
    {
        throw new ErrorException("No graphics environment");
    }

    public Dimension getPreferredSize()
    {
        return new Dimension(0, 0);
    }

    public int getWidth()
    {
        return 0;
    }

    public int getHeight()
    {
        return 0;
    }

    public String getParameter(String s)
    {
        String s1 = null;
        if(parameterTable != null)
            s1 = (String)parameterTable.get(s.toLowerCase());
        return s1;
    }

    public void setLayout(LayoutManager layoutmanager)
    {
        throw new ErrorException("No graphics environment");
    }

    public LayoutManager getLayout()
    {
        throw new ErrorException("No graphics environment");
    }

    public void setBackground(Color color)
    {
        throw new ErrorException("No graphics environment");
    }

    protected void addImpl(Component component, Object obj, int i)
    {
        throw new ErrorException("No graphics environment");
    }

    public void remove(int i)
    {
        throw new ErrorException("No graphics environment");
    }

    public void remove(Component component)
    {
        throw new ErrorException("No graphics environment");
    }

    public void removeAll()
    {
        throw new ErrorException("No graphics environment");
    }

    public void validate()
    {
        throw new ErrorException("No graphics environment");
    }

    public void repaint()
    {
        throw new ErrorException("No graphics environment");
    }

    public void destroy()
    {
    }

    public static void main(String args[])
    {
        HashMap hashmap = createParameterTable(args);
        JTFTools.setDebugOptions((String)hashmap.get("debug"));
        String s = (String)hashmap.get("code");
        if(s == null)
            s = JTFTools.getMainClass();
        Class class1 = null;
        CommandLineProgram commandlineprogram = null;
        if(s != null)
        {
            if(s.endsWith(".class"))
                s = s.substring(0, s.length() - 6);
            s = s.replace('/', '.');
            try
            {
                class1 = Class.forName(s);
            }
            catch(ClassNotFoundException classnotfoundexception) { }
        }
        if(class1 != null)
            try
            {
                Object obj = class1.newInstance();
                if(obj instanceof CommandLineProgram)
                    commandlineprogram = (CommandLineProgram)obj;
                else
                    throw new ErrorException("Main class does not specify a program");
            }
            catch(IllegalAccessException illegalaccessexception) { }
            catch(InstantiationException instantiationexception) { }
        if(commandlineprogram == null)
        {
            throw new ErrorException("Cannot determine the main class.");
        } else
        {
            commandlineprogram.setParameterTable(hashmap);
            commandlineprogram.start(null);
            return;
        }
    }

    public void menuAction(String s)
    {
        throw new ErrorException("No graphics environment");
    }

    protected void setMacMenuBarFlag(boolean flag)
    {
        throw new ErrorException("No graphics environment");
    }

    protected boolean getMacMenuBarFlag()
    {
        throw new ErrorException("No graphics environment");
    }

    protected Component getBorder(String s)
    {
        throw new ErrorException("No graphics environment");
    }

    protected String[] getArgumentArray()
    {
        if(parameterTable == null)
            return null;
        StringTokenizer stringtokenizer = new StringTokenizer((String)parameterTable.get("ARGS"), "\t", false);
        String as[] = new String[stringtokenizer.countTokens()];
        for(int i = 0; stringtokenizer.hasMoreTokens(); i++)
            as[i] = stringtokenizer.nextToken();

        return as;
    }

    protected boolean isStarted()
    {
        throw new ErrorException("No graphics environment");
    }

    protected void startHook()
    {
    }

    protected void endHook()
    {
    }

    protected void setAppletStub(AppletStub appletstub)
    {
        appletStub = appletstub;
    }

    protected AppletStub getAppletStub()
    {
        return appletStub;
    }

    protected void setParameterTable(HashMap hashmap)
    {
        parameterTable = hashmap;
    }

    protected HashMap getParameterTable()
    {
        return parameterTable;
    }

    protected static HashMap createParameterTable(String as[])
    {
        if(as == null)
            return null;
        HashMap hashmap = new HashMap();
        String s = "";
        for(int i = 0; i < as.length; i++)
        {
            String s1 = as[i];
            int j = s1.indexOf('=');
            if(j > 0)
            {
                String s2 = s1.substring(0, j).toLowerCase();
                String s3 = s1.substring(j + 1);
                hashmap.put(s2, s3);
                continue;
            }
            if(s.length() > 0)
                s = (new StringBuilder()).append(s).append('\t').toString();
            s = (new StringBuilder()).append(s).append(s1).toString();
        }

        hashmap.put("ARGS", s);
        return hashmap;
    }

    public static final String NORTH = "North";
    public static final String SOUTH = "South";
    public static final String EAST = "East";
    public static final String WEST = "West";
    public static final String CENTER = "Center";
    private HashMap parameterTable;
    private ArrayList finalizers;
    private AppletStub appletStub;
    private String myTitle;
    private IOConsole myConsole;
}
