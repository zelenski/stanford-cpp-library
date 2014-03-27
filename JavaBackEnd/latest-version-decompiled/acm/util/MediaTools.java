// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.applet.Applet;
import java.applet.AudioClip;
import java.awt.*;
import java.awt.image.*;
import java.io.*;
import java.net.*;
import java.util.HashMap;
import java.util.StringTokenizer;

// Referenced classes of package acm.util:
//            ErrorException, HexInputStream, SoundClip, SunAudioClip, 
//            NullAudioClip, ImageSaver, JTFTools

public class MediaTools
{

    private MediaTools()
    {
    }

    public static Image loadImage(String s)
    {
        return loadImage(s, ".:images");
    }

    public static Image loadImage(String s, String s1)
    {
        Image image;
        image = (Image)imageTable.get(s);
        if(image != null)
            return image;
        if(!s.startsWith("http:"))
            break MISSING_BLOCK_LABEL_66;
        image = loadImage(new URL(s));
        if(cachingEnabled)
            imageTable.put(s, image);
        return image;
        MalformedURLException malformedurlexception;
        malformedurlexception;
        throw new ErrorException("loadImage: Malformed URL");
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        StringTokenizer stringtokenizer = new StringTokenizer(s1, ":");
        do
        {
            if(image != null || !stringtokenizer.hasMoreTokens())
                break;
            String s2 = stringtokenizer.nextToken();
            s2 = s2.equals(".") ? "" : (new StringBuilder()).append(s2).append("/").toString();
            URL url = null;
            try
            {
                url = RESOURCE_CLASS.getResource((new StringBuilder()).append("/").append(s2).append(s).toString());
                URLConnection urlconnection = url.openConnection();
                if(urlconnection == null || urlconnection.getContentLength() <= 0)
                    url = null;
            }
            catch(Exception exception) { }
            if(url == null)
            {
                Applet applet = JTFTools.getApplet();
                if(applet != null)
                {
                    URL url1 = applet.getCodeBase();
                    if(url1 != null)
                        try
                        {
                            url = new URL(url1, (new StringBuilder()).append(s2).append(s).toString());
                        }
                        catch(MalformedURLException malformedurlexception1) { }
                }
            }
            if(url == null)
                try
                {
                    if((new File((new StringBuilder()).append(s2).append(s).toString())).canRead())
                        image = toolkit.getImage((new StringBuilder()).append(s2).append(s).toString());
                }
                catch(SecurityException securityexception) { }
            else
                image = loadImage(url, false);
        } while(true);
        if(image == null)
            throw new ErrorException((new StringBuilder()).append("Cannot find an image named ").append(s).toString());
        loadImage(image);
        if(cachingEnabled)
            imageTable.put(s, image);
        return image;
    }

    public static Image loadImage(URL url)
    {
        return loadImage(url, true);
    }

    public static Image loadImage(Image image)
    {
        MediaTracker mediatracker = new MediaTracker(JTFTools.createEmptyContainer());
        mediatracker.addImage(image, 0);
        try
        {
            mediatracker.waitForID(0);
        }
        catch(InterruptedException interruptedexception)
        {
            throw new ErrorException("Image loading process interrupted");
        }
        return image;
    }

    public static void defineImage(String s, Image image)
    {
        imageTable.put(s, image);
    }

    public static void flushImage(String s)
    {
        imageTable.remove(s);
    }

    public static Image createImage(int ai[][])
    {
        int i = ai.length;
        int j = ai[0].length;
        int ai1[] = new int[j * i];
        for(int k = 0; k < i; k++)
            System.arraycopy(ai[k], 0, ai1, k * j, j);

        return createImage(ai1, j, i);
    }

    public static Image createImage(int ai[], int i, int j)
    {
        Image image = Toolkit.getDefaultToolkit().createImage(new MemoryImageSource(i, j, ai, 0, i));
        loadImage(image);
        return image;
    }

