package buggle;

public class linearHashTable<T> extends Hashtable<T> {

    linearHashTable(){
        super();
    }

    linearHashTable(int size){

        super(size);
    }

    public int procPos(T s){

        int position = Math.abs(s.hashCode()) % array.length;

        if(array[position]==null){
            return position;
        }else{

            while(!(array[position]==null)){

                if(!array[position].isActive()){
                    return position;
                }

                if(array[position].isActive()){

                    if(array[position].getValor().equals(s)){
                        return position;
                    }

                    if(position==array.length-1){
                        position=0;
                    }else{
                        position++;
                    }
                }
            }
            return position;
        }
    }
}