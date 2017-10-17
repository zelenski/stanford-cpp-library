/*
 * @version 2017/10/12
 * - initial version
 */

package stanford.spl;

import javax.swing.JLabel;

public class GTextLabel extends GInteractor {
	public GTextLabel(String text) {
		super(new JLabel(text));
	}
	
	@Override
	public JLabel getInteractor() {
		return (JLabel) super.getInteractor();
	}
	
	public String getText() {
		return getInteractor().getText();
	}
	
	public void setText(String text) {
		getInteractor().setText(text);
	}
}
