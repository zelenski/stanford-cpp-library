// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.Color;
import java.io.IOException;
import java.io.OutputStream;

// Referenced classes of package acm.util:
//            ReverseColorMap, DirectGif89Frame, IndexGif89Frame, Gif89Frame

class GifColorTable
{

    GifColorTable()
    {
        theColors = new int[256];
        transparentIndex = -1;
        ciCount = 0;
        ciLookup = new ReverseColorMap();
    }

    GifColorTable(Color acolor[])
    {
        theColors = new int[256];
        transparentIndex = -1;
        ciCount = 0;
        int i = Math.min(theColors.length, acolor.length);
        for(int j = 0; j < i; j++)
            theColors[j] = acolor[j].getRGB();

    }

    int getDepth()
    {
        return colorDepth;
    }

    int getTransparent()
    {
        return transparentIndex;
    }

    void setTransparent(int i)
    {
        transparentIndex = i;
    }

    void processPixels(Gif89Frame gif89frame)
        throws IOException
    {
        if(gif89frame instanceof DirectGif89Frame)
            filterPixels((DirectGif89Frame)gif89frame);
        else
            trackPixelUsage((IndexGif89Frame)gif89frame);
    }

    void closePixelProcessing()
    {
        colorDepth = computeColorDepth(ciCount);
    }

    void encode(OutputStream outputstream)
        throws IOException
    {
        int i = 1 << colorDepth;
        for(int j = 0; j < i; j++)
        {
            outputstream.write(theColors[j] >> 16 & 0xff);
            outputstream.write(theColors[j] >> 8 & 0xff);
            outputstream.write(theColors[j] & 0xff);
        }

    }

    private void filterPixels(DirectGif89Frame directgif89frame)
        throws IOException
    {
        if(ciLookup == null)
            throw new IOException("RGB frames require palette autodetection");
        int ai[] = (int[])(int[])directgif89frame.getPixelSource();
        byte abyte0[] = directgif89frame.getPixelSink();
        int i = ai.length;
        for(int j = 0; j < i; j++)
        {
            int k = ai[j];
            if(k >>> 24 < 128)
                if(transparentIndex == -1)
                    transparentIndex = ciCount;
                else
                if(k != theColors[transparentIndex])
                {
                    abyte0[j] = (byte)transparentIndex;
                    continue;
                }
            int l = ciLookup.getPaletteIndex(k & 0xffffff);
            if(l == -1)
            {
                if(ciCount == 256)
                    throw new IOException("can't encode as GIF (> 256 colors)");
                theColors[ciCount] = k;
                ciLookup.put(k & 0xffffff, ciCount);
                abyte0[j] = (byte)ciCount;
                ciCount++;
            } else
            {
                abyte0[j] = (byte)l;
            }
        }

    }

    private void trackPixelUsage(IndexGif89Frame indexgif89frame)
    {
        byte abyte0[] = (byte[])(byte[])indexgif89frame.getPixelSource();
        int i = abyte0.length;
        for(int j = 0; j < i; j++)
            if(abyte0[j] >= ciCount)
                ciCount = abyte0[j] + 1;

    }

    private int computeColorDepth(int i)
    {
        if(i <= 2)
            return 1;
        if(i <= 4)
            return 2;
        return i > 16 ? 8 : 4;
    }

    private int theColors[];
    private int colorDepth;
    private int transparentIndex;
    private int ciCount;
    private ReverseColorMap ciLookup;
}
