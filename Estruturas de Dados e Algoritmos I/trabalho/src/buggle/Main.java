package buggle;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;

public class Main {

    public static void main(String args[]) throws IOException {
        Boogle jogo = new Boogle();
        jogo.hashes("allWords.txt");
        ArrayList<String> buggleInput = new ArrayList<>(Files.readAllLines(Paths.get("buggle.txt")));
        jogo.ler_boggle(buggleInput.get(0));
        jogo.FirstSearch();
    }
}


