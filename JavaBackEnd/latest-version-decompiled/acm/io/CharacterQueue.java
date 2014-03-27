// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOConsole.java

package acm.io;


class CharacterQueue
{

    public CharacterQueue()
    {
        buffer = "";
    }

    public synchronized void enqueue(char c)
    {
        new StringBuilder();
        this;
        JVM INSTR dup_x1 ;
        buffer;
        append();
        c;
        append();
        toString();
        buffer;
        notifyAll();
        return;
    }

    public synchronized void enqueue(String s)
    {
        new StringBuilder();
        this;
        JVM INSTR dup_x1 ;
        buffer;
        append();
        s;
        append();
        toString();
        buffer;
        notifyAll();
        return;
    }

    public synchronized char dequeue()
    {
        while(buffer.length() == 0) 
            try
            {
                isWaiting = true;
                wait();
                isWaiting = false;
            }
            catch(InterruptedException interruptedexception) { }
        char c = buffer.charAt(0);
        buffer = buffer.substring(1);
        return c;
    }

    public synchronized void clear()
    {
        buffer = "";
        notifyAll();
    }

    public boolean isWaiting()
    {
        return isWaiting;
    }

    private String buffer;
    private boolean isWaiting;
}
