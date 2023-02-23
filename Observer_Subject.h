class Observer
{
public:
    virtual ~Observer() = default;
    virtual void OnNotify(int event) = 0;
};

class Subject
{
public:
    Subject();
    virtual ~Subject() = default;
    void AddObserver(Observer* observer);
    void removeObserver(Observer* observer);

    void Notify(int event);

private:
    static constexpr int MAX_OBSERVERS = 10;
    Observer* observers[MAX_OBSERVERS];
    int numObservers;
};
