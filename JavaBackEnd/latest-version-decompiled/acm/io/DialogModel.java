// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IODialog.java

package acm.io;


interface DialogModel
{

    public abstract void popupMessage(String s);

    public abstract void popupErrorMessage(String s);

    public abstract String popupLineInputDialog(String s, boolean flag);

    public abstract Boolean popupBooleanInputDialog(String s, String s1, String s2, boolean flag);
}
