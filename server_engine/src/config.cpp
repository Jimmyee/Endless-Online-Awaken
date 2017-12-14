// Endless Online Awaken

#include "config.hpp"

#include <fstream>
#include <iostream>
#include <string.h>
#include <stdexcept>

Config::Config()
{

}

Config::Config(std::string filename)
{
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

    std::size_t pos = 0;
    for(auto &it : lines)
    {
        line = it;
        pos = line.find_first_of('[');
        if(pos == std::string::npos) continue;

        std::size_t equation_pos = line.find_first_of('=');
        if(equation_pos == std::string::npos)
        {
            throw std::runtime_error("Config: syntax error.");
        }

        std::string key = line.substr(pos + 1, equation_pos - pos - 1);

        pos = line.find_first_of(']');
        if(pos == std::string::npos)
        {
            throw std::runtime_error("Config: syntax error.");
        }

        std::string value = line.substr(equation_pos + 1, pos - equation_pos - 1);
        this->entries.push_back(Entry(key, value));
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
