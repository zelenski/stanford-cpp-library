package stanford.spl;

import acm.util.TokenScanner;
import java.io.PrintStream;

class File_openFileDialog
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str3 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    System.out.println("result:" + paramJavaBackEnd.openFileDialog(str1, str2, str3));
    System.out.flush();
  }
}
