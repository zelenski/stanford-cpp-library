package stanford.cs106.junit;

import java.util.Map;

public interface JUnitListener {
	public void setTestCounts(int passCount, int testCount);
	public void setTestDetails(String testName, Map<String, String> details);
	public void setTestDetailsMessage(String testName, String detailsMessage);
	public void setTestingCompleted(boolean completed);
	public boolean setTestResult(String testName, String result);
	public boolean setTestRuntime(String testName, int runtimeMS);
}
