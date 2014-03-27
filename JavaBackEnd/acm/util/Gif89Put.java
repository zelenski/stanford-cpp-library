// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.io.IOException;
import java.io.OutputStream;

final class Gif89Put
{

    Gif89Put()
    {
    }

    static void ascii(String s, OutputStream outputstream)
        throws IOException
    {
        byte abyte0[] = new byte[s.length()];
        for(int i = 0; i < abyte0.length; i++)
            abyte0[i] = (byte)s.charAt(i);

        outputstream.write(abyte0);
    }

    static void leShort(int i, OutputStream outputstream)
        throws IOException
    {
        outputstream.write(i & 0xff);
        outputstream.write(i >> 8 & 0xff);
    }
}
