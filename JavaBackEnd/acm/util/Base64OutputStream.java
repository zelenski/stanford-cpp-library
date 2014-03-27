// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   JTFTools.java

package acm.util;

import java.io.*;

class Base64OutputStream extends OutputStream
{

    public Base64OutputStream(PrintStream printstream)
    {
        out = printstream;
        out.flush();
        byteCount = 0;
        columnCount = 0;
        buffer = 0;
    }

    public void write(int i)
        throws IOException
    {
        buffer = buffer << 8 | i & 0xff;
        byteCount++;
        if(byteCount == 3)
        {
            out.write(BASE64[buffer >> 18 & 0x3f]);
            out.write(BASE64[buffer >> 12 & 0x3f]);
            out.write(BASE64[buffer >> 6 & 0x3f]);
            out.write(BASE64[buffer & 0x3f]);
            columnCount += 4;
            if(columnCount >= 76)
            {
                columnCount = 0;
                out.println();
            }
            byteCount = 0;
            buffer = 0;
        }
    }

    public void flush()
    {
        out.flush();
    }

    public void close()
    {
        pad();
        out.close();
    }

    public void pad()
    {
        switch(byteCount)
        {
        case 1: // '\001'
            out.write(BASE64[buffer >> 2 & 0x3f]);
            out.write(BASE64[buffer << 4 & 0x3f]);
            out.write(61);
            out.write(61);
            break;

        case 2: // '\002'
            out.write(BASE64[buffer >> 10 & 0x3f]);
            out.write(BASE64[buffer >> 4 & 0x3f]);
            out.write(BASE64[buffer << 2 & 0x3f]);
            out.write(61);
            break;
        }
        out.println();
        out.println();
    }

    private static final int COLUMNS = 76;
    private static final char BASE64[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
        'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 
        'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 
        'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', 
        '8', '9', '+', '/'
    };
    private int buffer;
    private int columnCount;
    private int byteCount;
    private PrintStream out;

}
