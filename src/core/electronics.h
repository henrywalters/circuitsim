//
// Created by henry on 9/10/26.
//

#ifndef CIRCUITSIM_ELECTRONICS_H
#define CIRCUITSIM_ELECTRONICS_H

#include "itf/ielectronic.h"

class ElectronicsFactory {
public:

    using instance_fn = std::function<std::unique_ptr<IElectronic>(hg::Entity* entity)>;

    using registry_t = std::unordered_map<std::string, instance_fn>;
    using instances_t = std::unordered_map<hg::utils::uuid_t, std::unique_ptr<IElectronic>>;

    template <IsIElectronic Comp>
    static size_t Register(std::string const& name);

    static registry_t& Registry();
    static instances_t& Instances();

    static hg::utils::uuid_t Instantiate(hg::Entity* parent, std::string const& name);
    static hg::utils::uuid_t InstantiateFromEntity(hg::Entity* parent, std::string const& name);
    static void Remove(hg::Scene* scene, hg::utils::uuid_t id);
    static IElectronic* GetInstance(hg::utils::uuid_t id);

private:

    static std::unique_ptr<instances_t> m_instances;
    static std::unique_ptr<registry_t> m_registry;

};

template <IsIElectronic Comp>
size_t ElectronicsFactory::Register(std::string const& name) {
    auto &registry = Registry();
    registry.insert(std::make_pair(name, [](hg::Entity* entity) {
        return std::make_unique<Comp>(entity);
    }));
    return registry.size();
}

#define CS_COMPONENT(Name) static size_t Component_##Name##_ID = ElectronicsFactory::Register<Name>(#Name);

#endif //CIRCUITSIM_ELECTRONICS_H
