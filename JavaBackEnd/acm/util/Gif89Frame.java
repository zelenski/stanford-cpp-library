// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.Point;
import java.io.IOException;
import java.io.OutputStream;

// Referenced classes of package acm.util:
//            GifPixelsEncoder, Gif89Put

abstract class Gif89Frame
{

    public void setPosition(Point point)
    {
        thePosition = new Point(point);
    }

    public void setInterlaced(boolean flag)
    {
        isInterlaced = flag;
    }

    public void setDelay(int i)
    {
        csecsDelay = i;
    }

    public void setDisposalMode(int i)
    {
        disposalCode = i;
    }

    Gif89Frame()
    {
        theWidth = -1;
        theHeight = -1;
        thePosition = new Point(0, 0);
        disposalCode = 1;
    }

    abstract Object getPixelSource();

    int getWidth()
    {
        return theWidth;
    }

    int getHeight()
    {
        return theHeight;
    }

    byte[] getPixelSink()
    {
        return ciPixels;
    }

    void encode(OutputStream outputstream, boolean flag, int i, int j)
        throws IOException
    {
        writeGraphicControlExtension(outputstream, flag, j);
        writeImageDescriptor(outputstream);
        (new GifPixelsEncoder(theWidth, theHeight, ciPixels, isInterlaced, i)).encode(outputstream);
    }

    private void writeGraphicControlExtension(OutputStream outputstream, boolean flag, int i)
        throws IOException
    {
        boolean flag1 = i != -1;
        if(flag1 || flag)
        {
            outputstream.write(33);
            outputstream.write(249);
            outputstream.write(4);
            outputstream.write(disposalCode << 2 | flag1);
            Gif89Put.leShort(csecsDelay, outputstream);
            outputstream.write(i);
            outputstream.write(0);
        }
    }

    private void writeImageDescriptor(OutputStream outputstream)
        throws IOException
    {
        outputstream.write(44);
        Gif89Put.leShort(thePosition.x, outputstream);
        Gif89Put.leShort(thePosition.y, outputstream);
        Gif89Put.leShort(theWidth, outputstream);
        Gif89Put.leShort(theHeight, outputstream);
        outputstream.write(isInterlaced ? 64 : 0);
    }

    public static final int DM_UNDEFINED = 0;
    public static final int DM_LEAVE = 1;
    public static final int DM_BGCOLOR = 2;
    public static final int DM_REVERT = 3;
    int theWidth;
    int theHeight;
    byte ciPixels[];
    private Point thePosition;
    private boolean isInterlaced;
    private int csecsDelay;
    private int disposalCode;
}
