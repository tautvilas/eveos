#ifndef _MAYBE_H_
#define _MAYBE_H_


struct None 
{};


template <typename Value>
class Maybe
{
public:    
    Maybe(Value val) : mVal(val), mEmpty(false) {}
    Maybe(None) : mEmpty(true) {}
    Maybe() : mEmpty(true) {}

    bool 
    empty() const 
    { 
        return mEmpty;
    }

    Value 
    value(Value emptyVal = Value()) const
    {
        return empty() ? emptyVal : mVal;
    }

    Maybe<Value>& 
    operator =(None)
    {
        mEmpty  = true;
        return *this;
    }

    Maybe<Value>& 
    operator =(const Value& val)
    {
        mEmpty  = false;
        mVal    = val;
        return *this;
    }

    bool 
    operator !() const
    {
        return empty();
    }
    
    
    // gx 2009-07-09: Quote about ios::operator void*() from
    // Eckel/Allison's "Thinking in C++ Volume Two: Practical Programming"
    // (the footnote on page 167):
    // "It is customary to use operator void *() in preference to operator 
    // bool() because the implicit conversions from bool to int may cause 
    // surprises, should you incorrectly place a stream in a context where 
    // an integer conversion can be applied. The operator void*() function 
    // will only be called implicitly in the body of a Boolean expression."
    //
    // This trick makes even safer bool operator because it prevents 
    // any implicit conversion. Even to void*.
    struct bool_operator { bool mTrue; };
    typedef bool (bool_operator::*bool_but_safer);
    operator bool_but_safer() const 
    { 
        return empty() ? 0 : &bool_operator::mTrue; 
    }
    
private:
    Value mVal;
    bool mEmpty;
    
};


#endif

