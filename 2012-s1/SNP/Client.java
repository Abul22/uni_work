import java.io.*;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;
import java.net.*;
public class Client {
  private static final String HOST = "localhost";
  private static final int PORT = 40415;
  public static void main(String[] args) throws Exception {
    SSLSocketFactory sf =
      (SSLSocketFactory)SSLSocketFactory.getDefault();
    SSLSocket s = (SSLSocket)sf.createSocket(HOST,PORT);
    PrintWriter pw = new PrintWriter(s.getOutputStream(),true);
    BufferedReader in = new
      BufferedReader(new InputStreamReader(s.getInputStream()));

    pw.println("From Client :Connection bulit!");
    pw.println("what is the time?");

    String line= null;
    while (((line = in.readLine())!= null)){
      System.out.println(line);
      if(line.equals("exit")){
	break;
      }
    }
    in.close();
    s.close();
  }
}
