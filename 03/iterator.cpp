#include <iostream>

template <typename T>
struct Iterator {
    virtual ~Iterator() = default;
    virtual bool Done() const = 0;
    virtual T* Current() const = 0;
    virtual T* Next() = 0;
};

template <typename T>
struct Collection {
    virtual ~Collection() = default;
    virtual std::unique_ptr<Iterator<T>> MakeIterator() const = 0;
};

// const int, потому что нет возможности менять промежуток
class IntRangeIterator : public Iterator<const int> {
public:
    IntRangeIterator(int current, int end, int step)
        : current_(current)
        , end_(end)
        , step_(step) {
    }
    bool Done() const override {
        return step_ < 0 ? current_ <= end_ : current_ >= end_;
    }
    const int* Current() const override {
        return Done() ? nullptr : &current_;
    }
    const int* Next() override {
        current_ += step_;
        return Current();
    }

private:
    int current_ = 0;
    int end_ = 0;
    int step_ = 0; // Переменная для шага может иметь отрицательное значение
};

class IntRange : public Collection<const int> {
public:
    IntRange(int min, int max, unsigned step = 1)
        : min_(min)
        , max_(max)
        , step_(min < max ? step : -step) {
    }
    std::unique_ptr<Iterator<const int>> MakeIterator() const override {
        return std::make_unique<IntRangeIterator>(min_, max_, step_);
    }

private:
    int min_ = 0;
    int max_ = 0;
    int step_ = 0;
};

int main() {
    // Последовательность от 0 до 5 с шагом в 1
    auto range = std::make_unique<IntRange>(0, 5);
    auto it = range->MakeIterator();
    const int* i = it->Current(); // Получаем текущее значение — 0
    while (!it->Done()) { // Продолжаем, пока не выйдем за пределы последовательности
        std::cout << *i << std::endl;
        i = it->Next(); // Переключаем итератор так, чтобы он сделал шаг вперёд
    }
}
