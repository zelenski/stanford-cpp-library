package stanford.cs106.io;

import java.io.*;

public class LimitedPrintStream extends PrintStream {
    public static class ExcessiveOutputException extends RuntimeException {
        private static final long serialVersionUID = 0;
        public ExcessiveOutputException() {
            super();
        }    
        public ExcessiveOutputException(String message) {
            super(message);
        }    
    }

    private static final int MAX_CALLS = 5000;
    private static final int MAX_CHARS = 50000;
    
    private int calls = 0;
    private int chars = 0;
    private int maxCalls = 0;
    private int maxChars = 0;
    
    public LimitedPrintStream(java.io.OutputStream stream) {
        this(stream, MAX_CALLS, MAX_CHARS);
    }
    
    public LimitedPrintStream(File file) throws java.io.FileNotFoundException {
        this(file, MAX_CALLS, MAX_CHARS);
    }

    public LimitedPrintStream(String file) throws java.io.FileNotFoundException {
        this(file, MAX_CALLS, MAX_CHARS);
    }
    
    public LimitedPrintStream(java.io.OutputStream stream, int maxCalls, int maxChars) {
        super(stream);
        this.maxCalls = maxCalls;
        this.maxChars = maxChars;
    }
    
    public LimitedPrintStream(File file, int maxCalls, int maxChars) throws java.io.FileNotFoundException {
        super(file);
        this.maxCalls = maxCalls;
        this.maxChars = maxChars;
    }

    public LimitedPrintStream(String file, int maxCalls, int maxChars) throws java.io.FileNotFoundException {
        super(file);
        this.maxCalls = maxCalls;
        this.maxChars = maxChars;
    }
    
    
    
    // shouldn't close System.out anyway
    public void close() {
    	// empty
    }
    
    public void print(int x) {
        print(String.valueOf(x));
    }
    
    public void print(double x) {
        print(String.valueOf(x));
    }
    
    public void print(float x) {
        print(String.valueOf(x));
    }
    
    public void print(long x) {
        print(String.valueOf(x));
    }
    
    public void print(short x) {
        print(String.valueOf(x));
    }
    
    public void print(byte x) {
        print(String.valueOf(x));
    }
    
    public void print(boolean x) {
        print(String.valueOf(x));
    }
    
    public void print(char x) {
        print(String.valueOf(x));
    }
    
    public void print(Object x) {
        print(String.valueOf(x));
    }
    
    public void print(String x) {
        calls++;
        chars += (x == null) ? 4 : x.length();
        
        if (calls < maxCalls && chars < maxChars) {
            super.print(x);
        } else {
            throw new ExcessiveOutputException();
        }
    }
    
    public void println(int x) {
        println(String.valueOf(x));
    }
    
    public void println(double x) {
        println(String.valueOf(x));
    }
    
    public void println(float x) {
        println(String.valueOf(x));
    }
    
    public void println(long x) {
        println(String.valueOf(x));
    }
    
    public void println(short x) {
        println(String.valueOf(x));
    }
    
    public void println(byte x) {
        println(String.valueOf(x));
    }
    
    public void println(boolean x) {
        println(String.valueOf(x));
    }
    
    public void println(char x) {
        println(String.valueOf(x));
    }
    
    public void println(Object x) {
        println(String.valueOf(x));
    }
    
    public void println(String x) {
        print(x);
        print("\n");
    }
    
//    public void write(int b) {
//        print((char) b);
//    }
//    
//    public void write(byte[] buf, int off, int len) {
//        for (int i = off; i < off + len; i++) {
//            write(buf[i]);
//        }
//    }
}
