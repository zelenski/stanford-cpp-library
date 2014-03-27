// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Program.java

package acm.program;

import acm.io.IODialog;
import acm.util.ErrorException;
import java.awt.Button;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.reflect.Method;

// Referenced classes of package acm.program:
//            Program

class DefaultActionListener
    implements ActionListener
{

    public DefaultActionListener()
    {
    }

    public void actionPerformed(ActionEvent actionevent)
    {
        Component component = (Component)actionevent.getSource();
        Program program = findProgram(component);
        if(program != null && countActionListeners(component) > 1)
            try
            {
                Class aclass[] = {
                    Class.forName("java.awt.event.ActionListener")
                };
                Object aobj[] = {
                    this
                };
                Method method1 = component.getClass().getMethod("removeActionListener", aclass);
                method1.invoke(component, aobj);
                return;
            }
            catch(Exception exception)
            {
                throw new ErrorException(exception);
            }
        String s = "No ActionListener is attached";
        if(component instanceof Button)
            s = (new StringBuilder()).append(s).append(" to button ").append(((Button)component).getLabel()).toString();
        else
            try
            {
                Method method = component.getClass().getMethod("getText", new Class[0]);
                s = (new StringBuilder()).append(s).append(" to button ").append((String)method.invoke(component, new Object[0])).toString();
            }
            catch(Exception exception1)
            {
                throw new ErrorException(exception1);
            }
        if(program == null)
        {
            throw new ErrorException(s);
        } else
        {
            program.getDialog().showErrorMessage(s);
            return;
        }
    }

    protected static int countActionListeners(Component component)
    {
        ActionListener aactionlistener[];
        Method method = component.getClass().getMethod("getActionListeners", new Class[0]);
        aactionlistener = (ActionListener[])(ActionListener[])method.invoke(component, new Object[0]);
        return aactionlistener.length;
        Exception exception;
        exception;
        return -1;
    }

    private Program findProgram(Component component)
    {
        if(component instanceof Program)
            return (Program)component;
        if(component != null)
            return findProgram(((Component) (component.getParent())));
        else
            return null;
    }
}
