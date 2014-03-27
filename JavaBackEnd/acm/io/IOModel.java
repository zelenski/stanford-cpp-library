// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOModel.java

package acm.io;


public interface IOModel
{

    public abstract void print(String s);

    public abstract void print(boolean flag);

    public abstract void print(char c);

    public abstract void print(double d);

    public abstract void print(float f);

    public abstract void print(int i);

    public abstract void print(long l);

    public abstract void print(Object obj);

    public abstract void println();

    public abstract void println(String s);

    public abstract void println(boolean flag);

    public abstract void println(char c);

    public abstract void println(double d);

    public abstract void println(float f);

    public abstract void println(int i);

    public abstract void println(long l);

    public abstract void println(Object obj);

    public abstract void showErrorMessage(String s);

    public abstract String readLine();

    public abstract String readLine(String s);

    public abstract int readInt();

    public abstract int readInt(int i, int j);

    public abstract int readInt(String s);

    public abstract int readInt(String s, int i, int j);

    public abstract double readDouble();

    public abstract double readDouble(double d, double d1);

    public abstract double readDouble(String s);

    public abstract double readDouble(String s, double d, double d1);

    public abstract boolean readBoolean();

    public abstract boolean readBoolean(String s);

    public abstract boolean readBoolean(String s, String s1, String s2);
}
