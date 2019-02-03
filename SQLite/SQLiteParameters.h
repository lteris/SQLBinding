#pragma once

#include "SQLiteStatement.h"

//---------------------------------------------------------------------------------------
// CLASS DECLARATION
//---------------------------------------------------------------------------------------
/// This is a dummy type that is used to mark the end of typelists.
class null_type
{
};

//---------------------------------------------------------------------------------------
// CLASS DECLARATION
//---------------------------------------------------------------------------------------
/// SQLParameterPack delares a list of parameters that will be passed to a query for bind
/// and column get operations.
/// It contains references to the objects that are bound or used to return values so it is
/// dependent on the lifetime of the objects that it refers to.
/// 
template<typename H, typename T>
struct SQLParameterPack
{
    H &mHead;
    T &mTail;

    SQLParameterPack(H &h, T &t) :
        mHead(h), mTail(t)
    {
    }
};

template<typename H>
struct SQLParameterPack<H, null_type>
{
    H &mHead;

    SQLParameterPack(H &h) :
        mHead(h)
    {
    }
};

/// SQLParameterTypes delares a typelist for the parameters that are passed to a 
/// query (bind and get_column).
///
template<typename H, typename T>
class SQLParameterTypes
{
public:
    typedef H tHead;
    typedef T tTail;
    
    static int Count()
    {
        return 1 + T::Count();
    }

    template<typename PT>
    static bool Get(SQLiteStatement &stmt, SQLParameterPack<H, PT> &parameters)
    {
        int position = Count();
       
        return stmt.SQLiteColumn(position, parameters.mHead) && T::Get(stmt, parameters.mTail);
    }
    
    template<typename PT>
    static bool Bind(SQLiteStatement &stmt, SQLParameterPack<H, PT> &parameters)
    {
        int position = Count();
        
        return stmt.SQLiteBind(position, parameters.mHead) && T::Bind(stmt, parameters.mTail);
    }
};

template<typename H>
class SQLParameterTypes<H, null_type>
{
public:
    typedef H tHead;
    typedef null_type tTail;
    
    static int Count()
    {
        return 0;
    }

    static bool Get(SQLiteStatement &stmt, SQLParameterPack<H, null_type> &parameters)
    {
        int position = Count();

        return stmt.SQLiteColumn(position, parameters.mHead);
    }

    static bool Bind(SQLiteStatement &stmt, SQLParameterPack<H, null_type> &parameters)
    {
        int position = Count();

        return stmt.SQLiteBind(position, parameters.mHead);
    }
};


/// Helper struct to construct SQLParameterTypes classes from the template.
template<typename T0, typename T1 = null_type, typename T2 = null_type, typename T3 = null_type,
    typename T4 = null_type, typename T5 = null_type, typename T6 = null_type, 
    typename T7 = null_type, typename T8 = null_type, typename T9 = null_type>
struct SQLParameterTypesCons
{
    typedef SQLParameterTypes<T0, typename SQLParameterTypesCons<T1, T2, T3, T4, T5, T6, T7, T8, T9>::tListType> tListType;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
    typename T5, typename T6, typename T7, typename T8>
struct SQLParameterTypesCons<T0, T1, T2, T3, T4, T5, T6, T7, T8, null_type>
{
    typedef SQLParameterTypes<T0, typename SQLParameterTypesCons<T1, T2, T3, T4, T5, T6, T7, T8>::tListType> tListType;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
    typename T5, typename T6, typename T7>
struct SQLParameterTypesCons<T0, T1, T2, T3, T4, T5, T6, T7, null_type, null_type>
{
    typedef SQLParameterTypes<T0, typename SQLParameterTypesCons<T1, T2, T3, T4, T5, T6, T7>::tListType> tListType;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
    typename T5, typename T6>
struct SQLParameterTypesCons<T0, T1, T2, T3, T4, T5, T6, null_type, null_type, null_type>
{
    typedef SQLParameterTypes<T0, typename SQLParameterTypesCons<T1, T2, T3, T4, T5, T6>::tListType> tListType;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4,
    typename T5>
struct SQLParameterTypesCons<T0, T1, T2, T3, T4, T5, null_type, null_type, null_type, null_type>
{
    typedef SQLParameterTypes<T0, typename SQLParameterTypesCons<T1, T2, T3, T4, T5>::tListType> tListType;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct SQLParameterTypesCons<T0, T1, T2, T3, T4, null_type, null_type, null_type, null_type, null_type>
{
    typedef SQLParameterTypes<T0, typename SQLParameterTypesCons<T1, T2, T3, T4>::tListType> tListType;
};

template<typename T0, typename T1, typename T2, typename T3>
struct SQLParameterTypesCons<T0, T1, T2, T3, null_type, null_type, null_type, null_type, 
                        null_type, null_type>
{
    typedef SQLParameterTypes<T0, typename SQLParameterTypesCons<T1, T2, T3>::tListType> tListType;
};

template<typename T0, typename T1, typename T2>
struct SQLParameterTypesCons<T0, T1, T2, null_type, null_type, null_type, null_type, 
                        null_type, null_type, null_type>
{
    typedef SQLParameterTypes<T0, typename SQLParameterTypesCons<T1, T2>::tListType> tListType;
};

template<typename T0, typename T1>
struct SQLParameterTypesCons<T0, T1, null_type, null_type, null_type, null_type, 
                        null_type, null_type, null_type, null_type>
{
    typedef SQLParameterTypes<T0, typename SQLParameterTypesCons<T1>::tListType> tListType;
};

template<typename T0>
struct SQLParameterTypesCons<T0, null_type, null_type, null_type, null_type, 
                       null_type, null_type, null_type, null_type, null_type>
{
    typedef SQLParameterTypes<T0, null_type> tListType;
};


/// Helper struct to construct SQLParameterPack objects that match the types from a given
/// SQLParameterTypes. The constructor takes up to 10 parameters and the types T0, T1 .. T9
/// will have to match the ones in the SQLParameterTypes given by PT.
template<typename PT>
struct SQLParameterPackCons
{
    typedef SQLParameterPack<typename PT::tHead, typename SQLParameterPackCons<typename PT::tTail>::tPack> tPack;
        
