//
// Created by henry on 9/10/26.
//
#include "electronics.h"

#include <format>
#include <hagame/core/scene.h>

#include "../components/electronic.h"

std::unique_ptr<ElectronicsFactory::instances_t> ElectronicsFactory::m_instances;
std::unique_ptr<ElectronicsFactory::registry_t> ElectronicsFactory::m_registry;

ElectronicsFactory::registry_t& ElectronicsFactory::Registry() {
    if (!m_registry) {
        m_registry = std::make_unique<registry_t>();
    }
    return *m_registry;
}

ElectronicsFactory::instances_t& ElectronicsFactory::Instances() {
    if (!m_instances) {
        m_instances = std::make_unique<instances_t>();
    }
    return *m_instances;
}

hg::utils::uuid_t ElectronicsFactory::Instantiate(hg::Entity* parent, std::string const &name) {
    auto& instances = Instances();
    auto& registry = Registry();

    auto entity = parent->add();
    entity->addComponent<ElectronicComponent>()->name = name;

    auto instance = registry.at(name)(entity);

    auto id = instance->entity->id();

    instance->create();

    instance->entity->addComponent<ElectronicComponent>()->name = name;

    instances.insert(std::make_pair(id, std::move(instance)));

    return id;
}

hg::utils::uuid_t ElectronicsFactory::InstantiateFromEntity(hg::Entity* parent, std::string const &name) {
    auto& instances = Instances();
    auto& registry = Registry();

    auto instance = registry.at(name)(parent);

    auto id = instance->entity->id();

    instance->entity->addComponent<ElectronicComponent>()->name = name;

    instances.insert(std::make_pair(id, std::move(instance)));

    return id;
}

IElectronic* ElectronicsFactory::GetInstance(hg::utils::uuid_t id) {
    auto& instances = Instances();
    if (instances.find(id) == instances.end()) {
        throw std::runtime_error(std::format("Instance {} does not exist", id));
    }
    return instances.at(id).get();
}

void ElectronicsFactory::Remove(hg::Scene *scene, hg::utils::uuid_t id) {
    auto& instances = Instances();
    auto instance = GetInstance(id);

    scene->entities.remove(instance->entity);

    instances.erase(id);
}