    public static Image createImage(InputStream inputstream)
    {
        Image image;
        ByteArrayOutputStream bytearrayoutputstream = new ByteArrayOutputStream();
        for(int i = inputstream.read(); i != -1; i = inputstream.read())
            bytearrayoutputstream.write(i);

        image = Toolkit.getDefaultToolkit().createImage(bytearrayoutputstream.toByteArray());
        loadImage(image);
        return image;
        Exception exception;
        exception;
        throw new ErrorException((new StringBuilder()).append("Exception: ").append(exception).toString());
    }

    public static Image createImage(String as[])
    {
        return createImage(((InputStream) (new HexInputStream(as))));
    }

    public static int[][] getPixelArray(Image image)
    {
        Component component = getImageObserver();
        int i = image.getWidth(component);
        int j = image.getHeight(component);
        int ai[] = new int[i * j];
        int ai1[][] = new int[j][i];
        PixelGrabber pixelgrabber = new PixelGrabber(image.getSource(), 0, 0, i, j, ai, 0, i);
        try
        {
            pixelgrabber.grabPixels();
        }
        catch(InterruptedException interruptedexception)
        {
            throw new ErrorException("Transfer interrupted");
        }
        if((pixelgrabber.getStatus() & 0x80) != 0)
            throw new ErrorException("Transfer aborted");
        for(int k = 0; k < j; k++)
            System.arraycopy(ai, k * i, ai1[k], 0, i);

        return ai1;
    }

    public static void saveImage(Image image, String s)
    {
        saveImage(image, new File(s));
    }

    public static void saveImage(Image image, File file)
    {
        String s = file.getName();
        int i = s.lastIndexOf('.');
        if(i <= 0)
            throw new ErrorException("saveImage: No image suffix in file name");
        String s1 = s.substring(i + 1);
        ImageSaver imagesaver = findImageSaver(s1);
        if(imagesaver == null)
            throw new ErrorException((new StringBuilder()).append("saveImage: No support for .").append(s1).append(" format").toString());
        if(file.exists() && !file.delete())
            throw new ErrorException((new StringBuilder()).append("saveImage: Cannot replace ").append(s).toString());
        try
        {
            BufferedOutputStream bufferedoutputstream = new BufferedOutputStream(new FileOutputStream(file));
            imagesaver.setOutputStream(bufferedoutputstream);
            imagesaver.saveImage(image);
            imagesaver.updateFileType(file);
            bufferedoutputstream.close();
        }
        catch(IOException ioexception)
        {
            throw new ErrorException((new StringBuilder()).append("saveImage: ").append(ioexception.getMessage()).toString());
        }
    }

    public static AudioClip loadAudioClip(String s)
    {
        return loadAudioClip(s, ".:sounds");
    }

    public static AudioClip loadAudioClip(String s, String s1)
    {
        AudioClip audioclip;
        audioclip = (AudioClip)audioClipTable.get(s);
        if(audioclip != null)
            return audioclip;
        if(!s.startsWith("http:"))
            break MISSING_BLOCK_LABEL_81;
        audioclip = loadAudioClip(new URL(s));
        if(audioclip instanceof SoundClip)
            ((SoundClip)audioclip).setName(s);
        if(cachingEnabled)
            audioClipTable.put(s, audioclip);
        return audioclip;
        MalformedURLException malformedurlexception;
        malformedurlexception;
        throw new ErrorException("loadAudioClip: Malformed URL");
        StringTokenizer stringtokenizer = new StringTokenizer(s1, ":");
        do
        {
            if(audioclip != null || !stringtokenizer.hasMoreTokens())
                break;
            String s2 = stringtokenizer.nextToken();
            s2 = s2.equals(".") ? "" : (new StringBuilder()).append(s2).append("/").toString();
            URL url = null;
            try
            {
                url = RESOURCE_CLASS.getResource((new StringBuilder()).append("/").append(s2).append(s).toString());
                URLConnection urlconnection = url.openConnection();
                if(urlconnection == null || urlconnection.getContentLength() <= 0)
                    url = null;
            }
            catch(Exception exception) { }
            if(url == null)
            {
                Applet applet = JTFTools.getApplet();
                if(applet != null)
                {
                    URL url1 = applet.getCodeBase();
                    if(url1 != null)
                        try
                        {
                            url = new URL(url1, (new StringBuilder()).append(s2).append(s).toString());
                        }
                        catch(MalformedURLException malformedurlexception1) { }
                }
            }
            if(url == null)
                try
                {
                    File file = new File((new StringBuilder()).append(s2).append(s).toString());
                    if(file.canRead())
                        audioclip = createAudioClip(new FileInputStream(file));
                }
                catch(Exception exception1) { }
            else
                audioclip = loadAudioClip(url, false);
        } while(true);
        if(audioclip == null)
            throw new ErrorException((new StringBuilder()).append("Cannot find an audio clip named ").append(s).toString());
        if(audioclip instanceof SoundClip)
            ((SoundClip)audioclip).setName(s);
        if(cachingEnabled)
            audioClipTable.put(s, audioclip);
        return audioclip;
    }

