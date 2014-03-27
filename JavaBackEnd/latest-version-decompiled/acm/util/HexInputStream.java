// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.io.InputStream;

class HexInputStream extends InputStream
{

    public HexInputStream(String as[])
    {
        hex = as;
        arrayIndex = 0;
        charIndex = 0;
    }

    public int read()
    {
        if(arrayIndex >= hex.length)
            return -1;
        if(charIndex >= hex[arrayIndex].length())
        {
            arrayIndex++;
            charIndex = 0;
            return read();
        } else
        {
            int i = Character.digit(hex[arrayIndex].charAt(charIndex++), 16) << 4;
            i |= Character.digit(hex[arrayIndex].charAt(charIndex++), 16);
            return i;
        }
    }

    private String hex[];
    private int arrayIndex;
    private int charIndex;
}
