#include <iostream>
#include <map>
#include <string>

struct IDatabase {
    virtual ~IDatabase() = default;

    virtual std::string GetData(const std::string& key) const = 0;
};

class VeryHeavyDatabase : public IDatabase {
public:
    std::string GetData(const std::string& key) const override {
        return "Very Big Data String: " + key;
    }
};

class CacheProxyDB : public IDatabase {
public:
    explicit CacheProxyDB(std::shared_ptr<IDatabase> db)
        : real_db_(std::move(db)) {
    }

    std::string GetData(const std::string& key) const override {
        if (auto it = cache_.find(key); it != cache_.end()) {
            std::cout << "Get from cache" << std::endl;
            return it->second;
        } else {
            std::cout << "Get from real object" << std::endl;
            return cache_.emplace(key, real_db_->GetData(key)).first->second;
        }
    }

private:
    mutable std::map<std::string, std::string> cache_;
    std::shared_ptr<IDatabase> real_db_;
};

class TestDB : public IDatabase {
public:
    explicit TestDB(std::shared_ptr<IDatabase> db)
        : real_db_(std::move(db)) {
    }

    std::string GetData(const std::string&) const override {
        return "test_data\n";
    }

private:
    std::shared_ptr<IDatabase> real_db_;
};

int main() {
    std::shared_ptr<IDatabase> real_db = std::make_shared<VeryHeavyDatabase>();

    auto cached_db = std::make_unique<CacheProxyDB>(real_db);
    auto test_db = std::make_unique<TestDB>(real_db);

    std::cout << cached_db->GetData("key") << std::endl;
    std::cout << cached_db->GetData("key") << std::endl;
    std::cout << test_db->GetData("key") << std::endl;
    return 0;
}
