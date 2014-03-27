// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.Image;
import java.io.File;
import java.io.IOException;

// Referenced classes of package acm.util:
//            ImageSaver, Gif89Encoder, ErrorException, Gif89Frame, 
//            Platform

class GIF89ImageSaver extends ImageSaver
{

    GIF89ImageSaver()
    {
    }

    public void saveImage(Image image)
    {
        try
        {
            Gif89Encoder gif89encoder = new Gif89Encoder(image);
            gif89encoder.setTransparentIndex(0);
            gif89encoder.getFrameAt(0).setInterlaced(true);
            gif89encoder.encode(getOutputStream());
        }
        catch(IOException ioexception)
        {
            throw new ErrorException((new StringBuilder()).append("saveImage: ").append(ioexception.getMessage()).toString());
        }
    }

    public void updateFileType(File file)
    {
        Platform.setFileTypeAndCreator(file, "GIFf", "prvw");
    }
}
