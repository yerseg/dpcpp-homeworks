#include <iostream>
#include <memory>
#include <thread>

namespace {

class SimpleSingleton {
public:
    // Метод для получения экземпляра
    static std::shared_ptr<SimpleSingleton> GetInstance(int value) {
        if (instance_) {
            return instance_;
        }

        instance_ = std::shared_ptr<SimpleSingleton>(new SimpleSingleton(value));
        return instance_;
    }

    void DoJob() const {
        std::cout << value_ << std::endl;
    }

    // Удаленные конструктор копирования и копирующий оператор присваивания
    SimpleSingleton(const SimpleSingleton& s) = delete;
    void operator=(const SimpleSingleton& s) = delete;

private:
    // Приватный конструктор
    explicit SimpleSingleton(int value)
        : value_(value) {
    }
    int value_;
    // Статический указатель на единственный экземпляр
    inline static std::shared_ptr<SimpleSingleton> instance_ = nullptr;
};

void worker1() {
    const auto singleton = SimpleSingleton::GetInstance(42);
    singleton->DoJob();
}

void worker2() {
    const auto singleton = SimpleSingleton::GetInstance(43);
    singleton->DoJob();
}

} // namespace

int main() {
    std::thread t1(worker1);
    std::thread t2(worker2);
    t1.join();
    t2.join();
    return 0;
}
