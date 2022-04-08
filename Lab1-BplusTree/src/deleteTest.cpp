#include "BplusTree.h"

std::vector<int> v = {1, 4, 7, 10, 17, 21, 31, 25, 19, 20, 28, 42};
std::vector<int> v2 = {21,31,20,10,7,25,42};

int main() {
    auto tree = new BplusTree();
    for(auto &x : v){
        tree->insert(x);
    }

    std::cout<<"Inorder:\n";
    tree->linealPrint();
    std::cout<<"BFS:\n";
    tree->bfsPrint();

    for(auto &it: v2){
        tree->erase(it);
    }

    std::cout<<"Inorder:\n";
    tree->linealPrint();
    std::cout<<"BFS:\n";
    tree->bfsPrint();
    
    delete tree;   

}