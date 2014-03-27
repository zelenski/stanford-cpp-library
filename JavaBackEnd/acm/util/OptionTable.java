// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   OptionTable.java

package acm.util;

import java.io.*;
import java.util.*;

// Referenced classes of package acm.util:
//            ErrorException

public class OptionTable
{

    public OptionTable(String s)
    {
        this(s, null);
    }

    public OptionTable(String s, String as[])
    {
        optionTable = new HashMap();
        try
        {
            StreamTokenizer streamtokenizer = createTokenizer(s);
            for(int i = streamtokenizer.nextToken(); i != -1;)
            {
                if(i != -3)
                    throw new ErrorException((new StringBuilder()).append("Illegal option string: ").append(s).toString());
                String s1 = streamtokenizer.sval;
                if(as != null && !keyExists(s1, as))
                    throw new ErrorException((new StringBuilder()).append("Unrecognized option: ").append(s1).toString());
                i = streamtokenizer.nextToken();
                if(i == 61)
                {
                    i = streamtokenizer.nextToken();
                    if(i != -3 && i != 34 && i != 39)
                        throw new ErrorException((new StringBuilder()).append("Illegal option string: ").append(s).toString());
                    optionTable.put(s1, streamtokenizer.sval);
                    i = streamtokenizer.nextToken();
                } else
                {
                    optionTable.put(s1, "");
                }
            }

        }
        catch(IOException ioexception)
        {
            throw new ErrorException((new StringBuilder()).append("Illegal option string: ").append(s).toString());
        }
    }

    public OptionTable(Map map)
    {
        optionTable = new HashMap();
        String s;
        String s1;
        for(Iterator iterator = map.keySet().iterator(); iterator.hasNext(); optionTable.put(s, s1))
        {
            s = (String)iterator.next();
            s1 = (String)map.get(s);
        }

    }

    public boolean isSpecified(String s)
    {
        return optionTable.containsKey(s);
    }

    public String getOption(String s)
    {
        return getOption(s, null);
    }

    public String getOption(String s, String s1)
    {
        String s2 = (String)optionTable.get(s);
        return s2 != null && !s2.equals("") ? s2 : s1;
    }

    public int getIntOption(String s)
    {
        return getIntOption(s, 0);
    }

    public int getIntOption(String s, int i)
    {
        String s1 = getOption(s, null);
        if(s1 == null || s1.equals(""))
            return i;
        else
            return Integer.decode(s1).intValue();
    }

    public double getDoubleOption(String s)
    {
        return getDoubleOption(s, 0.0D);
    }

    public double getDoubleOption(String s, double d)
    {
        String s1 = getOption(s, null);
        if(s1 == null || s1.equals(""))
            return d;
        else
            return Double.valueOf(s1).doubleValue();
    }

    public HashMap getMap()
    {
        return optionTable;
    }

    private StreamTokenizer createTokenizer(String s)
    {
        StreamTokenizer streamtokenizer = new StreamTokenizer(new StringReader(s));
        streamtokenizer.resetSyntax();
        streamtokenizer.wordChars(33, 60);
        streamtokenizer.wordChars(62, 126);
        streamtokenizer.quoteChar(34);
        streamtokenizer.quoteChar(39);
        streamtokenizer.whitespaceChars(32, 32);
        streamtokenizer.whitespaceChars(9, 9);
        return streamtokenizer;
    }

    private boolean keyExists(String s, String as[])
    {
        for(int i = 0; i < as.length; i++)
            if(s.equals(as[i]))
                return true;

        return false;
    }

    private HashMap optionTable;
    static final long serialVersionUID = 1L;
}
