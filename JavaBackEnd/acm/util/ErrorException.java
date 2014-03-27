// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   ErrorException.java

package acm.util;


public class ErrorException extends RuntimeException
{

    public ErrorException(String s)
    {
        super(s);
    }

    public ErrorException(Exception exception)
    {
        super((new StringBuilder()).append(exception.getClass().getName()).append(": ").append(exception.getMessage()).toString());
    }
}
