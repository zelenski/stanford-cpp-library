// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IntField.java

package acm.gui;

import acm.io.IODialog;
import acm.util.ErrorException;
import java.awt.*;
import java.text.*;
import java.util.Locale;
import javax.swing.JTextField;

public class IntField extends JTextField
{

    public IntField()
    {
        this("", 0x80000000, 0x7fffffff);
    }

    public IntField(int i)
    {
        this((new StringBuilder()).append("").append(i).toString(), 0x80000000, 0x7fffffff);
    }

    public IntField(int i, int j)
    {
        this("", i, j);
    }

    public IntField(int i, int j, int k)
    {
        this((new StringBuilder()).append("").append(i).toString(), j, k);
    }

    private IntField(String s, int i, int j)
    {
        parser = NumberFormat.getNumberInstance(Locale.US);
        setBackground(Color.WHITE);
        setHorizontalAlignment(4);
        minValue = i;
        maxValue = j;
        setText(s);
        exceptionOnError = false;
    }

    public int getValue()
    {
        String s;
        s = getText().trim();
        if(s.length() == 0)
            if(minValue <= 0 && maxValue >= 0)
                return 0;
            else
                return minValue;
        Object obj = null;
        boolean flag = false;
        int i = parser.parse(s).intValue();
        if(i < minValue || i > maxValue)
        {
            String s1;
            try
            {
                s1 = "Value is outside the specified range";
            }
            catch(ParseException parseexception)
            {
                s1 = "Illegal integer format";
            }
            if(exceptionOnError)
                throw new ErrorException(s1);
            String s2 = "Enter an integer";
            if(minValue != 0x80000000)
            {
                if(maxValue != 0x7fffffff)
                    s2 = (new StringBuilder()).append(s2).append(" between ").append(minValue).append(" and ").append(maxValue).toString();
                else
                    s2 = (new StringBuilder()).append(s2).append(" greater than ").append(minValue).toString();
            } else
            if(maxValue != 0x7fffffff)
                s2 = (new StringBuilder()).append(s2).append(" less than ").append(maxValue).toString();
            if(dialog == null)
                dialog = new IODialog(this);
            i = dialog.readInt(s2, minValue, maxValue);
        }
        setValue(i);
        return i;
    }

    public void setValue(int i)
    {
        setText(formatter != null ? formatter.format(i) : defaultFormat(i));
    }

    public String getFormat()
    {
        return formatString;
    }

    public void setFormat(String s)
    {
        String s1 = getText().trim();
        int i = 0;
        if(s1.length() != 0)
            try
            {
                i = parser.parse(s1).intValue();
            }
            catch(ParseException parseexception)
            {
                throw new ErrorException(parseexception);
            }
        formatString = s;
        if(s == null)
        {
            formatter = null;
            parser = NumberFormat.getNumberInstance(Locale.US);
        } else
        {
            formatter = s.length() != 0 ? new DecimalFormat(s) : new DecimalFormat();
            parser = formatter;
        }
        if(s1.length() != 0)
            setValue(i);
    }

    public void setExceptionOnError(boolean flag)
    {
        exceptionOnError = flag;
    }

    public boolean getExceptionOnError()
    {
        return exceptionOnError;
    }

    public Dimension getPreferredSize()
    {
        Dimension dimension = super.getPreferredSize();
        return new Dimension(Math.max(60, dimension.width), Math.max(22, dimension.height));
    }

    private String defaultFormat(int i)
    {
        String s = (new StringBuilder()).append("").append(i).toString();
        int j = getSize().width - 4;
        FontMetrics fontmetrics = getFontMetrics(getFont());
        if(fontmetrics.stringWidth(s) > j)
            for(s = ""; fontmetrics.stringWidth((new StringBuilder()).append(s).append("#").toString()) <= j; s = (new StringBuilder()).append(s).append("#").toString());
        return s;
    }

    private static final int MINIMUM_WIDTH = 60;
    private static final int MINIMUM_HEIGHT = 22;
    private static final int PIXEL_MARGIN = 2;
    private boolean exceptionOnError;
    private int minValue;
    private int maxValue;
    private String formatString;
    private DecimalFormat formatter;
    private NumberFormat parser;
    private IODialog dialog;
    static final long serialVersionUID = 1L;
}
