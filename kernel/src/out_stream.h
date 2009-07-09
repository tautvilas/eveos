#ifndef _OUT_STREAM_H_
#define _OUT_STREAM_H_


class OutStream
{
public:
    
    virtual 
    ~OutStream();

    virtual void 
    write(char) = 0;
};


#endif // _OUT_STREAM_H_
