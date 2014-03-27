package stanford.spl;

import acm.graphics.GLabel;
import java.awt.Color;
import java.awt.Font;
import javax.swing.JComponent;
import javax.swing.JLabel;

public class JBELabel
  extends GLabel
{
  private JLabel jlabel;
  
  public JBELabel(String paramString)
  {
    super(paramString);
    this.jlabel = null;
  }
  
  public JComponent getInteractor()
  {
    if (this.jlabel == null)
    {
      this.jlabel = new JLabel(getLabel());
      this.jlabel.setFont(getFont());
      this.jlabel.setForeground(getColor());
    }
    return this.jlabel;
  }
  
  public void setFont(Font paramFont)
  {
    super.setFont(paramFont);
    if (this.jlabel != null) {
      this.jlabel.setFont(paramFont);
    }
  }
  
  public void setColor(Color paramColor)
  {
    super.setColor(paramColor);
    if (this.jlabel != null) {
      this.jlabel.setForeground(paramColor);
    }
  }
}
