package stanford.cs106.net;

/*
 * General reusable version of event-driven network manager object.
 *
 * @author Marty Stepp
 * @version 2017/05/26
 * - bug fix for shutdown socket exception
 * @version 2015/05/24
 * - initial version for 15sp
 */

import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.*;

import acm.gui.Event;
import stanford.cs106.io.IORuntimeException;

public class NetworkManager {
	// class constants
	private static final boolean DEBUG = false;
	
	private static final String IP_SERVICE_URL = "http://www.martystepp.com/critters/ip.php";

	private static String hostName = null;
	private static String ipAddress = null;
	private static String ipAddresses = "";
	
	private static class NetworkRunner implements Runnable {
		private ActionListener listener;
		
		public NetworkRunner(ActionListener listener) {
			this.listener = listener;
		}
		
		public void run() {
			if (ipAddress == null) {
				// use host name to name classes sent over the wire
				// also grab IP address(es) of local computer
				try {
					InetAddress localhost = InetAddress.getLocalHost();
					hostName = localhost.getHostName();
					ipAddress = localhost.getHostAddress();

					for (InetAddress addr : InetAddress.getAllByName(hostName)) {
						ipAddresses += " " + addr.getHostAddress();
					}
					ipAddresses = ipAddresses.trim();
				} catch (UnknownHostException e) {
					hostName = "unknown_host";
					ipAddress = ipAddresses = "127.0.0.1";
				} catch (SecurityException e) {
					hostName = "unknown_host";
					ipAddress = ipAddresses = "127.0.0.1";
				}
			}

			try {
				// try to find out my IP address over the network
				URL ipServiceURL = new URL(IP_SERVICE_URL);
				Scanner input = new Scanner(ipServiceURL.openStream());
				if (input.hasNextLine()) {
					ipAddress = input.nextLine().trim();
					ipAddresses = ipAddress + "  (local: " + ipAddresses + ")";
				}
				input.close();
			} catch (Exception e) {
				if (DEBUG) {
					e.printStackTrace();
				}
			}
			
			listener.actionPerformed(null);
		}
	}

	public static void findIPAddress(ActionListener listener) {
		Thread networkThread = new Thread(new NetworkRunner(listener));
		networkThread.start();
	}

	// Returns this computer's host name.
	public static String getHostName() {
		return hostName;
	}

	// Returns this computer's IP address.
	public static String getIpAddress() {
		return ipAddress;
	}

	// Returns this computer's IP address.
	public static String getIpAddresses() {
		return ipAddresses;
	}

	// fields
	private int port;
	private boolean shouldContinue;
	private List<Message> outQueue; // queue of messages to send
	private Thread sendThread;
	private Thread receiveThread;
	private ServerSocket receiveSrvSock;
	private Event<String[]> receive; // events to represent messages received
	private Event<Exception> error; // and errors that occur

	// Constructs a network manager at the given port.
	public NetworkManager(int port) {
		this.port = port;
		shouldContinue = true;
		outQueue = Collections.synchronizedList(new LinkedList<Message>());
		receive = new Event<String[]>();
		error = new Event<Exception>();
	}

	// Returns this network manager's observable error event object,
	// so observers can be notified about errors and respond to them.
	public Event<? extends Exception> getErrorEvent() {
		return error;
	}

	// Returns this network manager's port.
	public int getPort() {
		return port;
	}

	// Returns this network manager's observable receive event object,
	// so observers can be notified when messages are received.
	public Event<String[]> getReceiveEvent() {
		return receive;
	}

	// Sends a message to the given host containing the given strings.
	public void send(String host, String... strings) {
		synchronized (outQueue) {
			Message message = new Message(host, strings);
			outQueue.add(message);
		}
	}
	
