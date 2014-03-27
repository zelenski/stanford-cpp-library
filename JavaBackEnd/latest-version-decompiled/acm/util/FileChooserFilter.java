// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   FileChooserFilter.java

package acm.util;

import java.io.File;
import java.util.StringTokenizer;
import javax.swing.filechooser.FileFilter;

// Referenced classes of package acm.util:
//            JTFTools

public class FileChooserFilter extends FileFilter
{

    public FileChooserFilter(String s)
    {
        this(s, (new StringBuilder()).append(s.isEmpty() ? "All" : s).append(" files").toString());
    }

    public FileChooserFilter(String s, String s1)
    {
        filenameDescription = s1;
        int i = Math.max(s.lastIndexOf("/"), s.lastIndexOf('\\'));
        filenamePattern = s.substring(i + 1);
        dir = i != -1 ? s.substring(0, i) : "";
        if(dir.isEmpty())
            dir = System.getProperty("user.dir");
        else
        if(!dir.startsWith("/"))
            dir = (new StringBuilder()).append(System.getProperty("user.dir")).append("/").append(dir).toString();
    }

    public String getDirectory()
    {
        return dir;
    }

    public String getPattern()
    {
        return filenamePattern;
    }

    public String getDescription()
    {
        return filenameDescription;
    }

    public boolean accept(File file)
    {
        if(file.isDirectory() || filenamePattern.isEmpty())
            return true;
        for(StringTokenizer stringtokenizer = new StringTokenizer(filenamePattern, ";"); stringtokenizer.hasMoreTokens();)
        {
            String s = stringtokenizer.nextToken();
            if(s.length() > 0 && JTFTools.matchFilenamePattern(file.getName(), s))
                return true;
        }

        return false;
    }

    private String filenamePattern;
    private String filenameDescription;
    private String dir;
}
