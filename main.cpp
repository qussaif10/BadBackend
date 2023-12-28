#include "server.h"

std::promise<void> exitSignal;

void signalHandler(int signum) {
    exitSignal.set_value();
}

int main() {
    Server server(U("http://localhost:8001/login"));
    std::cout << "Server is running..." << std::endl;

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    exitSignal.get_future().wait();

    return 0;
}