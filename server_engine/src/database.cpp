// Endless Online Awaken

#include "database.hpp"
#include <iostream>

bool Database::initialized_ = false;;
sqlite3 *Database::db = NULL;

Database::Database()
{
    if(!this->initialized_)
    {
        this->initialized_ = true;
    }
}

Database::Database(std::string name)
{
    if(!this->initialized_)
    {
        this->initialized_ = true;
    }

    this->Load(name);
}

void Database::Load(std::string name)
{
    int rc = sqlite3_open(name.c_str(), &this->db);

    if(rc)
    {
        std::string sqlite_msg = sqlite3_errmsg(this->db);
        throw std::runtime_error("Can't open database: " + sqlite_msg + '\n');
    }
    else
    {
        std::cout << "Database opened." << std::endl;
    }
}

void Database::Close()
{
    sqlite3_close(this->db);
}

sqlite3 *Database::Get()
{
    return db;
}

int Database::Execute(const char *sql, sqlite_callback callback, void *data)
{
    char *error_msg = 0;

    if(!this->db)
    {
        std::cout << "Error: no connection with database." << std::endl;
        return 0;
    }

    int ret = sqlite3_exec(this->db, sql, callback, data, &error_msg);

    if(ret != SQLITE_OK)
    {
        std::cout << "Error: " << error_msg << std::endl;
        //throw std::runtime_error(error_msg + '\n');
    }

    return ret;
}
