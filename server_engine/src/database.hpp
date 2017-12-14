// Endless Online Awaken

#ifndef DATABASE_HPP_INCLUDED
#define DATABASE_HPP_INCLUDED

#include <sqlite3.h>
#include <string>

typedef int(*sqlite_callback)(void *, int, char **, char **);

class Database
{
private:
    static bool initialized_;
    static sqlite3 *db;

public:
    Database(std::string name);
    Database();
    void Load(std::string name);
    void Close();
    sqlite3 *Get();
    int Execute(const char *sql, sqlite_callback callback, void *data = 0);
};

#endif // DATABASE_HPP_INCLUDED
