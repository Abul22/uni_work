package alm.server;
import java.io.*;
import java.net.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.*;
import alm.util.*;

public class ALMServer extends ServerSocket  {

	private static final int port = 40415;

	public static void main(String[] args) throws IOException {
		new ALMServer(port);
	}

	public ALMServer(int port) throws IOException {
		super(port);
		// TODO Auto-generated constructor stub
		try {

			System.out.println("Server waiting for connections at port " + port
					+ "...");
			ArrayList<ClientDeail> clientList = new ArrayList<ClientDeail>();
			int pointer = 0;
			while (true) {
				Socket clientSocket = accept();
				BufferedReader in = new BufferedReader(new InputStreamReader(
						clientSocket.getInputStream()));
				String line = in.readLine();
				if (line.equals("what is the time?")) {
					InetAddress clientAddress = clientSocket.getInetAddress();
					int clientPort = clientSocket.getPort();
					UUID id = UUID.randomUUID();
					ClientDeail clientDeail = new ClientDeail(id,
							clientAddress, clientPort);
					clientList.add(clientDeail);
					pointer++;
					InetAddress firstInetAddress = clientList.get(0).clientAddress;
					int firstPort = clientList.get(0).clientPort;
					Socket sendSocket = new Socket(firstInetAddress, firstPort);
					Timer timer = new Timer();
					SendTime sendTime = new SendTime(sendSocket, clientList,pointer);
					timer.schedule(sendTime, 1000, 5000);
				}
			}
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println(e.getMessage());
		} finally {
			close();
		}
	}

	class SendTime extends TimerTask {

		private Socket sendSocket;

		private ObjectOutputStream out;
		private ArrayList<ClientDeail> clientList;

		private int pointer;

		public SendTime(Socket sendSocket, ArrayList<ClientDeail> clientList, int pointer) {
			// TODO Auto-generated constructor stub
			this.sendSocket = sendSocket;
			this.clientList = clientList;
			this.pointer=pointer;
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			try {
				
				out = new ObjectOutputStream(sendSocket.getOutputStream());

			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}

			try {
				DateFormat dateFormat = new SimpleDateFormat(
						"yyyy-MM-dd   HH:mm:ss ");
				String timeInfo = ">>From Server : The local time is "+ dateFormat.format(new Date());
				MassageInfo massageInfo = new MassageInfo(pointer,timeInfo, clientList);
				out.writeObject(massageInfo);
				// out.println("\n>>From Server : The local time is "
				// + dateFormat.format(new Date()));
			} catch (Exception e) {
				// TODO: handle exception
				System.out.println(e.getMessage());
			}

		}

	}


}
