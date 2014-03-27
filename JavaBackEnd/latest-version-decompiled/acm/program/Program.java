// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Program.java

package acm.program;

import acm.gui.TableLayout;
import acm.io.*;
import acm.util.*;
import java.applet.AppletStub;
import java.awt.*;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.*;
import javax.swing.*;

// Referenced classes of package acm.program:
//            ProgramAppletStub, ProgramFrame, ProgramMenuBar, AppletStarter, 
//            ProgramStartupListener, ProgramContentPaneLayout, ProgramWindowListener, CommandLineProgram

public abstract class Program extends JApplet
    implements IOModel, Runnable, MouseListener, MouseMotionListener, KeyListener, ActionListener
{

    protected Program()
    {
        JTFTools.registerApplet(this);
        appletMode = checkForAppletMode();
        shown = false;
        parameterTable = null;
        finalizers = new ArrayList();
        myTitle = getClass().getName();
        myTitle = myTitle.substring(myTitle.lastIndexOf(".") + 1);
        appletStub = new ProgramAppletStub(this);
        setAppletStub(appletStub);
        initContentPane(getContentPane());
        setVisible(false);
        setConsole(createConsole());
        myDialog = createDialogIO();
        myDialog.setAssociatedConsole(myConsole);
        myMenuBar = createMenuBar();
        myConsole.setMenuBar(myMenuBar);
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
        return appletMode;
    }

    public void setConsole(IOConsole ioconsole)
    {
        myConsole = ioconsole;
    }

    public IOConsole getConsole()
    {
        return myConsole;
    }

    public void setDialog(IODialog iodialog)
    {
        myDialog = iodialog;
    }

    public IODialog getDialog()
    {
        return myDialog;
    }

    public void setInputModel(IOModel iomodel)
    {
        inputModel = iomodel;
    }

    public void setOutputModel(IOModel iomodel)
    {
        outputModel = iomodel;
    }

    public IOModel getInputModel()
    {
        return ((IOModel) (inputModel != null ? inputModel : myConsole));
    }

    public IOModel getOutputModel()
    {
        return ((IOModel) (outputModel != null ? outputModel : myConsole));
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
        if(s.equals("North"))
            return northPanel;
        if(s.equals("South"))
            return southPanel;
        if(s.equals("West"))
            return westPanel;
        if(s.equals("East"))
            return eastPanel;
        if(s.equals("Center"))
            return centerPanel;
        else
            throw new ErrorException((new StringBuilder()).append("getRegionPanel: Illegal region ").append(s).toString());
    }

    public void add(Component component, String s, Object obj)
    {
        if(s.equals("North"))
            northPanel.add(component, obj);
        else
        if(s.equals("South"))
            southPanel.add(component, obj);
        else
        if(s.equals("West"))
            westPanel.add(component, obj);
        else
        if(s.equals("East"))
            eastPanel.add(component, obj);
        else
        if(s.equals("Center"))
            centerPanel.add(component, obj);
        else
            throw new ErrorException((new StringBuilder()).append("add: Illegal region ").append(s).toString());
    }

    public void addActionListeners()
    {
        addActionListeners(((ActionListener) (this)));
    }

    public void addActionListeners(ActionListener actionlistener)
    {
        addActionListeners(((Component) (getContentPane())), actionlistener);
    }

    public void setTitle(String s)
    {
        myTitle = s;
        if(programFrame != null)
            programFrame.setTitle(s);
    }

    public String getTitle()
    {
        return myTitle;
    }

    public ProgramMenuBar getMenuBar()
    {
        return myMenuBar;
    }

    public void start(String as[])
    {
        if(parameterTable == null)
            parameterTable = createParameterTable(as);
        if(getParent() == null)
            initApplicationFrame();
        validate();
        setVisible(true);
        if(programFrame != null)
        {
            programFrame.validate();
            int i = centerPanel.getComponentCount();
            i += northPanel.getComponentCount();
            i += southPanel.getComponentCount();
            i += westPanel.getComponentCount();
            i += eastPanel.getComponentCount();
            if(i > 0)
            {
                programFrame.setVisible(true);
                shown = true;
            }
        }
        started = true;
        init();
        if(programFrame != null && myMenuBar != null)
        {
            myMenuBar.install(programFrame);
            circumventFrameSizeBug(programFrame, programBounds.getSize());
        }
        validate();
        startRun();
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

        JTFTools.terminateAppletThreads(this);
        if(!appletMode)
            System.exit(0);
    }

    public void addExitHook(Object obj)
    {
        finalizers.add(obj);
    }

    public void setParameter(String s, String s1)
    {
        if(parameterTable == null)
            parameterTable = new HashMap();
        parameterTable.put(s.toLowerCase(), s1);
    }

    public Thread getMainThread()
    {
        return appletStarter != null ? appletStarter.getMainThread() : null;
    }

    public void pause(double d)
    {
        JTFTools.pause(d);
    }

    public Dimension getCentralRegionSize()
    {
        if(centerPanel == null)
            return super.getSize();
        if(initFinished)
        {
            return centerPanel.getSize();
        } else
        {
            Dimension dimension = programFrame != null ? programFrame.getSize() : super.getSize();
            Insets insets = programFrame != null ? programFrame.getInsets() : super.getInsets();
            dimension.width -= westPanel.getPreferredSize().width + eastPanel.getPreferredSize().width;
            dimension.width -= insets.left + insets.right;
            dimension.height -= northPanel.getPreferredSize().height + southPanel.getPreferredSize().height;
            dimension.height -= insets.top + insets.bottom;
            return dimension;
        }
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

    protected JFrame createProgramFrame()
    {
        return new ProgramFrame(getTitle());
    }

    protected IOConsole createConsole()
    {
        return IOConsole.SYSTEM_CONSOLE;
    }

    protected IODialog createDialogIO()
    {
        return new IODialog(getContentPane());
    }

    protected ProgramMenuBar createMenuBar()
    {
        return new ProgramMenuBar(this);
    }

    public Dimension getPreferredSize()
    {
        return computeProgramBounds().getSize();
    }

    public int getWidth()
    {
        String s = getMyCaller();
        if(s.startsWith("java.") || s.startsWith("javax."))
            return super.getWidth();
        else
            return getCentralRegionSize().width;
    }

    public int getHeight()
    {
        String s = getMyCaller();
        if(s.startsWith("java.") || s.startsWith("javax."))
            return super.getHeight();
        else
            return getCentralRegionSize().height;
    }

    public String getParameter(String s)
    {
        String s1 = null;
        if(parameterTable != null)
            s1 = (String)parameterTable.get(s.toLowerCase());
        if(s1 != null)
            return s1;
        else
            return super.getParameter(s);
    }

    public void setLayout(LayoutManager layoutmanager)
    {
        if(isRootPaneCheckingEnabled())
            centerPanel.setLayout(layoutmanager);
        else
            super.setLayout(layoutmanager);
    }

    public LayoutManager getLayout()
    {
        if(isRootPaneCheckingEnabled())
            return centerPanel.getLayout();
        else
            return super.getLayout();
    }

    public void setBackground(Color color)
    {
        if(isRootPaneCheckingEnabled())
            centerPanel.setBackground(color);
        super.setBackground(color);
    }

    protected void addImpl(Component component, Object obj, int i)
    {
        if(isRootPaneCheckingEnabled())
        {
            if(obj == null)
                centerPanel.add(component, i);
            else
            if(obj.equals("North"))
                northPanel.add(component, i);
            else
            if(obj.equals("South"))
                southPanel.add(component, i);
            else
            if(obj.equals("West"))
                westPanel.add(component, i);
            else
            if(obj.equals("East"))
                eastPanel.add(component, i);
            else
            if(obj.equals("Center"))
                centerPanel.add(component, i);
            else
                centerPanel.add(component, obj, i);
            if(!shown && programFrame != null)
            {
                programFrame.setVisible(true);
                shown = true;
            }
        } else
        {
            super.addImpl(component, obj, i);
        }
    }

    public void remove(int i)
    {
        if(isRootPaneCheckingEnabled())
            centerPanel.remove(i);
        else
            super.remove(i);
    }

    public void remove(Component component)
    {
        if(isRootPaneCheckingEnabled())
            centerPanel.remove(component);
        else
            super.remove(component);
    }

    public void removeAll()
    {
        if(isRootPaneCheckingEnabled())
            centerPanel.removeAll();
        else
            super.removeAll();
    }

    public void validate()
    {
        if(isRootPaneCheckingEnabled())
            getContentPane().validate();
        super.validate();
    }

    public void repaint()
    {
        if(isRootPaneCheckingEnabled())
            getContentPane().repaint();
        super.repaint();
    }

    public final void start()
    {
        appletMode = getParent() != null;
        if(appletMode)
        {
            if(!started)
            {
                started = true;
                validate();
                setVisible(true);
                appletStarter = new AppletStarter(this);
                appletStarter.start();
            }
        } else
        {
            start(null);
        }
    }

    public void destroy()
    {
        Animator.shutdown(this);
        if(appletStarter != null)
            appletStarter.stop();
    }

    public static void main(String args[])
    {
        HashMap hashmap = createParameterTable(args);
        JTFTools.setDebugOptions((String)hashmap.get("debug"));
        String s = (String)hashmap.get("code");
        if(s == null)
            s = JTFTools.getMainClass();
        Class class1 = null;
        Program program = null;
        if(s != null)
        {
            if(s.endsWith(".class"))
                s = s.substring(0, s.length() - 6);
            s = s.replace('/', '.');
            CommandLineProgram.checkIfHeadless(s);
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
                if(obj instanceof Program)
                {
                    program = (Program)obj;
                    program.setStartupObject(null);
                } else
                {
                    String s1 = (String)hashmap.get("program");
                    if(s1 == null)
                        throw new ErrorException("Main class does not specify a program");
                    program = (Program)Class.forName(s1).newInstance();
                    program.setStartupObject(obj);
                }
            }
            catch(IllegalAccessException illegalaccessexception) { }
            catch(InstantiationException instantiationexception) { }
            catch(ClassNotFoundException classnotfoundexception1) { }
        if(program == null)
        {
            throw new ErrorException("Cannot determine the main class.");
        } else
        {
            program.setParameterTable(hashmap);
            program.start();
            return;
        }
    }

    public boolean menuAction(ActionEvent actionevent)
    {
        String s = actionevent.getActionCommand();
        if(s.equals("Quit"))
        {
            exit();
        } else
        {
            if(s.equals("Print"))
            {
                Frame frame = JTFTools.getEnclosingFrame(this);
                if(frame == null)
                    return true;
                PrintJob printjob = frame.getToolkit().getPrintJob(frame, myTitle, null);
                if(printjob == null)
                {
                    return true;
                } else
                {
                    Graphics g = printjob.getGraphics();
                    g.translate(36, 36);
                    frame.printAll(g);
                    printjob.end();
                    return true;
                }
            }
            if(s.equals("Export Applet") || s.equals("Submit Project"))
            {
                JTFTools.executeExportAction(this, s);
                return true;
            }
        }
        return getConsole().menuAction(actionevent);
    }

    protected Component getBorder(String s)
    {
        if(s.equals("North"))
            return northBorder;
        if(s.equals("South"))
            return southBorder;
        if(s.equals("East"))
            return eastBorder;
        if(s.equals("West"))
            return westBorder;
        else
            throw new ErrorException((new StringBuilder()).append("Illegal border specification - ").append(s).toString());
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
        IOConsole ioconsole = getConsole();
        if(ioconsole == null)
            return false;
        if(ioconsole.getParent() == null)
        {
            return true;
        } else
        {
            Dimension dimension = ioconsole.getSize();
            return ioconsole.isShowing() && dimension.width != 0 && dimension.height != 0;
        }
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
        setStub(appletstub);
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

    protected void setStartupObject(Object obj)
    {
        startupObject = obj;
    }

    protected Object getStartupObject()
    {
        return startupObject;
    }

    protected void startRun()
    {
        ProgramStartupListener programstartuplistener = new ProgramStartupListener();
        javax.swing.JRootPane jrootpane = getRootPane();
        if(jrootpane.isShowing())
        {
            jrootpane.addComponentListener(programstartuplistener);
            jrootpane.validate();
            programstartuplistener.waitForStartup(this);
            jrootpane.update(jrootpane.getGraphics());
        }
        jrootpane.setCursor(Cursor.getDefaultCursor());
        initFinished = true;
        startHook();
        runHook();
        endHook();
        if(!jrootpane.isShowing() && !getContentPane().isShowing())
            exit();
    }

    protected void runHook()
    {
        run();
    }

    protected String getMyCaller()
    {
        StackTraceElement astacktraceelement[] = (new Throwable()).getStackTrace();
        return (new StringBuilder()).append(astacktraceelement[2].getClassName()).append(".").append(astacktraceelement[2].getMethodName()).toString();
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

    private void initContentPane(Container container)
    {
        container.setLayout(new ProgramContentPaneLayout(this));
        northPanel = new JPanel();
        southPanel = new JPanel();
        eastPanel = new JPanel();
        westPanel = new JPanel();
        centerPanel = new JPanel();
        northPanel.setLayout(new TableLayout(1, 0, 5, 5));
        southPanel.setLayout(new TableLayout(1, 0, 5, 5));
        westPanel.setLayout(new TableLayout(0, 1, 5, 5));
        eastPanel.setLayout(new TableLayout(0, 1, 5, 5));
        centerPanel.setLayout(new GridLayout(1, 0));
        container.add(northPanel, "North");
        container.add(southPanel, "South");
        container.add(eastPanel, "East");
        container.add(westPanel, "West");
        container.add(centerPanel, "Center");
    }

    private void addActionListeners(Component component, ActionListener actionlistener)
    {
        if(isButton(component))
        {
            if(!hasActionListener(component))
                try
                {
                    Class aclass[] = {
                        Class.forName("java.awt.event.ActionListener")
                    };
                    Object aobj[] = {
                        actionlistener
                    };
                    Method method = component.getClass().getMethod("addActionListener", aclass);
                    method.invoke(component, aobj);
                }
                catch(Exception exception)
                {
                    throw new ErrorException(exception);
                }
        } else
        if(component instanceof Container)
        {
            Container container = (Container)component;
            int i = container.getComponentCount();
            for(int j = 0; j < i; j++)
                addActionListeners(container.getComponent(j), actionlistener);

        }
    }

    private boolean isButton(Component component)
    {
        return (component instanceof Button) || (component instanceof JButton);
    }

    private boolean hasActionListener(Component component)
    {
        ActionListener aactionlistener[];
        Method method = component.getClass().getMethod("getActionListeners", new Class[0]);
        aactionlistener = (ActionListener[])(ActionListener[])method.invoke(component, new Object[0]);
        return aactionlistener.length > 0;
        Exception exception;
        exception;
        return false;
    }

    private void initApplicationFrame()
    {
        programFrame = createProgramFrame();
        ((ProgramAppletStub)appletStub).setFrame(programFrame);
        Container container = programFrame.getContentPane();
        container.setLayout(new BorderLayout());
        container.add(getContentPane(), "Center");
        programFrame.addWindowListener(new ProgramWindowListener(this));
        programBounds = computeProgramBounds();
        Insets insets = programFrame.getInsets();
        int i = programBounds.width + insets.left + insets.right;
        int j = programBounds.height + insets.top + insets.bottom;
        programFrame.setBounds(programBounds.x, programBounds.y, i, j);
    }

    private int decodeSizeParameter(String s, int i, int j)
    {
        String s1;
        s1 = getParameter(s);
        if(s1 != null)
            break MISSING_BLOCK_LABEL_97;
        Object obj;
        Class class1 = getClass();
        Field field = class1.getField((new StringBuilder()).append("APPLICATION_").append(s).toString());
        obj = field.get(null);
        if(obj instanceof Integer)
            return ((Integer)obj).intValue();
        if(obj instanceof String)
        {
            s1 = (String)obj;
            break MISSING_BLOCK_LABEL_97;
        }
        return i;
        Exception exception;
        exception;
        return i;
        if(s1.equals("*"))
            s1 = "100%";
        if(s1.endsWith("%"))
        {
            int k = Integer.parseInt(s1.substring(0, s1.length() - 1));
            return (int)Math.round(((double)k / 100D) * (double)j);
        } else
        {
            return Integer.parseInt(s1);
        }
    }

    private Rectangle computeProgramBounds()
    {
        Dimension dimension = Toolkit.getDefaultToolkit().getScreenSize();
        int i = decodeSizeParameter("WIDTH", 754, dimension.width);
        int j = decodeSizeParameter("HEIGHT", 492, dimension.height);
        int k = decodeSizeParameter("X", i < dimension.width ? 16 : 0, dimension.width);
        int l = decodeSizeParameter("Y", j < dimension.height ? 40 : 0, dimension.height);
        return new Rectangle(k, l, i, j);
    }

    private boolean checkForAppletMode()
    {
        StackTraceElement astacktraceelement[] = (new Throwable()).getStackTrace();
        for(int i = 1; i < astacktraceelement.length; i++)
            if(astacktraceelement[i].getMethodName().indexOf("Applet") >= 0)
                return true;

        return false;
    }

    private void circumventFrameSizeBug(Frame frame, Dimension dimension)
    {
        Container container = getContentPane();
        Dimension dimension1 = container.getSize();
        if(dimension.equals(dimension1) || dimension1.width == 0 || dimension1.height == 0)
        {
            return;
        } else
        {
            Dimension dimension2 = frame.getSize();
            dimension2.width += dimension.width - dimension1.width;
            dimension2.height += dimension.height - dimension1.height;
            frame.setSize(dimension2.width, dimension2.height);
            frame.validate();
            return;
        }
    }

    public static final String NORTH = "North";
    public static final String SOUTH = "South";
    public static final String EAST = "East";
    public static final String WEST = "West";
    public static final String CENTER = "Center";
    private static final int DEFAULT_X = 16;
    private static final int DEFAULT_Y = 40;
    private static final int DEFAULT_WIDTH = 754;
    private static final int DEFAULT_HEIGHT = 492;
    private static final int PRINT_MARGIN = 36;
    private ArrayList finalizers;
    private HashMap parameterTable;
    private JFrame programFrame;
    private AppletStub appletStub;
    private String myTitle;
    private ProgramMenuBar myMenuBar;
    private Component northBorder;
    private Component southBorder;
    private Component eastBorder;
    private Component westBorder;
    private JPanel northPanel;
    private JPanel southPanel;
    private JPanel eastPanel;
    private JPanel westPanel;
    private JPanel centerPanel;
    private IOConsole myConsole;
    private IODialog myDialog;
    private IOModel inputModel;
    private IOModel outputModel;
    private Object startupObject;
    private AppletStarter appletStarter;
    private Rectangle programBounds;
    private boolean started;
    private boolean shown;
    private boolean initFinished;
    private boolean appletMode;
}
