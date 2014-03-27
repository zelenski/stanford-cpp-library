// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   TableConstraints.java

package acm.gui;

import acm.util.ErrorException;
import acm.util.OptionTable;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.util.Map;

public class TableConstraints extends GridBagConstraints
{

    public TableConstraints()
    {
        this("");
    }

    public TableConstraints(String s)
    {
        this(((Map) ((new OptionTable(s.toLowerCase(), LEGAL_KEYS)).getMap())));
    }

    public TableConstraints(Map map)
    {
        OptionTable optiontable = new OptionTable(map);
        gridx = parseXYConstraint(optiontable.getOption("gridx"));
        gridy = parseXYConstraint(optiontable.getOption("gridy"));
        String s = optiontable.getOption("gridwidth");
        if(s == null)
            s = optiontable.getOption("rowspan");
        String s1 = optiontable.getOption("gridheight");
        if(s1 == null)
            s1 = optiontable.getOption("colspan");
        gridwidth = parseSpanConstraint(s);
        gridheight = parseSpanConstraint(s1);
        fill = parseFillConstraint(optiontable.getOption("fill"));
        anchor = parseAnchorConstraint(optiontable.getOption("anchor"));
        ipadx = optiontable.getIntOption("ipadx", 0);
        ipady = optiontable.getIntOption("ipady", 0);
        weightx = optiontable.getDoubleOption("weightx", 0.0D);
        weighty = optiontable.getDoubleOption("weighty", 0.0D);
        insets.left = optiontable.getIntOption("left", 0);
        insets.right = optiontable.getIntOption("right", 0);
        insets.top = optiontable.getIntOption("top", 0);
        insets.bottom = optiontable.getIntOption("bottom", 0);
        width = optiontable.getIntOption("width", -1);
        if(width == -1)
            width = 0;
        else
            width += insets.left + insets.right;
        height = optiontable.getIntOption("height", -1);
        if(height == -1)
            height = 0;
        else
            height += insets.top + insets.bottom;
        if(gridwidth != 1 && width != 0)
            throw new ErrorException("TableConstraints: Cannot specify both width and gridwidth");
        if(gridheight != 1 && height != 0)
            throw new ErrorException("TableConstraints: Cannot specify both height and gridheight");
        else
            return;
    }

    public TableConstraints(GridBagConstraints gridbagconstraints)
    {
        gridx = gridbagconstraints.gridx;
        gridy = gridbagconstraints.gridy;
        gridwidth = gridbagconstraints.gridwidth;
        gridheight = gridbagconstraints.gridheight;
        fill = gridbagconstraints.fill;
        anchor = gridbagconstraints.anchor;
        ipadx = gridbagconstraints.ipadx;
        ipady = gridbagconstraints.ipady;
        weightx = gridbagconstraints.weightx;
        weighty = gridbagconstraints.weighty;
        insets.left = gridbagconstraints.insets.left;
        insets.right = gridbagconstraints.insets.right;
        insets.top = gridbagconstraints.insets.top;
        insets.bottom = gridbagconstraints.insets.bottom;
        if(gridbagconstraints instanceof TableConstraints)
        {
            TableConstraints tableconstraints = (TableConstraints)gridbagconstraints;
            width = tableconstraints.width;
            height = tableconstraints.height;
        }
    }

    public int getAnchor()
    {
        return anchor;
    }

    public int getFill()
    {
        return fill;
    }

    public int getGridX()
    {
        return gridx;
    }

    public int getGridY()
    {
        return gridy;
    }

    public int getGridWidth()
    {
        return gridwidth;
    }

    public int getGridHeight()
    {
        return gridheight;
    }

    public int getIPadX()
    {
        return ipadx;
    }

    public int getIPadY()
    {
        return ipady;
    }

    public Insets getInsets()
    {
        return insets;
    }

    public double getWeightX()
    {
        return weightx;
    }

    public double getWeightY()
    {
        return weighty;
    }

    public int getWidth()
    {
        return width;
    }

    public int getHeight()
    {
        return height;
    }

