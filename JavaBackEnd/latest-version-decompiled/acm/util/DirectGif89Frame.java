// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.Image;
import java.awt.image.PixelGrabber;
import java.io.IOException;

// Referenced classes of package acm.util:
//            Gif89Frame

class DirectGif89Frame extends Gif89Frame
{

    public DirectGif89Frame(Image image)
        throws IOException
    {
        PixelGrabber pixelgrabber = new PixelGrabber(image, 0, 0, -1, -1, true);
        String s = null;
        try
        {
            if(!pixelgrabber.grabPixels())
                s = "can't grab pixels from image";
        }
        catch(InterruptedException interruptedexception)
        {
            s = "interrupted grabbing pixels from image";
        }
        if(s != null)
        {
            throw new IOException((new StringBuilder()).append(s).append(" (").append(getClass().getName()).append(")").toString());
        } else
        {
            theWidth = pixelgrabber.getWidth();
            theHeight = pixelgrabber.getHeight();
            argbPixels = (int[])(int[])pixelgrabber.getPixels();
            ciPixels = new byte[argbPixels.length];
            return;
        }
    }

    public DirectGif89Frame(int i, int j, int ai[])
    {
        theWidth = i;
        theHeight = j;
        argbPixels = new int[theWidth * theHeight];
        System.arraycopy(ai, 0, argbPixels, 0, argbPixels.length);
        ciPixels = new byte[argbPixels.length];
    }

    Object getPixelSource()
    {
        return argbPixels;
    }

    private int argbPixels[];
}
