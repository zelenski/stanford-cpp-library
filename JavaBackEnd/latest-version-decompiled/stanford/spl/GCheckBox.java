package stanford.spl;

import java.awt.event.ActionListener;
import javax.swing.JCheckBox;

public class GCheckBox
  extends GInteractor
{
  public GCheckBox(String paramString, ActionListener paramActionListener)
  {
    super(new JCheckBox(paramString));
    JCheckBox localJCheckBox = (JCheckBox)getInteractor();
    if (paramActionListener != null) {
      localJCheckBox.addActionListener(paramActionListener);
    }
  }
}
