// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   JTFTools.java

package acm.util;

import java.io.*;

class HexByteOutputStream extends OutputStream
{

    public HexByteOutputStream(PrintStream printstream)
    {
        out = printstream;
        out.flush();
        columnCount = 0;
    }

    public void write(int i)
        throws IOException
    {
        String s = Integer.toHexString(256 + (i & 0xff)).toUpperCase();
        out.write(s.charAt(1));
        out.write(s.charAt(2));
        columnCount += 2;
        if(columnCount >= 76)
        {
            columnCount = 0;
            out.println();
        }
    }

    public void flush()
    {
        out.flush();
    }

    public void close()
    {
        out.close();
    }

    private static final int COLUMNS = 76;
    private int columnCount;
    private PrintStream out;
}
