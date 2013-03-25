import java.io.*;
import java.net.*;



// public class ALMClient {
// 	private static final String HOST = "localhost";
//     private static final int PORT = 40415;
// 	public static void main(String[] args) throws Exception {
// 		// int serverPort = 40415;
// 		// String serverName = "locathost";
// 		Socket clientSocket =null;
// 		try{
// 		clientSocket = new Socket(HOST,PORT);
// 		System.out.println("Client Start ...");
// 		PrintWriter pw = new PrintWriter(clientSocket.getOutputStream(),true);
// 		BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
// 		BufferedReader buffer = new BufferedReader(new InputStreamReader(System.in));
// 		pw.println("what is the time?");
// 		String msg = null;
// 		while (((msg = in.readLine())!= null)) {
// 			System.out.println(msg);
// 		}
// 		pw.println(buffer.readLine());
// 		in.close();
// 		buffer.close();
// 		}catch(UnknownHostException e){
// 			e.printStackTrace();
// 		}catch(IOException e){
// 			e.printStackTrace();
// 		}finally{
// 			if(clientSocket!=null){
// 				try{
// 					clientSocket.close();
// 				}catch(IOException e){}
// 			}
// 		}

// 	}


// }
public class ALMClient {
	private static final String host = "localhost";
    private static final int port = 40415;
    private Socket socket;
    private BufferedReader reader;
    private BufferedReader listener;
	private PrintWriter writer;
	public ALMClient(String host, int port) {
		// TODO Auto-generated constructor stub
		try {
			socket = new Socket(host,port);
			System.out.println("Client Start ...");
//			reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			writer = new PrintWriter(socket.getOutputStream(),true);
			writer.println("what is the time?");
//			String received = reader.readLine();
//			System.out.println(received);
			int listenPort = socket.getLocalPort();
			socket.close();
			ServerSocket listenerSocket= new ServerSocket(listenPort);
			Socket reciveSocket = listenerSocket.accept();
//			writer = new PrintWriter(reciveSocket.getOutputStream(),true);
//			writer.println("what is the time?");
			listener = new BufferedReader(new InputStreamReader(reciveSocket.getInputStream()));
			String line = null;
			while((line = listener.readLine()) !=null){
			System.out.println(line);
			}
//			while (true) {
//				String line = listener.readLine();
//				System.out.println(line);

//				BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
//				String msg = in.readLine();
//				writer.println(msg);
//				writer.flush();
//				if (msg.equals("exit")) {
//					break;
//				}
//			}
//			writer.close();
//			reader.close();

		} catch (Exception e) {
			// TODO: handle exception
			System.out.println(e.getMessage());
		}

	}
	public static void main(String[] args) throws Exception {
		new ALMClient(host,port);

	}


}
