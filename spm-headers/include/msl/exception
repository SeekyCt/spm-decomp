/*
    Based on the decompilation in xenoblade by CelestialAmber
*/

#pragma once

// TODO: does this actually need to be hidden for modding?
#ifdef DECOMP

namespace std {

class exception
{
public:
    exception(){}

    virtual ~exception(){}

    virtual const char* what() const
    {
        return "exception";
    }
};

class bad_exception : public exception
{
public:
    bad_exception(){}

    virtual ~bad_exception(){}

    virtual const char* what() const
    {
        return "bad_exception";
    }
};

typedef void (*unexpected_handler)();
unexpected_handler set_unexpected(unexpected_handler handler);
void unexpected();

typedef void (*terminate_handler)();
terminate_handler set_terminate(terminate_handler handler);
void terminate();

}

#endif
