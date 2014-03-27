// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Program.java

package acm.program;

import acm.util.ErrorException;
import java.applet.*;
import java.awt.Frame;
import java.awt.Image;
import java.awt.image.ImageProducer;
import java.io.*;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.*;

// Referenced classes of package acm.program:
//            Program

class ProgramAppletStub
    implements AppletContext, AppletStub
{

    public ProgramAppletStub(Program program)
    {
        applet = program;
    }

    public void setFrame(Frame frame)
    {
        enclosure = frame;
    }

    public boolean isActive()
    {
        return true;
    }

    public URL getDocumentBase()
    {
        return getCodeBase();
    }

    public URL getCodeBase()
    {
        return new URL((new StringBuilder()).append("file:").append(getCanonicalPath(".")).toString());
        MalformedURLException malformedurlexception;
        malformedurlexception;
        throw new ErrorException("Error: Illegal document base URL");
    }

    public String getParameter(String s)
    {
        return null;
    }

    public AppletContext getAppletContext()
    {
        return this;
    }

    public void appletResize(int i, int j)
    {
        if(enclosure == null)
        {
            if(!recursiveResizeCheck)
            {
                recursiveResizeCheck = true;
                applet.resize(i, j);
                applet.validate();
                recursiveResizeCheck = false;
            }
        } else
        {
            enclosure.setSize(i, j);
            enclosure.validate();
        }
    }

    public AudioClip getAudioClip(URL url)
    {
        AudioClip audioclip = null;
        if(audioclip == null)
            audioclip = getNewAudioClip(url);
        return audioclip;
    }

    public Image getImage(URL url)
    {
        Object obj = url.getContent();
        if(obj instanceof ImageProducer)
            return applet.createImage((ImageProducer)obj);
        break MISSING_BLOCK_LABEL_28;
        IOException ioexception;
        ioexception;
        return null;
    }

    public Applet getApplet(String s)
    {
        return null;
    }

    public Enumeration getApplets()
    {
        return (new Vector()).elements();
    }

    public void showDocument(URL url)
    {
        if(applet != null)
            applet.getAppletContext().showDocument(url);
    }

    public void showDocument(URL url, String s)
    {
        if(applet != null)
            applet.getAppletContext().showDocument(url, s);
    }

    public void showStatus(String s)
    {
        if(applet == null)
            System.out.println(s);
        else
            applet.showStatus(s);
    }

    public void setStream(String s, InputStream inputstream)
    {
        throw new ErrorException("setStream: unimplemented operation");
    }

    public InputStream getStream(String s)
    {
        throw new ErrorException("getStream: unimplemented operation");
    }

    public Iterator getStreamKeys()
    {
        throw new ErrorException("getStreamKeys: unimplemented operation");
    }

    private String getCanonicalPath(String s)
    {
        String s1;
        int i;
        for(s1 = (new File(s)).getAbsolutePath(); (i = s1.indexOf(' ')) != -1; s1 = (new StringBuilder()).append(s1.substring(0, i)).append("%20").append(s1.substring(i + 1)).toString());
        return s1;
    }

    private synchronized AudioClip getNewAudioClip(URL url)
    {
        Object aobj[];
        Method method;
        Class class1 = Class.forName("java.applet.Applet");
        Class aclass[] = {
            Class.forName("java.net.URL")
        };
        aobj = (new Object[] {
            url
        });
        method = class1.getMethod("newAudioClip", aclass);
        return (AudioClip)method.invoke(null, aobj);
        Exception exception;
        exception;
        return null;
    }

    private Applet applet;
    private Frame enclosure;
    private boolean recursiveResizeCheck;
}
