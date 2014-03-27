// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   TablePanel.java

package acm.gui;

import javax.swing.JPanel;

// Referenced classes of package acm.gui:
//            TableLayout

public class TablePanel extends JPanel
{

    TablePanel()
    {
    }

    public TablePanel(int i, int j)
    {
        this(i, j, 0, 0);
    }

    public TablePanel(int i, int j, int k, int l)
    {
        setLayout(new TableLayout(i, j, k, l));
    }

    public void setHorizontalAlignment(int i)
    {
        ((TableLayout)getLayout()).setHorizontalAlignment(i);
    }

    public int getHorizontalAlignment()
    {
        return ((TableLayout)getLayout()).getHorizontalAlignment();
    }

    public void setVerticalAlignment(int i)
    {
        ((TableLayout)getLayout()).setVerticalAlignment(i);
    }

    public int getVerticalAlignment()
    {
        return ((TableLayout)getLayout()).getVerticalAlignment();
    }

    public void setDefaultFill(int i)
    {
        ((TableLayout)getLayout()).setDefaultFill(i);
    }

    public int getDefaultFill()
    {
        return ((TableLayout)getLayout()).getDefaultFill();
    }

    public void setHgap(int i)
    {
        ((TableLayout)getLayout()).setHgap(i);
    }

    public int getHgap()
    {
        return ((TableLayout)getLayout()).getHgap();
    }

    public void setVgap(int i)
    {
        ((TableLayout)getLayout()).setVgap(i);
    }

    public int getVgap()
    {
        return ((TableLayout)getLayout()).getVgap();
    }

    public static final int NONE = 0;
    public static final int HORIZONTAL = 2;
    public static final int VERTICAL = 3;
    public static final int BOTH = 1;
    public static final int CENTER = 10;
    public static final int LEFT = 11;
    public static final int RIGHT = 12;
    public static final int TOP = 13;
    public static final int BOTTOM = 14;
    public static final int FILL = 1;
    static final long serialVersionUID = 1L;
}
