#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void setSystemMessage(const char* /*msg*/) {}

    virtual void appendSystemMessage(const char* /*msg*/) {}

    virtual void prependSystemMessage(const char* /*msg*/) {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
