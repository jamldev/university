package buggle;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;

public class Boogle {


    static final int M = 4, N = 4;
    char boggle[][] = new char[M][N];
    linearHashTable<String> hashTabled = new linearHashTable<>();
    linearHashTable<String> hashTablep = new linearHashTable<>();


    public void ler_boggle(String s){

        int g = 0;
        for (int y = 0; y < M; y++) {
            for (int x = 0; x < N; x++) {
                boggle[y][x] = s.charAt(g);
                g++;
            }
        }
    }

    public void hashes(String x) throws IOException {

        ArrayList<String> lines = new ArrayList<>(Files.readAllLines(Paths.get(x)));

        for (String palavra:lines) {
            hashTabled.insere(palavra);
        }
        for (String palavra:lines) {
            for (int i=1; i<palavra.length(); i++) {
                hashTablep.insere(palavra.substring(0,i));
            }
        }
    }



    public boolean DictionaryCheck(String word){

        if (hashTabled.procurar(word) == null) {
            return false;
        }else{
            return true;
        }

    }

    public void NextSearch(boolean check[][], int i, int j, String word, linkedList<Position> ll){
        if(i>=0 && i<M && j>=0 && j<M && !check[i][j]){

            check[i][j] = true;
            word += boggle[i][j];
            ll.add(new Position(Character.toString(boggle[i][j]).toUpperCase(), i, j));

            if (DictionaryCheck(word)) {
                System.out.print(word.toUpperCase() +" ");
                System.out.println(ll);
            }
            if(hashTablep.procurar(word)!=null){
                for (int row = i - 1; row <= i + 1; row++){
                    for (int col = j - 1; col <= j + 1; col++){
                        NextSearch(check, row, col, word, ll);
                    }
                }
            }
            ll.remove(ll.size()-1);
            check[i][j] = false;
        }


    }

    public void FirstSearch(){

        linkedList<Position> l2 = new linkedList<>();

        boolean visited[][] = new boolean[M][N];
        String str = "";

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                NextSearch(visited, i, j, str, l2);
    }
} 