// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   RandomGenerator.java

package acm.util;

import java.awt.Color;
import java.util.Random;

public class RandomGenerator extends Random
{

    public RandomGenerator()
    {
    }

    public int nextInt(int i)
    {
        return nextInt(0, i - 1);
    }

    public boolean nextBoolean()
    {
        return nextBoolean(0.5D);
    }

    public int nextInt(int i, int j)
    {
        return i + (int)((double)((j - i) + 1) * nextDouble());
    }

    public double nextDouble(double d, double d1)
    {
        return d + (d1 - d) * nextDouble();
    }

    public boolean nextBoolean(double d)
    {
        return nextDouble() < d;
    }

    public Color nextColor()
    {
        return new Color(nextInt(256), nextInt(256), nextInt(256));
    }

    public static RandomGenerator getInstance()
    {
        if(standardInstance == null)
            standardInstance = new RandomGenerator();
        return standardInstance;
    }

    private static RandomGenerator standardInstance;
    static final long serialVersionUID = 1L;
}
