#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

struct IDatastore {
    virtual ~IDatastore() = default;

    virtual std::optional<std::string> Get(const std::string& key) const = 0;
    virtual void Set(const std::string_view key, const std::string_view data) = 0;
};

class Datastore : public IDatastore {
public:
    std::optional<std::string> Get(const std::string& key) const override {
        if (auto it = data_.find(key); it != data_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void Set(const std::string_view key, const std::string_view data) override {
        data_.emplace(key, data);
    }

private:
    std::map<std::string, std::string> data_;
};

struct ILegacyDatastore {
    virtual ~ILegacyDatastore() = default;

    virtual std::shared_ptr<void> GetByIndex(size_t index, size_t& size) const = 0;
    virtual size_t Set(void* data, size_t size) = 0;
};

class LegacyDatastore : public ILegacyDatastore {
public:
    std::shared_ptr<void> GetByIndex(size_t index, size_t& size) const override {
        if (index < data_.size()) {
            size = data_[index].second;
            return data_[index].first;
        }
        return nullptr;
    }

    size_t Set(void* data, size_t size) override {
        data_.emplace_back(std::shared_ptr<void>(std::malloc(size), [](void* p) { std::free(p); }), size);
        std::memcpy(data_.back().first.get(), data, size);
        return data_.size() - 1;
    }

private:
    std::vector<std::pair<std::shared_ptr<void>, size_t>> data_;
};

class LegacyDatastoreAdapter : public IDatastore {
public:
    LegacyDatastoreAdapter(std::shared_ptr<ILegacyDatastore> legacy_datastore)
        : legacy_datastore_(std::move(legacy_datastore)) {
    }

public:
    std::optional<std::string> Get(const std::string& key) const override {
        if (!legacy_datastore_indices_.contains(key)) {
            return std::nullopt;
        }

        size_t size;
        auto data = legacy_datastore_->GetByIndex(legacy_datastore_indices_.at(key), size);
        if (data && size) {
            return std::string(static_cast<const char*>(data.get()), size);
        }
        return std::nullopt;
    }

    void Set(const std::string_view key, const std::string_view data) override {
        size_t size = data.size();
        const auto index = legacy_datastore_->Set(const_cast<char*>(data.data()), size);
        legacy_datastore_indices_.emplace(key, index);
    }

private:
    std::map<std::string, size_t> legacy_datastore_indices_;
    std::shared_ptr<ILegacyDatastore> legacy_datastore_;
};

class Application {
public:
    Application(std::vector<std::shared_ptr<IDatastore>> datastores)
        : datastores_(std::move(datastores)) {
    }

public:
    void Init() {
        for (auto& datastore : datastores_) {
            datastore->Set("key", "value");
        }
    }

    void DoWork() {
        for (auto& datastore : datastores_) {
            auto data = datastore->Get("key");
            if (data) {
                std::cout << "Loaded application data: " << data.value() << std::endl;
            }
        }
    }

private:
    std::vector<std::shared_ptr<IDatastore>> datastores_;
};

int main() {
    auto datastore = std::make_shared<Datastore>();
    auto legacy_datastore = std::make_shared<LegacyDatastore>();

    Application app(
        {std::make_shared<Datastore>(), std::make_shared<LegacyDatastoreAdapter>(legacy_datastore)}
    );
    app.Init();
    app.DoWork();
    return 0;
}
