
#include "SQLiteStatement.h"

bool SQLiteStatement::Prepare(sqlite3 *db)
{
    return (sqlite3_prepare_v2(db, mStatementText.c_str(), -1, &mStatement, NULL)
        == SQLITE_OK);
}

bool SQLiteStatement::SQLiteBind(int position, int parameter)
{
    return sqlite3_bind_int(mStatement, position, parameter) == SQLITE_OK;
}


bool SQLiteStatement::SQLiteBind(int position, const std::string &parameter)
{
    return sqlite3_bind_text(mStatement, position, parameter.c_str(),
        -1, SQLITE_STATIC) == SQLITE_OK;
}

bool SQLiteStatement::SQLiteBind(int position, const std::vector<char> &parameter)
{
    return sqlite3_bind_blob(mStatement, position, &parameter[0], 
        parameter.size(), SQLITE_STATIC) == SQLITE_OK;
}


bool SQLiteStatement::SQLiteColumn(int position, int &parameter)
{
    parameter = sqlite3_column_int(mStatement, position);
    return true;
}

bool SQLiteStatement::SQLiteColumn(int position, std::string &parameter)
{
    parameter = std::string(reinterpret_cast<const char*>
                (sqlite3_column_text(mStatement, position)));
    return true;
}

bool SQLiteStatement::SQLiteColumn(int position, std::vector<char> &parameter)
{
    const char *bytes = static_cast<const char*>(sqlite3_column_blob(mStatement, position));
    int bytesCount =  sqlite3_column_bytes(mStatement, position);
    
    parameter = std::vector<char>(bytes, bytes + bytesCount);
    return true;
}

void SQLiteStatement::Lock()
{
    mRunLock.Lock();
}

void SQLiteStatement::Unlock()
{
    mRunLock.Unlock();
}

bool SQLiteStatement::ExecQuery()
{
    return sqlite3_step(mStatement) == SQLITE_ROW;
}

bool SQLiteStatement::ExecUpdate()
{
    return sqlite3_step(mStatement) == SQLITE_DONE;
}
        
bool SQLiteStatement::Clean()
{
    (void)sqlite3_clear_bindings(mStatement);
    return (sqlite3_reset(mStatement) == SQLITE_OK);
}
