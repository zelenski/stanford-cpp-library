package stanford.cs106.io;

import java.io.*;

/**
 *
 * @author Marty Stepp
 */
public class ReaderInputStream extends InputStream {
    private Reader source;
    
    public ReaderInputStream(Reader source) {
        this.source = source;
    }
    
    @Override
    public void close() throws IOException {
        source.close();
    }
    
    @Override
    public void mark(int readAheadLimit) {
        try {
            source.mark(readAheadLimit);
        } catch (IOException ioe) {
        	// empty
        }
    }
    
    @Override
    public boolean markSupported() {
        return source.markSupported();
    }
    
    public int read() throws IOException {
        return source.read();
    }
    
    @Override
    public int read(byte[] b, int offset, int length) throws IOException {
        char[] cbuf = new char[b.length];
        int result = source.read(cbuf, offset, length);
        for (int i = offset; i < offset + result; i++) {
            b[i] = (byte) cbuf[i];
        }
        return result;
    }
    
    @Override
    public void reset() throws IOException {
        source.reset();
    }
    
    @Override
    public long skip(long n) throws IOException {
        return source.skip(n);
    }
}
