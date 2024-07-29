#include <iostream>
#include <vector>

struct IObserver {
    virtual ~IObserver() = default;

    virtual void OnStart() = 0;
    virtual void OnStop() = 0;
};

class Observer : public IObserver {
public:
    void OnStart() override {
        std::cout << "task started" << std::endl;
    }

    void OnStop() override {
        std::cout << "task stopped" << std::endl;
    }
};

class Observer2 : public IObserver {
public:
    void OnStart() override {
        std::cout << "TASK STARTED" << std::endl;
    }

    void OnStop() override {
        std::cout << "TASK STOPPED" << std::endl;
    }
};

class Observed {
public:
    void AddObserver(IObserver* observer) {
        observers_.push_back(observer);
    }

    void Start() {
        for (auto observer : observers_) {
            observer->OnStart();
        }
    }

    void Stop() {
        for (auto observer : observers_) {
            observer->OnStop();
        }
    }

private:
    std::vector<IObserver*> observers_;
};

int main() {
    Observer observer;
    Observer2 observer2;
    Observed observed;
    observed.AddObserver(&observer);
    observed.AddObserver(&observer2);

    observed.Start();
    observed.Stop();

    return 0;
}
