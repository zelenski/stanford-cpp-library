// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   SoundClip.java

package acm.util;

import javax.sound.sampled.*;

// Referenced classes of package acm.util:
//            ErrorException, SoundClip

class SoundPlayer
    implements Runnable
{

    public SoundPlayer(SoundClip soundclip)
    {
        soundClip = soundclip;
        oldVolume = 1.0D;
    }

    public void play()
    {
        if(thread != null)
        {
            return;
        } else
        {
            looping = false;
            thread = new Thread(this);
            thread.start();
            return;
        }
    }

    public void loop()
    {
        if(thread != null)
        {
            return;
        } else
        {
            looping = true;
            thread = new Thread(this);
            thread.start();
            return;
        }
    }

    public void stop()
    {
        if(thread == null)
        {
            return;
        } else
        {
            looping = false;
            thread = null;
            return;
        }
    }

    public void run()
    {
        byte abyte0[] = soundClip.getData();
        int i = soundClip.getChannelCount();
        int j = soundClip.getFrameIndex() * i * 2;
        int k = soundClip.getFrameCount() * i * 2;
        SourceDataLine sourcedataline = openSourceDataLine(Math.min(4096, k));
        sourcedataline.start();
        do
        {
            if(thread == null)
                break;
            int j1;
            for(int l = k - j; thread != null && l > 0; l -= j1)
            {
                setLineVolume(sourcedataline, soundClip.getVolume());
                int i1 = Math.min(4096, l);
                j1 = sourcedataline.write(abyte0, j, i1);
                j += j1;
            }

            if(!looping)
                break;
            j = 0;
        } while(true);
        if(thread == null)
            soundClip.setFrameIndex(j / i / 2);
        abyte0 = new byte[400];
        sourcedataline.write(abyte0, 0, abyte0.length);
        sourcedataline.drain();
        sourcedataline.stop();
        sourcedataline.close();
        thread = null;
    }

    private SourceDataLine openSourceDataLine(int i)
    {
        SourceDataLine sourcedataline;
        javax.sound.sampled.AudioFormat audioformat = soundClip.getFormat();
        Class class1 = Class.forName("javax.sound.sampled.SourceDataLine");
        javax.sound.sampled.DataLine.Info info = new javax.sound.sampled.DataLine.Info(class1, audioformat);
        if(!AudioSystem.isLineSupported(info))
            throw new ErrorException("SoundClip: Unsupported data line format");
        sourcedataline = (SourceDataLine)AudioSystem.getLine(info);
        sourcedataline.open(audioformat, i);
        return sourcedataline;
        Exception exception;
        exception;
        throw new ErrorException(exception);
    }

    private void setLineVolume(SourceDataLine sourcedataline, double d)
    {
        if(oldVolume == d)
            return;
        oldVolume = d;
        if(sourcedataline.isControlSupported(javax.sound.sampled.FloatControl.Type.VOLUME))
        {
            FloatControl floatcontrol = (FloatControl)sourcedataline.getControl(javax.sound.sampled.FloatControl.Type.VOLUME);
            floatcontrol.setValue((float)d);
        } else
        if(sourcedataline.isControlSupported(javax.sound.sampled.FloatControl.Type.MASTER_GAIN))
        {
            FloatControl floatcontrol1 = (FloatControl)sourcedataline.getControl(javax.sound.sampled.FloatControl.Type.MASTER_GAIN);
            double d1 = (20D * Math.log(Math.max(d, 9.9999999999999995E-007D))) / Math.log(10D);
            floatcontrol1.setValue((float)d1);
        }
    }

    private static final int PADDING = 400;
    private static final int BYTES_PER_SAMPLE = 2;
    private static final int MAX_BUFFER_SIZE = 4096;
    private static final double EPSILON = 9.9999999999999995E-007D;
    private SoundClip soundClip;
    private Thread thread;
    private boolean looping;
    private double oldVolume;
}
