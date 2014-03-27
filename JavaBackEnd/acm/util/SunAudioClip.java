// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.applet.AudioClip;
import java.io.InputStream;
import java.lang.reflect.*;

class SunAudioClip
    implements AudioClip
{

    public SunAudioClip(InputStream inputstream)
    {
        if(!initialized)
        {
            initStaticData();
            initialized = true;
        }
        try
        {
            Object aobj[] = {
                inputstream
            };
            Object obj = audioDataConstructor.newInstance(aobj);
            audioData = getData.invoke(obj, new Object[0]);
            player = audioPlayerClass.getField("player").get(null);
            Class aclass[] = {
                Class.forName("java.io.InputStream")
            };
            audioPlayerStart = player.getClass().getMethod("start", aclass);
            audioPlayerStop = player.getClass().getMethod("stop", aclass);
        }
        catch(Exception exception) { }
    }

    public void play()
    {
        try
        {
            Object aobj[] = {
                audioData
            };
            audioDataStream = audioDataStreamConstructor.newInstance(aobj);
            aobj[0] = audioDataStream;
            audioPlayerStart.invoke(player, aobj);
        }
        catch(Exception exception) { }
    }

    public void loop()
    {
        try
        {
            Object aobj[] = {
                audioData
            };
            continuousAudioDataStream = continuousAudioDataStreamConstructor.newInstance(aobj);
            aobj[0] = continuousAudioDataStream;
            audioPlayerStart.invoke(player, aobj);
        }
        catch(Exception exception) { }
    }

    public void stop()
    {
        try
        {
            Object aobj[] = new Object[1];
            if(continuousAudioDataStream != null)
            {
                aobj[0] = audioDataStream;
                audioPlayerStop.invoke(player, aobj);
            }
            if(audioDataStream != null)
            {
                aobj[0] = continuousAudioDataStream;
                audioPlayerStop.invoke(player, aobj);
            }
        }
        catch(Exception exception) { }
    }

    private static void initStaticData()
    {
        try
        {
            audioPlayerClass = Class.forName("sun.audio.AudioPlayer");
            audioStreamClass = Class.forName("sun.audio.AudioStream");
            audioDataClass = Class.forName("sun.audio.AudioData");
            audioDataStreamClass = Class.forName("sun.audio.AudioDataStream");
            continuousAudioDataStreamClass = Class.forName("sun.audio.ContinuousAudioDataStream");
            Class aclass[] = {
                Class.forName("java.io.InputStream")
            };
            audioDataConstructor = audioStreamClass.getConstructor(aclass);
            getData = audioStreamClass.getMethod("getData", new Class[0]);
            Class aclass1[] = {
                audioDataClass
            };
            audioDataStreamConstructor = audioDataStreamClass.getConstructor(aclass1);
            continuousAudioDataStreamConstructor = continuousAudioDataStreamClass.getConstructor(aclass1);
        }
        catch(Exception exception) { }
    }

    private static boolean initialized;
    private static Class audioPlayerClass;
    private static Class audioStreamClass;
    private static Class audioDataClass;
    private static Class audioDataStreamClass;
    private static Class continuousAudioDataStreamClass;
    private static Constructor audioDataConstructor;
    private static Constructor audioDataStreamConstructor;
    private static Constructor continuousAudioDataStreamConstructor;
    private static Method getData;
    private Object player;
    private Object audioData;
    private Object audioDataStream;
    private Object continuousAudioDataStream;
    private Method audioPlayerStart;
    private Method audioPlayerStop;
}
