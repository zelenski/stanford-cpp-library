// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GMath.java

package acm.graphics;


public class GMath
{

    private GMath()
    {
    }

    public static int round(double d)
    {
        return (int)Math.round(d);
    }

    public static double sinDegrees(double d)
    {
        return Math.sin(toRadians(d));
    }

    public static double cosDegrees(double d)
    {
        return Math.cos(toRadians(d));
    }

    public static double tanDegrees(double d)
    {
        return sinDegrees(d) / cosDegrees(d);
    }

    public static double toDegrees(double d)
    {
        return (d * 180D) / 3.1415926535897931D;
    }

    public static double toRadians(double d)
    {
        return (d * 3.1415926535897931D) / 180D;
    }

    public static double distance(double d, double d1)
    {
        return Math.sqrt(d * d + d1 * d1);
    }

    public static double distance(double d, double d1, double d2, double d3)
    {
        return distance(d2 - d, d3 - d1);
    }

    public static double angle(double d, double d1)
    {
        if(d == 0.0D && d1 == 0.0D)
            return 0.0D;
        else
            return toDegrees(Math.atan2(-d1, d));
    }

    public static double angle(double d, double d1, double d2, double d3)
    {
        return angle(d2 - d, d3 - d1);
    }
}
