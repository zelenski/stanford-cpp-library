/*
 * BasicController.
 * 
 * JavaZOOM : jlgui@javazoom.net
 *            http://www.javazoom.net
 *
 *-----------------------------------------------------------------------
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as published
 *   by the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *----------------------------------------------------------------------
 */
package javazoom.jlgui.basicplayer;

import java.io.File;
import java.io.InputStream;
import java.net.URL;

/**
 * This interface defines player controls available.  
 */
public interface BasicController
{
    /**
     * Open inputstream to play.
     * @param in
     * @throws BasicPlayerException
     */
    public void open(InputStream in) throws BasicPlayerException;

    /**
     * Open file to play.
     * @param file
     * @throws BasicPlayerException
     */
    public void open(File file) throws BasicPlayerException;

    /**
     * Open URL to play.
     * @param url
     * @throws BasicPlayerException
     */
    public void open(URL url) throws BasicPlayerException;

    /**
     * Skip bytes.
     * @param bytes
     * @return bytes skipped according to audio frames constraint.
     * @throws BasicPlayerException
     */
    public long seek(long bytes) throws BasicPlayerException;

    /**
     * Start playback.
     * @throws BasicPlayerException
     */
    public void play() throws BasicPlayerException;

    /**
     * Stop playback. 
     * @throws BasicPlayerException
     */
    public void stop() throws BasicPlayerException;

    /**
     * Pause playback. 
     * @throws BasicPlayerException
     */
    public void pause() throws BasicPlayerException;

    /**
     * Resume playback. 
     * @throws BasicPlayerException
     */
    public void resume() throws BasicPlayerException;

    /**
     * Sets Pan (Balance) value.
     * Linear scale : -1.0 <--> +1.0
     * @param pan value from -1.0 to +1.0
     * @throws BasicPlayerException
     */
    public void setPan(double pan) throws BasicPlayerException;

    /**
     * Sets Gain value.
     * Linear scale 0.0  <-->  1.0
     * @param gain value from 0.0 to 1.0
     * @throws BasicPlayerException
     */
    public void setGain(double gain) throws BasicPlayerException;
}
