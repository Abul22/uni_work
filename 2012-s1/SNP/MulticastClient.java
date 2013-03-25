import java.net.*;
import java.io.*;

public class MulticastClient{
  public static void main(String[] args)throws Exception {

    InetAddress group=InetAddress.getByName("224.0.0.2");
    int port = 40415;
    MulticastSocket ms = null;


    try{
      ms = new MulticastSocket(port);
      ms.joinGroup(group);
      byte[] buffer = new byte[256];
      System.out.println("Client Start ...");
      while(true){
	DatagramPacket dpr = new DatagramPacket(buffer,buffer.length);
	ms.receive(dpr);
	String msg = new String(dpr.getData(),0,dpr.getLength());
	System.out.println(msg);
	if(msg.equals("Do you want know the time?")){
	  System.out.println("Enter yes or no");
	  BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
	  String input = in.readLine();
	  byte[] buf = input.getBytes();
	  DatagramPacket dps = new DatagramPacket(buf,buf.length,group,port);
	  ms.send(dps);
	}
      }
    }catch(Exception e){
      e.printStackTrace();
    }finally{
      if(ms!=null){
	try{
	  ms.leaveGroup(group);
	  ms.close();
	}catch(Exception e2){}
      }
    }
  }
}
