// Endless Online Awaken

#include "file_handler.hpp"
#include "util.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <algorithm>

FileHandler::FileHandler(std::string filename)
{
    this->Load(filename);
}

bool FileHandler::Load(std::string filename)
{
    std::ifstream file;
    std::stringstream buffer;

    this->exists = false;

    file.open(filename, std::ifstream::binary);
    if(!file.is_open() || !file.good())
    {
        std::cout << "File handler: could not load input file." << std::endl;
        return false;
    }

    this->exists = true;

    buffer << file.rdbuf();

    file.close();

    this->data = buffer.str();

    return true;
}

void FileHandler::Save()
{
    if(this->filename.empty())
    {
        throw std::runtime_error("File handler: could not save file - empty file name given.");
    }

    std::ofstream file;

    file.open(this->filename, std::ofstream::trunc);
    if(!file.is_open() || !file.good())
    {
        throw std::runtime_error("Map: could not load output file.");
    }

    this->exists = true;

    file << this->data;

    file.close();
}

void FileHandler::Save(std::string filename)
{
    this->filename = filename;
    this->Save();
}

unsigned int FileHandler::GetNumber(std::size_t num_size)
{
    std::array<unsigned char, 4> bytes = { 254, 254, 254, 254 };

    std::copy_n(cbegin(this->data), std::min(num_size, this->data.size()), begin(bytes));

    this->data.erase(0, std::min(this->data.size(), num_size));

    return DecodeNumber(bytes[0], bytes[1], bytes[2], bytes[3]);
}

unsigned char FileHandler::GetChar()
{
    return this->GetNumber(1);
}

unsigned short FileHandler::GetShort()
{
    return this->GetNumber(2);
}

unsigned int FileHandler::GetThree()
{
    return this->GetNumber(3);
}

unsigned int FileHandler::GetInt()
{
    return this->GetNumber(4);
}

std::string FileHandler::GetString()
{
    std::string str = "";

    if(this->data.empty()) return str;

    std::size_t length = this->data.find_first_of((unsigned char)0xFF);

    if(length > this->data.size()) return str;

    str = this->data.substr(0, length);
    this->data.erase(0, length + 1);

    return str;
}

std::string FileHandler::GetData()
{
    return this->data;
}

void FileHandler::AddChar(unsigned char char_)
{
    this->data += EncodeNumber(char_)[0];
}

void FileHandler::AddShort(unsigned short num)
{
    this->data.append((char *)EncodeNumber(num).data(), 2);
}

void FileHandler::AddThree(unsigned int num)
{
    this->data.append((char *)EncodeNumber(num).data(), 3);
}

void FileHandler::AddInt(unsigned int num)
{
    this->data.append((char *)EncodeNumber(num).data(), 4);
}

void FileHandler::AddString(std::string str)
{
    this->data += str;
    this->data += (unsigned char)0xFF;
}
