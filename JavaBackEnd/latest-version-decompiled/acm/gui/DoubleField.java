// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   DoubleField.java

package acm.gui;

import acm.io.IODialog;
import acm.util.ErrorException;
import java.awt.*;
import java.text.*;
import java.util.Locale;
import javax.swing.JTextField;

public class DoubleField extends JTextField
{

    public DoubleField()
    {
        this("", (-1.0D / 0.0D), (1.0D / 0.0D));
    }

    public DoubleField(double d)
    {
        this((new StringBuilder()).append("").append(d).toString(), (-1.0D / 0.0D), (1.0D / 0.0D));
    }

    public DoubleField(double d, double d1)
    {
        this("", d, d1);
    }

    public DoubleField(double d, double d1, double d2)
    {
        this((new StringBuilder()).append("").append(d).toString(), d1, d2);
    }

    private DoubleField(String s, double d, double d1)
    {
        parser = NumberFormat.getNumberInstance(Locale.US);
        setBackground(Color.WHITE);
        setHorizontalAlignment(4);
        minValue = d;
        maxValue = d1;
        setText(s);
        exceptionOnError = false;
    }

    public double getValue()
    {
        String s;
        s = getText().trim();
        if(s.length() == 0)
            if(minValue <= 0.0D && maxValue >= 0.0D)
                return 0.0D;
            else
                return minValue;
        Object obj = null;
        double d = 0.0D;
        double d1 = parser.parse(s).doubleValue();
        if(d1 < minValue || d1 > maxValue)
        {
            String s1;
            try
            {
                s1 = "Value is outside the specified range";
            }
            catch(ParseException parseexception)
            {
                s1 = "Illegal numeric format";
            }
            if(exceptionOnError)
                throw new ErrorException(s1);
            String s2 = "Enter a number";
            if(minValue != (-1.0D / 0.0D))
            {
                if(maxValue != (1.0D / 0.0D))
                    s2 = (new StringBuilder()).append(s2).append(" between ").append(minValue).append(" and ").append(maxValue).toString();
                else
                    s2 = (new StringBuilder()).append(s2).append(" greater than ").append(minValue).toString();
            } else
            if(maxValue != (1.0D / 0.0D))
                s2 = (new StringBuilder()).append(s2).append(" less than ").append(maxValue).toString();
            if(dialog == null)
                dialog = new IODialog(this);
            d1 = dialog.readDouble(s2, minValue, maxValue);
        }
        setValue(d1);
        return d1;
    }

    public void setValue(double d)
    {
        setText(formatter != null ? formatter.format(d) : defaultFormat(d));
    }

    public String getFormat()
    {
        return formatString;
    }

    public void setFormat(String s)
    {
        String s1 = getText().trim();
        double d = 0.0D;
        if(s1.length() != 0)
            try
            {
                d = parser.parse(s1).doubleValue();
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
            setValue(d);
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

    private String defaultFormat(double d)
    {
        String s = (new StringBuilder()).append("").append(d).toString();
        int i = getSize().width - 4;
        FontMetrics fontmetrics = getFontMetrics(getFont());
        if(fontmetrics.stringWidth(s) <= i)
            return s;
        int j = s.indexOf("E");
        String s1 = "";
        if(j != -1)
        {
            s1 = s.substring(j);
            s = s.substring(0, j);
            try
            {
                d = parser.parse(s).doubleValue();
            }
            catch(ParseException parseexception)
            {
                throw new ErrorException(parseexception);
            }
        }
        NumberFormat numberformat = NumberFormat.getNumberInstance(Locale.US);
        numberformat.setGroupingUsed(false);
        String s2 = s.substring(0, s.indexOf('.') + 1);
        int k = i - fontmetrics.stringWidth((new StringBuilder()).append(s2).append(s1).toString());
        if(k > 0)
        {
            int l = k / fontmetrics.stringWidth("0");
            numberformat.setMaximumFractionDigits(l);
            return (new StringBuilder()).append(numberformat.format(d)).append(s1).toString();
        }
        for(s = ""; fontmetrics.stringWidth((new StringBuilder()).append(s).append("#").toString()) <= i; s = (new StringBuilder()).append(s).append("#").toString());
        return s;
    }

    private static final int MINIMUM_WIDTH = 60;
    private static final int MINIMUM_HEIGHT = 22;
    private static final int PIXEL_MARGIN = 2;
    private boolean exceptionOnError;
    private double minValue;
    private double maxValue;
    private String formatString;
    private DecimalFormat formatter;
    private NumberFormat parser;
    private IODialog dialog;
    static final long serialVersionUID = 1L;
}
