package buggle;

import java.util.Iterator;


public class linkedList<T> implements Iterable<T>{

    private singleNode<T> header;
    private int theSize;

    public linkedList(){
        header=new singleNode<T>();
        theSize=0;
    }


    public java.util.Iterator<T> iterator(){
        return new linkedListIterator<T>(header.next);
    }


    public int size(){
        return theSize;
    }


    public boolean isEmpty(){
        return theSize==0;
    }


    public singleNode<T> header(){
        return header;
    }

    public void add(T x){

        if(theSize==0){
            this.add(header,x);
        }
        else{
            this.add(this.getNode(theSize-1), x);
        }
    }


    public void add(int ind, T x){
        this.add(this.getNode(ind-1),x);
    }

    void add(singleNode<T> prev, T x){
        singleNode<T> newNode = new singleNode<T>(x,prev.getNext());
        prev.setNext(newNode);
        newNode.setPRev(prev);
        theSize++;
    }

    public singleNode<T> findPrevious(T x) {
        singleNode<T> p=header();
        for(T v:this){
            if (v.equals(x)) return p; else p=p.getNext();
        }
        throw new java.util.NoSuchElementException( );
    }


    public String toString(){
        StringBuilder sb = new StringBuilder( "" );
        int x0 = 0;
        for( T x : this ){
            if(x0==0){
                sb.append( x );
            }else{
                sb.append("->"+ x );
            }
            x0++;
        }

        return new String( sb );
    }

    public void set(int indx, T x){
        getNode(indx).setElement(x);
    }

    public T get(int ind){
        if (ind >=0 && ind<=size()-1){
            return getNode(ind).element();
        }else{
            throw new IndexOutOfBoundsException( "getNodeindex: " + ind + "; size: " + size( ) );
        }
    }

    public singleNode<T> getNode(int i){
        int ind=-1;
        singleNode<T> s=header();
        while(ind++<i){
            s=s.getNext();
        }
        return s;
    }

    public boolean contains(T x){
        Iterator<T> it = iterator();
        while(it.hasNext()){
            if(it.next().equals(x)){
                return true;
            }
        }
        return false;
    }

    void remove(singleNode<T> prev){

        singleNode<T> node = header;
        while(node!=null){
            if(node==prev){
                node.setNext(node.getNext().getNext());
            }
            node = node.getNext();
        }
        theSize--;
    }

    public void remove(int ind){
        singleNode<T> node = header;
        for(int j=-1; j<ind-1; j++){
            node = node.getNext();
        }
        remove(node);

    }

}
