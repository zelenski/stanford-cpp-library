package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import java.io.PrintStream;
import java.lang.reflect.Method;
import javax.swing.JComboBox;

class GChooser_addItem
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GObject localGObject = paramJavaBackEnd.getGObject(str1);
    if (localGObject != null)
    {
      JComboBox localJComboBox = (JComboBox)((GChooser)localGObject).getInteractor();
      Class localClass = localJComboBox.getClass();
      Class[] arrayOfClass = { Object.class };
      Object[] arrayOfObject = { str2 };
      try
      {
        Method localMethod = localClass.getMethod("addItem", arrayOfClass);
        localMethod.invoke(localJComboBox, arrayOfObject);
      }
      catch (Exception localException)
      {
        System.out.println("error: " + localException);
      }
    }
  }
}
