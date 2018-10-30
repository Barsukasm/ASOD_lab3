#include <iostream>
#include <list>
#define EMPTY_TREE 0
#define KEY_DOES_NOT_EXIST 1
#define ITERATOR_END 2

using namespace std;



template <class Data,class Key> class BSTtree{
    class Node{
    public:
        Data t;
        Key k;
        Node *left;
        Node *right;
        //Node();
        Node(Data x, Key y);
        //~Node();
    };
    friend class Node;

public:
    class Iterator{
    private:
        BSTtree *tree;
        Node *cur;
    public:
        Iterator(BSTtree<Data,Key> &tree);//конструктор
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
    bool add(Data data, Key key);//включение данных с заданным ключом
    bool remove(Key key);//удаление данных с заданным ключом
    void Lt_t_Rt();//обход узлов дерева по схеме Lt->t->Rt
    int ex_path_l();//определение длины внешнего пути дерева (рекурсивная форма алгоритма)

    void print();//вывод структуры дерева на экран
    int getOperations();//число просмотренных операций узлов дерева

    BSTtree();//конструктор по умолчанию
    BSTtree(BSTtree<Data,Key> &ttree);//конструктор копирования
    ~BSTtree();//деструктор

private:
    Node *root;//указатель на корень
    int length;//длина дерева

    int operations;//число просмотров

    //вспомогательные методы
    void show(Node *t,int level);//вспомогательный метод распечатки
    void levelCounter(Node *t, int level,  int &sum);
    Node* tree_successor(Node *x);
    Node* left_parent(Node *r, Node *x);
    Node* tree_predecessor(Node *x);
    Node* right_parent(Node *r, Node *x);
    void copy(Node *t);
};

//--------------Методы класса BSTtree-----------
template <class Data, class Key>
BSTtree<Data,Key>::BSTtree() {
    root=NULL;
    length=0;
}

template <class Data, class Key>
BSTtree<Data,Key>::BSTtree(BSTtree<Data, Key> &ttree) {
    root==NULL;
    length=0;
    copy(ttree.root);
}

template <class Data, class Key>
void BSTtree<Data,Key>::copy(BSTtree<Data, Key>::Node *t) {
    if(t==NULL) return;
    add(t->t,t->k);//добавляем узел в дерево
    copy(t->left);//делаем то же самое для его сыновей
    copy(t->right);
}

template <class Data, class Key>
BSTtree<Data,Key>::~BSTtree() {
    clear();
}

template <class Data, class Key>
int BSTtree<Data,Key>::getSize() { return length;}

template <class Data, class Key>
void BSTtree<Data,Key>::clear() {
    if(root!=NULL){
        std::list<Node*> s1, s2, s3;
        Node *cur;
        s1.push_back(root);
        while (!s1.empty()||!s2.empty()){
            if(!s1.empty()){
                cur=s1.back();
                s1.pop_back();
                s2.push_back(cur);
                if(cur->right!=NULL){
                    s1.push_back(cur->right);
                }
            } else{
                cur=s2.back();
                s2.pop_back();
                s3.push_back(cur);
                if(cur->left!=NULL){
                    s1.push_back(cur->left);
                }
            }
        }
        while (!s3.empty()){
            cur=s3.back();
            s3.pop_back();
            delete cur;
        }
        root=NULL;
    } else{
        length=0;
    }
}

template <class Data, class Key>
bool BSTtree<Data,Key>::isEmpty() {return (length == 0 && root == NULL);}

template <class Data, class Key>
Data& BSTtree<Data,Key>::read(Key key) {
    operations=0;
    if(root==NULL) throw EMPTY_TREE;
    Node *t=root;
    while(t!=NULL&&key != t->k){
        if(key<t->k){
            t=t->left;
        }else{
            t=t->right;
        }
        operations++;
    }
    if(t==NULL) throw KEY_DOES_NOT_EXIST;
    return t->t;
}

template <class Data, class Key>
bool BSTtree<Data,Key>::add(Data data, Key key) {
    operations=0;
    if(root==NULL){
        root=new Node(data,key);
        length++;
        return true;
    }
    Node *tmp=root;
    Node *pred=NULL;
    while (tmp!=NULL){
        pred=tmp;
        operations++;
        if(key==tmp->k) return false;//данный ключ уже занят
        if(key<tmp->k){
            tmp=tmp->left;
        } else{
            tmp=tmp->right;
        }
    }
    if(key<pred->k){
        pred->left=new Node(data,key);
    } else{
        pred->right=new Node(data,key);
    }
    operations++;
    length++;
    return true;
}

template <class Data, class Key>
bool BSTtree<Data,Key>::remove(Key key) {
    Node *t=root;
    Node *pred=NULL;
    Node *t0=NULL;
    Node *x=NULL;

    operations=1;
    while (t!=NULL&&t->k!=key){
        pred=t;
        if(key<t->k){
            t=t->left;
        } else{
            t=t->right;
        }
        operations++;
    }
    if(t==NULL){
        return false;//ключ не найден
    }
    if(t->left!=NULL&&t->right!=NULL){
        t0=t;
        pred=t;
        t=t->right;
        while (t->left!=NULL){
            pred=t;
            t=t->left;
            operations++;
        }
        x=t->right;
    } else{
        t0=NULL;
        if(t->left==NULL&&t->right==NULL){
            x=NULL;
        }
        if(t->left==NULL){
            x=t->right;
        }
        if(t->right==NULL){
            x=t->left;
        }
    }
    if(pred==NULL){
        root=x;
    } else if(t->k<pred->k){
        pred->left=x;
    } else{
        pred->right=x;
    }
    if(t0!=NULL){
        t0->k=t->k;
        t0->t=t->t;
    }
    delete t;
    length--;
    return true;
}

template <class Data, class Key>
void BSTtree<Data,Key>::Lt_t_Rt() {
    if(root!=NULL){
        std::list<Node*> s1, s2, s3;
        Node *cur;
        s1.push_back(root);
        while (!s1.empty()||!s2.empty()){
            if(!s1.empty()){
                cur=s1.back();
                s1.pop_back();
                s2.push_back(cur);
                if(cur->right!=NULL){
                    s1.push_back(cur->right);
                }
            } else{
                cur=s2.back();
                s2.pop_back();
                s3.push_back(cur);
                if(cur->left!=NULL){
                    s1.push_back(cur->left);
                }
            }
        }
        while (!s3.empty()){
            cur=s3.back();
            s3.pop_back();
            cout<<cur->k<<"->";
        }
        cout<<endl;
    }
}

template <class Data, class Key>
int BSTtree<Data,Key>::ex_path_l() {
    if(root==NULL) return -1;
    int sum=0;
    levelCounter(root,1,sum);
    return sum;
}

template <class Data, class Key>
void BSTtree<Data,Key>::levelCounter(BSTtree<Data, Key>::Node *t, int level, int &sum) {
    if(t==NULL) return;
    levelCounter(t->left,level+1,sum);
    levelCounter(t->right,level+1,sum);
    if(t->left==NULL||t->right==NULL) sum += level;
}

template <class Data, class Key>
void BSTtree<Data,Key>::print() {
    if(root==NULL) return;
    show(root,0);
}

template <class Data, class Key>
void BSTtree<Data,Key>::show(Node *t, int level){
    if(t==NULL) return;
    show(t->right,level+1);
    for(int i=0;i<=3*level;i++){
        cout << " ";
    }
    cout << t->k << endl;
    show(t->left,level+1);
}

template <class Data, class Key>
int BSTtree<Data,Key>::getOperations() { return operations;}

//вспомогательные методы
template <class Data, class Key>
typename BSTtree<Data,Key>::Node* BSTtree<Data,Key>::tree_successor(BSTtree<Data, Key>::Node *x) {
    if(x==NULL) return NULL;
    if(x->right!=NULL){
        Node *tmp=x->right;
        while (tmp->left!=NULL){
            tmp=tmp->left;
        }
        return tmp;
    } else return left_parent(root,x);
}

template <class Data, class Key>
typename BSTtree<Data,Key>::Node* BSTtree<Data,Key>::left_parent(BSTtree<Data, Key>::Node *r, BSTtree<Data, Key>::Node *x) {
    if(r==NULL) return NULL;
    if(x->k<r->k){
        Node *n=left_parent(r->left,x);
        if(n!=NULL) return n;
        else return r;
    } else return left_parent(r->right,x);

}

template <class Data, class Key>
typename BSTtree<Data,Key>::Node* BSTtree<Data,Key>::tree_predecessor(BSTtree<Data, Key>::Node *x) {
    if(x==NULL) return NULL;
    if(x->left!=NULL){
        Node *tmp=x->left;
        while (tmp->right!=NULL){
            tmp=tmp->right;
        }
        return tmp;
    } else return right_parent(root,x);
}

template <class Data, class Key>
typename BSTtree<Data,Key>::Node* BSTtree<Data,Key>::right_parent(BSTtree<Data, Key>::Node *r, BSTtree<Data, Key>::Node *x) {
    if(r==NULL) return NULL;
    if(x->k>r->k){
        Node *n=right_parent(r->right,x);
        if(n!=NULL) return n;
        else return r;
    } else return right_parent(r->left,x);
}

//--------------Методы класса Iterator----------
template <class Data, class Key>
BSTtree<Data,Key>::Iterator::Iterator(BSTtree<Data, Key> &tree) {
    this->tree=&tree;
    cur=NULL;
}

template <class Data, class Key>
bool BSTtree<Data,Key>::Iterator::status() {
    return (cur!=NULL);
}

template <class Data, class Key>
bool BSTtree<Data,Key>::Iterator::first() {
    cur=tree->root;
    if (cur != NULL){
        while (cur->left != NULL)
            cur = cur->left;
    return true;
    } else return false;
}

template <class Data, class Key>
bool BSTtree<Data,Key>::Iterator::last() {
    cur = tree->root;
    if (cur != NULL){
        while (cur->right != NULL)
            cur = cur->right;
        return true;
    } else return false;
}

template <class Data, class Key>
Data& BSTtree<Data,Key>::Iterator::operator*() {
    if (status()){
        return cur->t;
    } else throw ITERATOR_END;
}

template <class Data, class Key>
bool BSTtree<Data,Key>::Iterator::operator++(int) {
    if(status()){
        cur=tree->tree_successor(cur);
        return true;
    } else return false;
}

template <class Data, class Key>
bool BSTtree<Data,Key>::Iterator::operator--(int) {
    if(status()){
        cur=tree->tree_predecessor(cur);
        return true;
    } else return false;
}
//---------------Методы класса Node-------------
/*
template <class Data, class Key>
BSTtree<Data,Key>::Node::Node() {
    left=NULL;
    right=NULL;
}*/

template <class Data, class Key>
BSTtree<Data,Key>::Node::Node(Data x, Key y) {
    t=x;
    k=y;
    left=NULL;
    right=NULL;
}
/*
template <class Data, class Key>
BSTtree<Data,Key>::Node::~Node() {
    delete t;
    delete k;
    left=NULL;
    right=NULL;
}*/
