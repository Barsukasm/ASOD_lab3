#include <iostream>
#define EMPTY_TREE 0
#define KEY_DOES_NOT_EXIST 1
#define ITERATOR_END 2
#define ONE_ELEMENT_TREE 3

using namespace std;



template <class Data,class Key> class TwoThreeTree{

    class Element{
    public:
        int type;//0 - Leaf, 1 - Node
    };

    class Leaf: public Element{
    public:
        Data t;
        Key k;
        Leaf(Data data, Key key);
    };

    class Node: public Element{
    public:
        Element *son1, *son2, *son3;
        Key low2,low3;
        Node();
    };

public:
    class Iterator{
    private:
        TwoThreeTree *tree;
        Element *cur;
    public:
        Iterator(TwoThreeTree<Data,Key> &tree);//конструктор
        bool first();//установка на первый узел в дереве с минимальным ключом
        bool last();//установка на последний узел в дереве с максимальным ключом
        bool status();//проверка состояния итератора
        Data& operator*();//доступ по чтению и записи к данным текущего узла в дереве
        bool operator++(int);//переход к следующему по значению ключа узлу в дереве
        bool operator--(int);//переход к предыдущему по значению ключа узлу в дереве
    };

    friend class Iterator;

public://методы интерфейса
    int getSize();//опрос размера дерева
    void clear();//очистка дерева
    bool isEmpty();//проверка на пустоту
    Data& read(Key key);//доступ к данным по ключу
    bool insert(Data data, Key key);//включение данных с заданным ключом
    bool remove(Key key);//удаление данных с заданным ключом

    void print();//вывод структуры дерева на экран. Примечание: метод не расчитан на вывод дерева, состоящего из одного элемента
    int getOperations();//число просмотренных операций узлов дерева

    TwoThreeTree();//конструктор по умолчанию
    ~TwoThreeTree();//деструктор

    int reCount();//пересчитать длину дерева

private:
    Node *root;//указатель на корень
    int length;//длина дерева

    int operations;//число просмотров
    int cnt;//для пересчета

    //вспомогательные методы
    bool insert1(Element *t,Leaf *&lt,Element *&tup, Key &lup);
    bool remove1(Element *t, Key k, Element *&tlow1, bool &one_son);
    Data& read1(Element *t,Key key);
    void clear1(Element *t);
    void show(Element *t, int level);
    Leaf* toFirst(Element *t);
    Leaf* toLast(Element *t);
    Element* successor(Element *t, Key key);
    Element* predecessor(Element *t, Key key);
    void count(Element *t);
};

//пересчет длинны дерева
template <class Data,class Key>
int TwoThreeTree<Data,Key>::reCount() {
    cnt=0;
    count(root);
    return cnt;
}

template <class Data, class Key>
void TwoThreeTree<Data,Key>::count(TwoThreeTree<Data, Key>::Element *t) {
    if(t->type==1){
        if(((Node*)t)->son1!=NULL) count(((Node*)t)->son1);
        if(((Node*)t)->son2!=NULL) count(((Node*)t)->son2);
        if(((Node*)t)->son3!=NULL) count(((Node*)t)->son3);
    } else{
        cnt++;
    }
}


//Методы класса TwoThreeTree
template <class Data,class Key>
int TwoThreeTree<Data,Key>::getSize() {
    return length;
}

template <class Data,class Key>
bool TwoThreeTree<Data,Key>::isEmpty() {
    return (length == 0 && root == NULL);
}

template <class Data,class Key>
void TwoThreeTree<Data,Key>::clear() {
    if(root!=NULL){
        clear1(root);
        root=NULL;
    }
    length=0;
}

template <class Data, class Key>
void TwoThreeTree<Data,Key>::clear1(TwoThreeTree<Data, Key>::Element *t) {
    if(t->type==0){
        delete t;
    } else{
        if(((Node*)t)->son1!=NULL) clear1(((Node*)t)->son1);
        if(((Node*)t)->son2!=NULL) clear1(((Node*)t)->son2);
        if(((Node*)t)->son3!=NULL) clear1(((Node*)t)->son3);
        delete t;
    }
}


template <class Data,class Key>
Data& TwoThreeTree<Data,Key>::read(Key key) {

    operations=1;
    if(root==NULL){
        throw EMPTY_TREE;
    }
    if(root->son2==NULL){
        if(((Leaf*)root->son1)->k==key){
            return ((Leaf*)root->son1)->t;
        } else throw KEY_DOES_NOT_EXIST;
    }
    return read1(root,key);
}

