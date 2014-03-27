// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;


class ReverseColorMap
{
    private static class ColorRecord
    {

        int rgb;
        int ipalette;

        ColorRecord(int i, int j)
        {
            rgb = i;
            ipalette = j;
        }
    }


    ReverseColorMap()
    {
        hTable = new ColorRecord[2053];
    }

    int getPaletteIndex(int i)
    {
        ColorRecord colorrecord;
        for(int j = i % hTable.length; (colorrecord = hTable[j]) != null && colorrecord.rgb != i; j = ++j % hTable.length);
        if(colorrecord != null)
            return colorrecord.ipalette;
        else
            return -1;
    }

    void put(int i, int j)
    {
        int k;
        for(k = i % hTable.length; hTable[k] != null; k = ++k % hTable.length);
        hTable[k] = new ColorRecord(i, j);
    }

    private static final int HCAPACITY = 2053;
    private ColorRecord hTable[];
}
