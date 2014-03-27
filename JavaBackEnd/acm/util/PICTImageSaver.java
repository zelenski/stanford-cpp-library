// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.Image;

// Referenced classes of package acm.util:
//            ImageSaver, MediaTools

class PICTImageSaver extends ImageSaver
{

    PICTImageSaver()
    {
        paddingFlag = true;
    }

    public void saveImage(Image image)
    {
        pixels = MediaTools.getPixelArray(image);
        Object obj = image.getProperty("PSPreview", null);
        psPreview = (obj instanceof String[]) ? (String[])(String[])obj : null;
        width = pixels[0].length;
        height = pixels.length;
        rowBytes = 4 * width;
        if(paddingFlag)
            dumpPadding();
        dumpHeader();
        dumpDefHilite();
        dumpClipRegion();
        dumpBoundsMarkers();
        if(psPreview != null)
        {
            dumpShort(160);
            dumpShort(190);
        }
        dumpDirectBitsRect();
        if(psPreview != null)
        {
            dumpPSPreview();
            dumpShort(160);
            dumpShort(191);
        }
        dumpEndPict();
    }

    public void setPaddingFlag(boolean flag)
    {
        paddingFlag = flag;
    }

    private void dumpPadding()
    {
        for(int i = 0; i < 512; i++)
            dumpByte(0);

    }

    private void dumpHeader()
    {
        dumpShort(0);
        dumpShort(0);
        dumpShort(0);
        dumpShort(height);
        dumpShort(width);
        dumpShort(17);
        dumpShort(767);
        dumpShort(3072);
        dumpShort(65534);
        dumpShort(0);
        dumpShort(72);
        dumpShort(0);
        dumpShort(72);
        dumpShort(0);
        dumpShort(0);
        dumpShort(0);
        dumpShort(height);
        dumpShort(width);
        dumpLong(0);
    }

    private void dumpDefHilite()
    {
        dumpShort(30);
    }

    private void dumpClipRegion()
    {
        dumpShort(1);
        dumpShort(10);
        dumpShort(0);
        dumpShort(0);
        dumpShort(height);
        dumpShort(width);
    }

    private void dumpBoundsMarkers()
    {
        dumpShort(34);
        dumpShort(0);
        dumpShort(0);
        dumpShort(0);
        dumpShort(34);
        dumpShort(height);
        dumpShort(width);
        dumpShort(0);
    }

    private void dumpDirectBitsRect()
    {
        dumpShort(154);
        dumpPixMap();
        dumpShort(0);
        dumpShort(0);
        dumpShort(height);
        dumpShort(width);
        dumpShort(0);
        dumpShort(0);
        dumpShort(height);
        dumpShort(width);
        dumpShort(0);
        dumpPixelData();
    }

    private void dumpPixMap()
    {
        dumpLong(255);
        dumpShort(rowBytes | 0x8000);
        dumpShort(0);
        dumpShort(0);
        dumpShort(height);
        dumpShort(width);
        dumpShort(0);
        dumpShort(4);
        dumpLong(0);
        dumpShort(72);
        dumpShort(0);
        dumpShort(72);
        dumpShort(0);
        dumpShort(16);
        dumpShort(32);
        dumpShort(3);
        dumpShort(8);
        dumpLong(0);
        dumpLong(0);
        dumpLong(0);
    }

    private void dumpEndPict()
    {
        dumpShort(255);
    }

    private void dumpPixelData()
    {
        int i = 0;
        byte abyte0[] = new byte[rowBytes];
        for(int j = 0; j < height; j++)
        {
            int k = packScanLine(abyte0, pixels[j]);
            if(rowBytes > 250)
            {
                dumpShort(k);
                i += 2;
            } else
            {
                dumpByte(k);
                i++;
            }
            for(int l = 0; l < k; l++)
                dumpByte(abyte0[l]);

            i += k;
        }

        if(i % 2 == 1)
            dumpByte(0);
    }

