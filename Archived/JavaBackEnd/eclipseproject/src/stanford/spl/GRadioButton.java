/**
 * @author Marty Stepp
 * @version 2015/06/20
 */

package stanford.spl;

import java.util.*;
import javax.swing.*;

public class GRadioButton extends GInteractor {
	private static final long serialVersionUID = 1L;
	
	private static final Map<String, ButtonGroup> buttonGroups = new TreeMap<String, ButtonGroup>();
	
	public GRadioButton(String label, String group) {
		super(new JRadioButton(label));
		if (!buttonGroups.containsKey(group)) {
			buttonGroups.put(group, new ButtonGroup());
		}
		ButtonGroup bgroup = buttonGroups.get(group);
		bgroup.add((JRadioButton) this.getInteractor());
	}
	
	public boolean isSelected() {
		return ((JRadioButton) this.getInteractor()).isSelected();
	}
	
	public void setSelected(boolean selected) {
		((JRadioButton) this.getInteractor()).setSelected(selected);
	}
}
