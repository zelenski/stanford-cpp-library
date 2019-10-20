package stanford.karel;

import java.awt.FileDialog;

import acm.util.JTFTools;

class NewWorldDialog extends FileDialog {
	public NewWorldDialog(KarelWorld world) {
		super(JTFTools.getEnclosingFrame(world), "New World", FileDialog.SAVE);
		setDirectory(KarelProgram.getWorldDirectory());
	}
}