template <class Data,class Key>
Data& TwoThreeTree<Data,Key>::read1(TwoThreeTree<Data, Key>::Element *t, Key key) {
    Element *w;
    if(t==NULL){
        throw EMPTY_TREE;
    }


    if(t->type==0){
        if(((Leaf*)t)->k==key){
            operations++;
            return ((Leaf*)t)->t;
        } else throw KEY_DOES_NOT_EXIST;
    }
    if(key<((Node*)t)->low2){
        w=((Node*)t)->son1;
    } else if(((Node*)t)->son3==NULL||(((Node*)t)->son3!=NULL&&key<((Node*)t)->low3)){
        w=((Node*)t)->son2;
    } else{
        w=((Node*)t)->son3;
    }
    operations++;
    return read1(w,key);

}

template <class Data,class Key>
bool TwoThreeTree<Data,Key>::insert(Data data, Key key) {
    Element *tbk;
    Key lbk;
    operations=0;
    Leaf *lt = new Leaf(data,key);
    if (root==NULL){
        root=new Node();
        root->son1=lt;
        root->son2=root->son3=NULL;
        operations++;
        length++;
        return true;
    }

    if(root->son2==NULL ){
        if(((Leaf*)root->son1)->k<key){
            root->son2=lt;
            root->low2=key;
            operations++;
            length++;
            return true;
        } else if(((Leaf*)root->son1)->k>key){
            root->son2=root->son1;
            root->low2=((Leaf*)root->son1)->k;
            root->son1=lt;
            operations++;
            length++;
            return true;
        } else{
            delete lt;
            return false;
        }
    }
    bool inserted=insert1(root,lt,tbk,lbk);
    if(!inserted){
        delete lt;
        return false;
    }
    if(tbk!=NULL){
        Node *temp = root;
        root = new Node();
        root->son1=temp;
        root->son2=tbk;
        root->low2=lbk;
        root->son3=NULL;
    }
    operations++;
    length++;
    return true;
}

template <class Data,class Key>
bool TwoThreeTree<Data,Key>::insert1(TwoThreeTree<Data, Key>::Element *t,TwoThreeTree<Data, Key>::Leaf *&lt, TwoThreeTree<Data, Key>::Element *&tup, Key &lup) {
    tup=NULL;
    Element *w, *tbk;
    Key lbk;
    int child=0;
    operations++;
    if(t->type==0){
        if (((Leaf*)t)->k==((Leaf*)lt)->k) return false;
        else{
            tup=lt;
            if(((Leaf*)t)->k<((Leaf*)lt)->k){
                lup=((Leaf*)lt)->k;
            } else{
                lup=((Leaf*)t)->k;
                Key temp = ((Leaf*)t)->k;
                ((Leaf*)t)->k=((Leaf*)lt)->k;
                ((Leaf*)lt)->k=temp;
                Data temp2 = ((Leaf*)t)->t;
                ((Leaf*)t)->t = ((Leaf*)lt)->t;
                ((Leaf*)lt)->t=temp2;
            }
            return true;
        }
    }
    //t - внутренний узел
    if(((Leaf*)lt)->k<((Node*)t)->low2){
        child=1;
        w=((Node*)t)->son1;
    } else if (((Node*)t)->son3==NULL||(((Node*)t)->son3!=NULL&&((Leaf*)lt)->k < ((Node*)t)->low3)){
        child=2;
        w=((Node*)t)->son2;
    } else{
        child=3;
        w=((Node*)t)->son3;
    }
    bool inserted = insert1(w,lt,tbk,lbk);
    if(inserted){
        if(tbk!=NULL){
            if(((Node*)t)->son3==NULL){//узел имел 2-х сыновей
                if(child==2){
                    ((Node*)t)->son3=tbk;
                    ((Node*)t)->low3=lbk;
                } else{
                    ((Node*)t)->son3=((Node*)t)->son2;
                    ((Node*)t)->low3=((Node*)t)->low2;
                    ((Node*)t)->son2=tbk;
                    ((Node*)t)->low2=lbk;
                }
            }else{//узел имел 3-х сыновей
                tup=new Node();
                if(child==3){//выполнялась вставка в третье поддерево
                    ((Node*)tup)->son1=((Node*)t)->son3;
                    ((Node*)tup)->son2=tbk;
                    ((Node*)tup)->son3=NULL;
                    ((Node*)tup)->low2=lbk;
                    ((Node*)t)->son3=NULL;
                    lup=((Node*)t)->low3;
                }else{//выполнялась вставка в 1-е или 2-е поддерево
                    ((Node*)tup)->son2=((Node*)t)->son3;
                    ((Node*)tup)->low2=((Node*)t)->low3;
                    ((Node*)tup)->son3=NULL;
                    if(child==2){//выполнялась вставка во 2-е поддерево
                        ((Node*)tup)->son1=tbk;
                        lup=lbk;
                    }
                    if(child==1){//выполнялась вставка в 1-е поддерево
                        ((Node*)tup)->son1=((Node*)t)->son2;
                        ((Node*)t)->son2=tbk;
                        lup=((Node*)t)->low2;
                        ((Node*)t)->low2=lbk;
                    }
                }
                ((Node*)t)->son3=NULL;
            }
        }
    }
    return inserted;
}


