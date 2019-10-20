package stanford.cs106.util;

import java.lang.management.*;

public class ProcessUtils {
	private ProcessUtils() {
		// empty
	}
	
	public static int getPID() {
		RuntimeMXBean bean = ManagementFactory.getRuntimeMXBean();
		if (bean == null) {
			return -1;
		}
		String name = bean.getName();
		if (name == null || !name.contains("@")) {
			return -1;
		}
		String pidStr = name.substring(0, name.indexOf("@"));
		try {
			return Integer.parseInt(pidStr);
		} catch (NumberFormatException nfe) {
			return -1;
		}
	}
}
