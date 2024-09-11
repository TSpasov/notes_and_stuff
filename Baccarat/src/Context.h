#ifndef CONTEXT_H
#define CONTEXT_H

#include <functional> 

class Context
{
public:
    friend class State;

    Context();

std::function<void(const std::string)> ChaneState;
private:

};

#endif // CONTEXT_H
