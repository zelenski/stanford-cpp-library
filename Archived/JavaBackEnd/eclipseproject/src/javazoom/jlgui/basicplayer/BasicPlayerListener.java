/*
 * BasicPlayerListener.
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

import java.util.Map;

/**
 * This interface defines callbacks methods that will be notified for all
 * registered BasicPlayerListener of BasicPlayer.
 */
@SuppressWarnings("rawtypes")
public interface BasicPlayerListener {
	/**
	 * Open callback, stream is ready to play.
	 * 
	 * properties map includes audio format dependant features such as bitrate,
	 * duration, frequency, channels, number of frames, vbr flag, id3v2/id3v1
	 * (for MP3 only), comments (for Ogg Vorbis), ...
	 * 
	 * @param stream
	 *            could be File, URL or InputStream
	 * @param properties
	 *            audio stream properties.
	 */
	public void opened(Object stream, Map properties);

	/**
	 * Progress callback while playing.
	 * 
	 * This method is called severals time per seconds while playing. properties
	 * map includes audio format features such as instant bitrate, microseconds
	 * position, current frame number, ...
	 * 
	 * @param bytesread
	 *            from encoded stream.
	 * @param microseconds
	 *            elapsed (<b>reseted after a seek !</b>).
	 * @param pcmdata
	 *            PCM samples.
	 * @param properties
	 *            audio stream parameters.
	 */
	public void progress(int bytesread, long microseconds, byte[] pcmdata,
			Map properties);

	/**
	 * Notification callback for basicplayer events such as opened, eom ...
	 * 
	 * @param event
	 */
	public void stateUpdated(BasicPlayerEvent event);

	/**
	 * A handle to the BasicPlayer, plugins may control the player through the
	 * controller (play, stop, ...)
	 * 
	 * @param controller
	 *            : a handle to the player
	 */
	public void setController(BasicController controller);
}
