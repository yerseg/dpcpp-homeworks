#include <iostream>

class IntRangeIterator {
public:
    IntRangeIterator(int current, int step)
        : current_(current)
        , step_(step) {
    }

    IntRangeIterator& operator++() {
        current_ += step_;
        return *this;
    }

    bool operator==(IntRangeIterator other) const {
        return current_ == other.current_;
    }

    bool operator!=(IntRangeIterator other) const {
        return !(*this == other);
    }

    int operator*() const {
        return current_;
    }

private:
    int current_ = 0;
    int step_ = 0;
};

class IntRange {
public:
    IntRange(int min, int max, unsigned step = 1)
        : min_(min)
        , max_(max)
        , step_(min < max ? step : -step) {
    }

    IntRangeIterator begin() {
        return IntRangeIterator(min_, step_);
    }

    IntRangeIterator end() {
        return IntRangeIterator(max_, step_);
    }

private:
    int min_ = 0;
    int max_ = 0;
    int step_ = 0;
};

int main() {
    auto range = IntRange(5, -5, 2);
    for (auto i : range) {
        std::cout << i << std::endl;
    }
    return 0;
}
