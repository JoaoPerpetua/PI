import java.io.BufferedReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.OutputStream;
import java.io.PrintStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.logging.Level;
import java.util.logging.Logger;
//gui 
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;
import java.nio.file.Paths;
import java.io.*;
//gui
class host_gui
{
    public static void main(String srgs[])
    {   
        ServerSocket serverSocket = null;
        Socket socket = null;
        BufferedReader bufferedReader = null;
        PrintStream printStream = null;
        
        try{
            serverSocket = new ServerSocket(41623);
            // Debug de IP
            System.out.println("I'm waiting here: " 
                + serverSocket.getLocalPort());
            // Aceita a socket             
            socket = serverSocket.accept();
            // Debug
            System.out.println("from " + 
                socket.getInetAddress() + ":" + socket.getPort());
            //Inizializar o buffer
            InputStreamReader inputStreamReader = 
                new InputStreamReader(socket.getInputStream());
            bufferedReader = new BufferedReader(inputStreamReader);
            //receber a string
            save_iterat("                              ");
            String line;
            while((line=bufferedReader.readLine()) != null){
				// Print debug
                save_iterat(line+"\n");	//perpi
                System.out.println(line);
            }    
	    	
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
    
    
    static void save_iterat(String iterat) throws IOException{
        String fileName = "iterations.txt";
        PrintWriter pw = new PrintWriter(fileName);
        Files.write(Paths.get(fileName), iterat.getBytes(), StandardOpenOption.CREATE);
        pw.print("\n");
        
    }
}
