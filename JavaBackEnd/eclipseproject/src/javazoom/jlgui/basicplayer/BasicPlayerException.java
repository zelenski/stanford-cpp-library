/*
 * BasicPlayerException.
 * 
 * JavaZOOM : jlgui@javazoom.net
 *            http://www.javazoom.net
 *
 *-----------------------------------------------------------------------
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as published
 *   by the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *----------------------------------------------------------------------
 */
package javazoom.jlgui.basicplayer;

import java.io.PrintStream;
import java.io.PrintWriter;

/**
 * This class implements custom exception for basicplayer.
 */
public class BasicPlayerException extends RuntimeException {
    public static final String GAINCONTROLNOTSUPPORTED = "Gain control not supported";
    public static final String PANCONTROLNOTSUPPORTED = "Pan control not supported";
    public static final String WAITERROR = "Wait error";
    public static final String CANNOTINITLINE = "Cannot init line";
    public static final String SKIPNOTSUPPORTED = "Skip not supported";
    private Throwable cause = null;

    public BasicPlayerException()
    {
        super();
    }

    public BasicPlayerException(String msg)
    {
        super(msg);
    }

    public BasicPlayerException(Throwable cause)
    {
        super();
        this.cause = cause;
    }

    public BasicPlayerException(String msg, Throwable cause)
    {
        super(msg);
        this.cause = cause;
    }

    public Throwable getCause()
    {
        return cause;
    }

    /**
     * Returns the detail message string of this throwable. If it was
     * created with a null message, returns the following:
     * (cause==null ? null : cause.toString()).
     */
    public String getMessage()
    {
        if (super.getMessage() != null)
        {
            return super.getMessage();
        }
        else if (cause != null)
        {
            return cause.toString();
        }
        else
        {
            return null;
        }
    }

    public void printStackTrace()
    {
        printStackTrace(System.err);
    }

    public void printStackTrace(PrintStream out)
    {
        synchronized (out)
        {
            PrintWriter pw = new PrintWriter(out, false);
            printStackTrace(pw);
            pw.flush();
        }
    }

    public void printStackTrace(PrintWriter out)
    {
        if (cause != null) cause.printStackTrace(out);
    }
}
