// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   JTFTools.java

package acm.util;

import acm.program.Program;
import java.applet.Applet;
import java.awt.*;
import java.io.*;
import java.lang.reflect.Method;
import java.net.InetAddress;
import java.util.*;
import java.util.zip.*;
import javax.swing.JFileChooser;
import javax.swing.JProgressBar;

// Referenced classes of package acm.util:
//            EmptyContainer, ErrorException, FileChooserFilter, MailStream, 
//            HexByteOutputStream, Base64OutputStream, ThreadedMenuAction, ExportAppletDialog, 
//            NullOutputStream, SubmitOptions, Platform, DOSCommandLine, 
//            ProgressBarDialog

public class JTFTools
{

    private JTFTools()
    {
    }

    public static void pause(double d)
    {
        Applet applet = (Applet)appletTable.get(Thread.currentThread());
        if(applet == null)
        {
            Applet applet1 = mostRecentApplet;
            appletTable.put(Thread.currentThread(), applet1);
        }
        try
        {
            int i = (int)d;
            int j = (int)Math.round((d - (double)i) * 1000000D);
            Thread.sleep(i, j);
        }
        catch(InterruptedException interruptedexception) { }
    }

    public static String toHexString(int i)
    {
        return Integer.toHexString(i).toUpperCase();
    }

    public static String toHexString(int i, int j)
    {
        String s;
        for(s = toHexString(i); s.length() < j; s = (new StringBuilder()).append("0").append(s).toString());
        return s;
    }

    public static Container createEmptyContainer()
    {
        return new EmptyContainer();
    }

    public static Frame getEnclosingFrame(Component component)
    {
        if(component instanceof Program)
            component = ((Program)component).getContentPane();
        for(; component != null && !(component instanceof Frame); component = component.getParent());
        return (Frame)component;
    }

    public static Font getStandardFont(Font font)
    {
        if(!fontFamilyTableInitialized)
            initFontFamilyTable();
        if(font == null || fontFamilyTable == null)
            return font;
        String s = font.getFamily();
        if(fontFamilyTable.get(trimFamilyName(s)) != null)
            return font;
        if(s.equals("Serif") || s.equals("Times"))
            s = getFirstAvailableFontSubstitution(SERIF_SUBSTITUTIONS);
        else
        if(s.equals("SansSerif"))
            s = getFirstAvailableFontSubstitution(SANSSERIF_SUBSTITUTIONS);
        else
        if(s.equals("Monospaced"))
            s = getFirstAvailableFontSubstitution(MONOSPACED_SUBSTITUTIONS);
        else
            return font;
        if(s == null)
            return font;
        else
            return new Font(s, font.getStyle(), font.getSize());
    }

    public static String[] getFontList()
    {
        if(!fontFamilyTableInitialized)
            initFontFamilyTable();
        return fontFamilyArray;
    }

    public static String findFontFamily(String s)
    {
        if(!fontFamilyTableInitialized)
            initFontFamilyTable();
        for(StringTokenizer stringtokenizer = new StringTokenizer(s, ";", false); stringtokenizer.hasMoreTokens();)
        {
            String s1 = (String)fontFamilyTable.get(trimFamilyName(stringtokenizer.nextToken()));
            if(s1 != null)
                return s1;
        }

        return null;
    }

    public static Font decodeFont(String s)
    {
        return decodeFont(s, null);
    }

    public static Font decodeFont(String s, Font font)
    {
        String s1 = s;
        int i = font != null ? font.getStyle() : 0;
        int j = font != null ? font.getSize() : 12;
        int k = s.indexOf('-');
        if(k >= 0)
        {
            s1 = s.substring(0, k);
            s = s.substring(k + 1).toLowerCase();
            String s2 = s;
            k = s.indexOf('-');
            if(k >= 0)
            {
                s2 = s.substring(0, k);
                s = s.substring(k + 1);
            } else
            {
                s = "*";
            }
            if(Character.isDigit(s2.charAt(0)))
            {
                String s3 = s2;
                s2 = s;
                s = s3;
            }
            if(s2.equals("plain"))
                i = 0;
            else
            if(s2.equals("bold"))
                i = 1;
            else
            if(s2.equals("italic"))
                i = 2;
            else
            if(s2.equals("bolditalic"))
                i = 3;
            else
            if(!s2.equals("*"))
                throw new ErrorException("Illegal font style");
            if(!s.equals("*"))
                try
                {
                    j = Integer.valueOf(s).intValue();
                }
                catch(NumberFormatException numberformatexception)
                {
                    throw new ErrorException("Illegal font size");
                }
        }
        if(s1.equals("*"))
        {
            s1 = font != null ? font.getName() : "Default";
        } else
        {
            if(!fontFamilyTableInitialized)
                initFontFamilyTable();
            if(fontFamilyTable != null)
            {
                s1 = (String)fontFamilyTable.get(trimFamilyName(s1));
                if(s1 == null)
                    s1 = "Default";
            }
        }
        return getStandardFont(new Font(s1, i, j));
    }

