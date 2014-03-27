// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPen.java

package acm.graphics;

import java.awt.Color;
import java.awt.Polygon;
import java.io.Serializable;

class PathState
    implements Serializable
{

    PathState()
    {
    }

    double cx;
    double cy;
    double sx;
    double sy;
    Polygon region;
    Color fillColor;
}
