// Endless Online Awaken

#include "config.hpp"

#include <fstream>
#include <iostream>
#include <string.h>
#include <stdexcept>

bool Config::initialized_ = false;
std::vector<Config::Entry> Config::entries;

Config::Config()
{
    if(!this->initialized_)
    {
        this->initialized_ = true;
    }
}

Config::Config(std::string filename)
{
    if(!this->initialized_)
    {
        this->initialized_ = true;
    }

    this->Load(filename);
}

void Config::Load(std::string filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        throw std::runtime_error("Config: could not load file.");
    }

    std::vector<std::string> lines;
    std::string line;
    while(std::getline(file, line))
    {
        lines.push_back(line);
    }

    file.close();

    while(lines.size() > 0)
    {
        if(lines[0][0] == '#')
        {
            lines.erase(lines.begin());
            continue;
        }

        std::string key = lines[0];
        lines.erase(lines.begin());

        if(lines.size() == 0) continue;

        std::string value = lines[0];
        lines.erase(lines.begin());

        if(value.size() > 0)
        {
            this->entries.push_back(Entry(key, value));
        }
    }
}

void Config::Save(std::string filename)
{
    std::ofstream file(filename, std::ios::trunc);
    if(!file.is_open())
    {
        throw std::runtime_error("Config: could not open file.");
    }
    else if(!file.good())
    {
        printf("Data stream error");
        throw std::runtime_error("Config: data stream error.");
    }

    for(auto &it : this->entries)
    {
        std::string data = '[' + it.key + '=' + it.value + ']' + '\n';
        const char *cdata = data.c_str();
        file.write(cdata, (unsigned)strlen(cdata));

    }
    file.close();
}

Config::Entry Config::GetEntry(std::string key)
{
    for(auto &it : this->entries)
    {
        if(it.key == key)
        {
            return it;
        }
    }

    return Entry("", "");
}

std::string Config::GetValue(std::string key)
{
    Entry entry = this->GetEntry(key);

    return entry.value;
}

void Config::AddEntry(Entry entry)
{
    if(this->GetEntry(entry.key).key == "")
    {
        this->entries.push_back(entry);
    }
}

void Config::AddEntry(std::string key, std::string value)
{
    this->AddEntry(Entry(key, value));
}
