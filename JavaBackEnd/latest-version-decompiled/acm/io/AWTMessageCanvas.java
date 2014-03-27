// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IODialog.java

package acm.io;

import java.awt.*;
import java.util.StringTokenizer;

class AWTMessageCanvas extends Canvas
{

    public AWTMessageCanvas()
    {
        setFont(MESSAGE_FONT);
    }

    public void setMessage(String s)
    {
        message = s;
    }

    public void paint(Graphics g)
    {
        FontMetrics fontmetrics = g.getFontMetrics();
        int i = 8;
        int j = 8 + fontmetrics.getAscent();
        int k = getSize().width - 8;
        StringTokenizer stringtokenizer = new StringTokenizer(message, " ", true);
        do
        {
            if(!stringtokenizer.hasMoreTokens())
                break;
            String s = stringtokenizer.nextToken();
            int l = fontmetrics.stringWidth(s);
            if(i + l > k)
            {
                i = 8;
                j += fontmetrics.getHeight();
                if(s.equals(" "))
                    continue;
            }
            g.drawString(s, i, j);
            i += l;
        } while(true);
    }

    public static final int MARGIN = 8;
    public static final Font MESSAGE_FONT = new Font("Dialog", 0, 12);
    private String message;

}
