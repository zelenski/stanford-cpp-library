// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   SoundClip.java

package acm.util;

import java.applet.AudioClip;
import java.io.*;
import java.net.URL;
import java.text.DecimalFormat;
import javax.sound.sampled.*;

// Referenced classes of package acm.util:
//            ErrorException, SoundPlayer, JTFTools

public class SoundClip
    implements AudioClip
{

    public SoundClip()
    {
        this(1);
    }

    public SoundClip(int i)
    {
        if(i < 1 || i > 2)
        {
            throw new ErrorException("SoundClip: Illegal number of channels");
        } else
        {
            nChannels = i;
            frameRate = 22050F;
            soundName = "Untitled";
            frameCount = 0;
            clipVolume = 1.0D;
            data = new byte[0];
            format = new AudioFormat(frameRate, 16, i, true, true);
            return;
        }
    }

    public SoundClip(String s)
    {
        if(s.startsWith("http:"))
        {
            try
            {
                URL url = new URL(s);
                soundName = JTFTools.getURLSuffix(url.getPath());
                readSound(convertToPCM(getAudioInputStream(url.openStream())));
            }
            catch(Exception exception)
            {
                throw new ErrorException(exception);
            }
        } else
        {
            soundName = s;
            readSound(convertToPCM(getAudioInputStream(new File(s))));
        }
    }

    public SoundClip(File file)
    {
        try
        {
            soundName = file.getName();
            readSound(convertToPCM(getAudioInputStream(file)));
        }
        catch(Exception exception)
        {
            throw new ErrorException(exception);
        }
    }

    public SoundClip(URL url)
    {
        try
        {
            soundName = JTFTools.getURLSuffix(url.getPath());
            readSound(convertToPCM(getAudioInputStream(url.openStream())));
        }
        catch(Exception exception)
        {
            throw new ErrorException(exception);
        }
    }

    public SoundClip(InputStream inputstream)
    {
        soundName = "Untitled";
        readSound(convertToPCM(getAudioInputStream(inputstream)));
    }

    public SoundClip(int ai[])
    {
        this(1);
        addSampleData(ai);
    }

    public SoundClip(int ai[], int ai1[])
    {
        this(2);
        addSampleData(ai, ai1);
    }

    public synchronized void play()
    {
        if(player == null)
            player = new SoundPlayer(this);
        player.play();
    }

    public void loop()
    {
        if(player == null)
            player = new SoundPlayer(this);
        player.loop();
    }

    public synchronized void stop()
    {
        if(player != null)
            player.stop();
    }

    public void save(String s)
    {
        save(new File(System.getProperty("user.dir"), s));
    }

    public void save(File file)
    {
        try
        {
            AudioSystem.write(getAudioInputStream(), getFormatForFile(file.getName()), file);
        }
        catch(IOException ioexception)
        {
            throw new ErrorException((new StringBuilder()).append("save: I/O error - ").append(ioexception.getMessage()).toString());
        }
    }

    public String getName()
    {
        return soundName;
    }

    public void setName(String s)
    {
        soundName = s;
    }

    public int getChannelCount()
    {
        return nChannels;
    }

    public boolean isStereo()
    {
        return nChannels == 2;
    }

    public int getFrameCount()
    {
        return frameCount;
    }

    public double getFrameRate()
    {
        return (double)frameRate;
    }

    public double getDuration()
    {
        return (double)((float)frameCount / frameRate);
    }

    public int getFrameIndex()
    {
        return currentFrame;
    }

    public void setFrameIndex(int i)
    {
        currentFrame = i;
    }

    public void rewind()
    {
        setFrameIndex(0);
    }

    public double getVolume()
    {
        return clipVolume;
    }

    public void setVolume(double d)
    {
        clipVolume = d;
    }

    public int[] getSampleData()
    {
        return getSampleData(0);
    }

    public int[] getSampleData(int i)
    {
        standardize();
        if(i < 0 || i >= nChannels)
            throw new ErrorException("getSamples: Channel number out of range");
        int ai[] = new int[frameCount];
        int j = i * 2;
        for(int k = 0; k < frameCount; k++)
        {
            ai[k] = data[j] << 8 | data[j + 1] & 0xff;
            j += nChannels * 2;
        }

        return ai;
    }

    public void addSampleData(int i)
    {
        standardize();
        if(nChannels == 2)
        {
            addSampleData(i, i);
        } else
        {
            int j = frameCount * 2;
            frameCount++;
            if(j >= data.length)
            {
                byte abyte0[] = new byte[j + 10000];
                if(j > 0)
                    System.arraycopy(data, 0, abyte0, 0, j);
                data = abyte0;
            }
            data[j++] = (byte)(i >> 8);
            data[j++] = (byte)(i & 0xff);
        }
    }

    public void addSampleData(int i, int j)
    {
        standardize();
        if(nChannels != 2)
            throw new ErrorException("addSampleData: Sound is not stereo");
        int k = 2 * frameCount * 2;
        frameCount++;
        if(k >= data.length)
        {
            byte abyte0[] = new byte[k + 10000];
            if(k > 0)
                System.arraycopy(data, 0, abyte0, 0, k);
            data = abyte0;
        }
        data[k++] = (byte)(i >> 8);
        data[k++] = (byte)(i & 0xff);
        data[k++] = (byte)(j >> 8);
        data[k++] = (byte)(j & 0xff);
    }

    public void addSampleData(int ai[])
    {
        standardize();
        if(nChannels == 2)
        {
            addSampleData(ai, ai);
        } else
        {
            int i = frameCount * 2;
            frameCount += ai.length;
            byte abyte0[] = new byte[frameCount * 2];
            if(i > 0)
                System.arraycopy(data, 0, abyte0, 0, i);
            data = abyte0;
            for(int j = 0; j < ai.length; j++)
            {
                data[i++] = (byte)(ai[j] >> 8);
                data[i++] = (byte)(ai[j] & 0xff);
            }

        }
    }

    public void addSampleData(int ai[], int ai1[])
    {
        standardize();
        if(nChannels != 2)
            throw new ErrorException("addSampleData: Sound is not stereo");
        if(ai.length != ai1.length)
            throw new ErrorException("addSampleData: Channels have unequal length");
        int i = 2 * frameCount * 2;
        frameCount += ai.length;
        byte abyte0[] = new byte[2 * frameCount * 2];
        if(i > 0)
            System.arraycopy(data, 0, abyte0, 0, i);
        data = abyte0;
        for(int j = 0; j < ai.length; j++)
        {
            data[i++] = (byte)(ai[j] >> 8);
            data[i++] = (byte)(ai[j] & 0xff);
            data[i++] = (byte)(ai1[j] >> 8);
            data[i++] = (byte)(ai1[j] & 0xff);
        }

    }

    public String toString()
    {
        String s = soundName;
        s = (new StringBuilder()).append(s).append(" (").toString();
        s = (new StringBuilder()).append(s).append(nChannels != 1 ? "stereo" : "mono").toString();
        s = (new StringBuilder()).append(s).append(", ").toString();
        s = (new StringBuilder()).append(s).append((new DecimalFormat("#0.00")).format(getDuration())).toString();
        s = (new StringBuilder()).append(s).append(" sec)").toString();
        return s;
    }

    public static double sampleToIntensity(int i)
    {
        return Math.max(-1D, Math.min(1.0D, (double)i / 32767D));
    }

    public static int intensityToSample(double d)
    {
        return (int)Math.round(Math.max(-1D, Math.min(1.0D, d)) * 32767D);
    }

    protected byte[] getData()
    {
        return data;
    }

    protected AudioFormat getFormat()
    {
        return format;
    }

    protected AudioInputStream getAudioInputStream()
    {
        return new AudioInputStream(new ByteArrayInputStream(data), format, frameCount);
    }

    private void standardize()
    {
        if((double)frameRate == 22050D)
            return;
        double d = 22050D / (double)frameRate;
        int i = (int)((double)frameCount * d);
        byte abyte0[] = new byte[nChannels * i * 2];
        int j = 0;
        for(int k = 0; k < i; k++)
        {
            for(int l = 0; l < nChannels; l++)
            {
                int i1 = (int)((double)k / d) * 2 * nChannels + 2 * l;
                int j1 = data[i1] << 8 | data[i1 + 1] & 0xff;
                abyte0[j++] = (byte)(j1 >> 8);
                abyte0[j++] = (byte)(j1 & 0xff);
            }

        }

        data = abyte0;
        frameCount = i;
        format = new AudioFormat(frameRate, 16, nChannels, true, true);
    }

    private javax.sound.sampled.AudioFileFormat.Type getFormatForFile(String s)
    {
        s = s.toLowerCase();
        if(s.endsWith(".wav"))
            return javax.sound.sampled.AudioFileFormat.Type.WAVE;
        if(s.endsWith(".aif"))
            return javax.sound.sampled.AudioFileFormat.Type.AIFF;
        if(s.endsWith(".aiff"))
            return javax.sound.sampled.AudioFileFormat.Type.AIFF;
        else
            return javax.sound.sampled.AudioFileFormat.Type.AU;
    }

    private void readSound(AudioInputStream audioinputstream)
    {
        int i;
        boolean flag;
        format = audioinputstream.getFormat();
        nChannels = format.getChannels();
        frameRate = format.getFrameRate();
        long l = audioinputstream.getFrameLength();
        if(l > 0x7fffffffL)
            throw new ErrorException("SoundClip: Sound file is too large");
        frameCount = (int)l;
        if(frameCount < 0)
            data = new byte[10000];
        else
            data = new byte[frameCount * nChannels * 2];
        i = 0;
        flag = false;
_L4:
        int j;
        if(flag)
            break; /* Loop/switch isn't completed */
        j = data.length - i;
_L2:
        if(j <= 0)
            break; /* Loop/switch isn't completed */
        int k = audioinputstream.read(data, i, j);
        if(k == 0)
        {
            flag = true;
            break; /* Loop/switch isn't completed */
        }
        try
        {
            i += k;
            j -= k;
        }
        catch(IOException ioexception)
        {
            throw new ErrorException(ioexception);
        }
        if(true) goto _L2; else goto _L1
_L1:
        if(frameCount >= 0)
            flag = true;
        if(!flag)
        {
            byte abyte1[] = new byte[2 * data.length];
            System.arraycopy(data, 0, abyte1, 0, data.length);
            data = abyte1;
        }
        if(true) goto _L4; else goto _L3
_L3:
        if(i < data.length && frameCount < 0)
        {
            byte abyte0[] = new byte[i];
            System.arraycopy(data, 0, abyte0, 0, i);
            data = abyte0;
        }
        if(frameCount < 0)
            frameCount = i / (2 * nChannels);
        return;
    }

    private AudioInputStream convertToPCM(AudioInputStream audioinputstream)
    {
        AudioFormat audioformat = audioinputstream.getFormat();
        audioformat = new AudioFormat(audioformat.getSampleRate(), 16, audioformat.getChannels(), true, true);
        return AudioSystem.getAudioInputStream(audioformat, audioinputstream);
    }

    private AudioInputStream getAudioInputStream(InputStream inputstream)
    {
        if(!(inputstream instanceof BufferedInputStream))
            inputstream = new BufferedInputStream(inputstream);
        return AudioSystem.getAudioInputStream(inputstream);
        Object obj;
        obj;
        throw new ErrorException(createUnsupportedFormatMessage());
        obj;
        throw new ErrorException(((Exception) (obj)));
    }

    private AudioInputStream getAudioInputStream(File file)
    {
        return AudioSystem.getAudioInputStream(file);
        Object obj;
        obj;
        throw new ErrorException(createUnsupportedFormatMessage());
        obj;
        throw new ErrorException(((Exception) (obj)));
    }

    private String createUnsupportedFormatMessage()
    {
        int i = soundName.lastIndexOf('.');
        if(i == -1)
            return "Unsupported audio file format";
        else
            return (new StringBuilder()).append("The ").append(soundName.substring(i + 1)).append(" format is not supported by JavaSound").toString();
    }

    public static final int MONO = 1;
    public static final int STEREO = 2;
    public static final int LEFT = 0;
    public static final int RIGHT = 1;
    public static final double STANDARD_FRAME_RATE = 22050D;
    public static final int MAX_SAMPLE = 32767;
    private static final int BYTES_PER_SAMPLE = 2;
    private static final int BUFFER_INCREMENT = 10000;
    private AudioFormat format;
    private SoundPlayer player;
    private String soundName;
    private double clipVolume;
    private float frameRate;
    private int nChannels;
    private int frameCount;
    private int currentFrame;
    private byte data[];
}
