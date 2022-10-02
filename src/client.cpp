#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include "client.h"

// #include <iostream>

// // TODO - Check Necessaries

// #include <stdio.h>
// #include <stdlib.h> // Necessary?
// #include <unistd.h> // Necessary?
// #include <string.h>

// #include <cstring> // Necessary?

// #include <sys/types.h>
// #include <sys/socket.h>
// // #include <netinet/in.h>  // Necessary?
// #include <netdb.h> 

const int MAX_MESSAGE_LENGTH = 1024;

Client::Client(std::string hostname, unsigned short int port_number)
{
    int status;
    addrinfo *server_info;
    addrinfo hints {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM
    };
    std::string port_number_string = std::to_string(port_number);
    if ((status = getaddrinfo(hostname.c_str(), port_number_string.c_str(), 
                              &hints, &server_info)) != 0){
        throw std::runtime_error(std::string("Error getting address "
                                             "info of hostname: ") +
                                             std::string(gai_strerror(status)));
    }
    
    // TODO - add more error checking, look at Clients section of guide. 
    //        should iterate through address in server_info until connection
    //        works 
    
    sockfd = socket(server_info->ai_family, 
                    server_info->ai_socktype, server_info->ai_protocol);
    if (sockfd < 0) {
        throw std::runtime_error("Error opening socket");
    }


    if (connect(sockfd, server_info->ai_addr, server_info->ai_addrlen) < 0){
        throw std::runtime_error("Error opening socket");
    }
    freeaddrinfo(server_info);
}

Client::~Client()
{
    send_message("END");
    close(sockfd);
}

void Client::set(std::string key, std::string value)
{   
    std::string operation = "SET ";
    // TODO - leave comment explaining why key length is necessary
    send_message(operation + std::to_string(key.length()) + 
                 " " + key + " " + value);
}

bool Client::contains(std::string key)
{
    
    if (send_message("CNS " + key) == "FOUND"){
        return true;
    } else {
        return false;
    };
}

std::string Client::get(std::string key)
{
    return send_message("GET " + key);
}

void Client::remove(std::string key)
{
    send_message("RMV " + key);
}

void Client::clear()
{
    send_message("CLR");
}


std::string Client::send_message(std::string message)
{
    if (message.length() > MAX_MESSAGE_LENGTH) {
        throw std::runtime_error("Message length exceeds maximum");
    }
    char buffer[MAX_MESSAGE_LENGTH];
    bzero(buffer, MAX_MESSAGE_LENGTH);
    strcpy(buffer, message.c_str());
    int n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) {
        throw std::runtime_error("Error writing to socket");
    }
    bzero(buffer, MAX_MESSAGE_LENGTH);
    n = read(sockfd, buffer, MAX_MESSAGE_LENGTH);
    if (n < 0){
        throw std::runtime_error("Error reading from socket");
    }

    std::string result = buffer;
    //std::cerr << result;
    // result_contents excludes the part of the message that indicates errors
    std::string result_contents = result.substr(1, (result.length() - 1));
    // The first character of the result being 'S' indicates a
    // successful operation, 'F' indicates a failure
    if (result[0] == 'S'){
        return result_contents;
    } else {
        // In the case of failure, result_contents will contain a description 
        // of the error encountered 
        throw std::runtime_error("Key Value Store Encountered Error: " + 
                                 result_contents);
    }
    return buffer;
}
