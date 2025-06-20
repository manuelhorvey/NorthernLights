#pragma once

#include <atomic>
#include <httplib.h>
#include <memory>
#include <string>
#include <thread>

class RestServer
{
   public:
    RestServer(int port = 8080);
    ~RestServer();

    void start();
    void stop();

   private:
    void setup_routes();

    int                          port_;
    std::unique_ptr<std::thread> server_thread_;
    std::atomic<bool>            running_;

    httplib::Server server_;
};
