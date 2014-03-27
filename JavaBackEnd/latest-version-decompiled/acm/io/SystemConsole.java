// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOConsole.java

package acm.io;


// Referenced classes of package acm.io:
//            IOConsole, SystemConsoleModel, ConsoleModel

class SystemConsole extends IOConsole
{

    SystemConsole()
    {
    }

    protected ConsoleModel createConsoleModel()
    {
        return new SystemConsoleModel();
    }
}