	// Sends a message to the given host as an HTTP POST.
	// reference: http://stackoverflow.com/questions/4205980/java-sending-http-parameters-via-post-method-easily
	public String sendHttp(String url, Map<String, Object> params) {
        try {
			StringBuilder postData = new StringBuilder();
	        for (String key : params.keySet()) {
	            if (postData.length() != 0) postData.append('&');
	            postData.append(URLEncoder.encode(key, "UTF-8"));
	            postData.append('=');
	            postData.append(URLEncoder.encode(String.valueOf(params.get(key)), "UTF-8"));
	        }
	        byte[] postDataBytes = postData.toString().getBytes("UTF-8");
			
			URL uurl = new URL(url);
			HttpURLConnection conn = (HttpURLConnection) uurl.openConnection();           
			conn.setDoOutput( true );
			conn.setInstanceFollowRedirects(false);
			conn.setRequestMethod("POST");
			conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded"); 
			conn.setRequestProperty("charset", "utf-8");
			conn.setRequestProperty( "Content-Length", Integer.toString(postDataBytes.length));
	        conn.setDoOutput(true);
			conn.setUseCaches(false);
	        conn.getOutputStream().write(postDataBytes);
	        
	        StringBuilder out = new StringBuilder();
	        Reader in = new BufferedReader(new InputStreamReader(conn.getInputStream(), "UTF-8"));
	        for (int c = in.read(); c != -1; c = in.read()) {
	            out.append((char) c);
	        }
	        return out.toString();
        } catch (IOException ioe) {
        	throw new IORuntimeException(ioe);
        }
	}

	// Notifies this network manager that you want it to shut down
	// and stop listening for messages.
	public void stop() {
		shouldContinue = false;
		try {
			if (receiveSrvSock != null) {
				receiveSrvSock.close();
			}
		} catch (Exception e) {
			// empty
		}

		if (sendThread != null) {
			sendThread.interrupt();
			sendThread = null;
		}
		if (receiveThread != null) {
			receiveThread.interrupt();
			receiveThread = null;
		}
	}

	// Starts listening for messages on the network.
	public void start() throws IOException {
		shouldContinue = true;
		if (sendThread == null) {
			sendThread = new Thread(new SendRunnable());
			receiveThread = new Thread(new ReceiveRunnable());
		}

		sendThread.start();
		receiveThread.start();
	}

	// Listening thread that waits for messages to arrive.
	private class ReceiveRunnable implements Runnable {
		public ReceiveRunnable() throws IOException {
			receiveSrvSock = new ServerSocket(port);
		}

		// Runs the listening thread.
		public void run() {
			try {
				while (shouldContinue && !receiveSrvSock.isClosed()) {
					// wait for a message to arrive
					Socket sock = receiveSrvSock.accept();
					InputStream stream = sock.getInputStream();
					ObjectInputStream ois = new ObjectInputStream(stream);

					// read message
					String[] strings = (String[]) ois.readObject();
					if (DEBUG)
						System.out.println("Received on port " + port + ":\n"
								+ Arrays.toString(strings));

					// notify observers that message has arrived

					receive.fire(strings);
				}
			} catch (SocketException se) {
				// don't notify observers on socket exception; this probably means
				// the program is shutting down
				
				// (empty)
			} catch (Exception e) {
				// notify observers that an error occurred
				// don't notify observers on interrupt exception; this means thread/prog is shutting down
				if (!(e instanceof InterruptedException)) {
					error.fire(e);
				}
//			} catch (Throwable t) {
//				// don't notify observers on Errors
//				// (empty)
			}
		}
	}

	// Sending thread that outputs messages from the output queue to the
	// network.
	private class SendRunnable implements Runnable {
		public void run() {
			try {
				while (shouldContinue) {
					// test-and-test-and-set paradigm
					if (!outQueue.isEmpty()) {
						synchronized (outQueue) {
							if (!outQueue.isEmpty()) {
								// grab message from the queue
								Message message = outQueue.remove(0);
	
								try {
									// send the message
									Socket sock = new Socket(message.host, port);
									OutputStream stream = sock.getOutputStream();
									ObjectOutputStream oos = new ObjectOutputStream(
											stream);
									oos.writeObject(message.strings);
									if (DEBUG)
										System.out.println("Sent on port " + port
												+ ":\n" + message);
									sock.close();
								} catch (IOException e) {
									// notify observers that an error occurred
									error.fire(e);
								}
							}
						}
					}
	
					// wait 1 second between checks for messages so this thread
					// won't drain 100% of the CPU
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						// empty
					}
				}
			} catch (Exception e) {
				if (!(e instanceof InterruptedException)) {
					throw new RuntimeException(e);
				}
			}
		}
	}

	// Convenience class to represent messages about to be sent over the network.
	private static class Message implements Serializable {
		// class constants
		private static final long serialVersionUID = 0;

		// fields
		public String host;

		public String[] strings;

		// Constructs a message to the given host containing the given strings.
		public Message(String host, String[] strings) {
			this.host = host;
			this.strings = strings;
		}

		// Returns debug text about this message.
		public String toString() {
			return "Message{host=" + host + ", strings="
					+ Arrays.toString(strings) + "}";
		}
	}
}
