/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/04/26
 * - initial version; moved out of Program.java into its own file
 * - alphabetized methods
 */

package acm.program;

import java.lang.reflect.*;
import acm.util.JTFTools;

/* Package class: AppletStarter */
/**
 * This class creates a new thread in which to execute the <code>run</code>
 * method in the context of an applet.
 */

class AppletStarter implements Runnable {
	/* Private instance variables */
	private Program myProgram;
	private Thread mainThread;
	private Object executor;

	/* Constructor: AppletStarter(program) */
	/**
	 * Creates an object responsible for starting the main thread for the program.
	 */
	public AppletStarter(Program program) {
		myProgram = program;
	}

	/* Private method: forkUsingExecutor */
	/**
	 * The AppletRunner in Java 5.0 does not allow nontrusted packages to fork
	 * new threads.  This class uses the <code>ScheduledExecutor</code> class in
	 * <code>java.util.concurrent</code> to do the fork so that it comes from an
	 * acceptable place.
	 */
	private void forkUsingExecutor() {
		try {
			// TODO: no such class?
			Class<?> scheduledExecutorClass = Class.forName("java.util.concurrent.ScheduledExecutor");
			Class<?>[] types1 = { Integer.TYPE };
			Object[] args1 = { Integer.valueOf(1) };
			Constructor<?> constructor = scheduledExecutorClass.getConstructor(types1);
			executor = constructor.newInstance(args1);
			Class<?> timeUnitClass = java.util.concurrent.TimeUnit.class;
			Field secondsField = timeUnitClass.getField("SECONDS");
			Object seconds = secondsField.get(null);
			Class<?>[] types2 = { java.lang.Runnable.class,
					Long.TYPE,
					java.util.concurrent.TimeUnit.class };
			Object[] args2 = { this, Long.valueOf(0), seconds };
			Method schedule = executor.getClass().getMethod("schedule", types2);
			schedule.invoke(executor, args2);
		} catch (Exception ex) {
			if (JTFTools.testDebugOption("startup")) {
				System.out.println("Executor failed because " + ex);
			}
			executor = null;
			mainThread = Thread.currentThread();
			try {
				myProgram.startRun();
			} catch (Throwable t) {
				if (t instanceof Error) {
					throw (Error) t;
				} else if (t instanceof RuntimeException) {
					throw (RuntimeException) t;
				} else {
					Throwable cause = t;
					while (cause.getCause() != null) {
						cause = cause.getCause();
					}
					throw new RuntimeException(cause);
				}
			}
		}
	}

	/* Method: getMainThread() */
	/**
	 * Returns the thread that is running the main program.
	 */
	public Thread getMainThread() {
		return mainThread;
	}

	/* Method: run() */
	/**
	 * Starts the program by calling its <code>startRun</code> method, which will
	 * eventually call <code>run</code> in the program.
	 */
	public void run() {
		myProgram.startRun();
	}

	/* Method: start() */
	/**
	 * Starts a new thread for the program that will execute
	 * the <code>run</code> method.
	 */
	public void start() {
		try {
			mainThread = new Thread(new Runnable() {
				public void run() {
					try {
						AppletStarter.this.run();
					} catch (Throwable t) {
						if (t instanceof Error) {
							throw (Error) t;
						} else if (t instanceof RuntimeException) {
							throw (RuntimeException) t;
						} else {
							Throwable cause = t;
							while (cause.getCause() != null) {
								cause = cause.getCause();
							}
							throw new RuntimeException(cause);
						}
					}
				}
			});
			mainThread.start();
			if (JTFTools.testDebugOption("startup")) {
				System.out.println("Starting main thread using Thread package");
			}
		} catch (SecurityException ex) {
			if (JTFTools.testDebugOption("startup")) {
				System.out.println("Starting main thread using Executor because " + ex);
			}
			forkUsingExecutor();
		}
	}

	/* Method: stop() */
	/**
	 * Stops the main thread, using whichever strategy is appropriate for
	 * the implementation.
	 */
	public void stop() {
		try {
			if (executor == null) {
				Class<?> threadClass = java.lang.Thread.class;
				Method stop = threadClass.getMethod("stop", new Class[0]);
				stop.invoke(mainThread, new Object[0]);
			} else {
				Method shutdownNow = executor.getClass().getMethod("shutdownNow", new Class[0]);
				shutdownNow.invoke(executor, new Object[0]);
			}
		} catch (Exception ex) {
			/* Empty */
		}
	}
}
