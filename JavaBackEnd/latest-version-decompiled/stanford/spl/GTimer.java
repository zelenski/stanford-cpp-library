package stanford.spl;

import javax.swing.Timer;

class GTimer
  extends Timer
{
  private String id;
  
  public GTimer(String paramString, double paramDouble)
  {
    super((int)Math.round(paramDouble), null);
    this.id = paramString;
  }
  
  public String getId()
  {
    return this.id;
  }
}