template <class Data,class Key>
bool TwoThreeTree<Data,Key>::remove(Key key) {
    Element *tmin;
    bool one;
    operations=1;
    if(root==NULL){
        length=0;
        return false;
    }
    if(root->son2==NULL){
        if(((Leaf*)root->son1)->k==key){
            delete ((Leaf*)root->son1);
            delete root;
            root=NULL;
            length=0;
            operations++;
            return true;
        } else{
            return false;
        }
    }
    bool deleted=remove1(root,key,tmin,one);
    if(deleted){
        if(one){
            if(root->son1->type==1){
                Node *t= ((Node*)root->son1);
                delete root;
                root=t;
            }
        }
        operations++;
        length--;
    }
    return deleted;
}

template <class Data,class Key>
bool TwoThreeTree<Data,Key>::remove1(TwoThreeTree<Data, Key>::Element *t, Key k,
                                     TwoThreeTree<Data, Key>::Element *&tlow1, bool &one_son) {
    int child=0;
    Element *w, *tlow1_bk;
    bool one_son_bk;
    operations++;
    tlow1=NULL;
    one_son=false;

    if(((Node*)t)->son1->type==0){
        if(((Leaf*)((Node*)t)->son1)->k==k){
            delete (((Node*)t)->son1);
            ((Node*)t)->son1=((Node*)t)->son2;
            ((Node*)t)->son2=((Node*)t)->son3;
            ((Node*)t)->son3=NULL;
            ((Node*)t)->low2=((Node*)t)->low3;
        } else {
            if(((Leaf*)((Node*)t)->son2)->k==k){
                delete (((Node*)t)->son2);
                ((Node*)t)->son2=((Node*)t)->son3;
                ((Node*)t)->son3=NULL;
                ((Node*)t)->low2=((Node*)t)->low3;
            } else{
                if(((Node*)t)->son3!=NULL&&((Leaf*)((Node*)t)->son3)->k==k){
                       delete (((Node*)t)->son3);
                        ((Node*)t)->son3=NULL;
                } else return false;
            }
        }
        tlow1=((Node*)t)->son1;
        if(((Node*)t)->son2==NULL){
            one_son=true;
        }
        return true;
    }
    if(k<((Node*)t)->low2){
        child=1;
        w=((Node*)t)->son1;
    } else if(((Node*)t)->son3==NULL||k<((Node*)t)->low3){
        child=2;
        w=((Node*)t)->son2;
    } else{
        child=3;
        w=((Node*)t)->son3;
    }
    if (!remove1(w,k,tlow1_bk,one_son_bk)){
        return false;
    }
    tlow1=tlow1_bk;
    if(tlow1_bk!=NULL){
        if(child==2){
            ((Node*)t)->low2= ((Leaf*)tlow1_bk)->k;
            tlow1=NULL;
        }
        if(child==3){
            ((Node*)t)->low3= ((Leaf*)tlow1_bk)->k;
            tlow1=NULL;
        }
    }
    if(!one_son_bk){
        return true;
    }
    if(child==1){
        Node *y= ((Node*)((Node*)t)->son2);
        if(y->son3!=NULL){
            ((Node*)w)->son2=y->son1;
            ((Node*)w)->low2=((Node*)t)->low2;
            ((Node*)t)->low2=y->low2;
            y->son1=y->son2;
            y->son2=y->son3;
            y->low2=y->low3;
            y->son3=NULL;
        } else{
            y->son3=y->son2;
            y->low3=y->low2;
            y->son2=y->son1;
            y->low2=((Node*)t)->low2;
            y->son1=((Node*)w)->son1;
            delete w;
            ((Node*)t)->son1=((Node*)t)->son2;
            ((Node*)t)->son2=((Node*)t)->son3;
            ((Node*)t)->low2=((Node*)t)->low3;
            ((Node*)t)->son3=NULL;
            if(((Node*)t)->son2==NULL){
                one_son=true;
            }
        }
        return true;
    }
    if(child==2){
        Node *y= ((Node*)((Node*)t)->son1);
        if(y->son3!=NULL){
            ((Node*)w)->son2=((Node*)w)->son1;
            ((Node*)w)->low2=((Node*)t)->low2;
            ((Node*)w)->son1=y->son3;
            y->son3=NULL;
            ((Node*)t)->low2=y->low3;
            return true;
        } else{
            Node *z= ((Node*)((Node*)t)->son3);
            if(z!=NULL&&z->son3!=NULL){
                ((Node*)w)->son2=z->son1;
                ((Node*)w)->low2=((Node*)t)->low3;
                ((Node*)t)->low3=z->low2;
                z->son1=z->son2;
                z->son2=z->son3;
                z->low2=z->low3;
                z->son3=NULL;
                return true;
            }
        }
        //У t нет сыновей с 3 узлами
        y->son3=((Node*)w)->son1;
        y->low3=((Node*)t)->low2;
        delete w;
        ((Node*)t)->son2=((Node*)t)->son3;
        ((Node*)t)->low2=((Node*)t)->low3;
        ((Node*)t)->son3=NULL;
        if(((Node*)t)->son2==NULL){
            one_son=true;
        }
        return true;
    }
    Node *y= ((Node*)((Node*)t)->son2);
    if(y->son3!=NULL){
        ((Node*)w)->son2=((Node*)w)->son1;
        ((Node*)w)->low2=((Node*)t)->low3;
        ((Node*)w)->son1=y->son3;
        ((Node*)t)->low2=y->low3;
        y->son3=NULL;
    } else{
        y->son3=((Node*)w)->son1;
        y->low3=((Node*)t)->low3;
        ((Node*)t)->son3=NULL;
        delete w;
    }
    return true;
}

