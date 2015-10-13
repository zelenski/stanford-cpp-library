/*
 * Class that can automatically check for updates to the Stanford libraries
 * and download them and update them in place as needed.
 */

package stanford.spl;

import java.awt.*;
import java.io.*;
import javax.swing.*;

public class LibraryUpdater {
	private static final boolean AUTO_DOWNLOAD = false;
	private static final String LIBRARY_FOLDER_URL = "http://stanford.edu/~stepp/cppdoc/";
	private static final String SPL_JAR_VERSION_URL = LIBRARY_FOLDER_URL + "CURRENTVERSION_SPLJAR.txt";
	private static final String CPP_ZIP_VERSION_URL = LIBRARY_FOLDER_URL + "CURRENTVERSION_CPPLIB.txt";
	private static final String CPP_ZIP_FILENAME = "StanfordCPPLib.zip";
	private static final String CPP_ZIP_URL = LIBRARY_FOLDER_URL + CPP_ZIP_FILENAME;
	private static final String SPL_JAR_FILENAME = "spl.jar";
	private static final String SPL_JAR_URL = LIBRARY_FOLDER_URL + SPL_JAR_FILENAME;
	
	public boolean checkForUpdates() {
		return checkForUpdates(/* parent */ null);
	}
	
	public boolean checkForUpdates(final Component parent) {
		String currentVersion = Version.isJavaProject() ? Version.getLibraryVersion() : Version.getCppLibraryVersion();
		
		final String latestVersionUrl = Version.isJavaProject() ? SPL_JAR_VERSION_URL : CPP_ZIP_VERSION_URL;
		
		final UrlDownloader downloader = UrlDownloader.getInstance();
		String latestVersion = "";
		try {
			latestVersion = downloader.download(latestVersionUrl);
		} catch (IOException ioe) {
			JOptionPane.showMessageDialog(parent,
					"Unable to look up latest version from web: " + ioe.toString());
			return false;
		}
		
		currentVersion = currentVersion.trim().replace("-", "/");
		latestVersion = latestVersion.trim().replace("-", "/");
		boolean outOfDate = currentVersion.compareTo(latestVersion) < 0;
		if (!outOfDate) {
			JOptionPane.showMessageDialog(parent,
					"This project already has the latest version \nof the Stanford libraries (" + currentVersion + ").");
			return false;
		}
		
		String outOfDateMessage = "There is an updated version of the Stanford libraries available.\n\n"
				+ "This project's library version: " + currentVersion + "\n"
				+ "Current newest library version: " + latestVersion + "\n";

		if (AUTO_DOWNLOAD) {
			outOfDateMessage += "Do you want to update your project now?";
			int confirmResult = JOptionPane.showConfirmDialog(parent,
					/* message */ outOfDateMessage,					
					/* title */ "Update library?",
					/* type */ JOptionPane.YES_NO_OPTION);
			if (confirmResult != JOptionPane.YES_OPTION) {
				return false;
			}
		} else {
			// don't download; just tell user about new version and stop
			outOfDateMessage += "Go to " + LIBRARY_FOLDER_URL + " to get the new version.";
			JOptionPane.showMessageDialog(parent, outOfDateMessage);
			return false;
		}
		
		final String libUrl;
		final String libFileName;
		final File libOutputFile;
		
		if (Version.isJavaProject()) {
			libUrl = SPL_JAR_URL;
			libFileName = SPL_JAR_FILENAME;
			
			// figure out where to save to
			// (some projects put in lib/ dir, others in root)
			File libDir = new File("lib");
			if (libDir.isDirectory() && new File(libDir, libFileName).exists()) {
				// put in lib/ dir (Marty Stepp-style project structure)
				libOutputFile = new File(libDir, libFileName);
			} else if (new File(libFileName).exists()) {
				// fallback to current dir
				libOutputFile = new File(libFileName);
			} else {
				// dunno where spl.jar is; set null and prompt for it later
				libOutputFile = chooseLibFile(parent, libFileName, /* dirOnly */ false);
			}
		} else {
			libUrl = CPP_ZIP_URL;
			libFileName = CPP_ZIP_FILENAME;
			
			// figure out where to save to
			// (TODO: look around .. for build folder, project folder(s), etc.?)
			// - what if they put multiple projects in same place?
			// - pop up JFileChooser to select destination directory?
			
			// libOutputFile = new File(CPP_ZIP_FILENAME);
			libOutputFile = chooseLibFile(parent, libFileName, /* dirOnly */ true);
		}
		
		if (libOutputFile == null) {
			return false;
		}
		
		// okay, update the library!
		final TextFileDialog dialog = TextFileDialog.showDialog(
				/* parent */ parent,
				/* title */ "Updating Stanford libraries",
				/* text */ "",
				/* rows */ 20,
				/* cols */ 70,
				/* modal */ false);
		dialog.setSize(500, 300);
		
		dialog.println("Downloading library from:\n" + libUrl + "\n");
		
		Runnable runnable = new Runnable() {
			public void run() {
				try {
					downloader.download(libUrl, libOutputFile);
					dialog.println("Downloaded successfully: " + libOutputFile.getName() + "\n");
					if (Version.isJavaProject()) {
						dialog.println("Refresh your project and re-run it");
						dialog.println("to begin using the new library.");
						dialog.println("(Right-click your project name in the left");
						dialog.println("Package Explorer and choose Refresh.)");
					} else {
//						dialog.println("You will need to un-zip the new version\ninto your project's library folder as appropriate.");
//						dialog.println("If you are not sure how to do this,");
//						dialog.println("please seek assistance from your instructor or TA.");
					}
				} catch (IOException ioe) {
					dialog.println("Unable to download library: \n" + ioe.toString());
				}
			}
		};
		Thread thread = new Thread(runnable);
		thread.start();
		
		// wait for download to finish
		while (thread.isAlive()) {
			try {
				Thread.sleep(20);
			} catch (InterruptedException ie) {
				// empty
			}
		}
		
		return outOfDate;
	}
	
	private File chooseLibFile(Component parent, String libFileName, boolean dirOnly) {
		JFileChooser fileChooser = new JFileChooser(System.getProperty("user.dir"));
		if (dirOnly) {
			fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		} else {
			fileChooser.setFileFilter(new javax.swing.filechooser.FileFilter() {
				public boolean accept(File f) {
					return f != null && f.getName().endsWith(".jar");
				}
				public String getDescription() {
					return "Java Archives (*.jar)";
				}
			});
		}
		int result = fileChooser.showSaveDialog(parent);
		if (result != JFileChooser.APPROVE_OPTION) {
			return null;
		}
		File selectedDir = fileChooser.getSelectedFile();
		if (selectedDir == null || !selectedDir.exists() || !selectedDir.isDirectory()) {
			return null;
		}
		File libOutputFileToUse = new File(selectedDir, libFileName);
		return libOutputFileToUse;
	}
}
