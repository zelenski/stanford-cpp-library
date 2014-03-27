// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   CommandLineProgram.java

package acm.program;

import acm.util.ErrorException;
import acm.util.JTFTools;
import java.io.*;
import java.lang.reflect.Method;
import java.net.URL;
import java.util.HashMap;

class CommandLineProgramLoader extends ClassLoader
{

    public CommandLineProgramLoader(String s)
    {
        targetName = s;
        try
        {
            Class class1 = Class.forName("java.lang.ClassLoader");
            Method method = class1.getMethod("getSystemClassLoader", new Class[0]);
            realLoader = (ClassLoader)method.invoke(null, new Object[0]);
        }
        catch(Exception exception)
        {
            throw new ErrorException(exception);
        }
    }

    public Class loadClass(String s, boolean flag)
        throws ClassNotFoundException
    {
        if(s.equals(targetName))
        {
            InputStream inputstream = getResourceAsStream((new StringBuilder()).append(s).append(".class").toString());
            superclassOffset = findSuperclassOffset(inputstream);
            inputstream = getResourceAsStream((new StringBuilder()).append(s).append(".class").toString());
            byte abyte0[] = patchClassData(inputstream);
            return defineClass(s, abyte0, 0, abyte0.length);
        } else
        {
            return realLoader.loadClass(s);
        }
    }

    public InputStream getResourceAsStream(String s)
    {
        return realLoader.getResourceAsStream(s);
    }

    public URL getResource(String s)
    {
        return realLoader.getResource(s);
    }

    private byte[] patchClassData(InputStream inputstream)
    {
        ByteArrayOutputStream bytearrayoutputstream;
        bytearrayoutputstream = new ByteArrayOutputStream();
        JTFTools.copyBytes(inputstream, bytearrayoutputstream, 8L);
        int i = inputstream.read() << 8 | inputstream.read();
        bytearrayoutputstream.write(i >> 8);
        bytearrayoutputstream.write(i & 0xff);
        for(int j = 1; j < i; j++)
        {
            int l = inputstream.read();
            bytearrayoutputstream.write(l);
            if(JTFTools.testDebugOption("constants"))
                System.out.println((new StringBuilder()).append(j).append(": ").append(getConstantTypeName(l)).toString());
            switch(l)
            {
            case 2: // '\002'
            default:
                break;

            case 3: // '\003'
                JTFTools.copyBytes(inputstream, bytearrayoutputstream, 4L);
                break;

            case 4: // '\004'
                JTFTools.copyBytes(inputstream, bytearrayoutputstream, 4L);
                break;

            case 5: // '\005'
                JTFTools.copyBytes(inputstream, bytearrayoutputstream, 8L);
                j++;
                break;

            case 6: // '\006'
                JTFTools.copyBytes(inputstream, bytearrayoutputstream, 8L);
                j++;
                break;

            case 7: // '\007'
                JTFTools.copyBytes(inputstream, bytearrayoutputstream, 2L);
                break;

            case 8: // '\b'
                JTFTools.copyBytes(inputstream, bytearrayoutputstream, 2L);
                break;

            case 9: // '\t'
                JTFTools.copyBytes(inputstream, bytearrayoutputstream, 4L);
                break;

            case 10: // '\n'
                JTFTools.copyBytes(inputstream, bytearrayoutputstream, 4L);
                break;

            case 11: // '\013'
                JTFTools.copyBytes(inputstream, bytearrayoutputstream, 4L);
                break;

            case 12: // '\f'
                JTFTools.copyBytes(inputstream, bytearrayoutputstream, 4L);
                break;

            case 1: // '\001'
                if(j == superclassOffset)
                {
                    int i1 = inputstream.read() << 8 | inputstream.read();
                    inputstream.skip(i1);
                    String s = "acm/program/CommandLineProgram";
                    i1 = s.length();
                    bytearrayoutputstream.write(i1 >> 8);
                    bytearrayoutputstream.write(i1 & 0xff);
                    for(int k1 = 0; k1 < i1; k1++)
                        bytearrayoutputstream.write((byte)s.charAt(k1));

                } else
                {
                    int j1 = inputstream.read() << 8 | inputstream.read();
                    bytearrayoutputstream.write(j1 >> 8);
                    bytearrayoutputstream.write(j1 & 0xff);
                    JTFTools.copyBytes(inputstream, bytearrayoutputstream, j1);
                }
                break;
            }
        }

        do
        {
            int k = inputstream.read();
            if(k == -1)
                break;
            bytearrayoutputstream.write(k);
        } while(true);
        return bytearrayoutputstream.toByteArray();
        IOException ioexception;
        ioexception;
        throw new ErrorException(ioexception);
    }

    private int findSuperclassOffset(InputStream inputstream)
    {
        classTable = new HashMap();
        inputstream.skip(8L);
        int i = inputstream.read() << 8 | inputstream.read();
        i += 2;
        for(int j = 1; j < i - 2; j++)
        {
            int k = inputstream.read();
            switch(k)
            {
            case 3: // '\003'
                inputstream.skip(4L);
                break;

            case 4: // '\004'
                inputstream.skip(4L);
                break;

            case 5: // '\005'
                inputstream.skip(8L);
                j++;
                break;

            case 6: // '\006'
                inputstream.skip(8L);
                j++;
                break;

            case 8: // '\b'
                inputstream.skip(2L);
                break;

            case 9: // '\t'
                inputstream.skip(4L);
                break;

            case 10: // '\n'
                inputstream.skip(4L);
                break;

            case 11: // '\013'
                inputstream.skip(4L);
                break;

            case 12: // '\f'
                inputstream.skip(4L);
                break;

            case 7: // '\007'
                int l = inputstream.read() << 8 | inputstream.read();
                classTable.put(new Integer(j), new Integer(l));
                break;

            case 1: // '\001'
                int i1 = inputstream.read() << 8 | inputstream.read();
                inputstream.skip(i1);
                break;
            }
        }

        inputstream.skip(4L);
        return ((Integer)classTable.get(new Integer(inputstream.read() << 8 | inputstream.read()))).intValue();
        IOException ioexception;
        ioexception;
        throw new ErrorException(ioexception);
    }

    private static String getConstantTypeName(int i)
    {
        switch(i)
        {
        case 1: // '\001'
            return "Utf8";

        case 3: // '\003'
            return "Integer";

        case 4: // '\004'
            return "Float";

        case 5: // '\005'
            return "Long";

        case 6: // '\006'
            return "Double";

        case 7: // '\007'
            return "Class";

        case 8: // '\b'
            return "String";

        case 9: // '\t'
            return "Fieldref";

        case 10: // '\n'
            return "Methodref";

        case 11: // '\013'
            return "InterfaceMethodref";

        case 12: // '\f'
            return "NameAndType";

        case 2: // '\002'
        default:
            return (new StringBuilder()).append("Type[").append(i).append("]").toString();
        }
    }

    private static final int CONSTANT_Utf8 = 1;
    private static final int CONSTANT_Integer = 3;
    private static final int CONSTANT_Float = 4;
    private static final int CONSTANT_Long = 5;
    private static final int CONSTANT_Double = 6;
    private static final int CONSTANT_Class = 7;
    private static final int CONSTANT_String = 8;
    private static final int CONSTANT_Fieldref = 9;
    private static final int CONSTANT_Methodref = 10;
    private static final int CONSTANT_InterfaceMethodref = 11;
    private static final int CONSTANT_NameAndType = 12;
    private HashMap classTable;
    private ClassLoader realLoader;
    private String targetName;
    private int superclassOffset;
}
