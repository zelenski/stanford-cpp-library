// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GraphicsProgram.java

package acm.program;

import acm.graphics.GDimension;
import acm.graphics.GObject;
import acm.util.ErrorException;
import java.lang.reflect.Method;

// Referenced classes of package acm.program:
//            GraphicsProgram

class GObjectProgram extends GraphicsProgram
{

    GObjectProgram()
    {
    }

    protected void runHook()
    {
        GObject gobject = (GObject)getStartupObject();
        GDimension gdimension = gobject.getSize();
        add(gobject, ((double)getWidth() - gdimension.getWidth()) / 2D, ((double)getHeight() - gdimension.getHeight()) / 2D);
        try
        {
            Class class1 = gobject.getClass();
            String s = class1.getName();
            s = s.substring(s.lastIndexOf(".") + 1);
            setTitle(s);
            Method method = class1.getMethod("run", new Class[0]);
            if(method == null)
                throw new ErrorException((new StringBuilder()).append(s).append(" has no run method").toString());
            method.invoke(gobject, new Object[0]);
        }
        catch(Exception exception)
        {
            throw new ErrorException(exception);
        }
    }
}
