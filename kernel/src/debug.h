#ifndef _DEBUG_H_
#define _DEBUG  _H_


#define DEBUG           1


#if DEBUG
#   include <out.h>
#   include <debug_str.h>
#   include <debug_int.h>
#   define DBG(expr)    { \
                            Out::dbg() << __FILE__ ":"  << __LINE__ \
                                    << ": " #expr " == "; \
                            debugDump(Out::dbg(), expr); \
                            Out::dbg() << '\n'; \
                        }

#   define ASSERT(expr) if (0 == (expr)) \
                        { \
                            Out::dbg() << __FILE__ ":" << __LINE__ \
                                    << ": FAILED ASSERT(" #expr ")\n"; \
                            kernel_stop(); \
                        }

#else
#   define DBG(expr)
#   define ASSERT(expr)
#endif

#endif // _DEBUG_H_