    private int packScanLine(byte abyte0[], int ai[])
    {
        int i = 0;
label0:
        for(int j = 0; j < 3; j++)
        {
            char c = "RGB".charAt(j);
            int k = i;
            int l = i + 1;
            for(int i1 = 0; i1 < width;)
            {
                int j1 = getPixelComponent(ai[i1++], c);
                abyte0[l++] = (byte)j1;
                int l1 = 1;
                boolean flag = false;
                if(i1 < width)
                {
                    int k2 = getPixelComponent(ai[i1], c);
                    flag = j1 == k2;
                    if(flag)
                        do
                        {
                            if(l1 >= 128 || i1 >= width)
                                break;
                            int l2 = getPixelComponent(ai[i1], c);
                            if(j1 != l2)
                                break;
                            l1++;
                            i1++;
                        } while(true);
                    else
                        do
                        {
                            if(l1 >= 128 || i1 >= width)
                                break;
                            int i3 = getPixelComponent(ai[i1], c);
                            if(j1 == i3)
                            {
                                l--;
                                l1--;
                                i1--;
                                break;
                            }
                            abyte0[l++] = (byte)i3;
                            j1 = i3;
                            l1++;
                            i1++;
                        } while(true);
                }
                if(flag)
                    abyte0[i] = (byte)(0x80 | 129 - l1);
                else
                    abyte0[i] = (byte)(l1 - 1);
                i = l++;
            }

            if(i - k <= width + width / 128)
                continue;
            int k1 = width;
            i = k;
            int i2 = 0;
            do
            {
                if(i2 >= width)
                    continue label0;
                if(i2 % 128 == 0)
                {
                    int j2 = k1 <= 128 ? k1 : 128;
                    abyte0[i++] = (byte)(j2 - 1);
                    k1 -= j2;
                }
                abyte0[i++] = (byte)getPixelComponent(ai[i2], c);
                i2++;
            } while(true);
        }

        return i;
    }

    private void dumpPSPreview()
    {
        addPSComment("/dictCount countdictstack def");
        addPSComment("/opCount count 1 sub def");
        addPSComment("500 dict begin");
        addPSComment("/showpage {} def");
        addPSComment("0 setgray 0 setlinecap");
        addPSComment("1 setlinewidth 0 setlinejoin");
        addPSComment("10 setmiterlimit [] 0 setdash");
        addPSComment("/languagelevel where {");
        addPSComment("  pop languagelevel");
        addPSComment("  1 ne { false setstrokeadjust false setoverprint } if");
        addPSComment("} if");
        addPSComment("gsave");
        addPSComment("clippath pathbbox");
        addPSComment((new StringBuilder()).append("pop pop ").append(height).append(" add translate").toString());
        addPSComment("1 -1 scale");
        for(int i = 0; i < psPreview.length; i++)
            addPSComment(psPreview[i]);

        addPSComment("grestore");
        addPSComment("end");
        addPSComment("count opCount sub {pop} repeat");
        addPSComment("countdictstack dictCount sub {end} repeat");
    }

    private void addPSComment(String s)
    {
        if(s.length() % 2 == 0)
            s = (new StringBuilder()).append(s).append(" ").toString();
        dumpShort(161);
        dumpShort(192);
        dumpShort(s.length() + 1);
        for(int i = 0; i < s.length(); i++)
            dumpByte(s.charAt(i));

        dumpByte(13);
    }

    private static final int OP_CLIP = 1;
    private static final int OP_VERSION = 17;
    private static final int OP_DEF_HILITE = 30;
    private static final int OP_SHORT_LINE = 34;
    private static final int OP_DIRECT_BITS_RECT = 154;
    private static final int OP_SHORT_COMMENT = 160;
    private static final int OP_LONG_COMMENT = 161;
    private static final int OP_END_PICT = 255;
    private static final int OP_HEADER = 3072;
    private static final int PS_BEGIN = 190;
    private static final int PS_END = 191;
    private static final int PS_HANDLE = 192;
    private static final int PS_DICT_SIZE = 500;
    private static final int VERSION = 767;
    private static final int PICT_PADDING = 512;
    private static final int RGB_DIRECT = 16;
    private static final int SRC_COPY = 0;
    private int pixels[][];
    private String psPreview[];
    private int width;
    private int height;
    private int rowBytes;
    private boolean paddingFlag;
}