    public static Color decodeColor(String s)
    {
        if(s.equalsIgnoreCase("desktop"))
            return SystemColor.desktop;
        if(s.equalsIgnoreCase("activeCaption"))
            return SystemColor.activeCaption;
        if(s.equalsIgnoreCase("activeCaptionText"))
            return SystemColor.activeCaptionText;
        if(s.equalsIgnoreCase("activeCaptionBorder"))
            return SystemColor.activeCaptionBorder;
        if(s.equalsIgnoreCase("inactiveCaption"))
            return SystemColor.inactiveCaption;
        if(s.equalsIgnoreCase("inactiveCaptionText"))
            return SystemColor.inactiveCaptionText;
        if(s.equalsIgnoreCase("inactiveCaptionBorder"))
            return SystemColor.inactiveCaptionBorder;
        if(s.equalsIgnoreCase("window"))
            return SystemColor.window;
        if(s.equalsIgnoreCase("windowBorder"))
            return SystemColor.windowBorder;
        if(s.equalsIgnoreCase("windowText"))
            return SystemColor.windowText;
        if(s.equalsIgnoreCase("menu"))
            return SystemColor.menu;
        if(s.equalsIgnoreCase("menuText"))
            return SystemColor.menuText;
        if(s.equalsIgnoreCase("text"))
            return SystemColor.text;
        if(s.equalsIgnoreCase("textText"))
            return SystemColor.textText;
        if(s.equalsIgnoreCase("textHighlight"))
            return SystemColor.textHighlight;
        if(s.equalsIgnoreCase("textHighlightText"))
            return SystemColor.textHighlightText;
        if(s.equalsIgnoreCase("textInactiveText"))
            return SystemColor.textInactiveText;
        if(s.equalsIgnoreCase("control"))
            return SystemColor.control;
        if(s.equalsIgnoreCase("controlText"))
            return SystemColor.controlText;
        if(s.equalsIgnoreCase("controlHighlight"))
            return SystemColor.controlHighlight;
        if(s.equalsIgnoreCase("controlLtHighlight"))
            return SystemColor.controlLtHighlight;
        if(s.equalsIgnoreCase("controlShadow"))
            return SystemColor.controlShadow;
        if(s.equalsIgnoreCase("controlDkShadow"))
            return SystemColor.controlDkShadow;
        if(s.equalsIgnoreCase("scrollbar"))
            return SystemColor.scrollbar;
        if(s.equalsIgnoreCase("info"))
            return SystemColor.info;
        if(s.equalsIgnoreCase("infoText"))
            return SystemColor.infoText;
        if(s.equalsIgnoreCase("black"))
            return Color.BLACK;
        if(s.equalsIgnoreCase("blue"))
            return Color.BLUE;
        if(s.equalsIgnoreCase("cyan"))
            return Color.CYAN;
        if(s.equalsIgnoreCase("darkGray"))
            return Color.DARK_GRAY;
        if(s.equalsIgnoreCase("DARK_GRAY"))
            return Color.DARK_GRAY;
        if(s.equalsIgnoreCase("gray"))
            return Color.GRAY;
        if(s.equalsIgnoreCase("green"))
            return Color.GREEN;
        if(s.equalsIgnoreCase("lightGray"))
            return Color.LIGHT_GRAY;
        if(s.equalsIgnoreCase("LIGHT_GRAY"))
            return Color.LIGHT_GRAY;
        if(s.equalsIgnoreCase("magenta"))
            return Color.MAGENTA;
        if(s.equalsIgnoreCase("orange"))
            return Color.ORANGE;
        if(s.equalsIgnoreCase("pink"))
            return Color.PINK;
        if(s.equalsIgnoreCase("red"))
            return Color.RED;
        if(s.equalsIgnoreCase("white"))
            return Color.WHITE;
        if(s.equalsIgnoreCase("yellow"))
            return Color.YELLOW;
        int j;
        if(s.startsWith("0x"))
            s = s.substring(2);
        else
        if(s.startsWith("#"))
            s = s.substring(1);
        int i = s.length();
        j = 255;
        if(i == 8)
        {
            j = Integer.parseInt(s.substring(0, 2), 16);
            s = s.substring(2);
        } else
        if(i != 6)
            throw new ErrorException("decodeColor: Colors must have 6 or 8 hexadecimal digits");
        return new Color(Integer.parseInt(s, 16) | j << 24, true);
        NumberFormatException numberformatexception;
        numberformatexception;
        throw new ErrorException("decodeColor: Illegal color value");
    }

    public static String showOpenDialog(String s, String s1)
    {
        FileChooserFilter filechooserfilter = new FileChooserFilter(s1);
        JFileChooser jfilechooser = new JFileChooser(filechooserfilter.getDirectory());
        jfilechooser.setFileFilter(filechooserfilter);
        jfilechooser.setDialogTitle(s);
        if(jfilechooser.showOpenDialog(null) == 0)
            return jfilechooser.getSelectedFile().getAbsolutePath();
        else
            return "";
    }

    public static String showSaveDialog(String s, String s1)
    {
        FileChooserFilter filechooserfilter = new FileChooserFilter(s1);
        String s2 = filechooserfilter.getDirectory();
        JFileChooser jfilechooser = new JFileChooser(s2);
        jfilechooser.setSelectedFile(new File(s2, filechooserfilter.getPattern()));
        jfilechooser.setDialogTitle(s);
        if(jfilechooser.showSaveDialog(null) == 0)
            return jfilechooser.getSelectedFile().getAbsolutePath();
        else
            return "";
    }

    public static boolean matchFilenamePattern(String s, String s1)
    {
        return recursiveMatch(s, 0, s1, 0);
    }

