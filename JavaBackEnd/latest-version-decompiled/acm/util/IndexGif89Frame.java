// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;


// Referenced classes of package acm.util:
//            Gif89Frame

class IndexGif89Frame extends Gif89Frame
{

    public IndexGif89Frame(int i, int j, byte abyte0[])
    {
        theWidth = i;
        theHeight = j;
        ciPixels = new byte[theWidth * theHeight];
        System.arraycopy(abyte0, 0, ciPixels, 0, ciPixels.length);
    }

    Object getPixelSource()
    {
        return ciPixels;
    }
}
