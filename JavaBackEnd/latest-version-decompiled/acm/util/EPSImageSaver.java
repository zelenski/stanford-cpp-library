// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.awt.Image;

// Referenced classes of package acm.util:
//            ImageSaver, ErrorException

class EPSImageSaver extends ImageSaver
{

    EPSImageSaver()
    {
    }

    public void saveImage(Image image)
    {
        throw new ErrorException("saveImage: Not yet implemented");
    }
}
