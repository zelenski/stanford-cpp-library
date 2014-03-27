// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IODialog.java

package acm.io;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

// Referenced classes of package acm.io:
//            AWTMessageCanvas, AWTIconCanvas

abstract class AWTDialog extends Dialog
    implements ActionListener
{

    public AWTDialog(Frame frame, String s, Image image, boolean flag)
    {
        super(frame, s, true);
        setLayout(new BorderLayout());
        Panel panel = new Panel();
        Panel panel1 = new Panel();
        Panel panel2 = new Panel();
        Panel panel3 = new Panel();
        panel.setLayout(new BorderLayout());
        panel1.setLayout(new FlowLayout());
        panel2.setLayout(new BorderLayout());
        panel3.setLayout(new BorderLayout());
        panel3.add(new Label(" "));
        messageArea = new AWTMessageCanvas();
        panel2.add(messageArea, "Center");
        initButtonPanel(panel1, flag);
        initDataPanel(panel2);
        panel.add(new AWTIconCanvas(image), "West");
        panel.add(panel2, "Center");
        add(panel, "Center");
        add(panel1, "South");
        add(panel3, "East");
        Rectangle rectangle = frame.getBounds();
        int i = rectangle.x + rectangle.width / 2;
        int j = rectangle.y + rectangle.height / 2;
        setBounds(i - 130, j - 57, 260, 115);
        validate();
    }

    public abstract void initButtonPanel(Panel panel, boolean flag);

    public abstract void initDataPanel(Panel panel);

    public abstract void actionPerformed(ActionEvent actionevent);

    public void setMessage(String s)
    {
        messageArea.setMessage(s);
    }

    public static final int WIDTH = 260;
    public static final int HEIGHT = 115;
    private AWTMessageCanvas messageArea;
}
