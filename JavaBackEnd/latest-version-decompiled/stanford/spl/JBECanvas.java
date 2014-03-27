package stanford.spl;

import acm.graphics.GCanvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Shape;
import java.awt.Toolkit;
import java.awt.geom.Arc2D.Double;
import java.awt.geom.Ellipse2D.Double;
import java.awt.geom.Line2D.Double;
import java.awt.geom.Rectangle2D.Double;

public class JBECanvas
  extends GCanvas
{
  private String windowId;
  private TopCompound topCompound;
  private Graphics2D osg;
  private Image offscreenImage;
  private int preferredWidth;
  private int preferredHeight;
  
  public JBECanvas(String paramString, int paramInt1, int paramInt2)
  {
    this.windowId = paramString;
    setBackground(Color.WHITE);
    setForeground(Color.BLACK);
    setOpaque(false);
    setLayout(null);
    this.preferredWidth = paramInt1;
    this.preferredHeight = paramInt2;
  }
  
  protected Graphics2D getOSG()
  {
    return this.osg;
  }
  
  protected void setTopCompound(TopCompound paramTopCompound)
  {
    this.topCompound = paramTopCompound;
    paramTopCompound.setCanvas(this);
    paramTopCompound.setParent(this);
  }
  
  protected TopCompound getTopCompound()
  {
    return this.topCompound;
  }
  
  protected void initOffscreenImage()
  {
    Dimension localDimension = Toolkit.getDefaultToolkit().getScreenSize();
    this.offscreenImage = createImage(localDimension.width, localDimension.height);
    this.osg = ((Graphics2D)this.offscreenImage.getGraphics());
    this.osg.setColor(getBackground());
    this.osg.fillRect(0, 0, localDimension.width, localDimension.height);
    this.osg.setColor(getForeground());
  }
  
  public String getWindowId()
  {
    return this.windowId;
  }
  
  public Dimension getPreferredSize()
  {
    return new Dimension(this.preferredWidth, this.preferredHeight);
  }
  
  public void clear()
  {
    Dimension localDimension = Toolkit.getDefaultToolkit().getScreenSize();
    this.osg.setColor(Color.WHITE);
    this.osg.fillRect(0, 0, localDimension.width, localDimension.height);
    this.osg.setColor(Color.BLACK);
    repaint();
  }
  
  public void draw(Shape paramShape)
  {
    this.osg.draw(paramShape);
    repaint();
  }
  
  public void fill(Shape paramShape)
  {
    this.osg.fill(paramShape);
    repaint();
  }
  
  public void drawRect(double paramDouble1, double paramDouble2, double paramDouble3, double paramDouble4)
  {
    Rectangle2D.Double localDouble = new Rectangle2D.Double(paramDouble1, paramDouble2, paramDouble3, paramDouble4);
    this.osg.draw(localDouble);
    repaint();
  }
  
  public void fillRect(double paramDouble1, double paramDouble2, double paramDouble3, double paramDouble4)
  {
    Rectangle2D.Double localDouble = new Rectangle2D.Double(paramDouble1, paramDouble2, paramDouble3, paramDouble4);
    this.osg.fill(localDouble);
    this.osg.draw(localDouble);
    repaint();
  }
  
  public void drawOval(double paramDouble1, double paramDouble2, double paramDouble3, double paramDouble4)
  {
    Ellipse2D.Double localDouble = new Ellipse2D.Double(paramDouble1, paramDouble2, paramDouble3, paramDouble4);
    this.osg.draw(localDouble);
    repaint();
  }
  
  public void fillOval(double paramDouble1, double paramDouble2, double paramDouble3, double paramDouble4)
  {
    Ellipse2D.Double localDouble = new Ellipse2D.Double(paramDouble1, paramDouble2, paramDouble3, paramDouble4);
    this.osg.fill(localDouble);
    this.osg.draw(localDouble);
    repaint();
  }
  
  public void drawArc(double paramDouble1, double paramDouble2, double paramDouble3, double paramDouble4, double paramDouble5, double paramDouble6)
  {
    Arc2D.Double localDouble = new Arc2D.Double(paramDouble1, paramDouble2, paramDouble3, paramDouble4, paramDouble5, paramDouble6, 0);
    
    this.osg.draw(localDouble);
    repaint();
  }
  
  public void fillArc(double paramDouble1, double paramDouble2, double paramDouble3, double paramDouble4, double paramDouble5, double paramDouble6)
  {
    Arc2D.Double localDouble = new Arc2D.Double(paramDouble1, paramDouble2, paramDouble3, paramDouble4, paramDouble5, paramDouble6, 2);
    
    this.osg.fill(localDouble);
    this.osg.draw(localDouble);
    repaint();
  }
  
  public void drawLine(double paramDouble1, double paramDouble2, double paramDouble3, double paramDouble4)
  {
    Line2D.Double localDouble = new Line2D.Double(paramDouble1, paramDouble2, paramDouble3, paramDouble4);
    this.osg.draw(localDouble);
    repaint();
  }
  
  public void setColor(int paramInt)
  {
    this.osg.setColor(new Color(paramInt));
  }
  
  public void paint(Graphics paramGraphics)
  {
    paramGraphics.drawImage(this.offscreenImage, 0, 0, this);
    this.topCompound.paint(paramGraphics);
    super.paint(paramGraphics);
  }
}
