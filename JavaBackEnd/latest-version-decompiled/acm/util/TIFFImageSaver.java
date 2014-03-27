// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.Image;

// Referenced classes of package acm.util:
//            ImageSaver, MediaTools

class TIFFImageSaver extends ImageSaver
{

    TIFFImageSaver()
    {
    }

    public void saveImage(Image image)
    {
        pixels = MediaTools.getPixelArray(image);
        width = pixels[0].length;
        height = pixels.length;
        calculateOffsets();
        dumpHeader();
        dumpIFD();
        dumpNullIFD();
        dumpBitsPerSampleData();
        dumpResolutionData();
        dumpStripPointers();
        dumpStripByteCounts();
        dumpStripData();
    }

    private void dumpHeader()
    {
        dumpByte(77);
        dumpByte(77);
        dumpShort(42);
        dumpLong(8);
    }

    private void dumpIFD()
    {
        dumpShort(13);
        dumpIFDNewSubFileType();
        dumpIFDImageWidth();
        dumpIFDImageHeight();
        dumpIFDBitsPerSample();
        dumpIFDCompression();
        dumpIFDPhotometricInterpration();
        dumpIFDStripPointers();
        dumpIFDSamplesPerPixel();
        dumpIFDRowsPerStrip();
        dumpIFDStripByteCounts();
        dumpIFDXResolution();
        dumpIFDYResolution();
        dumpIFDResolutionUnit();
    }

    private void dumpIFDNewSubFileType()
    {
        dumpShort(254);
        dumpShort(4);
        dumpLong(1);
        dumpLong(0);
    }

    private void dumpIFDImageWidth()
    {
        dumpShort(256);
        dumpShort(4);
        dumpLong(1);
        dumpLong(width);
    }

    private void dumpIFDImageHeight()
    {
        dumpShort(257);
        dumpShort(4);
        dumpLong(1);
        dumpLong(height);
    }

    private void dumpIFDBitsPerSample()
    {
        dumpShort(258);
        dumpShort(4);
        dumpLong(3);
        dumpLong(offsetBitsPerSample);
    }

    private void dumpIFDCompression()
    {
        dumpShort(259);
        dumpShort(3);
        dumpLong(1);
        dumpShort(1);
        dumpShort(0);
    }

    private void dumpIFDPhotometricInterpration()
    {
        dumpShort(262);
        dumpShort(3);
        dumpLong(1);
        dumpShort(2);
        dumpShort(0);
    }

    private void dumpIFDStripPointers()
    {
        dumpShort(273);
        dumpShort(4);
        dumpLong(height);
        dumpLong(offsetStripPointers);
    }

    private void dumpIFDSamplesPerPixel()
    {
        dumpShort(277);
        dumpShort(3);
        dumpLong(1);
        dumpShort(3);
        dumpShort(0);
    }

    private void dumpIFDRowsPerStrip()
    {
        dumpShort(278);
        dumpShort(4);
        dumpLong(1);
        dumpLong(1);
    }

    private void dumpIFDStripByteCounts()
    {
        dumpShort(279);
        dumpShort(4);
        dumpLong(height);
        dumpLong(offsetStripByteCounts);
    }

    private void dumpIFDXResolution()
    {
        dumpShort(282);
        dumpShort(5);
        dumpLong(1);
        dumpLong(offsetXResolution);
    }

    private void dumpIFDYResolution()
    {
        dumpShort(283);
        dumpShort(5);
        dumpLong(1);
        dumpLong(offsetYResolution);
    }

    private void dumpIFDResolutionUnit()
    {
        dumpShort(296);
        dumpShort(3);
        dumpLong(1);
        dumpShort(1);
        dumpShort(0);
    }

    private void dumpNullIFD()
    {
        dumpLong(0);
    }

    private void dumpBitsPerSampleData()
    {
        dumpLong(8);
        dumpLong(8);
        dumpLong(8);
    }

    private void dumpResolutionData()
    {
        dumpLong(72);
        dumpLong(1);
        dumpLong(72);
        dumpLong(1);
    }

    private void dumpStripPointers()
    {
        for(int i = 0; i < height; i++)
            dumpLong(offsetData + i * stripDelta);

    }

    private void dumpStripByteCounts()
    {
        for(int i = 0; i < height; i++)
            dumpLong(3 * width);

    }

    private void dumpStripData()
    {
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                int l = pixels[i][j];
                dumpByte(getPixelComponent(l, 'R'));
                dumpByte(getPixelComponent(l, 'G'));
                dumpByte(getPixelComponent(l, 'B'));
            }

            for(int k = 3 * width; k < stripDelta; k++)
                dumpByte(0);

        }

    }

    private void calculateOffsets()
    {
        offsetIFD = 8;
        offsetNullIFD = offsetIFD + 156 + 2;
        offsetBitsPerSample = offsetNullIFD + 4;
        offsetXResolution = offsetBitsPerSample + 12;
        offsetYResolution = offsetXResolution + 8;
        offsetStripPointers = offsetYResolution + 8;
        offsetStripByteCounts = offsetStripPointers + 4 * height;
        offsetData = offsetStripByteCounts + 4 * height;
        stripDelta = 3 * width + 3 & -4;
    }

    private static final int HEADER_SIZE = 8;
    private static final int IFD_OP_COUNT = 13;
    public static final int TT_BYTE = 1;
    public static final int TT_ASCII = 2;
    public static final int TT_SHORT = 3;
    public static final int TT_LONG = 4;
    public static final int TT_RATIONAL = 5;
    private int pixels[][];
    private int width;
    private int height;
    private int offsetIFD;
    private int offsetNullIFD;
    private int offsetBitsPerSample;
    private int offsetXResolution;
    private int offsetYResolution;
    private int offsetStripPointers;
    private int offsetStripByteCounts;
    private int offsetData;
    private int stripDelta;
}
