// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOConsole.java

package acm.io;

import acm.program.ProgramMenuBar;
import acm.util.ErrorException;
import java.awt.*;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.IOException;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.text.*;

// Referenced classes of package acm.io:
//            ConsoleOutputMonitor, ConsoleInputMonitor, CharacterQueue, ConsoleActionListener, 
//            ConsoleModel, IOConsole

class StandardConsoleModel
    implements KeyListener, FocusListener, ConsoleModel
{

    public StandardConsoleModel()
    {
        outputMonitor = new ConsoleOutputMonitor(this);
        inputMonitor = new ConsoleInputMonitor(this);
        scrollPane = new JScrollPane(22, 32);
        textPane = new JTextPane();
        textPane.addKeyListener(this);
        textPane.addFocusListener(this);
        scrollPane.setViewportView(textPane);
        document = textPane.getDocument();
        lineSeparator = System.getProperty("line.separator");
        outputAttributes = new SimpleAttributeSet();
        inputAttributes = new SimpleAttributeSet();
        errorAttributes = new SimpleAttributeSet();
        buffer = new CharacterQueue();
        base = 0;
    }

    public void setConsole(IOConsole ioconsole)
    {
        console = ioconsole;
    }

    public IOConsole getConsole()
    {
        return console;
    }

    public void print(String s, int i)
    {
        outputMonitor.print(s, i);
    }

    public String readLine()
    {
        return inputMonitor.readLine();
    }

    public void setInputScript(BufferedReader bufferedreader)
    {
        inputScript = bufferedreader;
        if(buffer.isWaiting())
            try
            {
                String s = inputScript.readLine();
                buffer.enqueue((new StringBuilder()).append(s).append("\n").toString());
            }
            catch(IOException ioexception)
            {
                throw new ErrorException(ioexception);
            }
    }

    public BufferedReader getInputScript()
    {
        return inputScript;
    }

    public void clear()
    {
        textPane.setText("");
        base = 0;
        buffer.clear();
    }

    public String getText()
    {
        return textPane.getText();
    }

    public String getText(int i, int j)
    {
        return document.getText(i, j - i);
        BadLocationException badlocationexception;
        badlocationexception;
        throw new ErrorException(badlocationexception);
    }

    public int getLength()
    {
        return document.getLength();
    }

    public Component getConsolePane()
    {
        return scrollPane;
    }

    public Component getTextPane()
    {
        return textPane;
    }

    public void cut()
    {
        copy();
        deleteSelection();
    }

    public void copy()
    {
        textPane.copy();
    }

    public void paste()
    {
        if(textPane.getSelectionEnd() != document.getLength())
            return;
        int i = deleteSelection();
        textPane.setSelectionStart(i);
        textPane.paste();
        textPane.select(document.getLength(), document.getLength());
        if(document instanceof DefaultStyledDocument)
        {
            DefaultStyledDocument defaultstyleddocument = (DefaultStyledDocument)document;
            defaultstyleddocument.setCharacterAttributes(i, textPane.getSelectionEnd() - i, inputAttributes, true);
        }
    }

    public void selectAll()
    {
        textPane.selectAll();
    }

    public boolean isPointSelection()
    {
        return textPane.getSelectionStart() == textPane.getSelectionEnd();
    }

    public void print(PrintJob printjob)
    {
        Graphics g = printjob.getGraphics();
        Dimension dimension = printjob.getPageDimension();
        FontMetrics fontmetrics = textPane.getFontMetrics(textPane.getFont());
        int i = fontmetrics.getHeight();
        int j = fontmetrics.getAscent();
        int k = 36;
        int l = 36 + j;
        int i1 = (dimension.height - 72) / i;
        int j1 = i1;
        ElementIterator elementiterator = new ElementIterator(document);
        do
        {
label0:
            {
                Element element;
                do
                {
                    element = elementiterator.next();
                    if(element == null)
                        break label0;
                } while(!element.isLeaf());
                try
                {
                    int k1 = element.getEndOffset() - element.getStartOffset();
                    setStyleFromAttributes(g, element.getAttributes());
                    FontMetrics fontmetrics1 = g.getFontMetrics();
                    String s = document.getText(element.getStartOffset(), k1);
                    int l1 = 0;
                    do
                    {
                        boolean flag = false;
                        int i2 = 1;
                        int j2 = s.indexOf("\n", l1);
                        if(j2 == -1)
                        {
                            j2 = s.indexOf(lineSeparator, l1);
                            if(j2 == -1)
                            {
                                j2 = s.length();
                                flag = true;
                            } else
                            {
                                i2 = lineSeparator.length();
                            }
                        }
                        if(l1 < j2)
                        {
                            String s1 = s.substring(l1, j2);
                            g.drawString(s1, k, l);
                            k += fontmetrics1.stringWidth(s1);
                        }
                        if(flag)
                            break;
                        l1 = j2 + i2;
                        k = 36;
                        l += i;
                        if(--j1 <= 0)
                        {
                            g.dispose();
                            g = printjob.getGraphics();
                            l = 36 + j;
                            j1 = i1;
                        }
                    } while(true);
                }
                catch(BadLocationException badlocationexception)
                {
                    throw new ErrorException(badlocationexception);
                }
            }
        } while(true);
        printjob.end();
        return;
    }

    public void setInputStyle(int i)
    {
        if(getLength() != 0)
        {
            throw new ErrorException("Console styles and colors cannot be changed after I/O has started.");
        } else
        {
            inputAttributes.addAttribute(StyleConstants.Bold, new Boolean((i & 1) != 0));
            inputAttributes.addAttribute(StyleConstants.Italic, new Boolean((i & 2) != 0));
            return;
        }
    }

    public void setInputColor(Color color)
    {
        if(getLength() != 0)
        {
            throw new ErrorException("Console styles and colors cannot be changed after I/O has started.");
        } else
        {
            inputAttributes.addAttribute(StyleConstants.Foreground, color);
            return;
        }
    }

    public void setErrorStyle(int i)
    {
        if(getLength() != 0)
        {
            throw new ErrorException("Console styles and colors cannot be changed after I/O has started.");
        } else
        {
            errorAttributes.addAttribute(StyleConstants.Bold, new Boolean((i & 1) != 0));
            errorAttributes.addAttribute(StyleConstants.Italic, new Boolean((i & 2) != 0));
            return;
        }
    }

    public void setErrorColor(Color color)
    {
        if(getLength() != 0)
        {
            throw new ErrorException("Console styles and colors cannot be changed after I/O has started.");
        } else
        {
            errorAttributes.addAttribute(StyleConstants.Foreground, color);
            return;
        }
    }

    public void requestFocus()
    {
        if(textPane != null)
            textPane.requestFocus();
    }

    public void setMenuBar(ProgramMenuBar programmenubar)
    {
        menuBar = programmenubar;
    }

    public void focusGained(FocusEvent focusevent)
    {
        hasFocus = true;
        if(menuBar != null)
        {
            if(actionListener == null)
                actionListener = new ConsoleActionListener(console);
            menuBar.setFocusedListener(actionListener);
            console.updateMenuBar(menuBar);
        }
    }

    public void focusLost(FocusEvent focusevent)
    {
        hasFocus = false;
        if(menuBar != null)
            menuBar.setFocusedListener(null);
    }

    public void keyTyped(KeyEvent keyevent)
    {
        if(!keyevent.isMetaDown() && !keyevent.isControlDown())
        {
            buffer.enqueue(keyevent.getKeyChar());
            keyevent.consume();
        }
    }

    public void keyPressed(KeyEvent keyevent)
    {
        switch(keyevent.getKeyCode())
        {
        case 37: // '%'
            buffer.enqueue('\002');
            break;

        case 39: // '\''
            buffer.enqueue('\006');
            break;
        }
        if(menuBar != null)
            menuBar.fireAccelerator(keyevent);
        keyevent.consume();
    }

    public void keyReleased(KeyEvent keyevent)
    {
        keyevent.consume();
    }

    protected void printCallback(String s, int i)
    {
        insert(s, base, i);
        base += s.length();
        setCaretPosition(base);
    }

    protected String readLineCallback()
    {
        base = getLength();
        if(inputScript != null)
        {
            String s = null;
            try
            {
                s = inputScript.readLine();
            }
            catch(IOException ioexception)
            {
                throw new ErrorException(ioexception);
            }
            if(s != null)
            {
                insert(s, base, 1);
                insert("\n", base + s.length(), 0);
                base += s.length() + 1;
                return s;
            }
            try
            {
                inputScript.close();
            }
            catch(IOException ioexception1) { }
            inputScript = null;
        }
        setCaretPosition(base);
        do
        {
            char c;
            if((c = buffer.dequeue()) == '\n' || c == '\r')
                break;
            if(getCaretPosition() < base)
                setCaretPosition(getLength());
            int i = getSelectionStart();
            switch(c)
            {
            case 8: // '\b'
            case 127: // '\177'
                if(i == getSelectionEnd())
                {
                    if(i > base)
                    {
                        delete(i - 1, i);
                        i--;
                    }
                } else
                {
                    i = deleteSelection();
                }
                break;

            case 1: // '\001'
                selectAll();
                i = -1;
                break;

            case 2: // '\002'
                i = Math.max(getSelectionStart() - 1, base);
                break;

            case 3: // '\003'
                copy();
                i = -1;
                break;

            case 6: // '\006'
                i = Math.min(getSelectionEnd() + 1, getLength());
                break;

            case 16: // '\020'
                console.printConsole();
                i = -1;
                break;

            case 19: // '\023'
                console.save();
                i = -1;
                break;

            case 22: // '\026'
                paste();
                i = -1;
                break;

            case 24: // '\030'
                cut();
                i = -1;
                break;

            default:
                if(i != getSelectionEnd())
                    i = deleteSelection();
                insert((new StringBuilder()).append("").append(c).toString(), i, 1);
                i++;
                break;
            }
            if(i != -1)
            {
                select(i, i);
                setCaretPosition(i);
            }
        } while(true);
        int j = getLength() - base;
        String s1 = getText(base, base + j);
        insert("\n", base + j, 0);
        base += j + 1;
        return s1;
    }

    public boolean isCommandEnabled(String s)
    {
        return hasFocus;
    }

    private void insert(String s, int i, int j)
    {
        try
        {
            SimpleAttributeSet simpleattributeset = outputAttributes;
            switch(j)
            {
            case 1: // '\001'
                simpleattributeset = inputAttributes;
                break;

            case 2: // '\002'
                simpleattributeset = errorAttributes;
                break;
            }
            document.insertString(i, s, simpleattributeset);
        }
        catch(BadLocationException badlocationexception) { }
    }

    private void delete(int i, int j)
    {
        try
        {
            document.remove(i, j - i);
        }
        catch(BadLocationException badlocationexception)
        {
            throw new ErrorException(badlocationexception);
        }
    }

    private void setCaretPosition(int i)
    {
        textPane.setCaretPosition(i);
    }

    private int getCaretPosition()
    {
        return textPane.getCaretPosition();
    }

    private void select(int i, int j)
    {
        textPane.select(i, j);
    }

    private int getSelectionStart()
    {
        return textPane.getSelectionStart();
    }

    private int getSelectionEnd()
    {
        return textPane.getSelectionEnd();
    }

    private int deleteSelection()
    {
        int i = Math.max(base, getSelectionStart());
        int j = getSelectionEnd();
        if(j <= base)
        {
            return getLength();
        } else
        {
            delete(i, j);
            return i;
        }
    }

    private void setStyleFromAttributes(Graphics g, AttributeSet attributeset)
    {
        Font font = textPane.getFont();
        int i = 0;
        if(Boolean.TRUE.equals(attributeset.getAttribute(StyleConstants.Bold)))
            i |= 1;
        if(Boolean.TRUE.equals(attributeset.getAttribute(StyleConstants.Italic)))
            i |= 2;
        g.setFont(new Font(font.getName(), i, font.getSize()));
        Color color = (Color)attributeset.getAttribute(StyleConstants.Foreground);
        if(color == null)
            color = textPane.getForeground();
        g.setColor(color);
    }

    private static final int PRINT_MARGIN = 36;
    private ActionListener actionListener;
    private ConsoleOutputMonitor outputMonitor;
    private ConsoleInputMonitor inputMonitor;
    private BufferedReader inputScript;
    private CharacterQueue buffer;
    private SimpleAttributeSet outputAttributes;
    private SimpleAttributeSet inputAttributes;
    private SimpleAttributeSet errorAttributes;
    private JScrollPane scrollPane;
    private JTextPane textPane;
    private Document document;
    private String lineSeparator;
    private int base;
    private boolean hasFocus;
    private IOConsole console;
    private ProgramMenuBar menuBar;
}
