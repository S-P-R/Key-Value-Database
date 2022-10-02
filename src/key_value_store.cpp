

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <string>

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include "key_value_store.h"

const int MAX_MESSAGE_LENGTH = 1024;

KeyValueStore::KeyValueStore(unsigned short int port_number)
{
   addrinfo *server;
   addrinfo hints {
      .ai_family = AF_UNSPEC,
      .ai_socktype = SOCK_STREAM,
      .ai_flags = AI_PASSIVE
   };

   std::string port_number_string = std::to_string(port_number);
   getaddrinfo(NULL, port_number_string.c_str(), &hints, &server);

   sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   if (bind(sockfd, server->ai_addr, server->ai_addrlen) < 0){
      perror("ERROR on binding");
      exit(1);
   }
   freeaddrinfo(server);
}

KeyValueStore::~KeyValueStore()
{
    close(sockfd);
}

void KeyValueStore::run()
{
   listen(sockfd, 5);
   struct sockaddr_storage their_addr;
   socklen_t addr_size = sizeof their_addr;
   int newsockfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

   if (newsockfd < 0){
      perror("ERROR on accept");
      exit(1);
   }
   
    std::string message = "";
    while (message != "END"){
        char buffer[MAX_MESSAGE_LENGTH];
        bzero(buffer, MAX_MESSAGE_LENGTH);
        int status = read(newsockfd,buffer, MAX_MESSAGE_LENGTH);
        if (status < 0){
            perror("ERROR reading from socket");
            exit(1);
        }
        message = buffer;
        std::string message_result = parse_message(message);

        printf("Here is the message: %s\n",buffer);

        status = write(newsockfd, message_result.c_str(), 
                       message_result.length());
        if (status < 0){
            perror("ERROR writing to socket");
            exit(1);
        } 
       // message = buffer;
    }
    
    close(newsockfd);
}

std::string KeyValueStore::parse_message(std::string message){
    try {
        // TODO - use switch statement?
        std::string return_message = "S";
        if (message.substr(0, 3) == "SET"){
            std::stringstream ss (message);
            ss.ignore(3, ' ');
            int key_length;
            ss >> key_length;

            // Ignores the space between the keylength and the key itself
            ss.ignore(1, ' ');
            char *key = new char [key_length - 1];
            ss.read(key, key_length);
            // Ignores the space between the key and value
            ss.ignore(1, ' ');
            int value_length = message.length() - ss.tellg();
            char *value = new char [value_length - 1];
            ss.read(value, value_length);
            std::cerr << "Key:" << std::string(key) << "\n";
            std::cerr << "Value: " << std::string(value) << "\n";
            database.insert(std::string(key), std::string(value));
            delete[] key;
            delete[] value;
        } else if (message.substr(0,3) == "GET"){
            std::string key = message.substr(4, message.length() - 4);
            return_message += database.search(key);
        } else if (message.substr(0,3) == "RMV"){
            std::string key = message.substr(4, message.length() - 4);
            database.remove(key);
        } else if (message.substr(0,3) == "CNS"){
            std::string key = message.substr(4, message.length() - 4);
            if (database.contains(key)){
                return_message += "FOUND";
            } else {
                return_message += "NOT-FOUND";
            }
        } else if (message.substr(0, 3) == "CLR"){
            database.clear();
        } 
        return return_message;
    } catch (std::exception& e){
        return std::string("F", e.what()); 
    }

}