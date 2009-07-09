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
    init();

    /**
     *  Must be called in kernel cleanup code.
     *
     *  Calls global/static objects destructors if any.
     */
    void
    deinit();
};

#endif // _CPP_RUNTIME_
