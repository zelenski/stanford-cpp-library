// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   JTFTools.java

package acm.util;

import java.security.Permission;

class SecurityManagerThatFails extends SecurityManager
{

    SecurityManagerThatFails()
    {
    }

    public void checkRead(String s)
    {
        throw new SecurityException("always fail");
    }

    public void checkPermission(Permission permission)
    {
    }
}
