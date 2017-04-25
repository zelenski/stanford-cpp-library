package acm.program;

import java.util.*;

public interface ProgramInterface {
	public void exit();
	public void init();
	public void run();
	public void setParameterTable(Map<String, String> table);
	public void setStartupObject(Object obj);
	public void start();
	public void start(String[] args);
}
