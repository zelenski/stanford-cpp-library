// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Platform.java

package acm.util;

import java.awt.Toolkit;
import java.io.File;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.StringTokenizer;

public class Platform
{

    private Platform()
    {
    }

    public static int getPlatform()
    {
        if(platform != -1)
            return platform;
        String s = System.getProperty("os.name", "").toLowerCase();
        if(s.startsWith("mac"))
            return platform = 1;
        if(s.startsWith("windows"))
            return platform = 3;
        if(s.startsWith("microsoft"))
            return platform = 3;
        if(s.startsWith("ms"))
            return platform = 3;
        if(s.startsWith("unix"))
            return platform = 2;
        if(s.startsWith("linux"))
            return platform = 2;
        else
            return platform = 0;
    }

    public static boolean isMac()
    {
        return getPlatform() == 1;
    }

    public static boolean isWindows()
    {
        return getPlatform() == 3;
    }

    public static boolean isUnix()
    {
        return getPlatform() == 2;
    }

    public static void setFileTypeAndCreator(String s, String s1, String s2)
    {
        if(!isMac())
            return;
        try
        {
            setFileTypeAndCreator(new File(s), s1, s2);
        }
        catch(Exception exception) { }
    }

    public static void setFileTypeAndCreator(File file, String s, String s1)
    {
        if(!isMac())
            return;
        try
        {
            Class class1 = Class.forName("com.apple.mrj.MRJOSType");
            Class class2 = Class.forName("com.apple.mrj.MRJFileUtils");
            Class aclass[] = {
                Class.forName("java.lang.String")
            };
            Constructor constructor = class1.getConstructor(aclass);
            Class aclass1[] = {
                Class.forName("java.io.File"), class1, class1
            };
            Method method = class2.getMethod("setFileTypeAndCreator", aclass1);
            Object aobj[] = {
                (new StringBuilder()).append(s).append("    ").toString().substring(0, 4)
            };
            Object obj = constructor.newInstance(aobj);
            Object aobj1[] = {
                (new StringBuilder()).append(s1).append("    ").toString().substring(0, 4)
            };
            Object obj1 = constructor.newInstance(aobj1);
            Object aobj2[] = {
                file, obj, obj1
            };
            method.invoke(null, aobj2);
        }
        catch(Exception exception) { }
    }

    public static void copyFileTypeAndCreator(File file, File file1)
    {
        if(!isMac())
            return;
        try
        {
            Class class1 = Class.forName("com.apple.mrj.MRJOSType");
            Class class2 = Class.forName("com.apple.mrj.MRJFileUtils");
            Class aclass[] = {
                Class.forName("java.io.File")
            };
            Method method = class2.getMethod("getFileType", aclass);
            Method method1 = class2.getMethod("getFileCreator", aclass);
            Class aclass1[] = {
                Class.forName("java.io.File"), class1, class1
            };
            Method method2 = class2.getMethod("setFileTypeAndCreator", aclass1);
            Object aobj[] = {
                file
            };
            Object obj = method.invoke(null, aobj);
            Object obj1 = method1.invoke(null, aobj);
            Object aobj1[] = {
                file1, obj, obj1
            };
            method2.invoke(null, aobj1);
        }
        catch(Exception exception) { }
    }

    public static String getJTFVersion()
    {
        return "1.99.1";
    }

    public static int compareVersion(String s)
    {
        return compareVersion(System.getProperty("java.version"), s);
    }

    public static int compareVersion(String s, String s1)
    {
        StringTokenizer stringtokenizer = new StringTokenizer(s, ".");
        StringTokenizer stringtokenizer1;
        for(stringtokenizer1 = new StringTokenizer(s1, "."); stringtokenizer.hasMoreTokens() && stringtokenizer1.hasMoreTokens();)
        {
            int i = Integer.parseInt(stringtokenizer.nextToken());
            int j = Integer.parseInt(stringtokenizer1.nextToken());
            if(i != j)
                return i >= j ? 1 : -1;
        }

        if(stringtokenizer.hasMoreTokens())
            return 1;
        return !stringtokenizer1.hasMoreTokens() ? 0 : -1;
    }

    public static boolean isSwingAvailable()
    {
        if(!swingChecked)
        {
            swingChecked = true;
            isSwingAvailable = false;
            if(compareVersion("1.2") >= 0)
                try
                {
                    isSwingAvailable = Class.forName("javax.swing.JComponent") != null;
                }
                catch(Exception exception) { }
        }
        return isSwingAvailable;
    }

    public static boolean isSunAudioAvailable()
    {
        if(!sunAudioChecked)
        {
            sunAudioChecked = true;
            try
            {
                isSunAudioAvailable = Class.forName("sun.audio.AudioPlayer") != null;
            }
            catch(Exception exception)
            {
                isSunAudioAvailable = false;
            }
        }
        return isSunAudioAvailable;
    }

    public static boolean isJMFAvailable()
    {
        if(!jmfChecked)
        {
            jmfChecked = true;
            try
            {
                isJMFAvailable = Class.forName("javax.media.Player") != null;
            }
            catch(Exception exception)
            {
                isJMFAvailable = false;
            }
        }
        return isJMFAvailable;
    }

    public static boolean areCollectionsAvailable()
    {
        if(!collectionsChecked)
        {
            collectionsChecked = true;
            try
            {
                areCollectionsAvailable = Class.forName("java.util.ArrayList") != null;
            }
            catch(Exception exception)
            {
                areCollectionsAvailable = false;
            }
        }
        return areCollectionsAvailable;
    }

    public static boolean areStandardFontFamiliesAvailable()
    {
        if(!fontsChecked)
        {
            fontsChecked = true;
            try
            {
                Class class1 = Class.forName("java.awt.Toolkit");
                Method method = class1.getMethod("getFontList", new Class[0]);
                String as[] = (String[])(String[])method.invoke(Toolkit.getDefaultToolkit(), new Object[0]);
                int i = 0;
                for(int j = 0; j < as.length; j++)
                    if(as[j].equals("Serif") || as[j].equals("SansSerif") || as[j].equals("Monospaced"))
                        i++;

                areStandardFontFamiliesAvailable = i == 3;
            }
            catch(Exception exception)
            {
                areStandardFontFamiliesAvailable = false;
            }
        }
        return areStandardFontFamiliesAvailable;
    }

    public static final int UNKNOWN = 0;
    public static final int MAC = 1;
    public static final int UNIX = 2;
    public static final int WINDOWS = 3;
    private static final String JTF_VERSION = "1.99.1";
    private static int platform = -1;
    private static boolean areStandardFontFamiliesAvailable;
    private static boolean fontsChecked;
    private static boolean isSwingAvailable;
    private static boolean swingChecked;
    private static boolean areCollectionsAvailable;
    private static boolean collectionsChecked;
    private static boolean isSunAudioAvailable;
    private static boolean sunAudioChecked;
    private static boolean isJMFAvailable;
    private static boolean jmfChecked;

}