    public static AudioClip loadAudioClip(URL url)
    {
        return loadAudioClip(url, true);
    }

    public static void defineAudioClip(String s, AudioClip audioclip)
    {
        audioClipTable.put(s, audioclip);
    }

    public static void flushAudioClip(String s)
    {
        audioClipTable.remove(s);
    }

    public static AudioClip createAudioClip(InputStream inputstream)
    {
        return new SunAudioClip(inputstream);
        Exception exception;
        exception;
        return new NullAudioClip();
    }

    public static AudioClip createAudioClip(String as[])
    {
        return createAudioClip(((InputStream) (new HexInputStream(as))));
    }

    public static InputStream openDataFile(String s)
    {
        return openDataFile(s, ".:datafiles");
    }

    public static InputStream openDataFile(String s, String s1)
    {
        Object obj;
        obj = null;
        if(!s.startsWith("http:"))
            break MISSING_BLOCK_LABEL_34;
        return openDataFile(new URL(s));
        MalformedURLException malformedurlexception;
        malformedurlexception;
        throw new ErrorException("openDataFile: Malformed URL");
        StringTokenizer stringtokenizer = new StringTokenizer(s1, ":");
        do
        {
            if(obj != null || !stringtokenizer.hasMoreTokens())
                break;
            String s2 = stringtokenizer.nextToken();
            s2 = s2.equals(".") ? "" : (new StringBuilder()).append(s2).append("/").toString();
            URL url = null;
            try
            {
                url = RESOURCE_CLASS.getResource((new StringBuilder()).append("/").append(s2).append(s).toString());
                URLConnection urlconnection = url.openConnection();
                if(urlconnection == null || urlconnection.getContentLength() <= 0)
                    url = null;
            }
            catch(Exception exception) { }
            if(url == null)
            {
                Applet applet = JTFTools.getApplet();
                if(applet != null)
                {
                    URL url1 = applet.getCodeBase();
                    if(url1 != null)
                        try
                        {
                            url = new URL(url1, (new StringBuilder()).append(s2).append(s).toString());
                        }
                        catch(MalformedURLException malformedurlexception1) { }
                }
            }
            if(url == null)
                try
                {
                    File file = new File((new StringBuilder()).append(s2).append(s).toString());
                    if(file.canRead())
                        obj = new FileInputStream(file);
                }
                catch(Exception exception1) { }
            else
                obj = openDataFile(url, false);
        } while(true);
        if(obj == null)
            throw new ErrorException((new StringBuilder()).append("Cannot find a file named ").append(s).toString());
        else
            return ((InputStream) (obj));
    }

    public static InputStream openDataFile(URL url)
    {
        return openDataFile(url, true);
    }

