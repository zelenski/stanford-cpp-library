// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.Image;
import java.io.*;

// Referenced classes of package acm.util:
//            PICTImageSaver, JTFTools, Platform

class RTFImageSaver extends PICTImageSaver
{

    RTFImageSaver()
    {
    }

    public void saveImage(Image image)
    {
        int i = image.getWidth(null);
        int j = image.getHeight(null);
        PrintStream printstream = new PrintStream(new BufferedOutputStream(getOutputStream()));
        setOutputStream(JTFTools.openHexByteOutputStream(printstream));
        for(int k = 0; k < RTF_HEADER.length; k++)
            printstream.println(RTF_HEADER[k]);

        printstream.println((new StringBuilder()).append("{{\\pict\\macpict\\picw").append(i).append("\\pich").append(j).toString());
        setPaddingFlag(false);
        super.saveImage(image);
        printstream.println("}}\\par");
        printstream.println("}");
        printstream.close();
    }

    public void updateFileType(File file)
    {
        Platform.setFileTypeAndCreator(file, "TEXT", "MSWD");
    }

    private static final String RTF_HEADER[] = {
        "{\\rtf1\\mac\\deff2", "{\\fonttbl{\\f20\\froman Times;} {\\f22\\fmodern Courier;}}", "{\\colortbl\\red0\\green0\\blue0;\\red0\\green0\\blue255;", "\\red0\\green255\\blue255;\\red0\\green255\\blue0;", "\\red255\\green0\\blue255;\\red255\\green0\\blue0;", "\\red255\\green255\\blue0;\\red255\\green255\\blue255;}", "{\\stylesheet{\\f20 \\sbasedon222\\snext0 Normal;}}", "\\widowctrl\\ftnbj \\sectd \\sbknone\\linemod0\\linex0\\cols1\\endnhere", "\\pard\\plain \\s0\\qc\\f20"
    };

}
