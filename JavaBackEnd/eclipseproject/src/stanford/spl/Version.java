package stanford.spl;

public class Version {
	private static final String JAVA_BACK_END_VERSION = "2014/11/26";
	
	public static final String getLibraryVersion() {
		return JAVA_BACK_END_VERSION;
	}
	
	public static void main(String[] args) {
		System.out.println(getLibraryVersion());
	}
}
