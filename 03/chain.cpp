#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>

struct Datastore {
    virtual ~Datastore() = default;

    virtual std::optional<std::string> Get(const std::string_view id) = 0;
    virtual void Set(const std::string_view id, const std::string_view data) = 0;
};

class DatabaseDatastore : public Datastore {
public:
    std::optional<std::string> Get(const std::string_view id) override {
        if (auto it = data_.find(id); it != data_.end()) {
            std::cout << "get from database" << std::endl;
            return it->second;
        }
        return std::nullopt;
    }

    void Set(const std::string_view id, const std::string_view data) override {
        data_.emplace(id, data);
    }

private:
    std::map<std::string, std::string, std::less<>> data_;
};

class InMemoryDatastore : public Datastore {
public:
    std::optional<std::string> Get(const std::string_view id) override {
        if (auto it = data_.find(id); it != data_.end()) {
            std::cout << "get from in-memory" << std::endl;
            return it->second;
        }
        return std::nullopt;
    }

    void Set(const std::string_view id, const std::string_view data) override {
        data_.emplace(id, data);
    }

private:
    std::map<std::string, std::string, std::less<>> data_;
};

class DiskDatastore : public Datastore {
public:
    std::optional<std::string> Get(const std::string_view id) override {
        return std::nullopt;
    }

    void Set(const std::string_view, const std::string_view) override {
    }
};

class AlternateDatastore : public Datastore {
public:
    void AddDatastore(std::unique_ptr<Datastore>&& datastore) {
        datastores_.push_back(std::move(datastore));
    }

public:
    std::optional<std::string> Get(const std::string_view id) override {
        for (auto& datastore : datastores_) {
            if (auto it = datastore->Get(id)) {
                return it;
            }
        }
        return std::nullopt;
    }

    void Set(const std::string_view, const std::string_view) override {
    }

private:
    std::vector<std::unique_ptr<Datastore>> datastores_;
};

int main() {
    auto store = std::make_unique<AlternateDatastore>();

    auto db = std::make_unique<DatabaseDatastore>();
    db->Set("key_0", "value_0");
    store->AddDatastore(std::move(db));

    auto in_memory = std::make_unique<InMemoryDatastore>();
    in_memory->Set("key_1", "value_1");
    store->AddDatastore(std::move(in_memory));

    auto disk = std::make_unique<DiskDatastore>();
    disk->Set("key_2", "value_2");
    store->AddDatastore(std::move(disk));

    std::cout << "key_0: " << store->Get("key_0").value_or("key not found") << std::endl;
    std::cout << "key_1: " << store->Get("key_1").value_or("key not found") << std::endl;
    std::cout << "key_2: " << store->Get("key_2").value_or("key not found") << std::endl;

    return 0;
}
