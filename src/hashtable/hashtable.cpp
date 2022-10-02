/* 
 * hashtable.cpp
 *
 *            Implementation of a simple hashtable where both keys and
 *            values must be strings. Collisions are resolved through chaining
 * 
 * Author:     Sean Reilly 
 * Date:       7/24/22
 */

#include "hashtable.h"

const int INIT_TABLE_SIZE = 1024;

/* @Purpose: Default constructor
 */ 
HashTable::HashTable()
{
    table = new ChainNode *[INIT_TABLE_SIZE];
    table_size = INIT_TABLE_SIZE;
    num_items = 0;

    for (int i = 0; i < table_size; i++){
        table[i] = nullptr;
    }
}

/* @Purpose: Copy constructor, ensures deep copies are created
 */ 
HashTable::HashTable(const HashTable &other)
{   
    table = new ChainNode *[other.table_size];
    table_size = other.table_size;
    num_items = 0;

    for (int i = 0; i < other.table_size; i++){
        table[i] = nullptr;
        ChainNode *curr = other.table[i];
        while (curr != nullptr){
            insert(curr->key, curr->value);
            curr = curr->next;
        }
    }
}

/* @Purpose: Overloaded assigment operator, insures '=' creates deep copies
 */ 
HashTable &HashTable::operator =(const HashTable &right_side) 
{
    if (this != &right_side) {
        
        clear();
        delete table;
        
        table = new ChainNode *[right_side.table_size];
        table_size = right_side.table_size;
        num_items = 0;

        for (int i = 0; i < right_side.table_size; i++){
            table[i] = nullptr;
            ChainNode *curr = right_side.table[i];
            while (curr != nullptr){
                insert(curr->key, curr->value);
                curr = curr->next;
            }
        }
    }
    return *this;
}

/* @Purpose: Destructor, deallocates all memoery associated with table
 */ 
HashTable::~HashTable()
{
    clear();
    // This is slightly inefficient, as the table is re-allocated in the clear
    // function only to be deleted again, but the increase in modularity
    // seems worth it
    delete table;
}

/* @Purpose: Inserts a key-value pair into the table
 * @Args:    key:   The key to be added
 *           value: The value to be added
 * @Returns: None
 * @Notes:   If the key's already present in the table, its value is updated
 */ 
void HashTable::insert(std::string key, std::string value)
{
    std::hash<std::string> string_hash;
    int hash_index = string_hash(key) % table_size;
    
    ChainNode *curr = table[hash_index];
    ChainNode *prev = nullptr;
  
    while (curr != nullptr){
        if (key == curr->key){
            curr->value = value;
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    ChainNode *new_node = new ChainNode;
    new_node->key = key;
    new_node->value = value;
    new_node->next = nullptr;
    if (prev != nullptr){
        prev->next = new_node;
    } else {
        table[hash_index] = new_node;
    }

    num_items++;
    if ((float) num_items / (float) table_size > .75){
        expand();
    }
}

/* @Purpose: Searches for the value associated with the given key in amortized
 *           O(1) time
 * @Args:    key: The key that's value is to be retrieved 
 * @Returns: The string value associated with the given key
 * @Notes:   Will throw an exception if the given key isn't found in the table 
 */ 
std::string HashTable::search(std::string key)
{   std::cerr << "In Search\n";
    std::hash<std::string> string_hash;
    std::cerr << "After hash\n";
    int hash_index = string_hash(key) % table_size;
    
    ChainNode *curr = table[hash_index];
    while (curr != nullptr){
        if (key == curr->key){
            std::cerr << "Before returning\n";
            return curr->value;
        }
        curr = curr->next;
    }
    throw std::out_of_range("Key not found in table\n");
}

/* @Purpose: Expands the size of the hashtable when the load factor exceeds
 *           .7, ensuring that the average time complexities for operations 
 *           remain constant
 * @Args:    None
 * @Returns: None
 */ 
void HashTable::expand()
{
    int old_table_size = table_size;
    table_size = (table_size * 2) + 1;
    ChainNode **old_table = table;
    table = new ChainNode *[table_size];
    
     for (int i = 0; i < table_size; i++){
        table[i] = nullptr;
    }
    
    // Rehash contents of old table
    num_items = 0;
    for (int i = 0; i < old_table_size; i++){
        ChainNode *curr = old_table[i];
        while (curr != nullptr){
            insert(curr->key, curr->value);
            ChainNode *temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
    
    delete old_table;
}

/* @Purpose: Checks if key is in table 
 * @Args:    key: The key to be checked for
 * @Returns: A boolean that's true if the key is found in the table and false
 *           otherwise
 */ 
bool HashTable::contains(std::string key)
{
    std::hash<std::string> string_hash;
    int hash_index = string_hash(key) % table_size;
    ChainNode *curr = table[hash_index];
    while (curr != nullptr){
        if (key == curr->key){
            return true;
        }
        curr = curr->next;
    }
    return false;
}

/* @Purpose: Removes a key and it's associated value from the table
 * @Args:    key: The key to be removed
 * @Returns: None
 * @Notes:   Will throw an exception if the given key isn't found in the table 
 */ 
void HashTable::remove(std::string key)
{
    std::hash<std::string> string_hash;
    int hash_index = string_hash(key) % table_size;
    
    ChainNode *curr = table[hash_index];
    ChainNode *prev = nullptr;
    while (curr != nullptr){
        if (key == curr->key){
            if (prev != nullptr){
                prev->next = curr->next;
            } else {
                table[hash_index] = nullptr;
            }
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    throw std::out_of_range("Key not found in table\n");
}

/* @Purpose: Removes all key-value pairs from the table, freeing the memory
 *           they occupy
 * @Args:    None
 * @Returns: None
 * @Notes:   Does not change the size of the table
 */ 
void HashTable::clear()
{
    for (int i = 0; i < table_size; i++){
        ChainNode *curr = table[i];
        while (curr != nullptr){
            ChainNode *temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    delete table;
    table = new ChainNode *[INIT_TABLE_SIZE];
    table_size = INIT_TABLE_SIZE;
    for (int i = 0; i < table_size; i++){
        table[i] = nullptr;
    }
    num_items = 0;
}


