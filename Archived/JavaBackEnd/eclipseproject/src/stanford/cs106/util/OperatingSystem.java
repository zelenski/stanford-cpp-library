package stanford.cs106.util;

public enum OperatingSystem {
	WINDOWS, MAC, LINUX, UNKNOWN;
	
	public static OperatingSystem get() {
		String osName = String.valueOf(System.getProperty("os.name")).toLowerCase();
		if (osName.contains("win") || osName.contains("surface") || osName.contains("metro")) {
			return WINDOWS;
		} else if (osName.contains("mac") || osName.contains("os x")
				|| osName.contains("osx") || osName.contains("ios")) {
			return MAC;
		} else if (osName.contains("nix") || osName.contains("nux")
				|| osName.contains("edora") || osName.contains("buntu")) {
			return LINUX;
		} else {
			return UNKNOWN;
		}
	}
}
