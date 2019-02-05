#pragma once

#include "SysLock.h"

#include <string>
#include <vector>

#include "../sqlite/sqlite3.h"


class SQLiteStatement
{
public:
    SQLiteStatement(const char* stmt):
            mStatement(NULL), mStatementText(stmt)
    {
    }
    
    ~SQLiteStatement()
    {
        sqlite3_finalize(mStatement);
    }

    bool Prepare(sqlite3 *db);
    bool SQLiteBind(int position, int parameter);
    bool SQLiteBind(int position, const std::string &parameter);
    bool SQLiteBind(int position, const std::vector<char> &parameter);
    bool SQLiteColumn(int position, int &parameter);
    bool SQLiteColumn(int position, std::string &parameter);
    bool SQLiteColumn(int position, std::vector<char> &parameter);
    void Lock();
    void Unlock();
    bool ExecQuery();
    bool ExecUpdate();
    bool Clean();

private:
    sqlite3_stmt *mStatement;
    System::SysLock mRunLock;
    std::string mStatementText;
};
