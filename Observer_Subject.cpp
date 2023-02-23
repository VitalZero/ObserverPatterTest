#include "Observer_Subject.h"

Subject::Subject()
    : observers({}), numObservers(0)
{
}

void Subject::AddObserver(Observer* observer)
{
    if(numObservers < MAX_OBSERVERS)
    {
        observers[numObservers++] = observer;
    }
}

void Subject::removeObserver(Observer* observer)
{
    Observer* temp = observers[numObservers];

    for(int i = 0; i < numObservers; ++i)
    {
        if(observer == observers[i])
        {
            observers[numObservers--] = nullptr;
            observers[i] = temp;
            break;
        }
    }
}

void Subject::Notify(int event)
{
    for(int i = 0; i < numObservers; ++i)
    {
        observers[i]->OnNotify(event);
    }
}
