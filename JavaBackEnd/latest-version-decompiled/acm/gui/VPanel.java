// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   VPanel.java

package acm.gui;


// Referenced classes of package acm.gui:
//            TablePanel, TableLayout

public class VPanel extends TablePanel
{

    public VPanel()
    {
        this(0, 0);
    }

    public VPanel(int i, int j)
    {
        TableLayout tablelayout = new TableLayout(0, 1, i, j);
        tablelayout.setHorizontalAlignment(1);
        tablelayout.setVerticalAlignment(1);
        setLayout(tablelayout);
    }

    static final long serialVersionUID = 1L;
}