    public static void registerApplet(Applet applet)
    {
        registerApplet(applet, Thread.currentThread());
        mostRecentApplet = applet;
    }

    public static void registerApplet(Applet applet, Thread thread)
    {
        appletTable.put(thread, applet);
    }

    public static Applet getApplet()
    {
        Applet applet = (Applet)appletTable.get(Thread.currentThread());
        if(applet == null)
            applet = mostRecentApplet;
        return applet;
    }

    public static void setDebugOptions(String s)
    {
        debugOptions = s != null ? (new StringBuilder()).append("+").append(s.toLowerCase()).append("+").toString() : null;
    }

    public static boolean testDebugOption(String s)
    {
        if(debugOptions == null)
            return false;
        else
            return debugOptions.indexOf((new StringBuilder()).append("+").append(s.toLowerCase()).append("+").toString()) >= 0;
    }

    public static String getCommandLine()
    {
        switch(Platform.getPlatform())
        {
        case 1: // '\001'
        case 2: // '\002'
            return getShellCommandLine();

        case 3: // '\003'
            return DOSCommandLine.getCommandLine();
        }
        return getShellCommandLine();
    }

    public static String getMainClass()
    {
        String s = null;
        try
        {
            s = System.getProperty("java.main");
        }
        catch(Exception exception) { }
        if(s == null)
            s = readMainClassFromClassPath();
        if(s == null)
        {
            String s1 = getCommandLine();
            s = readMainClassFromCommandLine(s1);
        }
        return s;
    }

    public static boolean checkIfLoaded(String s)
    {
        boolean flag;
        if(Platform.compareVersion("1.2") < 0)
            return false;
        flag = false;
        if(System.getSecurityManager() != null)
            return false;
        if(managerThatFails == null)
            try
            {
                Class class1 = Class.forName("acm.util.SecurityManagerThatFails");
                managerThatFails = (SecurityManager)class1.newInstance();
            }
            catch(Exception exception)
            {
                return false;
            }
        System.setSecurityManager(managerThatFails);
        flag = Class.forName(s) != null;
        System.setSecurityManager(null);
        break MISSING_BLOCK_LABEL_103;
        Object obj;
        obj;
        flag = true;
        System.setSecurityManager(null);
        break MISSING_BLOCK_LABEL_103;
        obj;
        System.setSecurityManager(null);
        break MISSING_BLOCK_LABEL_103;
        Exception exception1;
        exception1;
        System.setSecurityManager(null);
        throw exception1;
        obj;
        return flag;
    }

    public static void terminateAppletThreads(Applet applet)
    {
        try
        {
            Thread thread = Thread.currentThread();
            Class class1 = Class.forName("java.lang.Thread");
            Method method = class1.getMethod("stop", new Class[0]);
            Iterator iterator = appletTable.keySet().iterator();
            do
            {
                if(!iterator.hasNext())
                    break;
                Thread thread1 = (Thread)iterator.next();
                if(thread1 != thread && thread1.isAlive() && isAnonymous(thread1) && applet == appletTable.get(thread1))
                    method.invoke(thread1, new Object[0]);
            } while(true);
        }
        catch(Exception exception) { }
    }

    public static boolean isAnonymous(Thread thread)
    {
        String s = thread.getName();
        if(!s.startsWith("Thread-"))
            return false;
        for(int i = 7; i < s.length(); i++)
            if(!Character.isDigit(s.charAt(i)))
                return false;

        return true;
    }

    public static PrintStream openMailStream(String s, String s1, String s2)
    {
        return new MailStream(s, s1, s2);
    }

    public static void cancelMail(PrintStream printstream)
    {
        ((MailStream)printstream).cancel();
    }

    public static void sendStandardHeaders(PrintStream printstream, String s, String s1)
    {
        ((MailStream)printstream).sendStandardHeaders(s, s1);
    }

    public static OutputStream openHexByteOutputStream(PrintStream printstream)
    {
        return new HexByteOutputStream(printstream);
    }

    public static OutputStream openBase64OutputStream(PrintStream printstream)
    {
        return new Base64OutputStream(printstream);
    }

    public static void padBase64OutputStream(OutputStream outputstream)
    {
        ((Base64OutputStream)outputstream).pad();
    }

