#include <iostream>
#include <time.h>
#include <math.h>
#include "TwoThreeTree.h"

using namespace std;




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
                    iter.first();
                    break;
                }

                case 10:{
                    iter.last();
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
                    /*int number;
                    cout<<"Input tree length: ";
                    cin>>number;
                    cout<<endl;
                    cout<<"Random tree: "<<endl;
                    test_rand(number);
                    cout<<"--------------------------------------------"<<endl;
                    cout<<"Sorted tree: "<<endl;
                    srand((unsigned int)time(NULL));
                    test_sort(number);*/
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