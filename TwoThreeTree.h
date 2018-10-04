#include <iostream>


using namespace std;



template <class Data,class Key> class TwoThreeTree{

    class Element{
        Key k;
    };

    class Leaf: public Element{
    public:
        Data t;
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
Data& TwoThreeTree<Data,Key>::read(Key key) {}

template <class Data,class Key>
bool TwoThreeTree<Data,Key>::insert(Data data, Key key) {
    Element lt, *tbk;
    Key lbk;

    lt = new Leaf(data,key);
    if (root==NULL){
        root=new Node();
        root->son1=lt;
        root->son2=root->son3=NULL;
        return true;
    }

    if(root->son2==NULL ){
        if(root->son1->k<key/*&& typeid(root->son1)== typeid(Leaf)*/){
            root->son2=lt;
            root->low2=key;
        } else if(root->son1->k>key){
            root->son2=root->son1;
            root->low2=root->son1->k;
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
        Element temp = root;
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
    if(typeid(t)== typeid(Leaf)){
        if (t->k==lt->k) return false;
        else{
            tup=lt;
            if(t->k<lt->k){
                lup=lt->k;
            } else{
                lup=t->k;
                Key temp = t->k;
                t->k=lt->k;
                lt->k=temp;
                Data temp2 = t->t;
                t->t = lt->t;
                lt->t=temp2;
            }
            return true;
        }
    }
    if(lt->t<t->low2){
        child=1;
        w=t->son1;
    } else if (t->son3==NULL||(t->son3!=NULL&&lt->k<t->low3)){
        child=2;
        w=t->son2;
    } else{
        child=3;
        w=t->son3;
    }
    bool inserted = insert1(w,lt,tbk,lbk);
    if(inserted){
        if(tbk!=NULL){
            if(t->son3==NULL){
                if(child==2){
                    t->son3=tbk;
                    t->low3=lbk;
                } else{
                    t->son3=t->son2;
                    t->low3=t->low2;
                    t->son2=tbk;
                    t->low2=lbk;
            }
            }else{
                tup=new Node();
                if(child==3){
                    tup->son1=t->son3;
                    tup->son2=tbk;
                    tup->son3=NULL;
                    tup->low2=lbk;
                    t->son3=NULL;
                    lup=t->low3;
                }else{
                    tup->son2=t->son3;
                    tup->low2=t->low3;
                    tup->son3=NULL;
                    if(child==2){
                        tup->son1=tbk;
                        lup=lbk;
                    }
                    if(child==1){
                        tup->son1=t->son2;
                        t->son2=tbk;
                        lup=t->low2;
                        t->low2=lbk;
                        t->son3=NULL;
                    }
                }
            }
        }
    }
    return inserted;
}


template <class Data,class Key>
bool TwoThreeTree<Data,Key>::remove(Key key) {}

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
    Element::k=key;
}

template <class Data,class Key>
TwoThreeTree<Data,Key>::Node::Node() {
    son1=son2=son3=NULL;
    low2=low3=0;
}

//Методы итератора