    public static void exportJar(File file, File file1, String s, Object obj)
    {
        try
        {
            ZipOutputStream zipoutputstream = new ZipOutputStream(new FileOutputStream(file));
            dumpJarAndResources("", file1, zipoutputstream, null, s, null, false, obj);
            zipoutputstream.close();
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public static boolean executeExportAction(Program program, String s)
    {
        if(s.equals("Export Applet") || s.equals("Submit Project"))
        {
            (new Thread(new ThreadedMenuAction(program, s))).start();
            return true;
        } else
        {
            return false;
        }
    }

    public static String getLocalHostName()
    {
        InetAddress inetaddress;
        Method method;
        inetaddress = InetAddress.getLocalHost();
        Class class1 = inetaddress.getClass();
        method = class1.getMethod("getCanonicalHostName", new Class[0]);
        return (String)method.invoke(inetaddress, new Object[0]);
        Exception exception;
        exception;
        return null;
    }

    public static void copyFile(File file, File file1)
    {
        try
        {
            BufferedInputStream bufferedinputstream = new BufferedInputStream(new FileInputStream(file));
            BufferedOutputStream bufferedoutputstream = new BufferedOutputStream(new FileOutputStream(file1));
            copyBytes(bufferedinputstream, bufferedoutputstream, file.length());
            bufferedinputstream.close();
            bufferedoutputstream.close();
            Platform.copyFileTypeAndCreator(file, file1);
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public static void copyBytes(InputStream inputstream, OutputStream outputstream, long l)
        throws IOException
    {
        byte abyte0[] = new byte[4096];
        int i;
        for(; l > 0L; l -= i)
        {
            i = (int)Math.min(4096L, l);
            i = inputstream.read(abyte0, 0, i);
            if(i == -1)
                return;
            outputstream.write(abyte0, 0, i);
        }

    }

    static String getURLSuffix(String s)
    {
        return s.substring(s.lastIndexOf('/') + 1);
    }

    static void exportApplet(Program program, JProgressBar jprogressbar)
    {
        String s;
        String s1;
        File file;
        ExportAppletDialog exportappletdialog;
        File file2;
        File file3;
        try
        {
            s = program.getClass().getName();
            s1 = s.substring(s.lastIndexOf(".") + 1);
            file = new File(System.getProperty("user.dir"));
            File file1 = new File(System.getProperty("user.home"));
            exportappletdialog = new ExportAppletDialog(file1, program);
            file2 = exportappletdialog.chooseOutputDirectory();
            if(file2 == null)
                return;
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
        if(file2.exists())
        {
            if(!file2.isDirectory())
                file2 = new File(file2.getParent());
        } else
        {
            file2.mkdir();
        }
        if(jprogressbar != null)
        {
            jprogressbar.setMaximum(countResources(file, RESOURCE_EXTENSIONS, "acm.jar") + 1);
            ProgressBarDialog.popup(jprogressbar);
        }
        file3 = new File(file, "index.html");
        if(file3.canRead())
            copyFile(file3, new File(file2, "index.html"));
        else
            dumpHTMLIndex(file2, program, s, s1);
        if(jprogressbar == null)
            break MISSING_BLOCK_LABEL_213;
        if(ProgressBarDialog.hasBeenCancelled(jprogressbar))
            return;
        jprogressbar.setValue(jprogressbar.getValue() + 1);
        dumpJarAndResources(file, file2, (new StringBuilder()).append(s1).append(".jar").toString(), "acm.jar", jprogressbar, exportappletdialog.exportFiles(), null);
        if(jprogressbar != null)
            ProgressBarDialog.dismiss(jprogressbar);
    }

    private static void initFontFamilyTable()
    {
        fontFamilyTableInitialized = true;
        for(int i = 1; fontFamilyArray == null && i <= 2; i++)
            try
            {
                if(i == 1)
                {
                    Class class1 = Class.forName("java.awt.GraphicsEnvironment");
                    Method method = class1.getMethod("getLocalGraphicsEnvironment", new Class[0]);
                    Method method2 = class1.getMethod("getAvailableFontFamilyNames", new Class[0]);
                    Object obj = method.invoke(null, new Object[0]);
                    fontFamilyArray = (String[])(String[])method2.invoke(obj, new Object[0]);
                } else
                {
                    Class class2 = Class.forName("java.awt.Toolkit");
                    Method method1 = class2.getMethod("getFontList", new Class[0]);
                    fontFamilyArray = (String[])(String[])method1.invoke(Toolkit.getDefaultToolkit(), new Object[0]);
                }
            }
            catch(Exception exception) { }

        fontFamilyTable = new HashMap();
        for(int j = 0; j < fontFamilyArray.length; j++)
            fontFamilyTable.put(trimFamilyName(fontFamilyArray[j]), fontFamilyArray[j]);

        fontFamilyTable.put("serif", getFirstAvailableFontSubstitution(SERIF_SUBSTITUTIONS));
        fontFamilyTable.put("sansserif", getFirstAvailableFontSubstitution(SANSSERIF_SUBSTITUTIONS));
        fontFamilyTable.put("monospaced", getFirstAvailableFontSubstitution(MONOSPACED_SUBSTITUTIONS));
    }

    private static String getFirstAvailableFontSubstitution(String as[])
    {
        for(int i = 0; i < as.length; i++)
            if(fontFamilyTable.get(trimFamilyName(as[i])) != null)
                return as[i];

        return null;
    }

    private static String trimFamilyName(String s)
    {
        String s1 = "";
        for(int i = 0; i < s.length(); i++)
        {
            char c = s.charAt(i);
            if(c != ' ' && c != '-')
                s1 = (new StringBuilder()).append(s1).append(Character.toLowerCase(c)).toString();
        }

        return s1;
    }

    private static boolean recursiveMatch(String s, int i, String s1, int j)
    {
        int k = s.length();
        int l = s1.length();
        if(j == l)
            return i == k;
        char c = s1.charAt(j);
        if(c == '*')
        {
            for(int i1 = i; i1 <= k; i1++)
                if(recursiveMatch(s, i1, s1, j + 1))
                    return true;

            return false;
        }
        if(i == k)
            return false;
        char c1 = s.charAt(i);
        if(c == '[')
        {
            boolean flag = false;
            boolean flag1 = false;
            if(++j == l)
                throw new ErrorException("matchFilenamePattern: missing ]");
            if(s1.charAt(j) == '^')
            {
                j++;
                flag1 = true;
            }
            while(j < l && s1.charAt(j) != ']') 
                if(j + 2 < l && s1.charAt(j + 1) == '-')
                {
                    flag |= c1 >= s1.charAt(j) && c1 <= s1.charAt(j + 2);
                    j += 3;
                } else
                {
                    flag |= c1 == s1.charAt(j);
                    j++;
                }
            if(j == l)
                throw new ErrorException("matchFilenamePattern: missing ]");
            if(flag == flag1)
                return false;
        } else
        if(c != '?' && c != c1)
            return false;
        return recursiveMatch(s, i + 1, s1, j + 1);
    }

    private static String getShellCommandLine()
    {
        Process process;
        String s = Platform.isMac() ? "command" : "args";
        String as[] = {
            "bash", "-c", (new StringBuilder()).append("ps -p $PPID -o ").append(s).toString()
        };
        process = Runtime.getRuntime().exec(as);
        process.waitFor();
        if(process.getErrorStream().read() != -1)
            return null;
        BufferedReader bufferedreader;
        bufferedreader = new BufferedReader(new InputStreamReader(process.getInputStream()));
        bufferedreader.readLine();
        return bufferedreader.readLine();
        Exception exception;
        exception;
        return null;
    }

    private static String readMainClassFromManifest(String s)
    {
        ZipFile zipfile;
        ZipEntry zipentry;
        if(testDebugOption("main"))
            System.out.println((new StringBuilder()).append("Read class from JAR manifest in ").append(s).toString());
        zipfile = new ZipFile(s);
        zipentry = zipfile.getEntry("META-INF/MANIFEST.MF");
        if(zipentry == null)
            return null;
        BufferedReader bufferedreader;
        String s1;
        bufferedreader = new BufferedReader(new InputStreamReader(zipfile.getInputStream(zipentry)));
        s1 = bufferedreader.readLine();
_L1:
        String s2;
        if(s1 == null)
            break MISSING_BLOCK_LABEL_164;
        if(!s1.startsWith("Main-Class:"))
            break MISSING_BLOCK_LABEL_155;
        s2 = s1.substring("Main-Class:".length()).trim();
        if(testDebugOption("main"))
            System.out.println((new StringBuilder()).append("Main class = ").append(s2).toString());
        return s2;
        s1 = bufferedreader.readLine();
          goto _L1
        return null;
        IOException ioexception;
        ioexception;
        return null;
    }

    private static String readMainClassFromCommandLine(String s)
    {
        boolean flag;
        if(testDebugOption("main"))
            System.out.println((new StringBuilder()).append("Read class from command line: ").append(s).toString());
        if(s == null)
            return null;
        flag = false;
        StreamTokenizer streamtokenizer;
        boolean flag1;
        streamtokenizer = new StreamTokenizer(new StringReader(s));
        streamtokenizer.resetSyntax();
        streamtokenizer.wordChars(33, 255);
        streamtokenizer.quoteChar(34);
        streamtokenizer.quoteChar(39);
        streamtokenizer.whitespaceChars(32, 32);
        streamtokenizer.whitespaceChars(9, 9);
        flag1 = false;
_L8:
        int i;
        String s1;
        i = streamtokenizer.nextToken();
        s1 = streamtokenizer.sval;
        i;
        JVM INSTR lookupswitch 4: default 165
    //                   -3: 162
    //                   -1: 160
    //                   34: 162
    //                   39: 162;
           goto _L1 _L2 _L3 _L2 _L2
_L3:
        return null;
_L1:
        return null;
_L2:
        if(!flag1) goto _L5; else goto _L4
_L4:
        if(!s1.startsWith("-")) goto _L7; else goto _L6
_L6:
        if(s1.equals("-jar"))
            flag = true;
        else
        if(s1.equals("-cp") || s1.equals("-classpath"))
            streamtokenizer.nextToken();
          goto _L8
_L7:
        if(flag)
            return readMainClassFromManifest(s1);
        if(testDebugOption("main"))
            System.out.println((new StringBuilder()).append("Main class = ").append(s1).toString());
        return s1;
_L5:
        try
        {
            flag1 = true;
        }
        catch(IOException ioexception)
        {
            return null;
        }
          goto _L8
    }

    private static String readMainClassFromClassPath()
    {
        String s;
        StringTokenizer stringtokenizer;
        s = null;
        String s1 = System.getProperty("java.class.path");
        if(s1 == null)
            s1 = System.getProperty("user.dir");
        if(s1 == null)
            return null;
        if(testDebugOption("main"))
            System.out.println((new StringBuilder()).append("Read class from class path: ").append(s1).toString());
        stringtokenizer = new StringTokenizer(s1, ":;");
_L4:
        String as[];
        int i;
        do
        {
            if(!stringtokenizer.hasMoreTokens())
                break MISSING_BLOCK_LABEL_408;
            String s2 = stringtokenizer.nextToken();
            File file = new File(s2);
            as = null;
            if(file.isDirectory())
                as = file.list();
            else
            if(s2.endsWith(".jar") && !nameAppears(s2, SKIP_JARS))
                try
                {
                    ZipFile zipfile = new ZipFile(file);
                    ArrayList arraylist = new ArrayList();
                    for(Enumeration enumeration = zipfile.entries(); enumeration.hasMoreElements(); arraylist.add(((ZipEntry)enumeration.nextElement()).getName()));
                    as = new String[arraylist.size()];
                    for(int j = 0; j < as.length; j++)
                        as[j] = (String)arraylist.get(j);

                }
                catch(IOException ioexception)
                {
                    as = null;
                }
        } while(as == null);
        i = 0;
_L2:
        String s4;
        if(i >= as.length)
            break; /* Loop/switch isn't completed */
        String s3 = as[i];
        if(!s3.endsWith(".class"))
            break MISSING_BLOCK_LABEL_399;
        s4 = s3.substring(0, s3.lastIndexOf(".class"));
        if(s4.indexOf("/") != -1 || !checkIfLoaded(s4))
            break MISSING_BLOCK_LABEL_399;
        Class class1 = Class.forName(s4);
        Class aclass[] = {
            as.getClass()
        };
        if(class1.getMethod("main", aclass) == null)
            break MISSING_BLOCK_LABEL_399;
        if(testDebugOption("main"))
            System.out.println((new StringBuilder()).append("Main class = ").append(s4).toString());
        if(s != null)
            return null;
        try
        {
            s = s4;
        }
        catch(Exception exception) { }
        i++;
        if(true) goto _L2; else goto _L1
_L1:
        if(true) goto _L4; else goto _L3
_L3:
        return s;
    }

    protected static void submitProject(Program program, JProgressBar jprogressbar)
    {
        SubmitOptions submitoptions = getOptions(program);
        if(submitoptions.popup() && submitoptions.isComplete())
        {
            String s = program.getClass().getName();
            String s1 = s.substring(s.lastIndexOf(".") + 1);
            String s2 = (new StringBuilder()).append("==").append(System.currentTimeMillis()).append("==").toString();
            String s3 = submitoptions.getSMTPServer();
            String s4 = submitoptions.getAuthorName();
            String s5 = submitoptions.getAuthorEMail();
            String s6 = submitoptions.getSubmissionEMail();
            String s7 = s5;
            int i = s5.indexOf("@");
            if(i != -1)
                s7 = s7.substring(0, i);
            String s8 = (new StringBuilder()).append(s1).append("_").append(s7).toString();
            File file = new File(System.getProperty("user.dir"));
            if(jprogressbar != null)
            {
                jprogressbar.setMaximum(countResources(file, SUBMIT_EXTENSIONS, null));
                ProgressBarDialog.popup(jprogressbar);
            }
            PrintStream printstream = openMailStream(s3, s5, s6);
            sendStandardHeaders(printstream, s4, s1);
            printstream.println("Mime-Version: 1.0");
            printstream.println((new StringBuilder()).append("Content-Type: multipart/mixed; boundary=\"").append(s2).append('"').toString());
            printstream.println();
            printstream.println((new StringBuilder()).append("--").append(s2).toString());
            printstream.println("Content-Transfer-Encoding: base64");
            printstream.println((new StringBuilder()).append("Content-Type: application/zip; name=").append(s8).append(".zip").toString());
            printstream.println((new StringBuilder()).append("Content-Disposition: attachment; filename=").append(s8).append(".zip").toString());
            submitDirectory(printstream, s8, jprogressbar);
            if(ProgressBarDialog.hasBeenCancelled(jprogressbar))
                cancelMail(printstream);
            printstream.println((new StringBuilder()).append("--").append(s2).append("--").toString());
            printstream.close();
            if(jprogressbar != null)
                ProgressBarDialog.dismiss(jprogressbar);
        }
    }

    private static void dumpHTMLIndex(File file, Program program, String s, String s1)
        throws IOException
    {
        File file1 = new File(file, "index.html");
        PrintWriter printwriter = new PrintWriter(new FileWriter(file1));
        Dimension dimension = program.getSize();
        printwriter.println("<html>");
        printwriter.println("<head>");
        printwriter.println("<meta name=\"generator\" content=\"ACM Java Libraries V1.1\">");
        printwriter.println((new StringBuilder()).append("<title>").append(s1).append("</title>").toString());
        printwriter.println("</head>");
        printwriter.println("<body>");
        printwriter.println("<center>");
        printwriter.println("<table border=2 cellpadding=0 cellspacing=0>");
        printwriter.println("<tr><td>");
        printwriter.println((new StringBuilder()).append("<applet archive=\"").append(s1).append(".jar\"").toString());
        printwriter.println((new StringBuilder()).append("        code=\"").append(s.replace('.', '/')).append(".class\"").toString());
        printwriter.println((new StringBuilder()).append("        width=").append(dimension.width).append(" height=").append(dimension.height).append(">").toString());
        printwriter.println("</applet>");
        printwriter.println("</td></tr>");
        printwriter.println("</table>");
        printwriter.println("</center>");
        printwriter.println("</body>");
        printwriter.println("</html>");
        printwriter.close();
    }

    private static void dumpJarAndResources(File file, File file1, String s, String s1, JProgressBar jprogressbar, boolean flag, Object obj)
        throws IOException
    {
        File file2 = new File(file1, s);
        ZipOutputStream zipoutputstream = new ZipOutputStream(new FileOutputStream(file2));
        dumpJarAndResources("", file, zipoutputstream, file1, s1, jprogressbar, flag, obj);
        zipoutputstream.close();
    }

    private static void dumpJarAndResources(String s, File file, ZipOutputStream zipoutputstream, File file1, String s1, JProgressBar jprogressbar, boolean flag, Object obj)
        throws IOException
    {
        if(ProgressBarDialog.hasBeenCancelled(jprogressbar))
            return;
        dumpTree(s, file, zipoutputstream, file1, jprogressbar, 0, flag, obj);
        for(StringTokenizer stringtokenizer = new StringTokenizer(s1, ";"); stringtokenizer.hasMoreTokens();)
        {
            String s2 = stringtokenizer.nextToken().trim();
            File file2 = getLibrary(s2);
            if(file2 != null)
            {
                ZipFile zipfile = new ZipFile(file2);
                Enumeration enumeration = zipfile.entries();
                while(enumeration.hasMoreElements()) 
                {
                    ZipEntry zipentry = (ZipEntry)enumeration.nextElement();
                    String s3 = zipentry.getName();
                    if(!nameAppears(s3, SKIP_FILES))
                    {
                        BufferedInputStream bufferedinputstream = new BufferedInputStream(zipfile.getInputStream(zipentry));
                        if(obj != null && s3.endsWith(".class"))
                        {
                            zipentry = new ZipEntry(s3);
                            zipoutputstream.putNextEntry(zipentry);
                            transformClass(obj, bufferedinputstream, zipoutputstream);
                        } else
                        {
                            zipoutputstream.putNextEntry(zipentry);
                            copyBytes(bufferedinputstream, zipoutputstream, zipentry.getSize());
                        }
                        do
                        {
                            int i = bufferedinputstream.read();
                            if(i == -1)
                                break;
                            zipoutputstream.write(i);
                        } while(true);
                        zipoutputstream.closeEntry();
                        bufferedinputstream.close();
                    }
                    if(jprogressbar != null)
                        jprogressbar.setValue(jprogressbar.getValue() + 1);
                }
            }
        }

    }

    private static void dumpTree(String s, File file, ZipOutputStream zipoutputstream, File file1, JProgressBar jprogressbar, int i, boolean flag, Object obj)
        throws IOException
    {
        if(ProgressBarDialog.hasBeenCancelled(jprogressbar))
            return;
        String s1 = file.getName();
        if(file.isDirectory())
        {
            String as[] = file.list();
            if(i > 0)
                s = (new StringBuilder()).append(s).append(s1).append("/").toString();
            for(int j = 0; j < as.length; j++)
                dumpTree(s, new File(file, as[j]), zipoutputstream, file1, jprogressbar, i + 1, flag, obj);

        } else
        if(isResourceComponent(s1))
        {
            String s2 = (new StringBuilder()).append(s).append(s1).toString();
            if(s2.startsWith("Java Classes/"))
                s2 = s2.substring(s2.indexOf('/') + 1);
            ZipEntry zipentry = new ZipEntry(s2);
            BufferedInputStream bufferedinputstream = new BufferedInputStream(new FileInputStream(file));
            zipoutputstream.putNextEntry(zipentry);
            if(obj != null && s1.endsWith(".class"))
                transformClass(obj, bufferedinputstream, zipoutputstream);
            else
                copyBytes(bufferedinputstream, zipoutputstream, file.length());
            bufferedinputstream.close();
            if(flag && file1 != null && !s1.endsWith(".class"))
            {
                BufferedInputStream bufferedinputstream1 = new BufferedInputStream(new FileInputStream(file));
                file1 = new File(file1, s);
                File file2 = new File(file1, s1);
                file1.mkdirs();
                BufferedOutputStream bufferedoutputstream = new BufferedOutputStream(new FileOutputStream(file2));
                copyBytes(bufferedinputstream1, bufferedoutputstream, file.length());
                bufferedinputstream1.close();
                bufferedoutputstream.close();
            }
            if(jprogressbar != null)
                jprogressbar.setValue(jprogressbar.getValue() + 1);
        }
    }

    private static void transformClass(Object obj, InputStream inputstream, OutputStream outputstream)
    {
        Method method = null;
        try
        {
            Class aclass[] = {
                Class.forName("java.io.InputStream"), Class.forName("java.io.OutputStream")
            };
            method = obj.getClass().getMethod("transform", aclass);
        }
        catch(Exception exception)
        {
            throw new ErrorException("exportJar: Illegal class transformer object");
        }
        try
        {
            Object aobj[] = {
                inputstream, outputstream
            };
            method.invoke(obj, aobj);
        }
        catch(Exception exception1)
        {
            throw new ErrorException(exception1);
        }
    }

    private static int countResources(File file, String as[], String s)
    {
        String s1 = file.getName();
        int i = 0;
        if(file.isDirectory())
        {
            String as1[] = file.list();
            for(int k = 0; k < as1.length; k++)
                i += countResources(new File(file, as1[k]), as, null);

        } else
        {
            for(int j = 0; j < as.length && i == 0; j++)
                if(s1.endsWith(as[j]))
                    i = 1;

        }
        if(s != null)
        {
            File file1 = getLibrary(s);
            if(file1 != null)
                try
                {
                    ZipFile zipfile = new ZipFile(file1);
                    for(Enumeration enumeration = zipfile.entries(); enumeration.hasMoreElements(); enumeration.nextElement())
                        i++;

                }
                catch(IOException ioexception) { }
        }
        return i;
    }

    private static File getLibrary(String s)
    {
        if(s == null)
            return null;
        File file = new File(s);
        if(s.startsWith(".") || file.isAbsolute())
            return file;
        String s1 = System.getProperty("java.class.path");
        if(s1 == null)
            s1 = "";
        for(StringTokenizer stringtokenizer = new StringTokenizer(s1, ":"); stringtokenizer.hasMoreTokens();)
        {
            String s2 = stringtokenizer.nextToken();
            if(s2.equals(s) || s2.endsWith((new StringBuilder()).append("/").append(s).toString()))
                return new File(s2);
        }

        File file1 = new File(System.getProperty("user.dir"));
        file = new File(file1, s);
        return file.exists() ? file : null;
    }

    private static boolean isResourceComponent(String s)
    {
        for(int i = 0; i < RESOURCE_EXTENSIONS.length; i++)
            if(s.endsWith(RESOURCE_EXTENSIONS[i]))
                return true;

        return false;
    }

    private static void submitDirectory(PrintStream printstream, String s, JProgressBar jprogressbar)
    {
        try
        {
            OutputStream outputstream = openBase64OutputStream(printstream);
            ZipOutputStream zipoutputstream = new ZipOutputStream(new BufferedOutputStream(outputstream));
            ZipOutputStream zipoutputstream1 = new ZipOutputStream(new NullOutputStream());
            File file = new File(System.getProperty("user.dir"));
            dumpZip((new StringBuilder()).append(s).append("/").toString(), file, zipoutputstream, zipoutputstream1, true, jprogressbar);
            zipoutputstream1.close();
            zipoutputstream.finish();
            zipoutputstream.flush();
            padBase64OutputStream(outputstream);
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    private static void dumpZip(String s, File file, ZipOutputStream zipoutputstream, ZipOutputStream zipoutputstream1, boolean flag, JProgressBar jprogressbar)
        throws IOException
    {
        if(ProgressBarDialog.hasBeenCancelled(jprogressbar))
            return;
        String s1 = file.getName();
        if(file.isDirectory())
        {
            String as[] = file.list();
            if(!flag)
                s = (new StringBuilder()).append(s).append(s1).append("/").toString();
            for(int i = 0; i < as.length; i++)
                dumpZip(s, new File(file, as[i]), zipoutputstream, zipoutputstream1, false, jprogressbar);

        } else
        if(isSubmitComponent(s1))
        {
            String s2 = (new StringBuilder()).append(s).append(s1).toString();
            ZipEntry zipentry = new ZipEntry(s2);
            BufferedInputStream bufferedinputstream = new BufferedInputStream(new FileInputStream(file));
            if(zipoutputstream1 != null)
            {
                zipoutputstream1.putNextEntry(zipentry);
                copyBytes(bufferedinputstream, zipoutputstream1, file.length());
                zipoutputstream1.closeEntry();
                bufferedinputstream.close();
                bufferedinputstream = new BufferedInputStream(new FileInputStream(file));
            }
            zipoutputstream.putNextEntry(zipentry);
            copyBytes(bufferedinputstream, zipoutputstream, file.length());
            zipoutputstream.closeEntry();
            bufferedinputstream.close();
            if(jprogressbar != null)
                jprogressbar.setValue(jprogressbar.getValue() + 1);
        }
    }

    private static boolean isSubmitComponent(String s)
    {
        for(int i = 0; i < SUBMIT_EXTENSIONS.length; i++)
            if(s.endsWith(SUBMIT_EXTENSIONS[i]))
                return true;

        return false;
    }

    private static SubmitOptions getOptions(Program program)
    {
        SubmitOptions submitoptions = (SubmitOptions)optionsTable.get(program);
        if(submitoptions == null)
        {
            submitoptions = new SubmitOptions(program);
            optionsTable.put(program, submitoptions);
        }
        return submitoptions;
    }

    private static boolean nameAppears(String s, String as[])
    {
        for(int i = 0; i < as.length; i++)
            if(as[i].equals(s))
                return true;

        return false;
    }

    private static final String SKIP_FILES[] = {
        ".DS_Store", "FINDER.DAT", "RESOURCE.FRK"
    };
    private static final String RESOURCE_EXTENSIONS[] = {
        ".txt", ".dat", ".gif", ".jpg", ".jpeg", ".png", ".au", ".wav", ".class"
    };
    private static final String SUBMIT_EXTENSIONS[] = {
        ".java", ".html", ".txt", ".dat", ".gif", ".jpg", ".jpeg", ".png", ".au", ".wav"
    };
    private static HashMap optionsTable = new HashMap();
    private static final int BUFFER_SIZE = 4096;
    private static final String SERIF_SUBSTITUTIONS[] = {
        "Serif", "Times", "TimesRoman", "Times-Roman"
    };
    private static final String SANSSERIF_SUBSTITUTIONS[] = {
        "SansSerif", "Helvetica", "Arial"
    };
    private static final String MONOSPACED_SUBSTITUTIONS[] = {
        "Monospaced", "Courier", "Monaco"
    };
    private static final String SKIP_JARS[] = {
        "acm.jar", "acm11.jar", "swingall.jar", "patchJTF.jar"
    };
    private static boolean fontFamilyTableInitialized = false;
    private static String fontFamilyArray[] = null;
    private static HashMap fontFamilyTable = null;
    private static HashMap appletTable = new HashMap();
    private static Applet mostRecentApplet = null;
    private static SecurityManager managerThatFails = null;
    private static String debugOptions = null;

}
