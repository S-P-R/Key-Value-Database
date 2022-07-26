/* 
 * hashtable_tests.cpp
 *
 *             This file contains unit tests for the hashtable class
 * 
 * @Use:       Simply compile with the given Makefile and run with 
 *             './hashtable_tests'. A failed test is marked by a failed 
 *             assertion
 * 
 * Author:     Sean Reilly 
 * Date:       7/24/22
 */

#include <iostream>
#include <assert.h> 
#include "hashtable.h"

void insert_test();
void contains_test();
void expand_test();
void remove_test();
void clear_test();
void copy_constructor_test();
void assignment_overload_operator_test();

int main() 
{
    insert_test();
    std::cout << "Insert test completed\n";
    contains_test();
    std::cout << "Contains test completed\n";
    expand_test();
    std::cout << "Expand test completed\n";

    remove_test();
    std::cout << "Remove test completed\n";
    clear_test();
    std::cout << "Clear test completed\n";

    copy_constructor_test();
    std::cout << "Copy constructor test completed\n";
    assignment_overload_operator_test();
     std::cout << "Overloaded Assignment Operator test completed\n";
}

void insert_test()
{
    HashTable test;
    test.insert("key", "value");
    assert(test.search("key") == "value");
    test.insert("key", "new_value");
    assert(test.search("key") == "new_value");
    
    try {
        test.search("Does not exist");
    }
    catch (const std::exception &exc){
        std::string error_string = exc.what();
        assert(error_string == "Key not found in table\n");
    }
}

void contains_test()
{
    HashTable test;
    test.insert("key", "value");
    assert(test.contains("key"));
    assert(not test.contains("Does not exist"));
}

void expand_test()
{
    HashTable test;
    for (int i = 0; i < 4000; i++){
        test.insert(std::to_string(i), std::to_string(i));
    }

    for (int i = 0; i < 4000; i++){
        assert(test.search(std::to_string(i)) == std::to_string(i));
    }
}

void remove_test()
{
    HashTable test;
    test.insert("key", "value");
    assert(test.contains("key"));
    test.remove("key");
    assert(not test.contains("key"));

    try {
        test.remove("Does not exist");
    }
    catch (const std::exception &exc){
        std::string error_string = exc.what();
        assert(error_string == "Key not found in table\n");
    }
}

void clear_test()
{
    HashTable test;
    test.insert("entry1", "value1");
    test.insert("entry2", "value2");
    test.insert("entry3", "value3");
    assert(test.contains("entry1"));
    assert(test.contains("entry2"));
    assert(test.contains("entry3"));

    test.clear();
    assert(not test.contains("entry1"));
    assert(not test.contains("entry2"));
    assert(not test.contains("entry3"));
}

void copy_constructor_test()
{
    HashTable table1;
    table1.insert("entry1", "value1");
    table1.insert("entry2", "value2");

    // Check that copy constructor creates a deep copy
    HashTable table2 = table1;
    assert(table2.contains("entry1"));
    table2.remove("entry1");
    assert(not table2.contains("entry1"));
    assert(table1.contains("entry1"));

    HashTable table3 (table2);
    assert(table3.contains("entry2"));
}

void assignment_overload_operator_test()
{
    HashTable table1;
    table1.insert("entry1", "value1");
    table1.insert("entry2", "value2");

    HashTable table2;
    table2.insert("entry3", "value3");

    table2 = table1;
    assert(not table2.contains("entry3"));
    assert(table2.contains("entry1"));
    assert(table2.contains("entry2"));

    // Check that assignment overload operator creates a deep copy
    table2.remove("entry1");
    assert(table1.contains("entry1"));
}

