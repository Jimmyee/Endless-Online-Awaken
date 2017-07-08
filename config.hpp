// Endless Online Bot v0.0.1

#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED

#include <string>
#include <vector>

class Config
{
public:
    struct Entry
    {
        std::string key;
        std::string value;

        Entry(std::string key, std::string value) { this->key = key; this->value = value; }
    };
public:
    std::vector<Entry> entries;

    Config();
    Config(std::string filename);
    bool Load(std::string filename);
    void Save(std::string filename);
    Entry GetEntry(std::string key);
    std::string GetValue(std::string key);
};

#endif // CONFIG_HPP_INCLUDED
