import java.io.*;
import java.net.*;

/**
 * Figure 3.28 (Operating System Concepts, 10th Ed.)
 * Java TCP Date Client connecting to DateServer on port 6013.
 */
public class DateClient {
    public static void main(String[] args) {
        String host = (args.length > 0) ? args[0] : "127.0.0.1";
        final int PORT = 6013;

        try (Socket sock = new Socket(host, PORT);
             InputStream in = sock.getInputStream();
             BufferedReader bin = new BufferedReader(new InputStreamReader(in))) {

            String line;
            while ((line = bin.readLine()) != null) {
                System.out.println("Server Date: " + line);
            }
        } catch (IOException ioe) {
            System.err.println("Connection error: " + ioe.getMessage());
            System.err.println("Ensure DateServer is running before starting DateClient.");
        }
    }
}
