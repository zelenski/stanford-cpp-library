/*
 * @version 2017/06/06
 * - added addTest, containsTest
 */

package stanford.cs106.junit;

import java.util.Map;

public interface JUnitListener {
	public void addTest(String testName);
	public void addTest(String testName, String categoryName);
	public boolean containsTest(String testFullName);
	public void setTestCounts(int passCount, int testCount);
	public void setTestDetails(String testFullName, Map<String, String> details);
	public void setTestDetailsMessage(String testFullName, String detailsMessage);
	public void setTestingCompleted(boolean completed);
	public boolean setTestResult(String testFullName, String result);
	public boolean setTestRuntime(String testFullName, int runtimeMS);
}
