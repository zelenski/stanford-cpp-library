// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.Image;
import java.io.*;
import javax.imageio.ImageIO;
import javax.imageio.stream.ImageOutputStream;
import javax.imageio.stream.MemoryCacheImageOutputStream;

// Referenced classes of package acm.util:
//            ErrorException, MediaTools

class ImageSaver
{

    public ImageSaver()
    {
    }

    public ImageSaver(String s, int i)
    {
        formatName = s;
        bufferType = i;
    }

    public void setOutputStream(OutputStream outputstream)
    {
        out = outputstream;
    }

    public OutputStream getOutputStream()
    {
        return out;
    }

    public void saveImage(Image image)
    {
        java.awt.image.BufferedImage bufferedimage = MediaTools.createBufferedImage(image, bufferType);
        MemoryCacheImageOutputStream memorycacheimageoutputstream = new MemoryCacheImageOutputStream(getOutputStream());
        try
        {
            if(!ImageIO.write(bufferedimage, formatName, memorycacheimageoutputstream))
                throw new IOException("ImageIO.write failed");
            memorycacheimageoutputstream.close();
        }
        catch(IOException ioexception)
        {
            throw new ErrorException((new StringBuilder()).append("saveImage: ").append(ioexception.getMessage()).toString());
        }
    }

    public void updateFileType(File file)
    {
    }

    public void dumpByte(int i)
    {
        try
        {
            out.write(i);
        }
        catch(IOException ioexception)
        {
            throw new ErrorException((new StringBuilder()).append("saveImage: ").append(ioexception.getMessage()).toString());
        }
    }

    public void dumpShort(int i)
    {
        try
        {
            out.write(i >> 8);
            out.write(i);
        }
        catch(IOException ioexception)
        {
            throw new ErrorException((new StringBuilder()).append("saveImage: ").append(ioexception.getMessage()).toString());
        }
    }

    public void dumpLong(int i)
    {
        try
        {
            out.write(i >> 24);
            out.write(i >> 16);
            out.write(i >> 8);
            out.write(i);
        }
        catch(IOException ioexception)
        {
            throw new ErrorException((new StringBuilder()).append("saveImage: ").append(ioexception.getMessage()).toString());
        }
    }

    public int getPixelComponent(int i, char c)
    {
        int j = i >> 24 & 0xff;
        switch(c)
        {
        case 82: // 'R'
            i >>= 16;
            break;

        case 71: // 'G'
            i >>= 8;
            break;

        default:
            throw new ErrorException("getPixelComponent: Illegal color");

        case 66: // 'B'
            break;
        }
        i &= 0xff;
        return (j * i + (255 - j) * 255) / 255;
    }

    private OutputStream out;
    private String formatName;
    private int bufferType;
}
