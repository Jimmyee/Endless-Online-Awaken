// Endless Online Awaken v0.0.1

#include "config.hpp"

#include <fstream>
#include <cstdio>

Config::Config(std::string filename)
{
    this->Load(filename);
}

bool Config::Load(std::string filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::ate);
    if(!file.is_open())
    {
        printf("Config: Could not load file");
        return false;
    }

    std::streampos filesize;
    filesize = file.tellg();
    char *filedata = new char[filesize];
    file.seekg(0, std::ios::beg);
    file.read(filedata, filesize);
    file.close();

    std::string filedata_str(filedata);
    std::size_t pos = 0;

    do
    {
        pos = filedata_str.find_first_of('[');
        if(pos == std::string::npos) continue;

        std::size_t equation_pos = filedata_str.find_first_of('=');
        if(equation_pos == std::string::npos)
        {
            printf("Config: syntax error");
            break;
        }

        std::string keyword = filedata_str.substr(pos + 1, equation_pos - pos - 1);

        pos = filedata_str.find_first_of(']');
        if(pos == std::string::npos)
        {
            printf("Config: syntax error");
            break;
        }

        std::string value = filedata_str.substr(equation_pos + 1, pos - equation_pos - 1);
        filedata_str = filedata_str.substr(pos + 1);
        this->values[keyword] = value;
    } while(pos != std::string::npos);

    return true;
}
