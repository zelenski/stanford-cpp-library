package stanford.spl;

import java.util.*;

import acm.util.TokenScanner;

// AutograderUnitTest.setTestDetails(
//   "Test19_caseSensitivity",
//   "UnitTestDetails{
//        testType=TEST_ASSERT_EQUALS,
//        expected=No+word+ladder+found+from+DATa+back+to+CoDe.,
//        student=No+word+ladder+found+from+DATa+back+to+CoDe.,
//        valueType=string,
//        passed=false}")
public class AutograderUnitTest_setTestDetails extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String testName = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String details = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		boolean isStyleCheck = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		// parse the deets
		// "UnitTestDetails{testType=TEST_ASSERT_EQUALS,
		//                  expected=No+word+ladder+found+from+DATa+back+to+CoDe.,
		//                  student=No+word+ladder+found+from+DATa+back+to+CoDe.,
		//                  valueType=string,passed=false}"
		details = details.replace("UnitTestDetails{", "").replace("}", "");
		String[] deetTokens = details.split(",[ \t\r\n]{0,99}");
		Map<String, String> deetMap = new LinkedHashMap<String, String>();
		for (String token : deetTokens) {
			String[] parts = token.split("=");
			if (parts.length < 2) continue;
			deetMap.put(parts[0], SplPipeDecoder.decode(parts[1]));
		}
		
		AutograderUnitTestGUI gui = AutograderUnitTestGUI.getInstance(paramJavaBackEnd, isStyleCheck);
		gui.setTestDetails(testName, deetMap);
	}
}
