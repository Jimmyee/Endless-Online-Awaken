// Endless Online Awaken

#ifndef FILE_HANDLER_HPP_INCLUDED
#define FILE_HANDLER_HPP_INCLUDED

#include <string>

class FileHandler
{
private:
    std::string filename;
    std::string data;
    bool exists;

public:
    FileHandler() { this->filename = ""; this->data = ""; this->exists = false; }
    FileHandler(std::string filename);
    bool Load(std::string filename);
    void Save();
    void Save(std::string filename);
    bool Exists() { return this->exists; }

    unsigned int GetNumber(std::size_t num_size);
    unsigned char GetChar();
    unsigned short GetShort();
    unsigned int GetThree();
    unsigned int GetInt();
    std::string GetString();
    std::string GetData();

    void AddChar(unsigned char char_);
    void AddShort(unsigned short num);
    void AddThree(unsigned int num);
    void AddInt(unsigned int num);
    void AddString(std::string str);
};

#endif // FILE_HANDLER_HPP_INCLUDED