template <class Data,class Key>
void TwoThreeTree<Data,Key>::print() {
    if(root!=NULL&&length>1){
        show(root,0);
    } else throw EMPTY_TREE;
}

template <class Data, class Key>
void TwoThreeTree<Data,Key>::show(TwoThreeTree<Data, Key>::Element *t, int level) {
    int sp=7;
    if(t->type==0){
       for(int i=0;i<sp*level;i++){
           cout<<" ";
       }
       cout<< ((Leaf*)t)->k<<endl;
    } else{
        if(((Node*)t)->son3!=NULL) show(((Node*)t)->son3,level+1);
        show(((Node*)t)->son2,level+1);
        for(int i=0;i<sp*level;i++){
            cout<<" ";
        }
        if(((Node*)t)->son3==NULL)cout<< ((Node*)t)->low2<<", -"<<endl;
        else cout<< ((Node*)t)->low2<<", "<< ((Node*)t)->low3<<endl;
        show(((Node*)t)->son1,level+1);
    }
}

template <class Data,class Key>
typename TwoThreeTree<Data, Key>::Leaf * TwoThreeTree<Data,Key>::toLast(TwoThreeTree<Data, Key>::Element *t) {
    if(t==NULL) throw ONE_ELEMENT_TREE;
    if(t->type==0){
        return ((Leaf*)t);
    }
    if(((Node*)t)->son3!=NULL) return toLast(((Node*)t)->son3);
    if(((Node*)t)->son2!=NULL) return toLast(((Node*)t)->son2);
    return NULL;
}

template <class Data, class Key>
typename TwoThreeTree<Data, Key>::Leaf * TwoThreeTree<Data,Key>::toFirst(TwoThreeTree<Data, Key>::Element *t) {
    if(t==NULL) throw ONE_ELEMENT_TREE;
    if(t->type==0){
        return ((Leaf*)t);
    } else return toFirst(((Node*)t)->son1);
}

