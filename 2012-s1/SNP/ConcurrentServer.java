import java.io.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import javax.net.ssl.*;
import java.net.*;
public class ConcurrentServer {
  private static final int PORT = 40415;
  public static void main (String[] args) throws IOException {

    SSLServerSocketFactory ssf =
      (SSLServerSocketFactory)SSLServerSocketFactory.getDefault();
    SSLServerSocket ss = (SSLServerSocket)
      ssf.createServerSocket(PORT);
    System.out.println("Server waiting for connections at port" + PORT +"...");
    while(true){
      SSLSocket s = (SSLSocket) ss.accept();
      for(int i = 0;i<10;i++){
	runnable(s);
      }
    }
  }
  private static void  runnable(final SSLSocket s)throws IOException{
    new Thread(new Runnable(){
      public void run(){
	BufferedReader in = null;
	PrintWriter out =null;
	DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd   HH:mm:ss ");
	try{
	  in = new BufferedReader(new InputStreamReader(s.getInputStream()));
	  out= new PrintWriter(s.getOutputStream());

	  while(true){
	    String massage = in.readLine();
	    System.out.println(massage);
	    if (massage.equals("what is the time?")) {
	      out.println("\n>>From Server : The local time is "+dateFormat.format(new Date()));
	      out.flush();
	    }
	    else if(massage.equals("exit")){
	      break;
	    }
	  }
	}catch(IOException e){
	  e.printStackTrace();
	}finally{
	  try{
	    in.close();
	  }catch(Exception e){}
	  try{
	    out.close();
	  }catch(Exception e){}
	  try{
	    s.close();
	  }catch(Exception e){}
	}
      }
    }).start();

  }
}
