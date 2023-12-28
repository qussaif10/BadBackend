#include "server.h"


Server::Server(const utility::string_t &address) : listener(address), data(data) {
    std::cout << "Server starting on address: " << wideStringToNarrowString(address) << std::endl;
    listener.support(web::http::methods::POST, std::bind(&Server::handePOST, this, std::placeholders::_1));
    listener.support(web::http::methods::OPTIONS, [](web::http::http_request request) {
        std::cout << "Received OPTIONS request for CORS preflight." << std::endl;
        web::http::http_response response(web::http::status_codes::OK);
        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
        response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
        request.reply(response);
    });
    listener.open().wait();
    std::cout << "Server started and listening for requests on " << wideStringToNarrowString(address) << std::endl;
}

Server::~Server() {
    std::cout << "Server shutting down." << std::endl;
    listener.close().wait();
    std::cout << "Server shut down." << std::endl;
}

void Server::handePOST(const web::http::http_request &request) {
    std::cout << "Received POST request." << std::endl;
    request.extract_json()
            .then([this, request](web::json::value jsonPayload) {
                std::cout << "Extracting JSON payload." << std::endl;

                // Extract the data from the JSON payload
                utility::string_t username = jsonPayload[U("username")].as_string();
                utility::string_t password = jsonPayload[U("password")].as_string();
                std::cout << "Extracted username and password from payload." << std::endl;

                // Perform your processing (e.g., validate credentials)
                bool isValid = validateCredentials(username, password);
                std::cout << "Credentials validated: " << (isValid ? "true" : "false") << std::endl;

                // Prepare the JSON response
                web::json::value response;
                response[U("validCredentials")] = web::json::value::boolean(isValid);

                // Setting up the response headers for CORS
                web::http::http_response httpResponse;
                httpResponse.set_status_code(web::http::status_codes::OK);
                httpResponse.set_body(response);
                httpResponse.headers().add(U("Access-Control-Allow-Origin"), U("*"));
                // '*' allows all origins. Replace with 'http://localhost:5173' to be more specific

                std::cout << "Sending response back to client with CORS headers." << std::endl;
                return request.reply(httpResponse);
            })
            .then([](const pplx::task<void> &t) {
                try {
                    t.get();
                    std::cout << "Response sent successfully." << std::endl;
                }
                catch (const std::exception &e) {
                    std::cout << "Exception occurred: " << e.what() << std::endl;
                }
            });

}

bool Server::validateCredentials(const utility::string_t &username, const utility::string_t &password) {
    std::cout << "Validating credentials." << std::endl;
    std::cout << "Username: " << wideStringToNarrowString(username) << "Password: "
              << wideStringToNarrowString(password) << "\n";

    utility::string_t x = U("user1");
    utility::string_t y = U("password1");
    if (username == x && password == y) {
        std::cout << "yes";
        return true;
    }
    else {
        return false;
    }
}


std::string Server::wideStringToNarrowString(const utility::string_t &wstr) {
    if (wstr.empty()) return {};

    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, NULL, NULL);
    std::string str(bufferSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, NULL, NULL);

    return str;
}