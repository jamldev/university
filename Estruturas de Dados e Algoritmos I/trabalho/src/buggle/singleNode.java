package buggle;

public class singleNode<T> {
    T element;
    singleNode<T> next;
    singleNode<T> prev;

    public singleNode(){			//construtor vazio
        this(null);
    }

    public singleNode(T e){			//contrutor com elemento
        element=e;
        next=null;
    }

    public singleNode(T e, singleNode<T> n){		//construtor com elemento e seguinte
        element=e;
        next=n;
    }

    public T element(){		//saber o elemento, !!! utilizar excepçao
        return element;
    }

    public void setElement(T e){   //iniciar elemento
        this.element=e;
    }

    public void setNext(singleNode<T> n){ //iniciar no seguinte
        this.next=n;
    }

    public singleNode<T> getNext(){   //saber no  seguinte
        return next;
    }

    public void setPRev(singleNode<T> p){
        this.prev=p;
    }

    public singleNode<T> getPrev(){
        return prev;
    }

}