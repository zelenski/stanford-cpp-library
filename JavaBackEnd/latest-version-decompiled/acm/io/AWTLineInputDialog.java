// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IODialog.java

package acm.io;

import java.awt.*;
import java.awt.event.ActionEvent;

// Referenced classes of package acm.io:
//            AWTDialog

class AWTLineInputDialog extends AWTDialog
{

    public AWTLineInputDialog(Frame frame, String s, Image image, boolean flag)
    {
        super(frame, "Input", image, flag);
        setMessage(s);
    }

    public String getInput()
    {
        return input;
    }

    public void setVisible(boolean flag)
    {
        super.setVisible(flag);
        if(flag)
            textLine.requestFocus();
    }

    public void initButtonPanel(Panel panel, boolean flag)
    {
        okButton = new Button("OK");
        okButton.addActionListener(this);
        panel.add(okButton);
        if(flag)
        {
            cancelButton = new Button("Cancel");
            cancelButton.addActionListener(this);
            panel.add(cancelButton);
        }
    }

    public void initDataPanel(Panel panel)
    {
        textLine = new TextField();
        textLine.addActionListener(this);
        panel.add(textLine, "South");
    }

    public void actionPerformed(ActionEvent actionevent)
    {
        Component component = (Component)actionevent.getSource();
        if(component == okButton || component == textLine)
        {
            input = textLine.getText();
            setVisible(false);
        } else
        if(component == cancelButton)
        {
            input = null;
            setVisible(false);
        }
    }

    private Button cancelButton;
    private Button okButton;
    private TextField textLine;
    private String input;
}
