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
import java.lang.reflect.Field;
import java.net.Socket;
import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.Document;

// Referenced classes of package acm.util:
//            JTFTools

class SubmitOptions
    implements ActionListener, DocumentListener
{

    public SubmitOptions(Program program)
    {
        parent = program;
        authorNameField = new JTextField();
        authorEMailField = new JTextField();
        instructorEMailField = new JTextField();
        smtpServerField = new JTextField();
        cancelButton = new JButton("Cancel");
        submitButton = new JButton("Submit");
        authorEMailField.getDocument().addDocumentListener(this);
        instructorEMailField.getDocument().addDocumentListener(this);
        smtpServerField.getDocument().addDocumentListener(this);
        cancelButton.addActionListener(this);
        submitButton.addActionListener(this);
        initPreferences(program);
    }

    public String getAuthorName()
    {
        return authorNameField.getText().trim();
    }

    public String getAuthorEMail()
    {
        return authorEMailField.getText().trim();
    }

    public String getSubmissionEMail()
    {
        return instructorEMailField.getText().trim();
    }

    public String getSMTPServer()
    {
        return smtpServerField.getText().trim();
    }

    public boolean isComplete()
    {
        if(getAuthorEMail().indexOf('@') == -1)
            return false;
        if(getSubmissionEMail().indexOf('@') == -1)
            return false;
        return getSMTPServer().length() != 0;
    }

    public boolean popup()
    {
        java.awt.Frame frame = JTFTools.getEnclosingFrame(parent);
        if(frame == null)
        {
            return false;
        } else
        {
            dialog = new JDialog(frame, "Submit Project Options", true);
            Container container = dialog.getContentPane();
            container.setLayout(new TableLayout(6, 2, 0, 4));
            container.add(new JLabel("Instructor email ", 4));
            container.add(instructorEMailField, "width=300");
            container.add(new JLabel("Author name ", 4));
            container.add(authorNameField, "width=300");
            container.add(new JLabel("Author email ", 4));
            container.add(authorEMailField, "width=300");
            container.add(new JLabel("SMTP server ", 4));
            container.add(smtpServerField, "width=300");
            JPanel jpanel = new JPanel();
            jpanel.setLayout(new FlowLayout());
            jpanel.add(cancelButton);
            jpanel.add(submitButton);
            container.add(new JLabel(""));
            container.add(jpanel, "top=10");
            dialog.setSize(500, 230);
            submitFlag = false;
            submitButton.setEnabled(isComplete());
            dialog.setVisible(true);
            return submitFlag;
        }
    }

    public void actionPerformed(ActionEvent actionevent)
    {
        submitFlag = actionevent.getSource() == submitButton;
        dialog.setVisible(false);
    }

    public void changedUpdate(DocumentEvent documentevent)
    {
        submitButton.setEnabled(isComplete());
    }

    public void removeUpdate(DocumentEvent documentevent)
    {
        submitButton.setEnabled(isComplete());
    }

    public void insertUpdate(DocumentEvent documentevent)
    {
        submitButton.setEnabled(isComplete());
    }

    private void initPreferences(Program program)
    {
        instructorEMailField.setText(getDefaultField("INSTRUCTOR_EMAIL", program));
        authorNameField.setText(getDefaultField("AUTHOR_NAME", program));
        String s = getDefaultField("AUTHOR_EMAIL", program);
        if(s.length() == 0)
        {
            s = System.getProperty("user.name");
            if(s == null)
            {
                s = "";
            } else
            {
                String s1 = JTFTools.getLocalHostName();
                if(s1 != null)
                    s = (new StringBuilder()).append(s).append("@").append(s1).toString();
            }
        }
        authorEMailField.setText(s);
        String s2 = getDefaultField("SMTP_SERVER", program);
        if(s2.length() == 0)
        {
            s2 = System.getProperty("mail.smtp.host");
            if(s2 == null)
            {
                String s3 = JTFTools.getLocalHostName();
                if(s3 == null)
                {
                    s2 = "";
                } else
                {
                    s2 = (new StringBuilder()).append("smtp.").append(s3.substring(s3.indexOf('.') + 1)).toString();
                    try
                    {
                        (new Socket(s2, 25)).close();
                    }
                    catch(Exception exception)
                    {
                        s2 = "";
                    }
                }
            }
        }
        smtpServerField.setText(s2);
    }

    private String getDefaultField(String s, Program program)
    {
        String s1;
        Field field = program.getClass().getField(s);
        s1 = (String)field.get(program);
        if(s1 != null && s1.trim().length() != 0)
            return s1;
        break MISSING_BLOCK_LABEL_42;
        Exception exception;
        exception;
        return "";
    }

    private static final int FIELD_WIDTH = 300;
    private static final int DIALOG_WIDTH = 500;
    private static final int DIALOG_HEIGHT = 230;
    private static final int SMTP_PORT = 25;
    private Component parent;
    private JDialog dialog;
    private JTextField authorNameField;
    private JTextField authorEMailField;
    private JTextField instructorEMailField;
    private JTextField smtpServerField;
    private JButton cancelButton;
    private JButton submitButton;
    private boolean submitFlag;
}
