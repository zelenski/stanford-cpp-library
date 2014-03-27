// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.Image;
import java.io.File;
import java.util.Iterator;
import javax.imageio.ImageIO;

// Referenced classes of package acm.util:
//            ImageSaver, GIF89ImageSaver

class GIFImageSaver extends ImageSaver
{

    public GIFImageSaver()
    {
        super("GIF", 2);
        if(!ImageIO.getImageWritersBySuffix("gif").hasNext())
            gif89Saver = new GIF89ImageSaver();
    }

    public void saveImage(Image image)
    {
        if(gif89Saver != null)
        {
            gif89Saver.setOutputStream(getOutputStream());
            gif89Saver.saveImage(image);
        } else
        {
            super.saveImage(image);
        }
    }

    public void updateFileType(File file)
    {
        if(gif89Saver != null)
            gif89Saver.updateFileType(file);
    }

    private GIF89ImageSaver gif89Saver;
}