    SQLParameterPackCons<typename PT::tTail> mConsPack;
    tPack mPack;
    
    template <typename T0, typename T1, typename T2, typename T3, typename T4,
              typename T5, typename T6, typename T7, typename T8, typename T9>
    SQLParameterPackCons(T0 &t0, T1 &t1, T2 &t2, T3 &t3, T4 &t4, T5 &t5, T6 &t6, T7 &t7, T8 &t8, T9 &t9)
        :mConsPack(t1, t2, t3, t4, t5, t6, t7, t8, t9), mPack(t0, mConsPack.mPack)
    {}
    
    template <typename T0, typename T1, typename T2, typename T3, typename T4,
              typename T5, typename T6, typename T7, typename T8>
    SQLParameterPackCons(T0 &t0, T1 &t1, T2 &t2, T3 &t3, T4 &t4, T5 &t5, T6 &t6, T7 &t7, T8 &t8)
        :mConsPack(t1, t2, t3, t4, t5, t6, t7, t8), mPack(t0, mConsPack.mPack)
    {}
    
    template <typename T0, typename T1, typename T2, typename T3, typename T4,
              typename T5, typename T6, typename T7>
    SQLParameterPackCons(T0 &t0, T1 &t1, T2 &t2, T3 &t3, T4 &t4, T5 &t5, T6 &t6, T7 &t7)
        :mConsPack(t1, t2, t3, t4, t5, t6, t7), mPack(t0, mConsPack.mPack)
    {}
    
    template <typename T0, typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6>
    SQLParameterPackCons(T0 &t0, T1 &t1, T2 &t2, T3 &t3, T4 &t4, T5 &t5, T6 &t6)
        :mConsPack(t1, t2, t3, t4, t5, t6), mPack(t0, mConsPack.mPack)
    {}
    
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    SQLParameterPackCons(T0 &t0, T1 &t1, T2 &t2, T3 &t3, T4 &t4, T5 &t5)
        :mConsPack(t1, t2, t3, t4, t5), mPack(t0, mConsPack.mPack)
    {}
    
    template <typename T0, typename T1, typename T2, typename T3, typename T4>
    SQLParameterPackCons(T0 &t0, T1 &t1, T2 &t2, T3 &t3, T4 &t4)
        :mConsPack(t1, t2, t3, t4), mPack(t0, mConsPack.mPack)
    {}
    
    template <typename T0, typename T1, typename T2, typename T3>
    SQLParameterPackCons(T0 &t0, T1 &t1, T2 &t2, T3 &t3):mConsPack(t1, t2, t3), mPack(t0, mConsPack.mPack)
    {}
    
    template <typename T0, typename T1, typename T2>
    SQLParameterPackCons(T0 &t0, T1 &t1, T2 &t2):mConsPack(t1, t2), mPack(t0, mConsPack.mPack)
    {}
    
    template <typename T0, typename T1>
    SQLParameterPackCons(T0 &t0, T1 &t1):mConsPack(t1), mPack(t0, mConsPack.mPack)
    {}
    
    template <typename T0>
    SQLParameterPackCons(T0 &t0):mPack(t0)
    {}
};

template<>
struct SQLParameterPackCons<null_type>
{
    typedef null_type tPack;
    tPack mPack;
};