    public static BufferedImage createBufferedImage(Image image, int i)
    {
        int j = image.getWidth(null);
        int k = image.getHeight(null);
        BufferedImage bufferedimage = new BufferedImage(j, k, i);
        Graphics2D graphics2d = bufferedimage.createGraphics();
        switch(i)
        {
        case 2: // '\002'
        case 3: // '\003'
        case 6: // '\006'
        case 7: // '\007'
            graphics2d.setComposite(AlphaComposite.Src);
            // fall through

        case 4: // '\004'
        case 5: // '\005'
        default:
            graphics2d.drawImage(image, 0, 0, null);
            break;
        }
        graphics2d.dispose();
        return bufferedimage;
    }

    public static InputStream getHexInputStream(String as[])
    {
        return new HexInputStream(as);
    }

    public static void setCachingEnabled(boolean flag)
    {
        cachingEnabled = flag;
    }

    public static boolean isCachingEnabled()
    {
        return cachingEnabled;
    }

    public static Component getImageObserver()
    {
        return JTFTools.createEmptyContainer();
    }

    public static void beep()
    {
        Toolkit.getDefaultToolkit().beep();
    }

    public static Image loadImage(URL url, boolean flag)
    {
        Image image = null;
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        try
        {
            URLConnection urlconnection = url.openConnection();
            if(isResource(url) || urlconnection.getContentLength() > 0)
            {
                Object obj = urlconnection.getContent();
                if(obj instanceof ImageProducer)
                    image = toolkit.createImage((ImageProducer)obj);
                else
                if(obj != null)
                    image = toolkit.getImage(url);
            }
        }
        catch(IOException ioexception) { }
        if(flag)
        {
            if(image == null)
                throw new ErrorException((new StringBuilder()).append("Cannot load image from ").append(url).toString());
            loadImage(image);
        }
        return image;
    }

    private static ImageSaver findImageSaver(String s)
    {
        s = s.toUpperCase();
        HashMap hashmap = suffixTable;
        JVM INSTR monitorenter ;
        ImageSaver imagesaver;
        imagesaver = (ImageSaver)suffixTable.get(s);
        if(imagesaver != null)
            break MISSING_BLOCK_LABEL_79;
        try
        {
            Class class1 = Class.forName((new StringBuilder()).append("acm.util.").append(s).append("ImageSaver").toString());
            imagesaver = (ImageSaver)class1.newInstance();
        }
        catch(Exception exception)
        {
            return null;
        }
        suffixTable.put(s, imagesaver);
        imagesaver;
        hashmap;
        JVM INSTR monitorexit ;
        return;
        Exception exception1;
        exception1;
        throw exception1;
    }

    public static AudioClip loadAudioClip(URL url, boolean flag)
    {
        AudioClip audioclip = null;
        try
        {
            URLConnection urlconnection = url.openConnection();
            if(isResource(url) || urlconnection.getContentLength() > 0)
                audioclip = createAudioClip(urlconnection.getInputStream());
        }
        catch(IOException ioexception) { }
        if(flag && audioclip == null)
            throw new ErrorException((new StringBuilder()).append("Cannot load audio clip from ").append(url).toString());
        else
            return audioclip;
    }

    public static InputStream openDataFile(URL url, boolean flag)
    {
        InputStream inputstream = null;
        try
        {
            URLConnection urlconnection = url.openConnection();
            if(isResource(url) || urlconnection.getContentLength() > 0)
                inputstream = urlconnection.getInputStream();
        }
        catch(IOException ioexception) { }
        if(flag && inputstream == null)
            throw new ErrorException((new StringBuilder()).append("Cannot read data from ").append(url).toString());
        else
            return inputstream;
    }

    private static boolean isResource(URL url)
    {
        String s = url.toString().toLowerCase();
        return s.startsWith("jar:") || s.startsWith("file:");
    }

    public static final String DEFAULT_IMAGE_PATH = ".:images";
    public static final String DEFAULT_AUDIO_PATH = ".:sounds";
    public static final String DEFAULT_DATAFILE_PATH = ".:datafiles";
    private static boolean cachingEnabled = false;
    private static HashMap imageTable = new HashMap();
    private static HashMap audioClipTable = new HashMap();
    private static HashMap suffixTable = new HashMap();
    private static final Class RESOURCE_CLASS = (new MediaTools()).getClass();

}
