package buggle;

public class linkedListIterator<E> implements java.util.Iterator<E>{

    private singleNode<E> current;

    public linkedListIterator(singleNode<E> c){
        current=c;
    }

    public boolean hasNext(){
        return current!=null;
    }
    public E next(){
        if(!hasNext()){
            throw new java.util.NoSuchElementException( );
        }
        E nextItem = current.element();
        current = current.getNext();
        return nextItem;

    }
}