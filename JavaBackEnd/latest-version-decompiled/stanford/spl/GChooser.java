package stanford.spl;

import java.awt.event.ActionListener;
import javax.swing.JComboBox;

public class GChooser
  extends GInteractor
{
  public GChooser(ActionListener paramActionListener)
  {
    super(new JComboBox());
    JComboBox localJComboBox = (JComboBox)getInteractor();
    if (paramActionListener != null)
    {
      localJComboBox.setEditable(false);
      localJComboBox.addActionListener(paramActionListener);
    }
  }
}
