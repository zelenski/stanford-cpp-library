package stanford.karel;

import java.awt.FileDialog;
import java.io.File;
import java.io.FilenameFilter;
import acm.util.JTFTools;

class LoadWorldDialog extends FileDialog implements FilenameFilter {

	public LoadWorldDialog(KarelWorld world) {
		super(JTFTools.getEnclosingFrame(world), "Load World");
		setDirectory(KarelProgram.getWorldDirectory());
		setFilenameFilter(this);
	}

	public boolean accept(File dir, String name) {
		return (name.endsWith(KarelProgram.WORLD_EXTENSION));
	}
}

