#include <iostream>
#include <time.h>
#include <math.h>
#include "TwoThreeTree.h"
#include "BST-tree.h"

using namespace std;

typedef unsigned long long INT_64;

INT_64 lrand(){
    return rand()<<16|rand();
}


void test_rand(int n){
    TwoThreeTree<int, INT_64 > tree;
    BSTtree<int, INT_64 > tree_BST;

    INT_64* m=new INT_64[n];

    for(int i=0; i<n; i++) {
        m[i]=lrand();
        tree.insert(1, m[i]);
        tree_BST.add(1, m[i]);
    }


    double I=0, I_BST=0;
    double D=0, D_BST=0;
    double S=0, S_BST=0;
    for(int i=0;i<n/2;i++) {
        if (i % 10 == 0) {
            //Generate keys
            int r_key = lrand();
            int ind_key = rand() % n;

            //BST remove
            tree_BST.remove(r_key);
            D_BST+=tree_BST.getOperations();
            //2-3 remove
            tree.remove(r_key);
            D += tree.getOperations();

            //BST insert
            tree_BST.add(1,m[ind_key]);
            I_BST+=tree_BST.getOperations();

            //2-3 insert
            tree.insert(1, m[ind_key]);
            I += tree.getOperations();

            //Gen read key
            int read_key = lrand();

            //BST read
            try {
                tree_BST.read(read_key);
                S_BST+=tree_BST.getOperations();
            }catch (int code){S_BST+=tree_BST.getOperations();}

            //2-3 read
            try {
                tree.read(read_key);
                S += tree.getOperations();
            } catch (int code) { S += tree.getOperations(); }
        } else {
            int ind = rand() % n;
            INT_64 key = lrand();

            //BST remove
            tree_BST.remove(m[ind]);
            D_BST+=tree_BST.getOperations();

            //2-3 remove
            tree.remove(m[ind]);
            D += tree.getOperations();

            //BST insert
            tree_BST.add(1,key);
            I_BST+=tree_BST.getOperations();

            //2-3 insert
            tree.insert(1, key);
            I += tree.getOperations();


            m[ind] = key;

            int ind_read = rand() % n;
            //BST read
            try {
                tree_BST.read(m[ind_read]);
                S_BST+=tree_BST.getOperations();
            }catch (int code){S_BST+=tree_BST.getOperations();}


            //2-3 read
            try {
                tree.read(m[ind_read]);
                S += tree.getOperations();
            } catch (int code) { S += tree.getOperations(); }

        }

    }

    cout<<"BST items count:"<<tree_BST.getSize()<<"| 2-3 tree items count:"<<tree.getSize()<<" |Re-counted size: "<<tree.reCount()<<endl;
    cout<<"BST theoretical: 1.39*log2(n)="<<1.39*(log(n)/log(2))<<"| 2-3 tree theoretical: log2(n)="<<(log(n)/log(2))<<endl;
    cout<<"BST count insert: "<< I_BST/(n/2)<<"| 2-3 tree count insert: "<< I/(n/2) <<endl;
    cout<<"BST count delete: " << D_BST/(n/2)<<"|2-3 tree count delete: " << D/(n/2) <<endl;
    cout<<"BST count search: "<< S_BST/(n/2)<<"| 2-3 tree count search: "<< S/(n/2) <<endl;
    delete[] m;
}

void test_sort(int n){
    TwoThreeTree<int, INT_64 > tree;
    BSTtree<int, INT_64 > tree_BST;

    INT_64* m=new INT_64[n];

    for(int i=0;i<n;i++){
        m[i]=i*1000;
        tree.insert(1, m[i]);
        tree_BST.add(1, m[i]);
    }

    cout<<"items count:"<<tree.getSize()<<endl;
    double I=0, I_BST=0;
    double D=0, D_BST=0;
    double S=0, S_BST=0;

    for(int i=0;i<n/2;i++) {
        if (i % 10 == 0) {
            int k = lrand() % (1000 * n);
            k = k + !(k % 2);

            //BST remove
            tree_BST.remove(k);
            D_BST+=tree_BST.getOperations();

            //2-3 remove
            tree.remove(k);
            D += tree.getOperations();


            int ins_key=rand() % n;
            //BST insert
            tree_BST.add(1, m[rand() % n]);
            I_BST+=tree_BST.getOperations();

            //2-3 insert
            tree.insert(1, m[ins_key]);
            I += tree.getOperations();


            k = lrand() % (1000 * n);
            k = k + !(k % 2);

            //BST read
            try {
                tree_BST.read(k);
                S_BST+=tree_BST.getOperations();
            }catch(int code){S_BST+=tree_BST.getOperations();}


            //2-3 read
            try {
                tree.read(k);
                S += tree.getOperations();
            } catch (int code) { S += tree.getOperations(); }
        } else {
            int ind = rand() % n;
            int k = lrand() % (1000 * n);
            k = k + k % 2;

            //BST remove
            tree_BST.remove(m[ind]);
            D_BST+=tree_BST.getOperations();

            //2-3 remove
            tree.remove(m[ind]);
            D += tree.getOperations();

            //BST insert
            tree_BST.add(1,k);
            I_BST+=tree_BST.getOperations();

            //2-3 insert
            tree.insert(1, k);
            I += tree.getOperations();
            m[ind] = k;

            int read_key = rand() % n;
            //BST read
            try{
                tree_BST.read(m[read_key]);
                S_BST+=tree_BST.getOperations();
            }catch (int code){S_BST+=tree_BST.getOperations();}

            //2-3 read
            try {
                tree.read(m[read_key]);
                S += tree.getOperations();
            } catch (int code) { S += tree.getOperations(); }

        }
    }
    cout<<"BST items count:"<<tree_BST.getSize()<<"| 2-3 tree items count:"<<tree.getSize()<<" |Re-counted size: "<<tree.reCount()<<endl;
    cout<<"BST theoretical: n/2="<<n/2<<"| 2-3 tree theoretical: log2(n)="<<(log(n)/log(2))<<endl;
    cout<<"BST count insert: "<< I_BST/(n/2)<<"| 2-3 tree count insert: "<< I/(n/2) <<endl;
    cout<<"BST count delete: " << D_BST/(n/2)<<"|2-3 tree count delete: " << D/(n/2) <<endl;
    cout<<"BST count search: "<< S_BST/(n/2)<<"| 2-3 tree count search: "<< S/(n/2) <<endl;
    delete[] m;
}





