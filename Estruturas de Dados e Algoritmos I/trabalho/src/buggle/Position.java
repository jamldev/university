package buggle;

public class Position {
    String s;
    int x, y;
    public Position(String s, int x, int y) {
        this.s=s;
        this.x = x;
        this.y = y;
    }


    @Override
    public String toString() {
        return s + ":" + "(" + x + "," + y + ")";
    }
}
