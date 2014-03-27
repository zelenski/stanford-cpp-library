// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GContainer.java

package acm.graphics;

import acm.util.ErrorException;
import java.util.Iterator;

// Referenced classes of package acm.graphics:
//            GContainer, GObject

class GIterator
    implements Iterator
{

    public GIterator(GContainer gcontainer, int i)
    {
        switch(i)
        {
        case 0: // '\0'
        case 1: // '\001'
            dir = i;
            break;

        default:
            throw new ErrorException("Illegal direction for iterator");
        }
        cont = gcontainer;
        index = 0;
        nElements = gcontainer.getElementCount();
    }

    public boolean hasNext()
    {
        return index < nElements;
    }

    public GObject next()
    {
        if(dir == 1)
            return cont.getElement(nElements - index++ - 1);
        else
            return cont.getElement(index++);
    }

    public GObject nextElement()
    {
        return next();
    }

    public void remove()
    {
        if(dir == 1)
            cont.remove(cont.getElement(nElements - --index - 1));
        else
            cont.remove(cont.getElement(--index));
        nElements--;
    }

    public volatile Object next()
    {
        return next();
    }

    private GContainer cont;
    private int dir;
    private int index;
    private int nElements;
}
