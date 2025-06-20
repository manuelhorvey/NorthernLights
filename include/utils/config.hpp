#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <variant>

class ConfigLoader
{
   public:
    using ConfigValue = std::variant<int, double, bool, std::string>;
    using ConfigMap   = std::unordered_map<std::string, ConfigValue>;

    ConfigLoader() = default;

    bool load(const std::string& filepath);

    template <typename T>
    T get(const std::string& key, const T& default_val = T()) const;

    void print() const;

   private:
    ConfigMap configs;

    void        flatten_json(const std::string& prefix, const nlohmann::json& j);
    std::string join_keys(const std::string& prefix, const std::string& key) const;
};

// Explicit template instantiations
extern template int         ConfigLoader::get<int>(const std::string&, const int&) const;
extern template double      ConfigLoader::get<double>(const std::string&, const double&) const;
extern template bool        ConfigLoader::get<bool>(const std::string&, const bool&) const;
extern template std::string ConfigLoader::get<std::string>(const std::string&,
                                                           const std::string&) const;
