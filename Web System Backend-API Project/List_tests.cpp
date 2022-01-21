// Project UID c1f28c309e55405daf00c565d57ff9ad

#include "List.h"
#include <string>
#include "unit_test_framework.h"

using namespace std;

// Add your test cases here

TEST(test_push_back) {
    // Add test code here
    List<int> list1;
    list1.push_back(4);
    list1.push_back(3);
    list1.push_back(19);
    List<int>::Iterator it = list1.begin();
    ASSERT_EQUAL(*it, 4);
    ++it;
    ASSERT_EQUAL(*it, 3);
    ++it;
    ASSERT_EQUAL(*it, 19);
    ASSERT_EQUAL(list1.front(), 4);
    ASSERT_EQUAL(list1.back(), 19);
}

TEST(test_push_front){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    List<int>::Iterator it = list1.begin();
    ASSERT_EQUAL(*it, 19);
    ++it;
    ASSERT_EQUAL(*it, 3);
    ++it;
    ASSERT_EQUAL(*it, 4);
    ASSERT_EQUAL(list1.front(), 19);
    ASSERT_EQUAL(list1.back(), 4);
}

TEST(test_push_front_emp){
    List<int> list1;
    list1.push_front(4);
    List<int>::Iterator it = list1.begin();
    ASSERT_EQUAL(*it, 4);
    ASSERT_EQUAL(list1.front(), 4);
    ASSERT_EQUAL(list1.back(), 4);
}

TEST(test_push_back_emp){
    List<int> list1;
    list1.push_back(4);
    List<int>::Iterator it = list1.begin();
    ASSERT_EQUAL(*it, 4);
    ASSERT_EQUAL(list1.front(), 4);
    ASSERT_EQUAL(list1.back(), 4);
}

TEST(test_push_front_1){
    List<int> list1;
    list1.push_back(3);
    list1.push_front(4);
    List<int>::Iterator it = list1.begin();
    ASSERT_EQUAL(*it, 4);
    ++it;
    ASSERT_EQUAL(*it, 3);
    ASSERT_EQUAL(list1.front(), 4);
    ASSERT_EQUAL(list1.back(), 3);
}

TEST(test_push_back_1){
    List<int> list1;
    list1.push_front(3);
    list1.push_back(4);
    List<int>::Iterator it = list1.begin();
    ASSERT_EQUAL(*it, 3);
    ++it;
    ASSERT_EQUAL(*it, 4);
    ASSERT_EQUAL(list1.front(), 3);
    ASSERT_EQUAL(list1.back(), 4);
}

TEST(test_pop_back){
    List<int> list1;
    list1.push_back(4);
    list1.push_back(3);
    list1.push_back(19);
    list1.pop_back();
    ASSERT_EQUAL(list1.back(), 3);
    ASSERT_EQUAL(list1.front(), 4);
}

TEST(test_pop_front){
    List<int> list1;
    list1.push_back(4);
    list1.push_back(3);
    list1.push_back(19);
    list1.pop_front();
    List<int>::Iterator it = list1.begin();
    ASSERT_EQUAL(*it, 3);
    ++it;
    ASSERT_EQUAL(*it, 19);
}

TEST(test_pop_back_1){
    List<int> list1;
    list1.push_front(4);
    ASSERT_EQUAL(list1.size(), 1);
    ASSERT_EQUAL(list1.front(), 4);
    ASSERT_EQUAL(list1.back(), 4);
    list1.pop_back();
    ASSERT_TRUE(list1.size() == 0);
    ASSERT_TRUE(list1.empty());
}

TEST(test_pop_front_1){
    List<int> list1;
    list1.push_front(4);
    ASSERT_EQUAL(list1.size(), 1);
    ASSERT_EQUAL(list1.front(), 4);
    ASSERT_EQUAL(list1.back(), 4);
    list1.pop_front();
    ASSERT_TRUE(list1.size() == 0);
    ASSERT_TRUE(list1.empty());
}

TEST(test_empty){
    List<int> list1;
    list1.push_front(5);
    ASSERT_FALSE(list1.empty());
}

TEST(test_size){
    List<int> list1;
    list1.push_back(5);
    list1.push_back(10);
    ASSERT_EQUAL(list1.size(), 2);
}

TEST(test_front){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    ASSERT_EQUAL(list1.front(), 19);
}

TEST(test_back){
    List<int> list1;
    list1.push_back(4);
    list1.push_back(3);
    list1.push_back(19);    
    ASSERT_EQUAL(list1.back(), 19);
}

TEST(test_clear){
    List<int> list1;
    list1.push_back(5);
    list1.push_back(10);
    list1.push_back(15);
    list1.clear();
    ASSERT_TRUE(list1.empty());
}

