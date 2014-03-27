package stanford.spl;

import acm.gui.TableLayout;
import java.awt.BorderLayout;
import java.awt.event.WindowEvent;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class JBEWindow
  extends JFrame
{
  private JavaBackEnd jbe;
  private JBECanvas canvas;
  private String windowId;
  private JPanel northPanel;
  private JPanel eastPanel;
  private JPanel southPanel;
  private JPanel westPanel;
  
  public JBEWindow(JavaBackEnd paramJavaBackEnd, String paramString1, String paramString2, int paramInt1, int paramInt2)
  {
    super(paramString2);
    this.jbe = paramJavaBackEnd;
    this.windowId = paramString1;
    setLayout(new BorderLayout());
    this.canvas = new JBECanvas(paramString1, paramInt1, paramInt2);
    addWindowListener(this.jbe);
    this.canvas.addComponentListener(this.jbe);
    this.canvas.addMouseListener(this.jbe);
    this.canvas.addMouseMotionListener(this.jbe);
    this.canvas.addKeyListener(this.jbe);
    add(this.canvas, "Center");
    createSidePanels();
  }
  
  public JBECanvas getCanvas()
  {
    return this.canvas;
  }
  
  public String getWindowId()
  {
    return this.windowId;
  }
  
  public void close()
  {
    WindowEvent localWindowEvent = new WindowEvent(this, 201);
    processWindowEvent(localWindowEvent);
  }
  
  public void clear()
  {
    this.canvas.clear();
    int i = 0;
    if (this.northPanel != null)
    {
      this.northPanel.removeAll();
      i = 1;
    }
    if (this.eastPanel != null)
    {
      this.eastPanel.removeAll();
      i = 1;
    }
    if (this.southPanel != null)
    {
      this.southPanel.removeAll();
      i = 1;
    }
    if (this.westPanel != null)
    {
      this.westPanel.removeAll();
      i = 1;
    }
    if (i != 0) {
      validate();
    }
  }
  
  public void addToRegion(JComponent paramJComponent, String paramString)
  {
    JPanel localJPanel = null;
    if (paramString.equalsIgnoreCase("NORTH")) {
      localJPanel = this.northPanel;
    } else if (paramString.equalsIgnoreCase("EAST")) {
      localJPanel = this.eastPanel;
    } else if (paramString.equalsIgnoreCase("SOUTH")) {
      localJPanel = this.southPanel;
    } else if (paramString.equalsIgnoreCase("WEST")) {
      localJPanel = this.westPanel;
    }
    if (localJPanel != null)
    {
      localJPanel.add(paramJComponent);
      validate();
    }
  }
  
  public void setRegionAlignment(String paramString1, String paramString2)
  {
    JPanel localJPanel = null;
    if (paramString1.equalsIgnoreCase("NORTH")) {
      localJPanel = this.northPanel;
    } else if (paramString1.equalsIgnoreCase("EAST")) {
      localJPanel = this.eastPanel;
    } else if (paramString1.equalsIgnoreCase("SOUTH")) {
      localJPanel = this.southPanel;
    } else if (paramString1.equalsIgnoreCase("WEST")) {
      localJPanel = this.westPanel;
    }
    int i = 10;
    if (paramString2.equalsIgnoreCase("LEFT")) {
      i = 11;
    } else if (paramString2.equalsIgnoreCase("RIGHT")) {
      i = 12;
    }
    if (localJPanel != null) {
      ((TableLayout)localJPanel.getLayout()).setHorizontalAlignment(i);
    }
  }
  
  private void createSidePanels()
  {
    this.northPanel = new JPanel();
    this.southPanel = new JPanel();
    this.eastPanel = new JPanel();
    this.westPanel = new JPanel();
    this.northPanel.setLayout(new TableLayout(1, 0, 5, 5));
    this.southPanel.setLayout(new TableLayout(1, 0, 5, 5));
    this.westPanel.setLayout(new TableLayout(0, 1, 5, 5));
    this.eastPanel.setLayout(new TableLayout(0, 1, 5, 5));
    add(this.northPanel, "North");
    add(this.southPanel, "South");
    add(this.eastPanel, "East");
    add(this.westPanel, "West");
  }
}
