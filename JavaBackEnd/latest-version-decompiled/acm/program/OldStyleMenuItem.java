// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   ProgramMenuBar.java

package acm.program;

import java.awt.MenuItem;
import java.awt.MenuShortcut;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

class OldStyleMenuItem extends MenuItem
    implements ActionListener, ChangeListener
{

    public OldStyleMenuItem(JMenuItem jmenuitem)
    {
        super(jmenuitem.getText());
        twin = jmenuitem;
        addActionListener(this);
        twin.addChangeListener(this);
        setEnabled(twin.isEnabled());
        KeyStroke keystroke = twin.getAccelerator();
        if(keystroke != null)
            setShortcut(createShortcut(keystroke));
    }

    public void actionPerformed(ActionEvent actionevent)
    {
        twin.doClick(0);
    }

    public void stateChanged(ChangeEvent changeevent)
    {
        setEnabled(twin.isEnabled());
    }

    private MenuShortcut createShortcut(KeyStroke keystroke)
    {
        boolean flag = (keystroke.getModifiers() & 1) != 0;
        return new MenuShortcut(keystroke.getKeyCode(), flag);
    }

    private JMenuItem twin;
}
