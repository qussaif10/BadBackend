#ifndef SERVER_H
#define SERVER_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <unordered_map>
#include <string>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <iostream>
#include <csignal>
#include <future>

class Server
{
public:
    explicit Server(const utility::string_t &address);
    ~Server();

private:
    web::http::experimental::listener::http_listener listener;
    std::unordered_map<std::string, std::string> data;

    void handePOST(const web::http::http_request& request);
    bool validateCredentials(const utility::string_t &username, const utility::string_t &password);
    std::string wideStringToNarrowString(const utility::string_t &wstr);
};

#endif