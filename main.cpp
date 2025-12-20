#include <iostream>
#include "redblack_tree.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    RedBlackTree<int, int> tree ;
    std::unordered_map<int, bool> keys ;

    std::srand(time(nullptr)) ;

    for (int i = 0; i < 10000; i++) {
        keys[i] = false ;
    }

    for (int i = 0; i < 10000; i++) {
        auto key = rand() % 10000 ;
        std::cout << key << "\n" ;
        if (keys[key]) {
            std::cout << "Suppression de " << key << "\n" ;
            tree.supprimer(key) ;
            keys[key] = false ;
        }
        else {
            std::cout << "Insertion de " << key << "\n" ;
            tree.inserer(key, key) ;
            keys[key] = true ;
        }
    }

}