package stanford.spl;

import java.awt.event.ActionListener;
import javax.swing.JButton;

public class GButton
  extends GInteractor
{
  public GButton(String paramString, ActionListener paramActionListener)
  {
    super(new JButton(paramString));
    JButton localJButton = (JButton)getInteractor();
    if (paramActionListener != null) {
      localJButton.addActionListener(paramActionListener);
    }
  }
}
