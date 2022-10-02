#ifndef KEY_VALUE_STORE
#define KEY_VALUE_STORE

#include "hashtable/hashtable.h"


class KeyValueStore {
    public:
        KeyValueStore(unsigned short int port_number);
        ~KeyValueStore();
        
        void run();
    private:
        int sockfd;
        HashTable database;

        std::string parse_message(std::string message);
};
#endif
