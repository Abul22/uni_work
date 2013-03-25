import java.io.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import javax.net.ssl.*;
import java.net.*;
public class IterativeServer {
  private static final int PORT = 40415;
  public static void main (String[] args) throws Exception {
    try{
      SSLServerSocketFactory ssf =
	(SSLServerSocketFactory)SSLServerSocketFactory.getDefault();
      SSLServerSocket ss = (SSLServerSocket)
	ssf.createServerSocket(PORT);
      System.out.println("Server waiting for connections at port" + PORT +"...");
      SSLSocket s = (SSLSocket) ss.accept();
      Thread first = new Thread(new runnable(s));
      first.start();
      ss.close();
    }catch (IOException e){}
  }
}
class runnable implements Runnable{
  private SSLSocket s;
  public runnable(SSLSocket s){
    this.s=s;
  }
  public void run(){
    try{
      PrintWriter pw = new PrintWriter(s.getOutputStream(),true);
      BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
      pw.println("From Server :Connection bulit!");
      DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd   HH:mm:ss ");
      String line = null;
      while (((line = in.readLine())!= null)) {
	if(line.equals("what is the time?")){

	  pw.println("\n>>From Server : The local time is "+dateFormat.format(new Date()));
	}
	System.out.println(line);
      }
      in.close();
      pw.close();
      return;
    }catch(IOException e){}
  }
}
