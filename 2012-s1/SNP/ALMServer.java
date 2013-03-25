import java.io.*;
import java.net.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;
import java.util.UUID;



public class ALMServer extends ServerSocket {
  private static final int port = 40415;

  public static void main(String[] args) throws IOException {
    new ALMServer(port);
  }

  public ALMServer(int port) throws IOException {
    super(port);
    // TODO Auto-generated constructor stub
    try {
      
      System.out.println("Server waiting for connections at port "
              + port + "...");
      ArrayList<ClientDeail> clientList =new ArrayList<ClientDeail>();
      while (true) {
  Socket clientSocket = accept();
  BufferedReader in = new BufferedReader(new InputStreamReader(
      clientSocket.getInputStream()));
  String line =in.readLine();
  if (line.equals("what is the time?")){
   InetAddress clientAddress =
     clientSocket.getInetAddress();
     int clientPort = clientSocket.getPort();
     UUID id = UUID.randomUUID();
     ClientDeail  clientDeail= new ClientDeail(id,clientAddress,clientPort);
     clientList.add(clientDeail);
     for(int i=0;i<clientList.size();i++){
        System.out.println(clientList.get(i));
     }
   
     Socket sendSocket= new Socket(clientAddress,clientPort);
     Timer timer = new Timer();
     SendTime sendTime = new SendTime(sendSocket);
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
    private PrintWriter out;
    public SendTime(Socket sendSocket) {
      // TODO Auto-generated constructor stub
      this.sendSocket = sendSocket;
    }

    @Override
      public void run() {
  // TODO Auto-generated method stub
      try {
      out = new PrintWriter(sendSocket.getOutputStream(), true);
    } catch (IOException e1) {
      // TODO Auto-generated catch block
      e1.printStackTrace();
    }
  DateFormat dateFormat = new SimpleDateFormat(
      "yyyy-MM-dd   HH:mm:ss ");
  try {

    out.println("\n>>From Server : The local time is "
        + dateFormat.format(new Date()));
  } catch (Exception e) {
    // TODO: handle exception
    System.out.println(e.getMessage());
  }

      }

  }
  
  class ClientDeail{
    private InetAddress clientAddress;
    private int clientPort;
    private UUID id;
    public ClientDeail(UUID id,InetAddress clientAddress, int clientPort) {
    // TODO Auto-generated constructor stub
      this.clientAddress=clientAddress;
      this.clientPort=clientPort;
      this.id=id;
  }
    public String toString(){
    return ("UUID: "+id+","+"clientAddress: "+clientAddress+","+"clientPort: "+clientPort);
      
    }
  
  }

}
