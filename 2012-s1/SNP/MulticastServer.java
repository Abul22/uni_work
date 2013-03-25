import java.net.*;
import java.io.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MulticastServer{
  public static void main(String[] args) throws Exception {

    InetAddress group=InetAddress.getByName("224.0.0.2");
    int port = 40415;
    MulticastSocket ms = null;
    try{
      ms = new MulticastSocket(port);
      ms.joinGroup(group);
      System.out.println("Server start ...");
      String ask = "Do you want know the time?";
      byte [] bf = ask.getBytes();
      DatagramPacket dp = new DatagramPacket(bf,bf.length,group,port);
      ms.send(dp);

      while(true){
	byte [] buff = new byte[256];
	DatagramPacket dpr = new DatagramPacket(buff,buff.length);
	ms.receive(dpr);
	String query = new String(dpr.getData(),0,dpr.getLength());
	// System.out.println("From Clinet"+ query);

	if(query.equals("yes")){

	  MyThread thread = new MyThread(ms, group,port);
	  thread.start();

	}
	else if(query.equals("no")){
	  break;
	}

      }
    }catch(Exception e){
      e.printStackTrace();
    }finally{
      try{
	if(ms!=null){
	  ms.leaveGroup(group);
	  ms.close();
	}
      }catch (Exception e2){}
    }
  }



}
class MyThread extends Thread{
  private MulticastSocket ms;
  private InetAddress group;
  private int port;

  public MyThread(MulticastSocket ms,InetAddress group,int port) {
    // TODO Auto-generated constructor stub
    this.ms = ms;
    this.group = group;
    this.port =port;


  }
  public void run(){

    DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd   HH:mm:ss ");
    String msg="From Server : The local time is "+dateFormat.format(new Date());
    byte[] buffer = msg.getBytes();
    DatagramPacket dps = new DatagramPacket(buffer,buffer.length,group,port);
    try {
      ms.send(dps);
    } catch (IOException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
    System.out.println("Send the time to"+group+":"+port);
    try {
      Thread.sleep(5000);
    } catch (InterruptedException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
  }
}
