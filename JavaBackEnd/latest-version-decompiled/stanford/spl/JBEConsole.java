package stanford.spl;

import acm.io.IOConsole;
import java.awt.Dimension;

public class JBEConsole
  extends IOConsole
{
  private int preferredWidth;
  private int preferredHeight;
  
  public void setPreferredSize(int paramInt1, int paramInt2)
  {
    this.preferredWidth = paramInt1;
    this.preferredHeight = paramInt2;
  }
  
  public Dimension getPreferredSize()
  {
    return new Dimension(this.preferredWidth, this.preferredHeight);
  }
}
