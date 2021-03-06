#ifndef OBSERVATOR_H
#define OBSERVATOR_H

#include <json.hpp>

class Observer
{
public:
    virtual void update(nlohmann::json)  = 0;
};

#endif // OBSERVATOR_H
