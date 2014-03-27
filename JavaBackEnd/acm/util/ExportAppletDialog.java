// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   JTFTools.java

package acm.util;

import acm.program.Program;
import java.awt.*;
import java.io.File;
import javax.swing.*;

class ExportAppletDialog extends JFileChooser
{

    public ExportAppletDialog(File file, Program program)
    {
        super(file);
        setFileSelectionMode(1);
        setDialogTitle("Export Applet");
        String s = program.getClass().getName();
        String s1 = s.substring(s.lastIndexOf('.') + 1);
        setSelectedFile(new File(file, s1));
        exportFilesCheckBox = new JCheckBox("Export resource files");
        exportFilesCheckBox.setSelected(true);
        target = program;
    }

    protected JDialog createDialog(Component component)
    {
        JDialog jdialog = super.createDialog(component);
        JPanel jpanel = new JPanel();
        jpanel.setLayout(new FlowLayout(0));
        jpanel.add(exportFilesCheckBox);
        jdialog.getContentPane().add(jpanel, "South");
        jdialog.validate();
        return jdialog;
    }

    public File chooseOutputDirectory()
    {
        int i = showSaveDialog(target);
        if(i == 1)
            return null;
        else
            return getSelectedFile();
    }

    public boolean exportFiles()
    {
        return exportFilesCheckBox.isSelected();
    }

    private JCheckBox exportFilesCheckBox;
    private Component target;
}
