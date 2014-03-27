package stanford.spl;

import acm.graphics.GCompound;

public class TopCompound
  extends GCompound
{
  private JBECanvas owner;
  
  public void setCanvas(JBECanvas paramJBECanvas)
  {
    this.owner = paramJBECanvas;
  }
  
  public JBECanvas getCanvas()
  {
    return this.owner;
  }
}
