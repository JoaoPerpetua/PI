import java.io.BufferedReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.OutputStream;
import java.io.PrintStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.lang.*;

//gui 
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;
import java.nio.file.Paths;
//gui

class sender_gui {
    
    public static void main(String srgs[]){
        ServerSocket serverSocket = null;
        Socket socket = null;
        BufferedReader bufferedReader = null;
        PrintStream printStream = null;
        OutputStream time = null;
        
        try{
            
            socket = new Socket("192.168.1.103",8080);
            // ip do recetor
            
            System.out.println("I'm waiting here: " 
                + socket.getLocalPort());
           
                
            OutputStream outputStream = socket.getOutputStream();
            printStream = new PrintStream(outputStream);
            printStream.print("teste");
 
        
	    	
        }catch(IOException e){
            System.out.println(e.toString());
        }finally{
            if(bufferedReader!=null){
                try {
                    bufferedReader.close();
                } catch (IOException ex) {
                    System.out.print(ex.toString());
                }
            }
            
            if(printStream!=null){
                printStream.close();
            }
            
            if(socket!=null){
                try {
                    socket.close();
                } catch (IOException ex) {
                    System.out.print(ex.toString());
                }
            }
        }
    }
}
