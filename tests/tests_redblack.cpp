//
// Created by Pascal Charpentier on 2025-04-10.
//


#include "gtest/gtest.h"
#include "redblack_tree.h"

TEST(redblack, insert) {
    RedBlackTree<int, int> rb ;
    std::vector<int> res = {1} ;

    rb.inserer(1, 100) ;
    EXPECT_EQ(res, rb.parcourirEnOrdre()) ;
}

TEST(redblack, insert_two_keys_in_direct_order) {
    RedBlackTree<int, int> rb ;
    std::vector<int> res = {1, 2} ;

    rb.inserer(1, 100) ;
    rb.inserer(2, 200) ;
    EXPECT_EQ(res, rb.parcourirEnOrdre()) ;
}

TEST(redblack, insert_two_keys_in_reverse_order) {
    RedBlackTree<int, int> rb ;
    std::vector<int> res = {1, 2} ;

    rb.inserer(2, 200) ;
    rb.inserer(1, 100) ;
    EXPECT_EQ(res, rb.parcourirEnOrdre()) ;
}

TEST(redblack, insert_three_keys_in_direct_order) {
    RedBlackTree<int, int> rb ;
    std::vector<int> res = {1, 2, 3} ;

    rb.inserer(1, 100) ;
    rb.inserer(2, 200) ;
    rb.inserer(3, 300) ;
    EXPECT_EQ(res, rb.parcourirEnOrdre()) ;
}

TEST(redblack, insert_one_key_suppress_one_key) {
    RedBlackTree<int, int> rb ;
    std::vector<int> res ;

    rb.inserer(1, 100) ;
    rb.supprimer(1) ;
    EXPECT_EQ(res, rb.parcourirEnOrdre()) ;
}

TEST(redblack, insert_two_keys_suppress_first_key) {
    RedBlackTree<int, int> rb ;
    std::vector<int> res {2} ;

    rb.inserer(1, 100) ;
    rb.inserer(2, 200) ;
    rb.supprimer(1) ;
    EXPECT_EQ(res, rb.parcourirEnOrdre()) ;

}

TEST(redblack, insert_two_keys_suppress_second_key) {
    RedBlackTree<int, int> rb ;
    std::vector<int> res {1} ;

    rb.inserer(1, 100) ;
    rb.inserer(2, 200) ;
    rb.supprimer(2) ;
    EXPECT_EQ(res, rb.parcourirEnOrdre()) ;
}

TEST(redblack, insert_many_keys_suppress_one) {
    RedBlackTree<int, int> rb ;
    std::vector<int> res {0, 1, 2, 4, 5} ;

    for (int i = 0; i < 6; ++i) {
        rb.inserer(i, i*10) ;
    }
    rb.supprimer(3) ;
    EXPECT_EQ(res, rb.parcourirEnOrdre()) ;
}