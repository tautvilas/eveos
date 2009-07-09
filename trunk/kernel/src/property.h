#ifndef _PROPERTY_H_
#define _PROPERTY_H_


#define HAS_PROPERTIES(Owner) \
            template <class T> struct Property \
                    : public ::Generic::Property<T, Owner> \
            { Property(T v, Owner* o) : ::Generic::Property<T, Owner>(v, o) {} };


namespace Generic {
            

template <class Value, class Owner>
class Property
{
public:

    Property(Value value, Owner* owner) : mValue(value), mOwner(owner) {}
    
    Owner&
    operator ()(Value value)
    {
        mValue = value; 
        return *mOwner;
    }
    
    Value
    operator ()() const
    {
        return mValue;
    }
    
private:
    Value   mValue;
    Owner*  mOwner;
    
    void
    operator =(const Property<Value, Owner>&);
};


}   // namespace Generic

#endif  // _PROPERTY_H_

