package alm.util;
import java.util.*;
import java.io.*;
import java.net.*;


public class ClientDeail implements Serializable {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;
		public InetAddress clientAddress;
		public int clientPort;
		public UUID id;

		public ClientDeail(UUID id, InetAddress clientAddress, int clientPort) {
			// TODO Auto-generated constructor stub
			this.clientAddress = clientAddress;
			this.clientPort = clientPort;
			this.id = id;
		}

		public String toString() {
			return ("UUID: " + id + "," + "clientAddress: " + clientAddress
					+ "," + "clientPort: " + clientPort);

		}

	}
	