#include "../include/utils/config.hpp"
#include "../include/utils/logger.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool ConfigLoader::load(const std::string& filepath)
{
    std::ifstream file(filepath);
    auto          logger = Logger::get_instance();

    if (!file.is_open())
    {
        logger->error("❌ Failed to open config file: {}", filepath);
        return false;
    }

    json parsed;
    try
    {
        file >> parsed;
    }
    catch (const json::parse_error& e)
    {
        logger->error("❌ JSON parse error in {}: {}", filepath, e.what());
        return false;
    }

    flatten_json("", parsed);

    logger->info("✅ Loaded config from {}", filepath);
    return true;
}

void ConfigLoader::flatten_json(const std::string& prefix, const json& j)
{
    for (auto it = j.begin(); it != j.end(); ++it)
    {
        std::string full_key = join_keys(prefix, it.key());

        if (it->is_primitive())
        {
            if (it->is_string())
                configs[full_key] = it->get<std::string>();
            else if (it->is_boolean())
                configs[full_key] = it->get<bool>();
            else if (it->is_number_integer())
                configs[full_key] = it->get<int>();
            else if (it->is_number_float())
                configs[full_key] = it->get<double>();
            else
            {
                Logger::get_instance()->warn("⚠️ Unhandled JSON type for key '{}'", full_key);
            }
        }
        else if (it->is_object())
        {
            flatten_json(full_key, *it);
        }
    }
}

std::string ConfigLoader::join_keys(const std::string& prefix, const std::string& key) const
{
    return prefix.empty() ? key : prefix + "." + key;
}

template <typename T>
T ConfigLoader::get(const std::string& key, const T& default_val) const
{
    auto it = configs.find(key);
    if (it != configs.end())
    {
        try
        {
            return std::get<T>(it->second);
        }
        catch (const std::bad_variant_access&)
        {
            Logger::get_instance()->warn("⚠️ Type mismatch for config key '{}'", key);
        }
    }
    return default_val;
}

// Explicit template instantiations
template int         ConfigLoader::get<int>(const std::string&, const int&) const;
template double      ConfigLoader::get<double>(const std::string&, const double&) const;
template bool        ConfigLoader::get<bool>(const std::string&, const bool&) const;
template std::string ConfigLoader::get<std::string>(const std::string&, const std::string&) const;

void ConfigLoader::print() const
{
    auto logger = Logger::get_instance();
    logger->info("🧾 Config Values:");
    for (const auto& [key, val] : configs)
    {
        std::visit([&](auto&& v) { logger->info("  {} = {}", key, v); }, val);
    }
}
