#ifndef CLIENT
#define CLIENT

#include <iostream>

class Client {
    public:
        Client(std::string hostname, unsigned short int port_number);
        ~Client();
        
        void set(std::string key, std::string value);
        bool contains(std::string key);
        std::string get(std::string key);
        void remove(std::string key);
        void clear();

    private:
        int sockfd;
        
        std::string send_message(std::string message);
};
#endif