void showMenu(){
    cout << endl;

    cout << "1. Watch tree" << endl;
    cout << "2. Clear tree" << endl;
    cout << "3. Empty check" << endl;
    cout << "4. Show value by key" << endl;
    cout << "5. Change value by key" << endl;
    cout << "6. Add value by key" << endl;
    cout << "7. Delete value by key" << endl;
    cout << "8. Tree size" << endl;
    cout << "9. Iterator to first key position" << endl;
    cout << "10. Iterator to last key position" << endl;
    cout << "11. Change value at iterator's position" << endl;
    cout << "12. Show value at iterator's position" << endl;
    cout << "13. Next key" << endl;
    cout << "14. Prev key" << endl;
    cout << "15. Test" << endl;
    cout << "0. Exit" << endl;
}

int main() {
    TwoThreeTree<int,int> tree;
    TwoThreeTree<int ,int >::Iterator iter(tree);
    srand((unsigned int)time(NULL));


    showMenu();

    bool exit = false;
    int com;
    while (!exit){
        cout<<"input command:";
        cin>>com;
        cout<< endl;
        try{
            switch (com){
                case 0: exit= true;
                    break;
                default: cout << "No such command exist"<<endl;
                    break;

                case 1:{
                    tree.print();
                    break;
                }

                case 2:{
                    tree.clear();
                    break;
                }

                case 3:{
                    cout<<tree.isEmpty()<<endl;
                    break;
                }

                case 4:{
                    cout<<"Insert key: ";
                    int k;
                    cin>>k;
                    cout<<endl;
                    cout<<tree.read(k)<<endl;
                    break;
                }

                case 5:{
                    cout<<"Insert key: ";
                    int k;
                    cin>>k;
                    cout<<endl;
                    cout<<"Insert new value: ";
                    cin>>tree.read(k);
                    cout<<endl;
                    break;
                }

                case 6:{
                    cout<<"Insert key: ";
                    int k;
                    cin>>k;
                    cout<<endl;
                    cout<<"Insert new value: ";
                    int val;
                    cin>>val;
                    cout<<tree.insert(val,k)<<endl;
                    break;
                }

                case 7:{
                    cout<<"Insert key: ";
                    int k;
                    cin>>k;
                    cout<<endl;
                    cout<<tree.remove(k)<<endl;
                    break;
                }

                case 8:{
                    cout<<"Tree length: "<<tree.getSize()<<endl;
                    break;
                }

                case 9:{
                    cout<<iter.first()<<endl;
                    break;
                }

                case 10:{
                    cout<<iter.last()<<endl;
                    break;
                }

                case 11:{
                    cout<<"Insert new value: ";
                    cin>>*iter;
                    cout<<endl;
                    break;
                }

                case 12:{
                    cout<<*iter<<endl;
                    break;
                }

                case 13:{
                    cout<<iter++<<endl;
                    break;
                }

                case 14:{
                    cout<<iter--<<endl;
                    break;
                }

                case 15:{
                    int number;
                    cout<<"Input tree length: ";
                    cin>>number;
                    cout<<endl;
                    cout<<"Random tree: "<<endl;
                    test_rand(number);
                    cout<<"--------------------------------------------"<<endl;
                    cout<<"Sorted tree: "<<endl;
                    test_sort(number);
                    break;
                }

                case -1:{
                    showMenu();
                    break;
                }
                case -2:{//preset for remove test
                    tree.clear();
                    tree.insert(1,2);
                    tree.insert(1,5);
                    tree.insert(1,7);
                    tree.insert(1,8);
                    tree.insert(1,16);
                    tree.insert(1,19);
                    tree.insert(1,12);
                    tree.insert(1,18);
                    tree.insert(1,10);
                    tree.print();
                    break;
                }

            }
        }catch(int code) {
            switch (code){
                case EMPTY_TREE:cout<<"Empty tree"<<endl;
                    break;
                case KEY_DOES_NOT_EXIST:cout<<"this key does not exist in tree"<<endl;
                    break;
                case ITERATOR_END:cout<<"Iterator is out of tree"<<endl;
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}