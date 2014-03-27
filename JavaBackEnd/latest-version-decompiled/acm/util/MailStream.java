// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   JTFTools.java

package acm.util;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.*;

// Referenced classes of package acm.util:
//            NullOutputStream, ErrorException

class MailStream extends PrintStream
{

    public MailStream(String s, String s1, String s2)
    {
        super(new NullOutputStream());
        try
        {
            sender = s1;
            recipient = s2;
            socket = new Socket(s, 25);
            out = new BufferedOutputStream(socket.getOutputStream());
            in = new BufferedInputStream(socket.getInputStream());
            verify("220");
            println((new StringBuilder()).append("HELO ").append(InetAddress.getLocalHost().getHostName()).toString());
            verify("250");
            println((new StringBuilder()).append("MAIL FROM: ").append(sender).toString());
            verify("250");
            for(StringTokenizer stringtokenizer = new StringTokenizer(s2, " ,"); stringtokenizer.hasMoreTokens(); verify("250"))
                println((new StringBuilder()).append("RCPT TO: ").append(stringtokenizer.nextToken()).toString());

            println("DATA");
            verify("354");
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public void print(boolean flag)
    {
        print((new StringBuilder()).append("").append(flag).toString());
    }

    public void print(char c)
    {
        print((new StringBuilder()).append("").append(c).toString());
    }

    public void print(int i)
    {
        print((new StringBuilder()).append("").append(i).toString());
    }

    public void print(long l)
    {
        print((new StringBuilder()).append("").append(l).toString());
    }

    public void print(float f)
    {
        print((new StringBuilder()).append("").append(f).toString());
    }

    public void print(double d)
    {
        print((new StringBuilder()).append("").append(d).toString());
    }

    public void print(char ac[])
    {
        print(new String(ac));
    }

    public void print(String s)
    {
        write(s);
    }

    public void print(Object obj)
    {
        print((new StringBuilder()).append("").append(obj).toString());
    }

    public void println()
    {
        try
        {
            out.write(10);
            out.flush();
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public void println(boolean flag)
    {
        print(flag);
        println();
    }

    public void println(char c)
    {
        print(c);
        println();
    }

    public void println(int i)
    {
        print(i);
        println();
    }

    public void println(long l)
    {
        print(l);
        println();
    }

    public void println(float f)
    {
        print(f);
        println();
    }

    public void println(double d)
    {
        print(d);
        println();
    }

    public void println(char ac[])
    {
        print(ac);
        println();
    }

    public void println(String s)
    {
        print(s);
        println();
    }

    public void println(Object obj)
    {
        print(obj);
        println();
    }

    public void write(int i)
    {
        try
        {
            if(i > 255)
                throw new ErrorException("Illegal character in mail stream");
            out.write(i);
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public void write(char ac[], int i, int j)
    {
        try
        {
            for(int k = 0; k < j; k++)
            {
                char c = ac[i + k];
                if(c > '\377')
                    throw new ErrorException("Illegal character in mail stream");
                out.write(c);
            }

        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public void write(char ac[])
    {
        write(ac, 0, ac.length);
    }

    public void write(String s)
    {
        try
        {
            for(int i = 0; i < s.length(); i++)
            {
                char c = s.charAt(i);
                if(c > '\377')
                    throw new ErrorException("Illegal character in mail stream");
                out.write(c);
            }

        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public boolean checkError()
    {
        return false;
    }

    public void flush()
    {
        try
        {
            out.flush();
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public void close()
    {
        try
        {
            println(".");
            verify("250");
            out.close();
            socket.close();
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public void cancel()
    {
        try
        {
            socket.close();
            out.close();
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    public void sendStandardHeaders(String s, String s1)
    {
        println((new StringBuilder()).append("To: ").append(recipient).toString());
        if(s != null && s.length() != 0)
        {
            boolean flag = false;
            int i = 0;
            do
            {
                if(i >= s.length())
                    break;
                char c = s.charAt(i);
                if(c != ' ' && c != '-' && !Character.isJavaIdentifierPart(c))
                {
                    flag = true;
                    break;
                }
                i++;
            } while(true);
            if(flag)
                sender = (new StringBuilder()).append('"').append(s).append('"').append(" <").append(sender).append(">").toString();
            else
                sender = (new StringBuilder()).append(s).append(" <").append(sender).append(">").toString();
        }
        println((new StringBuilder()).append("From: ").append(sender).toString());
        if(s1 != null && s1.length() != 0)
            println((new StringBuilder()).append("Subject: ").append(s1).toString());
        println((new StringBuilder()).append("Date: ").append(RFC2822_DATE_FORMAT.format(new Date())).toString());
    }

    private void verify(String s)
    {
        try
        {
            String s1 = "";
            do
            {
                int i = in.read();
                if(i == -1 || i == 10)
                    break;
                s1 = (new StringBuilder()).append(s1).append((char)i).toString();
            } while(true);
            if(s != null && !s1.startsWith(s))
                throw new ErrorException((new StringBuilder()).append("Unexpected SMTP response: ").append(s1).toString());
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
    }

    private static final int SMTP_PORT = 25;
    private static SimpleDateFormat RFC2822_DATE_FORMAT;
    private Socket socket;
    private InputStream in;
    private OutputStream out;
    private String sender;
    private String recipient;

    static 
    {
        RFC2822_DATE_FORMAT = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss z", Locale.US);
    }
}
