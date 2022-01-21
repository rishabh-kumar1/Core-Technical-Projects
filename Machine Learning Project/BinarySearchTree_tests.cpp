// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;

TEST(test_insert) {
    // Add your tests here
    BinarySearchTree<int> tree;

    tree.insert(9);
    ASSERT_EQUAL(tree.size(), 1);
    ASSERT_EQUAL(tree.height(), 1);
    ASSERT_FALSE(tree.empty());
}

TEST(test_empty){
    BinarySearchTree<int> tree;

    ASSERT_TRUE(tree.empty());
    tree.insert(3);
    ASSERT_FALSE(tree.empty());
}

TEST(test_find1){
    BinarySearchTree<int> tree;

    tree.insert(9);

    ASSERT_EQUAL(tree.find(9), tree.begin());
}

TEST(test_find_begin){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(4);
    tree.insert(11);

    ASSERT_EQUAL(tree.find(4), tree.begin());
}

TEST(test_find_root){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(4);
    tree.insert(11);

    BinarySearchTree<int>::Iterator it = tree.begin();
    ++it;
    
    ASSERT_EQUAL(it, tree.find(9));
}

TEST(test_not_found){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(4);
    tree.insert(10);

    ASSERT_EQUAL(tree.end(), tree.find(11));
}

TEST(test_sort_invariant){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(test_preorder){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);;
    ASSERT_TRUE(oss_preorder.str() == "9 4 2 5 10 ");
}

TEST(test_inorder){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == "2 4 5 9 10 ");
}

TEST(test_min){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);

    ASSERT_TRUE(*tree.min_element() == 2);
}

TEST(test_max){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);

    ASSERT_TRUE(*tree.max_element() == 10);
}

TEST(test_min1){
    BinarySearchTree<int> tree;

    tree.insert(9);

    ASSERT_TRUE(*tree.min_element() == 9);
}

TEST(test_max1){
    BinarySearchTree<int> tree;

    tree.insert(9);

    ASSERT_TRUE(*tree.max_element() == 9);
}

TEST(test_min_greater_1){
    BinarySearchTree<int> tree;

    tree.insert(9);

    ASSERT_TRUE(*tree.min_greater_than(2) == 9);
}

TEST(test_min_greater_2){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);
    
    ASSERT_TRUE(*tree.min_greater_than(9) == 10);
}

TEST(test_min_greater_3){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);
    
    ASSERT_TRUE(*tree.min_greater_than(4) == 5);
}

TEST(test_min_greater_max){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(1);
    tree.insert(5);
    
    ASSERT_TRUE(tree.min_greater_than(10) == tree.end());
}

TEST(test_min_greater_neg){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(-1);
    tree.insert(5);
    
    ASSERT_TRUE(*tree.min_greater_than(-7) == -1);
}

TEST(test_min_greater_zero){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);
    
    ASSERT_TRUE(*tree.min_greater_than(0) == 2);
}

TEST(test_min_greater_large){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);
    
    ASSERT_EQUAL(tree.min_greater_than(100), tree.end());
}

TEST(test_min_greater_emp){
    BinarySearchTree<int> tree;
    
    ASSERT_TRUE(tree.min_greater_than(1) == tree.end());
}

TEST(test_min_greater_4){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);

    ASSERT_TRUE(*tree.min_greater_than(7) == 9);
}


TEST(test_assignment){
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(5);
    
    BinarySearchTree<int> tree1 = tree;
    tree1.insert(1);
    
    BinarySearchTree<int>::Iterator it = tree.begin();
    BinarySearchTree<int>::Iterator it2 = tree1.begin();
    ASSERT_FALSE(*it == *it2);
}

TEST_MAIN()
