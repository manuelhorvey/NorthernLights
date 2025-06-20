#include "../include/infra/rest_server.hpp"
#include "../include/utils/logger.hpp"

RestServer::RestServer(int port) : port_(port), running_(false)
{
    setup_routes();
}

RestServer::~RestServer()
{
    stop();
}

void RestServer::setup_routes()
{
    server_.Get("/", [](const httplib::Request&, httplib::Response& res)
                { res.set_content("TheNorthernLights REST API is alive ", "text/plain"); });

    server_.Get("/health", [](const httplib::Request&, httplib::Response& res)
                { res.set_content("{\"status\":\"ok\"}", "application/json"); });

    // Add more API routes here, e.g., /status, /metrics, /orders etc.
}

void RestServer::start()
{
    if (running_)
        return;

    running_    = true;
    auto logger = Logger::get_instance();
    logger->info("🚀 Starting REST server on port {}", port_);

    server_thread_ = std::make_unique<std::thread>(
        [this]()
        {
            if (!server_.listen("0.0.0.0", port_))
            {
                auto logger = Logger::get_instance();
                logger->error(" Failed to start REST server on port {}", port_);
            }
        });
}

void RestServer::stop()
{
    if (!running_)
        return;

    running_ = false;
    server_.stop();

    if (server_thread_ && server_thread_->joinable())
    {
        server_thread_->join();
    }

    auto logger = Logger::get_instance();
    logger->info("🛑 REST server stopped.");
}
