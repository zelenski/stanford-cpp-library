// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.*;
import java.io.*;
import java.util.Vector;

// Referenced classes of package acm.util:
//            GifColorTable, Gif89Frame, DirectGif89Frame, IndexGif89Frame, 
//            Gif89Put

class Gif89Encoder
{

    public Gif89Encoder()
    {
        dispDim = new Dimension(0, 0);
        bgIndex = 0;
        loopCount = 1;
        vFrames = new Vector();
        colorTable = new GifColorTable();
    }

    public Gif89Encoder(Image image)
        throws IOException
    {
        this();
        addFrame(image);
    }

    public Gif89Encoder(Color acolor[])
    {
        dispDim = new Dimension(0, 0);
        bgIndex = 0;
        loopCount = 1;
        vFrames = new Vector();
        colorTable = new GifColorTable(acolor);
    }

    public Gif89Encoder(Color acolor[], int i, int j, byte abyte0[])
        throws IOException
    {
        this(acolor);
        addFrame(i, j, abyte0);
    }

    public int getFrameCount()
    {
        return vFrames.size();
    }

    public Gif89Frame getFrameAt(int i)
    {
        return isOk(i) ? (Gif89Frame)vFrames.elementAt(i) : null;
    }

    public void addFrame(Gif89Frame gif89frame)
        throws IOException
    {
        accommodateFrame(gif89frame);
        vFrames.addElement(gif89frame);
    }

    public void addFrame(Image image)
        throws IOException
    {
        addFrame(((Gif89Frame) (new DirectGif89Frame(image))));
    }

    public void addFrame(int i, int j, byte abyte0[])
        throws IOException
    {
        addFrame(((Gif89Frame) (new IndexGif89Frame(i, j, abyte0))));
    }

    public void insertFrame(int i, Gif89Frame gif89frame)
        throws IOException
    {
        accommodateFrame(gif89frame);
        vFrames.insertElementAt(gif89frame, i);
    }

    public void setTransparentIndex(int i)
    {
        colorTable.setTransparent(i);
    }

    public void setLogicalDisplay(Dimension dimension, int i)
    {
        dispDim = new Dimension(dimension);
        bgIndex = i;
    }

    public void setLoopCount(int i)
    {
        loopCount = i;
    }

    public void setComments(String s)
    {
        theComments = s;
    }

    public void setUniformDelay(int i)
    {
        for(int j = 0; j < vFrames.size(); j++)
            ((Gif89Frame)vFrames.elementAt(j)).setDelay(i);

    }

    public void encode(OutputStream outputstream)
        throws IOException
    {
        int i = getFrameCount();
        boolean flag = i > 1;
        colorTable.closePixelProcessing();
        Gif89Put.ascii("GIF89a", outputstream);
        writeLogicalScreenDescriptor(outputstream);
        colorTable.encode(outputstream);
        if(flag && loopCount != 1)
            writeNetscapeExtension(outputstream);
        if(theComments != null && theComments.length() > 0)
            writeCommentExtension(outputstream);
        for(int j = 0; j < i; j++)
            ((Gif89Frame)vFrames.elementAt(j)).encode(outputstream, flag, colorTable.getDepth(), colorTable.getTransparent());

        outputstream.write(59);
        outputstream.flush();
    }

    public static void main(String args[])
    {
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        BufferedOutputStream bufferedoutputstream = new BufferedOutputStream(new FileOutputStream("gif89out.gif"));
        if(args[0].toUpperCase().endsWith(".JPG"))
        {
            (new Gif89Encoder(toolkit.getImage(args[0]))).encode(bufferedoutputstream);
        } else
        {
            BufferedReader bufferedreader = new BufferedReader(new FileReader(args[0]));
            Gif89Encoder gif89encoder = new Gif89Encoder();
            String s;
            while((s = bufferedreader.readLine()) != null) 
                gif89encoder.addFrame(toolkit.getImage(s.trim()));
            gif89encoder.setLoopCount(0);
            gif89encoder.encode(bufferedoutputstream);
            bufferedreader.close();
        }
        bufferedoutputstream.close();
        System.exit(0);
        break MISSING_BLOCK_LABEL_158;
        Exception exception;
        exception;
        exception.printStackTrace();
        System.exit(0);
        break MISSING_BLOCK_LABEL_158;
        Exception exception1;
        exception1;
        System.exit(0);
        throw exception1;
    }

    private void accommodateFrame(Gif89Frame gif89frame)
        throws IOException
    {
        dispDim.width = Math.max(dispDim.width, gif89frame.getWidth());
        dispDim.height = Math.max(dispDim.height, gif89frame.getHeight());
        colorTable.processPixels(gif89frame);
    }

    private void writeLogicalScreenDescriptor(OutputStream outputstream)
        throws IOException
    {
        Gif89Put.leShort(dispDim.width, outputstream);
        Gif89Put.leShort(dispDim.height, outputstream);
        outputstream.write(0xf0 | colorTable.getDepth() - 1);
        outputstream.write(bgIndex);
        outputstream.write(0);
    }

    private void writeNetscapeExtension(OutputStream outputstream)
        throws IOException
    {
        outputstream.write(33);
        outputstream.write(255);
        outputstream.write(11);
        Gif89Put.ascii("NETSCAPE2.0", outputstream);
        outputstream.write(3);
        outputstream.write(1);
        Gif89Put.leShort(loopCount <= 1 ? 0 : loopCount - 1, outputstream);
        outputstream.write(0);
    }

    private void writeCommentExtension(OutputStream outputstream)
        throws IOException
    {
        outputstream.write(33);
        outputstream.write(254);
        int i = theComments.length() % 255;
        int j = theComments.length() / 255;
        int k = j + (i <= 0 ? 0 : 1);
        int l = 0;
        for(int i1 = 0; i1 < k; i1++)
        {
            int j1 = i1 >= j ? i : 255;
            outputstream.write(j1);
            Gif89Put.ascii(theComments.substring(l, l + j1), outputstream);
            l += j1;
        }

        outputstream.write(0);
    }

    private boolean isOk(int i)
    {
        return i >= 0 && i < vFrames.size();
    }

    private Dimension dispDim;
    private GifColorTable colorTable;
    private int bgIndex;
    private int loopCount;
    private String theComments;
    private Vector vFrames;
}
