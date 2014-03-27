// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IODialog.java

package acm.io;

import java.awt.*;
import java.awt.event.ActionEvent;

// Referenced classes of package acm.io:
//            AWTDialog

class AWTMessageDialog extends AWTDialog
{

    public AWTMessageDialog(Frame frame, String s, Image image, String s1)
    {
        super(frame, s, image, false);
        setMessage(s1);
    }

    public void initButtonPanel(Panel panel, boolean flag)
    {
        okButton = new Button("OK");
        okButton.addActionListener(this);
        panel.add(okButton);
    }

    public void initDataPanel(Panel panel)
    {
    }

    public void actionPerformed(ActionEvent actionevent)
    {
        if(actionevent.getSource() == okButton)
            setVisible(false);
    }

    private Button okButton;
}
