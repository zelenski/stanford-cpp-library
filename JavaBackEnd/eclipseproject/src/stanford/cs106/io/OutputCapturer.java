package stanford.cs106.io;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

/** for capturing console output (for methods that do printlns) */
public class OutputCapturer {
    private static final PrintStream SYSTEM_OUT = java.lang.System.out;
    private static final PrintStream SYSTEM_ERR = java.lang.System.err;
    private static ByteArrayOutputStream outputStream = null;
    
    /** Returns true if output is currently being captured. */
    public static boolean isCapturing() {
        return outputStream != null;
    }

    /** Begins capturing output with no limit as to its length. */
    public static synchronized void start() {
        start(true);
    }
    
    /** 
     * Begins capturing output.  Will throw an exception if the student 
     * printlns too much.
     */
    public static synchronized void start(boolean limit) {
        if (isCapturing()) {
            stop();
        }
        
        outputStream = new ByteArrayOutputStream(16384);
        PrintStream out;
        if (limit) {
            out = new LimitedPrintStream(outputStream);
        } else {
            out = new PrintStream(outputStream);
        }
        System.setOut(out);
        System.setErr(out);
    }
    
    /** Stops capturing output and returns the string of captured output. */
    public static synchronized String stop() {
        String actualOutput = "";
        if (isCapturing()) {
            System.out.flush();
            System.err.flush();
            System.setOut(SYSTEM_OUT);
            System.setErr(SYSTEM_ERR);
            if (isCapturing()) {
                actualOutput = outputStream.toString().replace("\r", "");
            }
            outputStream = null;
        }
        return actualOutput;
    }
}
