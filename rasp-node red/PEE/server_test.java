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
import java.io.*;
import java.util.*;
//gui

class sender_gui {
    
    public static void main(String srgs[]){
        ServerSocket serverSocket = null;
        Socket socket = null;
        BufferedReader bufferedReader = null;
        PrintStream printStream = null;
        OutputStream time = null;
        
        try{
			String data [] = read_file();
	    System.out.print("Ip:"+data[1]+"Time:"+data[0]+"\n");
            socket = new Socket(data[1],8080);
            // ip do recetor
            
            System.out.println("I'm waiting here: " 
                + socket.getLocalPort());
           
            
            OutputStream outputStream = socket.getOutputStream();
            printStream = new PrintStream(outputStream);
            printStream.print(data[0]);
 
        
	    	
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
    static String[] read_file() throws IOException{
		int count = 0; 
		String ip = "";
		String data []  = new String [2];
		File myObj = new File("userTimes.txt");
		Scanner myReader = new Scanner(myObj);
		while(myReader.hasNextLine())
		{
			if(count == 0)
			{ 
				data[0] = myReader.nextLine();
				count++; 
			}
			else
			{
				data[1] = myReader.nextLine(); 
			}
		}
		myReader.close();
		return data;
	}
}
