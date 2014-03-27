package stanford.spl;

import javax.swing.JSlider;
import javax.swing.event.ChangeListener;

public class GSlider
  extends GInteractor
{
  public GSlider(int paramInt1, int paramInt2, int paramInt3, ChangeListener paramChangeListener)
  {
    super(new JSlider(paramInt1, paramInt2, paramInt3));
    JSlider localJSlider = (JSlider)getInteractor();
    if (paramChangeListener != null) {
      localJSlider.addChangeListener(paramChangeListener);
    }
  }
}
