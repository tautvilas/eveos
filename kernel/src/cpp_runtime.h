#ifndef _CPP_RUNTIME_
#define _CPP_RUNTIME_

namespace CppRuntime
{
    /**
     *  Must be called before any other code.
     *
     *  Calls global/static objects constructors if any.
     */
    void
    Init();

    /**
     *  Must be called in kernel cleanup code.
     *
     *  Calls global/static objects destructors if any.
     */
    void
    Deinit();
};

#endif // _CPP_RUNTIME_

