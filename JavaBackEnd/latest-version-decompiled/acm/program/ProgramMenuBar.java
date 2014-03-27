// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   ProgramMenuBar.java

package acm.program;

import acm.util.ErrorException;
import acm.util.Platform;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;

// Referenced classes of package acm.program:
//            ProgramMenuBarListener, OldStyleCheckBoxMenuItem, OldStyleMenuItem, Program

public class ProgramMenuBar extends JMenuBar
    implements Iterable
{

    public ProgramMenuBar(Program program1)
    {
        program = program1;
        menuBarListener = new ProgramMenuBarListener(this);
        focusedListener = null;
        accelerators = new HashMap();
        focusedItems = new HashSet();
        macMenuBarFlag = true;
        addMenus();
    }

    public Program getProgram()
    {
        return program;
    }

    public JMenuItem createStandardItem(String s)
    {
        JMenuItem jmenuitem = null;
        if(s.equals("Quit"))
        {
            jmenuitem = createProgramItem(s);
            if(Platform.isMac())
                setAccelerator(jmenuitem, 81);
            else
                jmenuitem.setName("Exit");
        } else
        if(s.equals("Cut"))
        {
            jmenuitem = createFocusedItem(s, 88);
            if(!Platform.isMac())
                jmenuitem.setName("Cut (x)");
        } else
        if(s.equals("Copy"))
        {
            jmenuitem = createFocusedItem(s, 67);
            if(!Platform.isMac())
                jmenuitem.setName("Copy (c)");
        } else
        if(s.equals("Paste"))
        {
            jmenuitem = createFocusedItem(s, 86);
            if(!Platform.isMac())
                jmenuitem.setName("Paste (v)");
        } else
        if(s.equals("Select All"))
            jmenuitem = createFocusedItem(s, 65);
        else
        if(s.equals("Save"))
            jmenuitem = createFocusedItem(s, 83);
        else
        if(s.equals("Save As"))
            jmenuitem = createFocusedItem(s);
        else
        if(s.equals("Print"))
        {
            jmenuitem = createProgramItem(s, 80);
            jmenuitem.setName("Print...");
        } else
        if(s.equals("Print Console"))
            jmenuitem = createProgramItem(s);
        else
        if(s.equals("Script"))
        {
            jmenuitem = createProgramItem(s);
            jmenuitem.setName("Script...");
        } else
        if(s.equals("Export Applet"))
        {
            jmenuitem = createProgramItem(s);
            jmenuitem.setName("Export Applet...");
        } else
        if(s.equals("Submit Project"))
        {
            jmenuitem = createProgramItem(s);
            jmenuitem.setName("Submit Project...");
        } else
        {
            throw new ErrorException((new StringBuilder()).append("Illegal standard menu item: ").append(s).toString());
        }
        return jmenuitem;
    }

    public JMenuItem createProgramItem(String s)
    {
        JMenuItem jmenuitem = new JMenuItem(s);
        jmenuitem.setActionCommand(s);
        jmenuitem.addActionListener(menuBarListener);
        return jmenuitem;
    }

    public JMenuItem createProgramItem(String s, int i)
    {
        JMenuItem jmenuitem = createProgramItem(s);
        setAccelerator(jmenuitem, i);
        return jmenuitem;
    }

    public JMenuItem createFocusedItem(String s)
    {
        JMenuItem jmenuitem = createProgramItem(s);
        focusedItems.add(jmenuitem);
        return jmenuitem;
    }

    public JMenuItem createFocusedItem(String s, int i)
    {
        JMenuItem jmenuitem = createFocusedItem(s);
        setAccelerator(jmenuitem, i);
        return jmenuitem;
    }

    public boolean isFocusedItem(JMenuItem jmenuitem)
    {
        return focusedItems.contains(jmenuitem);
    }

    public void setAccelerator(JMenuItem jmenuitem, int i)
    {
        int j = Platform.isMac() ? 4 : 2;
        if(i > 0x10000)
        {
            i -= 0x20000;
            j |= 1;
        }
        KeyStroke keystroke = KeyStroke.getKeyStroke((char)i, j);
        accelerators.put(keystroke, jmenuitem);
        if(Platform.isMac())
            jmenuitem.setAccelerator(keystroke);
        else
            jmenuitem.setMnemonic(i);
    }

    public void setEnabled(String s, boolean flag)
    {
        int i = getMenuCount();
        for(int j = 0; j < i; j++)
            setEnabled(getMenu(j), s, flag);

    }

    public void install(Component component)
    {
        java.awt.Container container = program.getContentPane();
        while(component != null && !(component instanceof JFrame)) 
        {
            component = component.getParent();
            if(component == container && program.isAppletMode())
            {
                if(!Platform.isMac() || !macMenuBarFlag)
                    program.setJMenuBar(this);
                return;
            }
        }
        if(component == null)
            return;
        JFrame jframe = (JFrame)component;
        if(Platform.isMac() && macMenuBarFlag)
        {
            if(oldStyleMenuBar == null)
                oldStyleMenuBar = createOldStyleMenuBar();
            jframe.setMenuBar(oldStyleMenuBar);
        } else
        {
            jframe.setJMenuBar(this);
            jframe.validate();
        }
    }

    public void setMacMenuBarFlag(boolean flag)
    {
        macMenuBarFlag = flag;
    }

    public boolean getMacMenuBarFlag()
    {
        return macMenuBarFlag;
    }

    public void fireActionListeners(ActionEvent actionevent)
    {
        if(focusedListener != null && focusedItems.contains(actionevent.getSource()))
            focusedListener.actionPerformed(actionevent);
        else
            program.menuAction(actionevent);
    }

    public boolean fireAccelerator(KeyEvent keyevent)
    {
        KeyStroke keystroke = KeyStroke.getKeyStrokeForEvent(keyevent);
        JMenuItem jmenuitem = (JMenuItem)accelerators.get(keystroke);
        if(jmenuitem != null)
        {
            jmenuitem.doClick(0);
            return true;
        } else
        {
            return false;
        }
    }

    public void setFocusedListener(ActionListener actionlistener)
    {
        focusedListener = actionlistener;
    }

    public Iterator iterator()
    {
        ArrayList arraylist = new ArrayList();
        for(int i = 0; i < getMenuCount(); i++)
            addItemToList(arraylist, getMenu(i));

        return arraylist.iterator();
    }

    protected void addMenus()
    {
        addFileMenu();
        addEditMenu();
    }

    protected void addFileMenu()
    {
        JMenu jmenu = new JMenu("File");
        jmenu.setMnemonic('F');
        addFileMenuItems(jmenu);
        add(jmenu);
    }

    protected void addEditMenu()
    {
        JMenu jmenu = new JMenu("Edit");
        jmenu.setMnemonic('E');
        addEditMenuItems(jmenu);
        add(jmenu);
    }

    protected void addFileMenuItems(JMenu jmenu)
    {
        jmenu.add(createStandardItem("Save"));
        jmenu.add(createStandardItem("Save As"));
        jmenu.addSeparator();
        jmenu.add(createStandardItem("Print"));
        jmenu.add(createStandardItem("Print Console"));
        jmenu.add(createStandardItem("Script"));
        jmenu.addSeparator();
        jmenu.add(createStandardItem("Export Applet"));
        jmenu.add(createStandardItem("Submit Project"));
        jmenu.addSeparator();
        jmenu.add(createStandardItem("Quit"));
    }

    protected void addEditMenuItems(JMenu jmenu)
    {
        jmenu.add(createStandardItem("Cut"));
        jmenu.add(createStandardItem("Copy"));
        jmenu.add(createStandardItem("Paste"));
        jmenu.add(createStandardItem("Select All"));
    }

    private void addItemToList(ArrayList arraylist, JMenuItem jmenuitem)
    {
        if(jmenuitem == null)
            return;
        if(jmenuitem instanceof JMenu)
        {
            JMenu jmenu = (JMenu)jmenuitem;
            for(int i = 0; i < jmenu.getItemCount(); i++)
                addItemToList(arraylist, jmenu.getItem(i));

        } else
        {
            arraylist.add(jmenuitem);
        }
    }

    private MenuBar createOldStyleMenuBar()
    {
        MenuBar menubar = new MenuBar();
        int i = getMenuCount();
        for(int j = 0; j < i; j++)
            menubar.add(createOldStyleMenu(getMenu(j)));

        return menubar;
    }

    private Menu createOldStyleMenu(JMenu jmenu)
    {
        Menu menu = new Menu(jmenu.getText());
        int i = jmenu.getItemCount();
        for(int j = 0; j < i; j++)
            menu.add(createOldStyleMenuItem(jmenu.getItem(j)));

        return menu;
    }

    private MenuItem createOldStyleMenuItem(Object obj)
    {
        if(obj == null)
            return new MenuItem("-");
        if(obj instanceof JMenu)
            return createOldStyleMenu((JMenu)obj);
        if(obj instanceof JCheckBoxMenuItem)
            return new OldStyleCheckBoxMenuItem((JCheckBoxMenuItem)obj);
        if(obj instanceof JMenuItem)
            return new OldStyleMenuItem((JMenuItem)obj);
        else
            throw new ErrorException("Unsupported menu item type");
    }

    private void setEnabled(JMenu jmenu, String s, boolean flag)
    {
        JMenu jmenu1 = jmenu;
        int i = jmenu1.getItemCount();
        for(int j = 0; j < i; j++)
        {
            JMenuItem jmenuitem = jmenu1.getItem(j);
            if(jmenuitem != null)
                setEnabled(jmenuitem, s, flag);
        }

    }

    private void setEnabled(JMenuItem jmenuitem, String s, boolean flag)
    {
        if(s.equals(jmenuitem.getActionCommand()))
            jmenuitem.setEnabled(flag);
    }

    public static final int SHIFT = 0x20000;
    private Program program;
    private ActionListener menuBarListener;
    private ActionListener focusedListener;
    private HashMap accelerators;
    private HashSet focusedItems;
    private MenuBar oldStyleMenuBar;
    private boolean macMenuBarFlag;
}
