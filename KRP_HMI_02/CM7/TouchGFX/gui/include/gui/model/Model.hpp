#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void addSystemMessage(const char* msg);


protected:
    ModelListener* modelListener;

    bool hasNewMessage = false;
		char pendingMessage[64];
};

#endif // MODEL_HPP
