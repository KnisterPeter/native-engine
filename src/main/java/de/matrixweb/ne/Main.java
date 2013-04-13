package de.matrixweb.ne;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;

import de.matrixweb.ne.NativeEngine.NativeEngineException;

/**
 * @author markusw
 */
public class Main {

  /**
   * @param args
   * @throws IOException
   * @throws NativeEngineException
   */
  public static void main(final String[] args) throws IOException,
      NativeEngineException {
    final NativeEngine ne = new NativeEngine();
    try {
      String exec = null;
      for (int i = 0, n = args.length; i < n; i++) {
        if ("-s".equals(args[i])) {
          i++;
          System.err.println("Add script: '" + args[i] + "'");
          ne.addScript(readScript(args[i]));
        } else {
          exec = args[i];
        }
      }
      exec = readStdIn(exec);
      System.err.println("Exec: '" + exec + "'");
      String.format("", exec);
      final String result = ne.execute(exec);
      System.out.println(result);
    } finally {
      ne.dispose();
    }
  }

  private static String readScript(final String input) throws IOException {
    final File file = new File(input);
    if (file.exists()) {
      final BufferedReader reader = new BufferedReader(new FileReader(file));
      try {
        final StringBuilder sb = new StringBuilder();
        String line = reader.readLine();
        while (line != null) {
          sb.append(line);
          line = reader.readLine();
        }
        return sb.toString();
      } finally {
        reader.close();
      }
    }
    return input;
  }

  private static String readStdIn(String input) throws IOException {
    if (System.in.available() > 0) {
      final BufferedReader reader = new BufferedReader(new InputStreamReader(
          System.in));
      try {
        input = String.format(input, reader.readLine());
      } finally {
        reader.close();
      }
    }
    return input;
  }

}
