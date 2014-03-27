// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IODialog.java

package acm.io;

import java.awt.*;
import java.awt.event.ActionEvent;

// Referenced classes of package acm.io:
//            AWTDialog

class AWTBooleanInputDialog extends AWTDialog
{

    public AWTBooleanInputDialog(Frame frame, String s, Image image, String s1, String s2, boolean flag)
    {
        super(frame, "Input", image, flag);
        setMessage(s);
        trueButton.setLabel(s1);
        falseButton.setLabel(s2);
    }

    public Boolean getInput()
    {
        return input;
    }

    public void initButtonPanel(Panel panel, boolean flag)
    {
        trueButton = new Button("True");
        trueButton.addActionListener(this);
        panel.add(trueButton);
        falseButton = new Button("False");
        falseButton.addActionListener(this);
        panel.add(falseButton);
        if(flag)
        {
            cancelButton = new Button("Cancel");
            cancelButton.addActionListener(this);
            panel.add(cancelButton);
        }
    }

    public void initDataPanel(Panel panel)
    {
    }

    public void actionPerformed(ActionEvent actionevent)
    {
        Component component = (Component)actionevent.getSource();
        if(component == trueButton)
        {
            input = Boolean.TRUE;
            setVisible(false);
        } else
        if(component == falseButton)
        {
            input = Boolean.FALSE;
            setVisible(false);
        } else
        if(component == cancelButton)
        {
            input = null;
            setVisible(false);
        }
    }

    private Button trueButton;
    private Button falseButton;
    private Button cancelButton;
    private Boolean input;
}
