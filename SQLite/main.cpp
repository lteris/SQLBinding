#include "SQLiteQuery.h"


int main()
{
    int val1, val2, valo1;
    std::string str, stro;
    
    // do this once
    SQLiteStatement statement(NULL);
    
    typedef SQLQuery<SQLParameterTypesCons<int, int, std::string>::tListType,
					SQLParameterTypesCons<int, std::string>::tListType> tEmptyQuery;
    
    tEmptyQuery query(statement);
       
    // do this everytime you run the query
    SQLParameterPackCons<tEmptyQuery::tGetter> getterList_2(valo1, stro);
    SQLParameterPackCons<tEmptyQuery::tBinder> binderList_3(val2, val1, str);
    
    query.Run(binderList_3.mPack, getterList_2.mPack);
    
    return 0;
}