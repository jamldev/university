package buggle;


public class Elemento<T> {

    private T valor;
    private boolean state;


    Elemento(T x){
        this.valor=x;
        this.state=true;
    }

    public void setValor(T elemento){

        valor=elemento;
    }

    public void setState(boolean valor){

        state=valor;
    }

    public T getValor(){

        return valor;
    }

    public boolean isActive(){

        return state;
    }

}

