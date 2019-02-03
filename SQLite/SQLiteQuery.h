#pragma once

#include "SQLiteParameters.h"

/// This class manages SQL queries. B is a SQLOperationsList used at binding
/// and G is a SQLOperationsList used when getting the results.
/// It is a wrapper around an sql statement that is run with the given Bind
/// parameters and the results are read in the Get parameters.
template <typename B, typename G = null_type>
class SQLQuery
{
public:
    typedef B tBinder;
    typedef G tGetter;
    
    typedef typename B::tHead tBinderHead;
    typedef typename G::tHead tGetterHead;
    
    SQLQuery(SQLiteStatement &stmt) :
        mStatement(stmt)
    {
    }
    
    template <typename B_PT, typename G_PT>
    bool Run(SQLParameterPack<tBinderHead, B_PT> &bindParameters, 
        SQLParameterPack<tGetterHead, G_PT> &getParameters)
    {
        mStatement.Lock();
                
        if (!B::Bind(mStatement, bindParameters) ||
            !mStatement.ExecQuery() ||
            !G::Get(mStatement, getParameters))
        {
            mStatement.Clean();
            mStatement.Unlock();
            
            return false;
        }        
                
        mStatement.Clean();
        mStatement.Unlock();
        
        return true;
    }

    ~SQLQuery()
    {
    }

private:
    SQLiteStatement &mStatement;
};

/// Some queries have no input parameters, they only do get_column
template <typename G>
class SQLQuery<G, null_type>
{
public:
    typedef G tGetter;
    
    typedef typename G::tHead tGetterHead;
    
    SQLQuery(SQLiteStatement &stmt) :
        mStatement(stmt)
    {
    }
    
    template <typename G_PT>
    bool Run(SQLParameterPack<tGetterHead, G_PT> &getParameters)
    {
        mStatement.Lock();
                
        if (!mStatement.ExecQuery() ||
            !G::Get(mStatement, getParameters))
        {
            mStatement.Clean();
            mStatement.Unlock();
            
            return false;
        }        
                
        mStatement.Clean();
        mStatement.Unlock();
        
        return true;
    }

    ~SQLQuery()
    {
    }

private:
    SQLiteStatement &mStatement;
};



//TODO - test example, remove
static int test_query()
{
    int val1, val2, valo1;
    std::string str, stro;
    
    // do this once
    SQLiteStatement statement(NULL);
    
    typedef SQLQuery<SQLParameterTypesCons<int, int, std::string>::tListType,
					SQLParameterTypesCons<int, std::string>::tListType> tEmptyQuery;
    
    tEmptyQuery query(statement);
       
    // do this everytime you run the query
    SQLParameterPackCons<tEmptyQuery::tGetter> gl2(valo1, stro);
    SQLParameterPackCons<tEmptyQuery::tBinder> bl3(val2, val1, str);
    
    query.Run(bl3.mPack, gl2.mPack);
    
    return 0;
}

