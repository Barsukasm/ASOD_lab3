#include <iostream>
#define EMPTY_TREE 0
#define KEY_DOES_NOT_EXIST 1
#define ITERATOR_END 2

using namespace std;



template <class Data,class Key> class TwoThreeTree{

    class Element{
    public:
        virtual ~Element(){}
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

    void print();//вывод структуры дерева на экран
    int getOperations();//число просмотренных операций узлов дерева

    TwoThreeTree();//конструктор по умолчанию
    TwoThreeTree(TwoThreeTree<Data,Key> &ttree);//конструктор копирования
    ~TwoThreeTree();//деструктор

private:
    Node *root;//указатель на корень
    int length;//длина дерева

    int operations;//число просмотров

    //вспомогательные методы
    bool insert1(Element *t,Element *lt,Element *tup, Key &lup);
    bool remove1(Element *t, Key k, Element *tlow1, bool one_son);
    Data& read1(Element *t,Key key);
};


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
void TwoThreeTree<Data,Key>::clear() {}

template <class Data,class Key>
Data& TwoThreeTree<Data,Key>::read(Key key) {


    if(root==NULL){
        throw EMPTY_TREE;
    }
    if(root->son2==NULL){
        Leaf *tempSon1 = dynamic_cast<Leaf*>(root->son1);
        if(tempSon1->k==key){
            return tempSon1->t;
        }
    }
    return read1(root,key);
}

template <class Data,class Key>
Data& TwoThreeTree<Data,Key>::read1(TwoThreeTree<Data, Key>::Element *t, Key key) {


    if(t==NULL){
        throw EMPTY_TREE;
    }

    if(typeid(t)== typeid(Leaf*)){
        Leaf *tempT = dynamic_cast<Leaf*>(t);
        if(tempT->k==key){
            return tempT->t;
        }
    }
    Node *tempT = dynamic_cast<Node*>(t);

}

template <class Data,class Key>
bool TwoThreeTree<Data,Key>::insert(Data data, Key key) {
    Element *lt, *tbk;
    Key lbk;

    lt = new Leaf(data,key);
    if (root==NULL){
        root=new Node();
        root->son1=lt;
        root->son2=root->son3=NULL;
        return true;
    }

    if(root->son2==NULL ){
        Leaf *tempSon1 = dynamic_cast<Leaf*>(root->son1);
        if(tempSon1->k<key){
            root->son2=lt;
            root->low2=key;
        } else if(tempSon1->k>key){
            root->son2=tempSon1;
            root->low2=tempSon1->k;
            root->son1=lt;
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
        Element *temp = root;
        root = new Node();
        root->son1=temp;
        root->son2=tbk;
        root->low2=lbk;
        root->son3=NULL;
        return true;
    }
}

template <class Data,class Key>
bool TwoThreeTree<Data,Key>::insert1(TwoThreeTree<Data, Key>::Element *t,TwoThreeTree<Data, Key>::Element *lt, TwoThreeTree<Data, Key>::Element *tup, Key &lup) {
    tup=NULL;
    Element *w, *tbk;
    Key lbk;
    int child=0;

    Leaf *tempLt= dynamic_cast<Leaf*>(lt);
    if(typeid(t)== typeid(Leaf*)){
        Leaf *tempT = dynamic_cast<Leaf*>(t);
        if (tempT->k==tempLt->k) return false;
        else{
            tup=lt;
            if(tempT->k<tempLt->k){
                lup=tempLt->k;
            } else{
                lup=tempT->k;
                Key temp = tempT->k;
                tempT->k=tempLt->k;
                tempLt->k=temp;
                Data temp2 = tempT->t;
                tempT->t = tempLt->t;
                tempLt->t=temp2;
            }
            return true;
        }
    }
    Node *tempT = dynamic_cast<Node*>(t);
    if(tempLt->t<tempT->low2){
        child=1;
        w=tempT->son1;
    } else if (tempT->son3==NULL||(tempT->son3!=NULL&&tempLt->k<tempT->low3)){
        child=2;
        w=tempT->son2;
    } else{
        child=3;
        w=tempT->son3;
    }
    bool inserted = insert1(w,lt,tbk,lbk);
    if(inserted){
        if(tbk!=NULL){
            if(tempT->son3==NULL){
                if(child==2){
                    tempT->son3=tbk;
                    tempT->low3=lbk;
                } else{
                    tempT->son3=tempT->son2;
                    tempT->low3=tempT->low2;
                    tempT->son2=tbk;
                    tempT->low2=lbk;
            }
            }else{
                tup=new Node();
                Node *tempTup= dynamic_cast<Node*>(tup);
                if(child==3){
                    tempTup->son1=tempT->son3;
                    tempTup->son2=tbk;
                    tempTup->son3=NULL;
                    tempTup->low2=lbk;
                    tempT->son3=NULL;
                    lup=tempT->low3;
                }else{
                    tempTup->son2=tempT->son3;
                    tempTup->low2=tempT->low3;
                    tempTup->son3=NULL;
                    if(child==2){
                        tempTup->son1=tbk;
                        lup=lbk;
                    }
                    if(child==1){
                        tempTup->son1=tempT->son2;
                        tempT->son2=tbk;
                        lup=tempTup->low2;
                        tempT->low2=lbk;
                        tempT->son3=NULL;
                    }
                }
            }
        }
    }
    return inserted;
}


template <class Data,class Key>
bool TwoThreeTree<Data,Key>::remove(Key key) {
    Element *tmin;
    bool one;

    if(root==NULL){
        return false;
    }
    if(root->son2==NULL){
        Leaf *tempSon1= dynamic_cast<Leaf*>(root->son1);
        if(tempSon1->k==key){
            delete tempSon1;
            delete root;
            root=NULL;
            return true;
        } else{
            return false;
        }
    }
    bool deleted=remove1(root,key,tmin,one);
    if(deleted){
        if(one){
            if(typeid(root->son1)!= typeid(Leaf*)){
                Node *t= dynamic_cast<Node*>(root->son1);
                delete root;
                root=t;
            }
        }
    }
    return deleted;
}

template <class Data,class Key>
bool TwoThreeTree<Data,Key>::remove1(TwoThreeTree<Data, Key>::Element *t, Key k,
                                     TwoThreeTree<Data, Key>::Element *tlow1, bool one_son) {
    int child=0;
    Element *w, *tlow1_bk;
    bool one_son_bk;

    tlow1=NULL;
    one_son=false;
    Node *tempT= dynamic_cast<Node*>(t);
    if(typeid(tempT->son1)== typeid(Leaf*)){
        Leaf *tempSon1= dynamic_cast<Leaf*>(tempT->son1);
        if(tempSon1->k==k){
            delete tempSon1;
            tempT->son1=tempT->son2;
            tempT->son2=tempT->son3;
            tempT->son3=NULL;
            tempT->low2=tempT->low3;
        } else {
            Leaf *tempSon2= dynamic_cast<Leaf*>(tempT->son2);
            if(tempSon2->k==k){
                delete tempSon2;
                tempT->son2=tempT->son3;
                tempT->son3=NULL;
                tempT->low2=tempT->low3;
            } else{
                if(tempT->son3!=NULL){
                    Leaf *tempSon3= dynamic_cast<Leaf*>(tempT->son3);
                    if(tempSon3->k==k){
                       delete tempSon3;
                       tempT->son3=NULL;
                    }else return false;
                } else return false;
            }
        }
        tlow1=tempT->son1;
        if(tempT->son2==NULL){
            one_son=true;
        }
        return true;
    }
    if(k<tempT->low2){
        child=1;
        w=tempT->son1;
    } else if(tempT->son3==NULL||k<tempT->low3){
        child=2;
        w=tempT->son2;
    } else{
        child=3;
        w=tempT->son3;
    }
    if (!remove1(w,k,tlow1_bk,one_son_bk)){
        return false;
    }
    tlow1=tlow1_bk;
    if(tlow1_bk!=NULL){
        if(child==2){
            tempT->low2= dynamic_cast<Leaf*>(tlow1_bk)->k;
            tlow1=NULL;
        }
        if(child==3){
            tempT->low3= dynamic_cast<Leaf*>(tlow1_bk)->k;
            tlow1=NULL;
        }
    }
    if(!one_son_bk){
        return true;
    }
    if(child==1){
        Node *y= dynamic_cast<Node*>(tempT->son2);
        Node *tempW= dynamic_cast<Node*>(w);
        if(y->son3!=NULL){
            tempW->son2=y->son1;
            tempW->low2=tempT->low2;
            tempT->low2=y->low2;
            y->son1=y->son2;
            y->son2=y->son3;
            y->low2=y->low3;
            y->son3=NULL;
        } else{
            y->son3=y->son2;
            y->low3=y->low2;
            y->son2=y->son1;
            y->low2=tempT->low2;
            y->son1=tempW->son1;
            delete w;
            tempT->son1=tempT->son2;
            tempT->son2=tempT->son3;
            tempT->low2=tempT->low3;
            tempT->son3=NULL;
            if(tempT->son2==NULL){
                one_son=true;
            }
        }
        return true;
    }
    if(child==2){
        Node *y= dynamic_cast<Node*>(tempT->son1);
        Node *tempW= dynamic_cast<Node*>(w);
        if(y->son3!=NULL){
            tempW->son2=tempW->son1;
            tempW->low2=tempT->low2;
            tempW->son1=y->son3;
            y->son3=NULL;
            tempT->low2=y->low3;
            return true;
        } else{
            Node *z= dynamic_cast<Node*>(tempT->son3);
            if(z!=NULL&&z->son3!=NULL){
                tempW->son2=z->son1;
                tempW->low2=tempT->low3;
                tempT->low3=z->low2;
                z->son1=z->son2;
                z->son2=z->son3;
                z->low2=z->low3;
                z->son3=NULL;
                return true;
            }
        }
        y->son3=tempW->son1;
        y->low3=tempT->low2;
        delete w;
        tempT->son2=tempT->son3;
        tempT->low2=tempT->low3;
        tempT->son3=NULL;
        if(tempT->son2==NULL){
            one_son=true;
        }
        return true;
    }
    Node *y= dynamic_cast<Node*>(tempT->son2);
    Node *tempW= dynamic_cast<Node*>(w);
    if(y->son3!=NULL){
        tempW->son2=tempW->son1;
        tempW->low2=tempT->low3;
        tempW->son1=y->son3;
        tempT->low2=y->low3;
        y->son3=NULL;
    } else{
        y->son3=tempW->son1;
        y->low3=tempT->low3;
        tempT->son3=NULL;
        delete w;
    }
    return true;
}

template <class Data,class Key>
void TwoThreeTree<Data,Key>::print() {}

template <class Data,class Key>
int TwoThreeTree<Data,Key>::getOperations() { return operations;}

template <class Data,class Key>
TwoThreeTree<Data,Key>::TwoThreeTree() {
    root=NULL;
    length=0;
}

template <class Data,class Key>
TwoThreeTree<Data,Key>::TwoThreeTree(TwoThreeTree<Data, Key> &ttree) {
    root=NULL;
    length=0;
    //copy(ttree.root);
}

template <class Data,class Key>
TwoThreeTree<Data,Key>::~TwoThreeTree() {clear();}

//Конструкторы для элементов дерева
template <class Data,class Key>
TwoThreeTree<Data,Key>::Leaf::Leaf(Data data, Key key) {
    t=data;
    k=key;
}

template <class Data,class Key>
TwoThreeTree<Data,Key>::Node::Node() {
    son1=son2=son3=NULL;
    low2=low3=0;
}

//Методы итератора