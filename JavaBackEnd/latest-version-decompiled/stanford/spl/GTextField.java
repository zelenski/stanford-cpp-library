package stanford.spl;

import java.awt.event.ActionListener;
import javax.swing.JTextField;

public class GTextField
  extends GInteractor
{
  public GTextField(int paramInt, ActionListener paramActionListener)
  {
    super(new JTextField(paramInt));
    JTextField localJTextField = (JTextField)getInteractor();
    if (paramActionListener != null) {
      localJTextField.addActionListener(paramActionListener);
    }
  }
}