template <class Data,class Key>
typename TwoThreeTree<Data, Key>::Element * TwoThreeTree<Data,Key>::successor(TwoThreeTree<Data, Key>::Element *t, Key key) {
    Element *w;
    int child=0;

    if(t==NULL){
        throw EMPTY_TREE;
    }

    if(((Node*)t)->son1->type==0){//Если мы пришли к узлу, в котором содержится ключ
        if(((Leaf*)((Node*)t)->son1)->k==key){
            return ((Node*)t)->son2;
        } else if(((Leaf*)((Node*)t)->son2)->k==key){
            return ((Node*)t)->son3;
        } else return NULL;//Если текущий ключ был у третьего сына
    }

    if(key<((Node*)t)->low2){
        child=1;
        w=((Node*)t)->son1;
    } else if(((Node*)t)->son3==NULL||(((Node*)t)->son3!=NULL&&key<((Node*)t)->low3)){
        child=2;
        w=((Node*)t)->son2;
    } else{
        child=3;
        w=((Node*)t)->son3;
    }
    Element *el = successor(w,key);
    if(el!=NULL){//Если следующий по значению ключ был найден
        return el;
    } else{//Если ключ был крайним правым в своем дереве
        if(child==1){//Ищем наименьший по значению элемент в правом сыне
            return toFirst(((Node*)t)->son2);
        }
        if(child==2&&((Node*)t)->son3!=NULL){
            return toFirst(((Node*)t)->son3);
        } else{
            return NULL;
        }
    }
}

template <class Data,class Key>
typename TwoThreeTree<Data, Key>::Element * TwoThreeTree<Data,Key>::predecessor(TwoThreeTree<Data, Key>::Element *t,
                                                                                Key key) {
    Element *w;
    int child=0;

    if(t==NULL){
        throw EMPTY_TREE;
    }

    if(((Node*)t)->son1->type==0){//Если мы пришли к узлу, в котором содержится ключ
        if(((Node*)t)->son3!=NULL){
            if(((Leaf*)((Node*)t)->son3)->k==key){
                return ((Node*)t)->son2;
            }
        }
        if(((Leaf*)((Node*)t)->son2)->k==key){
            return ((Node*)t)->son1;
        } else return NULL;//Если текущий ключ был у первого сына
    }

    if(key<((Node*)t)->low2){
        child=1;
        w=((Node*)t)->son1;
    } else if(((Node*)t)->son3==NULL||(((Node*)t)->son3!=NULL&&key<((Node*)t)->low3)){
        child=2;
        w=((Node*)t)->son2;
    } else{
        child=3;
        w=((Node*)t)->son3;
    }
    Element *el = predecessor(w,key);
    if(el!=NULL){//Если следующий по значению ключ был найден
        return el;
    } else{//Если ключ был крайним левым в своем дереве
        if(child==3){//Ищем наименьший по значению элемент в левом от сыне
            return toLast(((Node*)t)->son2);
        }
        if(child==2){
            return toLast(((Node*)t)->son1);
        } else{
            return NULL;
        }
    }
}

template <class Data,class Key>
int TwoThreeTree<Data,Key>::getOperations() { return operations;}

template <class Data,class Key>
TwoThreeTree<Data,Key>::TwoThreeTree() {
    root=NULL;
    length=0;
}


template <class Data,class Key>
TwoThreeTree<Data,Key>::~TwoThreeTree() {clear();}

//Конструкторы для элементов дерева
template <class Data,class Key>
TwoThreeTree<Data,Key>::Leaf::Leaf(Data data, Key key) {
    t=data;
    k=key;
    Element::type=0;
}

template <class Data,class Key>
TwoThreeTree<Data,Key>::Node::Node() {
    son1=son2=son3=NULL;
    Element::type=1;
}

//Методы итератора
template <class Data, class Key>
TwoThreeTree<Data,Key>::Iterator::Iterator(TwoThreeTree<Data, Key> &tree) {
    this->tree=&tree;
    cur=NULL;
}

template <class Data, class Key>
bool TwoThreeTree<Data,Key>::Iterator::status() {
    return (cur!=NULL);
}

template <class Data, class Key>
bool TwoThreeTree<Data,Key>::Iterator::last() {
    if(tree->root!=NULL){
        cur=tree->toLast(tree->root);
        return true;
    } else return false;
}

template <class Data, class Key>
bool TwoThreeTree<Data,Key>::Iterator::first() {
    if(tree->root!=NULL){
        cur=tree->toFirst(tree->root);
        return true;
    } else return false;
}

template <class Data, class Key>
bool TwoThreeTree<Data,Key>::Iterator::operator--(int) {
    if(status()){
        cur = tree->predecessor(tree->root,((Leaf*)cur)->k);
        return true;
    } else return false;
}

template <class Data, class Key>
bool TwoThreeTree<Data,Key>::Iterator::operator++(int) {
    if(status()){
    cur = tree->successor(tree->root,((Leaf*)cur)->k);
    return true;
    } else return false;
}

template <class Data, class Key>
Data& TwoThreeTree<Data,Key>::Iterator::operator*() {
    if (status()){
        return ((Leaf*)cur)->t;
    } else throw ITERATOR_END;
}