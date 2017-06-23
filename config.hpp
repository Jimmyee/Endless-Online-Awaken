// Endless Online Awaken v0.0.1

#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED

#include <string>
#include <unordered_map>

class Config
{
public:
    std::unordered_map<std::string, std::string> values;

    Config(std::string filename);
    bool Load(std::string filename);
};

#endif // CONFIG_HPP_INCLUDED
