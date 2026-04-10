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

TEST(redblack, insert_many_keys_suppress_all) {
    RedBlackTree<int, int> rb ;
    std::vector<int> res ;
    for (int i = 0; i < 6; ++i) {
        rb.inserer(i, i*10) ;
    }
    rb.supprimer(4) ;
    rb.inserer(6, 100) ;
    rb.supprimer(1) ;
    rb.inserer(7, 200) ;
    rb.supprimer(5) ;
    rb.supprimer(2) ;
    rb.supprimer(3) ;
    rb.supprimer(0) ;
}

TEST(redblack, insert_problematic_keys) {
    RedBlackTree<int, int> rb ;
    std::vector<int> res ;
    rb.inserer(1513, 1513) ;
    rb.inserer(1900, 1900) ;
    rb.inserer(7708, 7708) ;
    rb.inserer(2009, 2009) ;
    rb.inserer(8268, 8268) ;
    rb.inserer(6837, 6937) ;
    rb.inserer(2525, 2525) ;
}

// ... existing code ...
TEST(redblack, copy_empty_tree) {
    RedBlackTree<int, int> original ;
    RedBlackTree<int, int> copy = original ;

    EXPECT_EQ(std::vector<int>{}, original.parcourirEnOrdre()) ;
    EXPECT_EQ(std::vector<int>{}, copy.parcourirEnOrdre()) ;
}

TEST(redblack, copy_non_empty_tree_keeps_same_order) {
    RedBlackTree<int, int> original ;
    for (int i = 0; i < 6; ++i) {
        original.inserer(i, i * 10) ;
    }

    RedBlackTree<int, int> copy = original ;

    EXPECT_EQ(original.parcourirEnOrdre(), copy.parcourirEnOrdre()) ;
}

TEST(redblack, copy_is_independent_from_original) {
    RedBlackTree<int, int> original ;
    for (int i = 0; i < 6; ++i) {
        original.inserer(i, i * 10) ;
    }

    RedBlackTree<int, int> copy = original ;

    copy.supprimer(2) ;
    copy.inserer(42, 420) ;

    EXPECT_EQ((std::vector<int>{0, 1, 2, 3, 4, 5}), original.parcourirEnOrdre()) ;
    EXPECT_EQ((std::vector<int>{0, 1, 3, 4, 5, 42}), copy.parcourirEnOrdre()) ;
}

TEST(redblack, const_iterator_for_loop) {
    RedBlackTree<int, int> arbre ;
    std::vector<int> keys {4, 8, 1, 0, 2, 3, 5, 11, 9} ;
    std::vector<int> expected {0, 10, 20, 30, 40, 50, 80, 90, 110} ;
    std::vector<int> actual ;

    for (auto k: keys) arbre.inserer(k, 10 * k) ;
    for (const auto& v: arbre) actual.push_back(v) ;
    EXPECT_EQ(actual, expected) ;
}

TEST(redblack, preorder_fold_tree) {
    RedBlackTree<int, int> arbre ;
    std::vector<int> keys {4, 8, 1, 0, 2, 3, 5, 11, 9} ;
    for (auto k: keys) arbre.inserer(k, 10 * k) ;

    auto res = arbre.preorder_fold_tree(0, [](int acc, int k, int v) {
        return acc + v ;
    }) ;
    EXPECT_EQ(430, res) ;
}


