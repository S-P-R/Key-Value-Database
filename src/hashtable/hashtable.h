/* 
 * hashtable.h
 *
 *            Interface for a simple hashtable where both keys and
 *            values must be strings
 * 
 * Author:     Sean Reilly 
 * Date:       7/24/22
 */

#ifndef HASH_TABLE
#define HASH_TABLE

#include <iostream>

// TODO Add include guard

class HashTable {
    public:
        HashTable();
        HashTable(const HashTable &other);
        HashTable &operator =(const HashTable &right_side);
        ~HashTable();
        
        void insert(std::string key, std::string value);
        std::string search(std::string key);
        bool contains(std::string key);
        void remove(std::string key);
        void clear();
    private: 
        struct ChainNode {
            std::string key;
            std::string value;
            ChainNode *next; 
        };
        
        int table_size;
        int num_items;
        ChainNode **table;

        void expand();
};
#endif