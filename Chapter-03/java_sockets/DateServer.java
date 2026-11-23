import java.io.*;
import java.net.*;
import java.util.Date;

/**
 * Figure 3.27 (Operating System Concepts, 10th Ed.)
 * Java TCP Date Server on port 6013.
 */
public class DateServer {
    public static void main(String[] args) {
        final int PORT = 6013;

        try (ServerSocket server = new ServerSocket(PORT)) {
            System.out.println("DateServer listening on port " + PORT + "...");

            while (true) {
                // Listen for incoming connection
                Socket client = server.accept();
                System.out.println("Client connected: " + client.getInetAddress());

                // Write current date string to socket
                PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
                pout.println(new Date().toString());

                // Close client socket
                client.close();
            }
        } catch (IOException ioe) {
            System.err.println("Server exception: " + ioe);
        }
    }
}
