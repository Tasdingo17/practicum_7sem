#include "FuncFabric.h"
#include <unordered_map>

class TFactory::TImpl {
private:
    class ICreator {
    public:
        virtual ~ICreator(){}
        virtual std::unique_ptr<TFunction> Create(const std::initializer_list<double>& lst) const = 0;
    };
    using TCreatorPtr = std::shared_ptr<ICreator>;
    using TRegisteredCreators = std::unordered_map<std::string, TCreatorPtr>;
    TRegisteredCreators RegisteredCreators;
public:
    template <class TCurrentObject>
    class TCreator : public ICreator{
    private:
        std::unique_ptr<TFunction> Create(const std::initializer_list<double>& lst) const override{
            return std::make_unique<TCurrentObject>(lst);
        }
    };

    TImpl(){ RegisterAll(); }

    template <typename T>
    void RegisterCreator(const std::string& name) {
        RegisteredCreators[name] = std::make_shared<TCreator<T>>();
    }

    void RegisterAll() {
        RegisterCreator<IdentF>("ident");
        RegisterCreator<ConstF>("const");
        RegisterCreator<PowerF>("power");
        RegisterCreator<ExpF>("exp");
        RegisterCreator<PolynomialF>("polynomial");
    }

    std::unique_ptr<TFunction> CreateObject(const std::string& s, const std::initializer_list<double>& lst) const {
        auto creator = RegisteredCreators.find(s);
        if (creator == RegisteredCreators.end()) {
            return nullptr;
        }
    return creator->second->Create(lst);
    }

    std::vector<std::string> GetAvailableObjects () const {
        std::vector<std::string> result;
        for (const auto& creatorPair : RegisteredCreators) {
            result.push_back(creatorPair.first);
        }
        return result;
    }
};


TFactory::TFactory() : Impl(std::make_shared<TFactory::TImpl>()) {}
TFactory::~TFactory(){}

std::unique_ptr<TFunction> TFactory::CreateObject(const std::string& n, const std::initializer_list<double>& lst) const {
    return Impl->CreateObject(n, lst);
}

std::vector<std::string> TFactory::GetAvailableObjects() const {
    return Impl->GetAvailableObjects();
}