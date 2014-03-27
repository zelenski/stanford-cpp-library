package stanford.spl;

import acm.io.IOConsole;
import acm.program.Program;
import acm.program.ProgramMenuBar;
import javax.swing.JMenu;

public class JBEMenuBar
  extends ProgramMenuBar
{
  public JBEMenuBar(JavaBackEnd paramJavaBackEnd, IOConsole paramIOConsole)
  {
    super(new JBEDummyProgram(paramJavaBackEnd));
    getProgram().setConsole(paramIOConsole);
  }
  
  protected void addFileMenuItems(JMenu paramJMenu)
  {
    paramJMenu.add(createStandardItem("Save As"));
    paramJMenu.add(createStandardItem("Quit"));
  }
}