    public String toString()
    {
        String s = getClass().getName();
        s = (new StringBuilder()).append(s).append("[gridx=").append(gridx).append(",gridy=").append(gridy).toString();
        switch(fill)
        {
        case 3: // '\003'
            s = (new StringBuilder()).append(s).append(",fill=VERTICAL").toString();
            break;

        case 2: // '\002'
            s = (new StringBuilder()).append(s).append(",fill=HORIZONTAL").toString();
            break;

        case 1: // '\001'
            s = (new StringBuilder()).append(s).append(",fill=BOTH").toString();
            break;
        }
        switch(anchor)
        {
        case 11: // '\013'
            s = (new StringBuilder()).append(s).append(",anchor=NORTH").toString();
            break;

        case 15: // '\017'
            s = (new StringBuilder()).append(s).append(",anchor=SOUTH").toString();
            break;

        case 13: // '\r'
            s = (new StringBuilder()).append(s).append(",anchor=EAST").toString();
            break;

        case 17: // '\021'
            s = (new StringBuilder()).append(s).append(",anchor=WEST").toString();
            break;

        case 12: // '\f'
            s = (new StringBuilder()).append(s).append(",anchor=NORTHEAST").toString();
            break;

        case 18: // '\022'
            s = (new StringBuilder()).append(s).append(",anchor=NORTHWEST").toString();
            break;

        case 14: // '\016'
            s = (new StringBuilder()).append(s).append(",anchor=SOUTHEAST").toString();
            break;

        case 16: // '\020'
            s = (new StringBuilder()).append(s).append(",anchor=SOUTHWEST").toString();
            break;

        case 19: // '\023'
            s = (new StringBuilder()).append(s).append(",anchor=PAGE_START").toString();
            break;

        case 20: // '\024'
            s = (new StringBuilder()).append(s).append(",anchor=PAGE_END").toString();
            break;

        case 21: // '\025'
            s = (new StringBuilder()).append(s).append(",anchor=LINE_START").toString();
            break;

        case 22: // '\026'
            s = (new StringBuilder()).append(s).append(",anchor=LINE_END").toString();
            break;

        case 23: // '\027'
            s = (new StringBuilder()).append(s).append(",anchor=FIRST_LINE_START").toString();
            break;

        case 24: // '\030'
            s = (new StringBuilder()).append(s).append(",anchor=FIRST_LINE_END").toString();
            break;

        case 25: // '\031'
            s = (new StringBuilder()).append(s).append(",anchor=LAST_LINE_START").toString();
            break;

        case 26: // '\032'
            s = (new StringBuilder()).append(s).append(",anchor=LAST_LINE_END").toString();
            break;
        }
        if(gridwidth != 1)
            s = (new StringBuilder()).append(s).append(",gridwidth=").append(gridwidth).toString();
        if(gridheight != 1)
            s = (new StringBuilder()).append(s).append(",gridheight=").append(gridheight).toString();
        if(ipadx != 0)
            s = (new StringBuilder()).append(s).append(",ipadx=").append(ipadx).toString();
        if(ipady != 0)
            s = (new StringBuilder()).append(s).append(",ipady=").append(ipady).toString();
        if(insets.left != 0)
            s = (new StringBuilder()).append(s).append(",left=").append(insets.left).toString();
        if(insets.right != 0)
            s = (new StringBuilder()).append(s).append(",right=").append(insets.right).toString();
        if(insets.top != 0)
            s = (new StringBuilder()).append(s).append(",top=").append(insets.top).toString();
        if(insets.bottom != 0)
            s = (new StringBuilder()).append(s).append(",bottom=").append(insets.bottom).toString();
        if(width != 0)
            s = (new StringBuilder()).append(s).append(",width=").append(width).toString();
        if(height != 0)
            s = (new StringBuilder()).append(s).append(",height=").append(height).toString();
        s = (new StringBuilder()).append(s).append("]").toString();
        return s;
    }

    private int parseXYConstraint(String s)
    {
        if(s == null)
            return -1;
        if(s.equals("relative"))
            return -1;
        return Integer.decode(s).intValue();
        NumberFormatException numberformatexception;
        numberformatexception;
        throw new ErrorException("TableConstraints: Illegal grid coordinate");
    }

    private int parseSpanConstraint(String s)
    {
        if(s == null)
            return 1;
        if(s.equals("relative"))
            return -1;
        if(s.equals("remainder"))
            return 0;
        return Integer.decode(s).intValue();
        NumberFormatException numberformatexception;
        numberformatexception;
        throw new ErrorException("TableConstraints: Illegal span constraint");
    }

    private int parseAnchorConstraint(String s)
    {
        if(s == null)
            return 10;
        if(s.equals("center"))
            return 10;
        if(s.equals("north"))
            return 11;
        if(s.equals("south"))
            return 15;
        if(s.equals("east"))
            return 13;
        if(s.equals("west"))
            return 17;
        if(s.equals("northeast") || s.equals("ne"))
            return 12;
        if(s.equals("northwest") || s.equals("nw"))
            return 18;
        if(s.equals("southeast") || s.equals("se"))
            return 14;
        if(s.equals("southwest") || s.equals("sw"))
            return 16;
        if(s.equals("page_start"))
            return 19;
        if(s.equals("page_end"))
            return 20;
        if(s.equals("line_start"))
            return 21;
        if(s.equals("line_end"))
            return 22;
        if(s.equals("first_line_start"))
            return 23;
        if(s.equals("first_line_end"))
            return 24;
        if(s.equals("last_line_start"))
            return 25;
        if(s.equals("last_line_end"))
            return 26;
        else
            throw new ErrorException("TableConstraints: Illegal anchor specification");
    }

    private int parseFillConstraint(String s)
    {
        if(s == null || s.equals("none"))
            return 0;
        if(s.equals("horizontal"))
            return 2;
        if(s.equals("vertical"))
            return 3;
        if(s.equals("both"))
            return 1;
        else
            throw new ErrorException("TableConstraints: Illegal fill specification");
    }

    public int width;
    public int height;
    private static final int MY_PAGE_START = 19;
    private static final int MY_PAGE_END = 20;
    private static final int MY_LINE_START = 21;
    private static final int MY_LINE_END = 22;
    private static final int MY_FIRST_LINE_START = 23;
    private static final int MY_FIRST_LINE_END = 24;
    private static final int MY_LAST_LINE_START = 25;
    private static final int MY_LAST_LINE_END = 26;
    protected static final String LEGAL_KEYS[] = {
        "anchor", "bottom", "colspan", "fill", "gridwidth", "gridheight", "gridx", "gridy", "height", "ipadx", 
        "ipady", "left", "right", "rowspan", "top", "weightx", "weighty", "width"
    };
    static final long serialVersionUID = 1L;

}
