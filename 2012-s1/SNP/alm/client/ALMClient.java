package alm.client;
import java.io.*;
import java.net.*;
import java.util.*;
import alm.util.*;


public class ALMClient  {

  private static final String host = "localhost";
  private static final int port = 40415;
  private Socket socket;
  private ObjectInputStream in;
  private ObjectOutputStream out;
  private ArrayList<ClientDeail> clientList;
  private PrintWriter writer;
  public ALMClient(String host, int port) {
    // TODO Auto-generated constructor stub
    try {
      socket = new Socket(host,port);
      System.out.println("Client Start ...");
      //      reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      writer = new PrintWriter(socket.getOutputStream(),true);
      writer.println("what is the time?");
      //      String received = reader.readLine();
      //      System.out.println(received);
      int listenPort = socket.getLocalPort();
      socket.close();
      ServerSocket listenerSocket= new ServerSocket(listenPort);
    Socket reciveSocket = listenerSocket.accept();
      while(reciveSocket !=null){
          in = new ObjectInputStream(reciveSocket.getInputStream());
          MassageInfo massageInfo = (MassageInfo) in.readObject();
  //      listener = new BufferedReader(new InputStreamReader(reciveSocket.getInputStream()));
          String timeInfo = massageInfo.timeInfo;
          if(timeInfo !=null){
            clientList = massageInfo.clientList;
            System.out.println(timeInfo);
            for(int i=0;i<clientList.size();i++){
              System.out.println(clientList.get(i));
    }
            int nextPointer = massageInfo.pointer;
            nextPointer++;
            System.out.println(nextPointer);
            System.out.println(clientList.size());
            if(nextPointer <= clientList.size()){
            int nextPort =clientList.get(massageInfo.pointer).clientPort;     
              InetAddress nextInetAddress = clientList.get(massageInfo.pointer).clientAddress;
              Socket nextSocket = new Socket(nextInetAddress,nextPort);
              out = new ObjectOutputStream(nextSocket.getOutputStream());  
              MassageInfo nextMassageInfo = new MassageInfo(nextPointer, timeInfo, clientList);
              while(nextMassageInfo != null){
                out.writeObject(nextMassageInfo);
                out.flush();
              }
  
          }
  }
          
      }
      in.close();
      out.close();
      listenerSocket.close();
      
      //      while (true) {
      //        String line = listener.readLine();
      //        System.out.println(line);

      //        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
      //        String msg = in.readLine();
      //        writer.println(msg);
      //        writer.flush();
      //        if (msg.equals("exit")) {
      //          break;
      //        }
      //      }
      //      writer.close();
      //      reader.close();

    } catch (Exception e) {
      // TODO: handle exception
      System.out.println(e.getMessage());
    }


  }
  public static void main(String[] args) throws Exception {
    new ALMClient(host,port);

  }



}
