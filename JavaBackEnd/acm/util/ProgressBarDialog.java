// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   JTFTools.java

package acm.util;

import acm.gui.TableLayout;
import acm.program.Program;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

// Referenced classes of package acm.util:
//            JTFTools

class ProgressBarDialog extends JDialog
    implements ActionListener
{

    public ProgressBarDialog(Program program)
    {
        super(JTFTools.getEnclosingFrame(program), "Progress", false);
        Container container = getContentPane();
        container.setLayout(new TableLayout(2, 1));
        progressBar = new JProgressBar();
        container.add(progressBar, "weightx=1 left=5 right=5 height=25");
        JButton jbutton = new JButton("Cancel");
        jbutton.addActionListener(this);
        container.add(jbutton, "top=10 fill=NONE");
        Rectangle rectangle = program.getBounds();
        int i = rectangle.x + (rectangle.width - 250) / 2;
        int j = rectangle.y + (rectangle.height - 90) / 2;
        setBounds(i, j, 250, 90);
    }

    public JProgressBar getProgressBar()
    {
        return progressBar;
    }

    public void actionPerformed(ActionEvent actionevent)
    {
        progressBar.setString("Cancel");
        setVisible(false);
    }

    public static boolean hasBeenCancelled(JProgressBar jprogressbar)
    {
        if(jprogressbar == null)
        {
            return false;
        } else
        {
            String s = jprogressbar.getString();
            return s != null && s.equals("Cancel");
        }
    }

    public static void popup(JProgressBar jprogressbar)
    {
        for(Object obj = jprogressbar; obj != null; obj = ((Component) (obj)).getParent())
            if(obj instanceof ProgressBarDialog)
            {
                ((Component) (obj)).setVisible(true);
                ((Component) (obj)).repaint();
                return;
            }

    }

    public static void dismiss(JProgressBar jprogressbar)
    {
        for(Object obj = jprogressbar; obj != null; obj = ((Component) (obj)).getParent())
            if(obj instanceof ProgressBarDialog)
            {
                ((Component) (obj)).setVisible(false);
                return;
            }

    }

    private static final int DIALOG_WIDTH = 250;
    private static final int DIALOG_HEIGHT = 90;
    private static final int PROGRESS_BAR_HEIGHT = 25;
    private JProgressBar progressBar;
}
