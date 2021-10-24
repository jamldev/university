package buggle;

public abstract class Hashtable<T> {

    public Elemento[] array;
    private static final int SIZE=23;
    private int ocupados=0;

    public Hashtable(){
        array = new Elemento[SIZE];
    }

    public Hashtable(int n){
        array = new Elemento[n];
    }

    public int ocupados(){
        return ocupados;
    }

    public float factorCarga(){
        return (float)(ocupados)/array.length;
    }

    protected abstract int procPos(T s);

    public void alocarTabela(int dim){
        array = new Elemento[dim];
        ocupados=0;
    }

    public void tornarVazia(){
        for(int i=0;i<array.length;i++){
            array[i]=null;
        }
    }

    public T procurar(T x){
        if(array[procPos(x)]!=null && array[procPos(x)].getValor().equals(x) && array[procPos(x)].isActive()){
            return x;
        }

        else{
            return null;
        }
    }

    public void remove(T x){
        if(array[procPos(x)]!=null){
            array[procPos(x)].setState(false);
        }
    }

    public void insere(T x){
        int posiçao=procPos(x);

        if(array[posiçao]!=null && array[posiçao].isActive()){
            return;
        }else{
            array[posiçao]=new Elemento<T>(x);
            ocupados++;
            if(factorCarga()>=0.5){
                rehash();
            }
        }

    }

    public void rehash(){

        Elemento<T>[] temp= array;
        alocarTabela(nextPrime(array.length*2));
        ocupados=0;
        for(int i=0; i<temp.length; i++){
            if(temp[i]!=null && temp[i].isActive()){
                insere(temp[i].getValor());
            }
        }
    }

    public void print(){
        for(int i=0;i<array.length;i++){
            if(array[i]!=null){
                System.out.println(array[i].getValor());
            }else{
                System.out.println("null");
            }

        }

    }

    public boolean isPrime(int num){
        if(num==2){
            return true;
        }
        if(num % 2==0){
            return false;
        }
        for(int i=3; i*i<num; i+=2){
            if(num%i==0){
                return false;
            }
        }
        return true;
    }

    public int nextPrime(int num){
        int result=num;
        while(!isPrime(result)){
            result++;

        }
        return result;
    }

    public int prevPrime(int num){
        int result =num;
        while(!isPrime(num-1)){
            result--;
        }
        return result;
    }


}