TEST(it_equals){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    List<int>::Iterator it = list1.begin();
    List<int>::Iterator it2 = list1.begin();
    ASSERT_TRUE(it == it2);
    ASSERT_FALSE(it != it2);
}

TEST(it_not_equals){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    List<int>::Iterator it = list1.begin();
    List<int>::Iterator it2 = list1.begin();
    ++it2;
    ASSERT_TRUE(it != it2);
    ASSERT_FALSE(it == it2);
}

TEST(test_erase){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    List<int>::Iterator it = list1.begin();
    ++it;
    list1.erase(it);
    List<int>::Iterator it2 = list1.begin();
    ASSERT_EQUAL(*it2, 19);
    ++it2;
    ASSERT_EQUAL(*it2, 4);
}

TEST(test_erase_1){
    List<int> list1;
    list1.push_front(4);
    List<int>::Iterator it = list1.begin();
    list1.erase(it);
    ASSERT_TRUE(list1.empty());
}

TEST(test_erase_last){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    list1.push_front(21);
    List<int>::Iterator it = list1.begin();
    ++it;
    ++it;
    ++it;
    list1.erase(it);
    List<int>::Iterator it2 = list1.begin();
    ASSERT_EQUAL(*it2, 21);
    ++it2;
    ASSERT_EQUAL(*it2, 19);
    ++it2;
    ASSERT_EQUAL(*it2, 3);
    ASSERT_EQUAL(list1.back(), 3);
}

TEST(test_insert){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    List<int>::Iterator it = list1.begin();
    ++it;
    list1.insert(it, 43);
    List<int>::Iterator it2 = list1.begin();
    ASSERT_EQUAL(*it2, 19);
    ++it2;
    ASSERT_EQUAL(*it2, 43);
    ++it2;
    ASSERT_EQUAL(*it2, 3);
    ++it2;
    ASSERT_EQUAL(*it2, 4);
}

TEST(test_insert_emp){
    List<int> list1;
    List<int>::Iterator it = list1.begin();
    list1.insert(it, 43);
    ASSERT_EQUAL(list1.front(), 43);
    ASSERT_EQUAL(list1.back(), 43);
}

TEST(test_insert_whole){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    list1.push_back(23);
    List<int>::Iterator it = list1.begin();
    list1.insert(it, 43);
    ASSERT_EQUAL(list1.front(), 43);
    List<int>::Iterator it2 = list1.begin();
    ASSERT_EQUAL(*it2, 43);
    ++it2;
    ASSERT_EQUAL(*it2, 19);
    ++it2;
    ASSERT_EQUAL(*it2, 3);
    ++it2;
    ASSERT_EQUAL(*it2, 4);
    ++it2;
    ASSERT_EQUAL(*it2, 23);
}

TEST(test_insert_end){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    List<int>::Iterator it = list1.begin();
    ++it;
    ++it;
    ++it;
    list1.insert(it, 43);
    ASSERT_EQUAL(list1.back(), 43);
    List<int>::Iterator it2 = list1.begin();
    ASSERT_EQUAL(*it2, 19);
    ++it2;
    ASSERT_EQUAL(*it2, 3);
    ++it2;
    ASSERT_EQUAL(*it2, 4);
    ++it2;
    ASSERT_EQUAL(*it2, 43);
}

TEST(test_add){
    List<int> list1;
    list1.push_back(2);
    list1.push_front(5);
    list1.push_front(4);
    list1.push_back(7);
    ASSERT_EQUAL(list1.back(), 7);
    ASSERT_EQUAL(list1.front(), 4);
    List<int>::Iterator it2 = list1.begin();
    ASSERT_EQUAL(*it2, 4);
    ++it2;
    ASSERT_EQUAL(*it2, 5);
    ++it2;
    ASSERT_EQUAL(*it2, 2);
    ++it2;
    ASSERT_EQUAL(*it2, 7);
}

TEST(test_itr_begin){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    List<int>::Iterator it = list1.begin();
    ASSERT_EQUAL(*it, 19);
}

TEST(test_itr_end){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    for(List<int>::Iterator it = list1.begin(); it != list1.end(); ++it){
        ASSERT_EQUAL(*it, *it);
    }
}

TEST(it_minus){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    List<int>::Iterator it = list1.begin();
    ++it;
    ++it;
    --it;
    ASSERT_EQUAL(*it, 3);
}

TEST(test_itr_comp){
     List<int> list1;
     list1.push_front(4);
     list1.push_front(3);
     list1.push_front(19);
     List<int> list2 = list1;
     list2.push_back(10);
     ASSERT_FALSE(list2.back() == list1.back());
}

TEST(test_copy_ctor){
    List<int> list1;
    list1.push_front(4);
    list1.push_front(3);
    list1.push_front(19);
    List<int> list2(list1);
    list2.push_front(23);
    ASSERT_FALSE(list1.front() == list2.front());
}

TEST_MAIN()
