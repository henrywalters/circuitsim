//
// Created by henry on 9/9/26.
//
#include "runtime.h"

#include <hagame/graphics/shaders/texture.h>
#include <hagame/graphics/components/quad.h>
#include <hagame/math/components/rectCollider.h>
#include <hge/components/entityViewer.h>

#include "imgui.h"
#include "../common/gameState.h"
#include "../components/lead.h"
#include "../components/pin.h"
#include "../systems/renderer.h"
#include "../core/electronics/wire.h"

#include "../core/electronics.h"
#include "../common/sizes.h"
#include "../components/electronic.h"
#include "../components/switch.h"

#include "../components/wire.h"
#include "../core/circuit.h"

using namespace hg;
using namespace hg::graphics;

Runtime::Runtime(Window* window):
    m_window(window),
    m_renderMesh(&m_renderQuad) {
    m_renderQuad.centered(false);
}

void Runtime::onActivate() {
    auto state = GameState::Get();
    state->editing = false;
    state->running = true;

    entities.clear();
    auto cameraEntity = entities.add();
    auto camera = cameraEntity->addComponent<OrthographicCamera>();
    camera->size = GAME_SIZE.cast<float>();
    camera->pixelsPerMeter = 200;
    camera->centered = true;
    cameraEntity->addComponent<OrthographicZoom>()
        ->zoomSpeed = 10;
    cameraEntity->addComponent<OrthographicPan>();

    m_components = entities.add();
}

bool compareGridPos(Vec2 a, Vec2 b) {
    for (int i = 0; i < 2; i++) {
        if (!approxEqual(a[i], b[i], GRID_SIZE)) {
            return false;
        }
    }

    return true;
}

void Runtime::onInit() {
    addSystem<Renderer>(m_window);
}

Entity* Runtime::getSwitch() {
    Entity* out = nullptr;
    auto state = GameState::Get();
    entities.forEach<SwitchComponent>([&](SwitchComponent* switchComp, Entity* entity) {
        if (out) return;
        auto collider = entity->getComponent<math::components::RectCollider>();
        auto rect = collider->getRect();
        if (rect.contains(state->dragPos)) {
            out = entity;
        }
    });

    return out;
}

Entity* Runtime::getDraggable() {
    Entity* out = nullptr;
    auto state = GameState::Get();
    entities.forEach<math::components::RectCollider>([&](hg::math::components::RectCollider* collider, hg::Entity* entity) {
        if (out) return;
        auto rect = collider->getRect();
        if (rect.contains(state->dragPos)) {
            out = entity;
        }
    });

    return out;
}

Entity *Runtime::getLead(Entity* ignore) {
    Entity* out = nullptr;
    auto state = GameState::Get();
    entities.forEach<LeadComponent>([&](LeadComponent* lead, Entity* entity) {
        if (out || entity == ignore) return;
        Circle circle{entity->position().resize<2>(), PIN_RADIUS};
        if (circle.contains(state->dragPos)) {
            out = entity;
        }
    });
    return out;
}

Entity* Runtime::getPin() {
    Entity* out = nullptr;
    auto state = GameState::Get();
    entities.forEach<PinComponent>([&](PinComponent* pin, Entity* entity) {
        if (out) return;
        Circle circle{entity->position().resize<2>(), PIN_RADIUS};
        if (circle.contains(state->dragPos)) {
            out = entity;
        }
    });
    return out;
}

void removeConnection(std::vector<Entity*>& connections, Entity* entity) {
    connections.erase(std::remove_if(connections.begin(), connections.end(), [&](Entity* other) {
        return entity == other;
    }), connections.end());
}

void Runtime::connect(Entity *leadEntity, Entity *connection) {
    leadEntity->getComponent<LeadComponent>()->connectedTo.push_back(connection);

    if (connection->hasComponent<PinComponent>()) {
        removeConnection(connection->getComponent<PinComponent>()->connectedTo, leadEntity);
        connection->getComponent<PinComponent>()->connectedTo.push_back(leadEntity);
        std::cout << "Connected Lead " << leadEntity->name << " to Pin " << connection->name << "\n";
    } else if (connection->hasComponent<LeadComponent>()) {
        connection->getComponent<LeadComponent>()->connectedTo.push_back(leadEntity);
        std::cout << "Connected Lead " << leadEntity->name << " to Lead " << connection->name << "\n";
    } else {
        throw std::runtime_error("Invalid connection!");
    }

    leadEntity->transform.position = connection->position();

    m_circuit.compile(entities);
}

void Runtime::disconnect(Entity *leadEntity) {
    auto lead = leadEntity->getComponent<LeadComponent>();

    for (const auto& connection : lead->connectedTo) {
        if (connection->hasComponent<PinComponent>()) {
            removeConnection(connection->getComponent<PinComponent>()->connectedTo, leadEntity);
            std::cout << "Disconnected Lead " << leadEntity->name << " from Pin " << connection->name << "\n";
        } else if (connection->hasComponent<LeadComponent>()) {
            removeConnection(connection->getComponent<LeadComponent>()->connectedTo, leadEntity);
            std::cout << "Disconnected Lead " << leadEntity->name << " from Lead " << connection->name << "\n";
        } else {
            throw std::runtime_error("Invali disconnection!");
        }
    }
    lead->connectedTo.clear();

    m_circuit.compile(entities);
}


nlohmann::json Runtime::saveCircuit() {
    nlohmann::json j;
    j["entities"] = m_components->save();
    j["leadConnections"] = nlohmann::json::array();
    j["pinConnections"] = nlohmann::json::array();

    entities.forEach<LeadComponent>([&](LeadComponent* lead, Entity* entity) {
        if (lead->connectedTo.empty()) return;

        nlohmann::json connection;
        connection["entity"] = entity->id();
        connection["connectedTo"] = nlohmann::json::array();

        for (const auto& connected : lead->connectedTo) {
            connection["connectedTo"].push_back(connected->id());
        }

        j["leadConnections"].push_back(connection);
    });

    entities.forEach<PinComponent>([&](PinComponent* pin, Entity* entity) {
        if (pin->connectedTo.empty()) return;

        nlohmann::json connection;
        connection["entity"] = entity->id();
        connection["connectedTo"] = nlohmann::json::array();

        for (const auto& connected : pin->connectedTo) {
            connection["connectedTo"].push_back(connected->id());
        }

        j["pinConnections"].push_back(connection);
    });

    return j;
}

void Runtime::loadCircuit(nlohmann::json const &json) {
    m_components->clear();
    m_components = entities.add();
    m_selected = std::nullopt;
    m_dragging = std::nullopt;
    loadEntity(json["entities"], {}, m_components);

    for (const auto& connection : json["leadConnections"]) {
        auto entity = entities.get(connection["entity"].get<utils::uuid_t>());
        auto lead = entity->getComponent<LeadComponent>();
        for (const auto& connected : connection["connectedTo"]) {
            lead->connectedTo.push_back(entities.get(connected.get<utils::uuid_t>()));
        }
    }

    for (const auto& connection : json["pinConnections"]) {
        auto entity = entities.get(connection["entity"].get<utils::uuid_t>());
        auto pin = entity->getComponent<PinComponent>();
        for (const auto& connected : connection["connectedTo"]) {
            pin->connectedTo.push_back(entities.get(connected.get<utils::uuid_t>()));
        }
    }

    entities.forEach<ElectronicComponent>([&](ElectronicComponent* comp, Entity* entity) {
        ElectronicsFactory::InstantiateFromEntity(entity, comp->name);
    });

    m_circuit.compile(entities);
}

void Runtime::onUpdate(double dt) {

    if (m_running) {
        uint64_t virtualNs = (uint64_t)(dt * 1e9);
        m_circuit.tick(virtualNs);
    }

    auto state = GameState::Get();

    state->dragPos = (state->mousePos / GRID_SIZE).rounded() * GRID_SIZE;

    ImGui::Begin("Electronic Components");

    ImGui::Text(std::format("Mode: {}", utils::GetEnumLabel(Mode::_ID, m_mode)).c_str());

    ImGui::SameLine();

    if (ImGui::Button(m_running ? "Stop" : "Play")) {
        m_running = !m_running;
    }

    if (m_window->input.devices.keyboardMouse()->buttonsPressed[input::devices::KeyboardButtons::W]) {
        m_mode = Mode::AddWire;
    }
    if (m_window->input.devices.keyboardMouse()->buttonsPressed[input::devices::KeyboardButtons::D]) {
        m_mode = Mode::Drag;
    }

    if (ImGui::Button("Clear")) {
        m_components->clear();
        m_components = entities.add();
        m_selected = std::nullopt;
        m_dragging = std::nullopt;
        ImGui::End();
        return;
    }

    if (ImGui::Button("Save")) {
        m_fileBrowser.saveFile("Save Circuit", [&](std::filesystem::path const& path) {
            auto json = saveCircuit();
            std::string pathStr = path.string();
            if (!utils::s_endsWith(pathStr, ".circuit")) {
                pathStr += ".circuit";
            }
            hg::utils::f_write(pathStr, json.dump(4));
        }, {".circuit"});
    }

    ImGui::SameLine();

    if (ImGui::Button("Load")) {
        m_fileBrowser.loadFile("Load Circuit", [&](std::filesystem::path const& path) {
            auto json = nlohmann::json::parse(hg::utils::f_read(path));
            loadCircuit(json);
            m_history.clear();
        }, {".circuit"});
    }

    m_fileBrowser.render();

    ImGui::Text(std::format("Mouse Pos {}", state->mousePos.toString()).c_str());

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("Add Component");
    }

    if (ImGui::BeginPopup("Add Component")) {
        for (const auto& [name, instantiate] : ElectronicsFactory::Registry()) {
            if (ImGui::Selectable(name.c_str())) {
                ElectronicsFactory::Instantiate(m_components, name);
            }
        }

        ImGui::EndPopup();
    }

    if (m_selected.has_value()) {
        ImGui::SeparatorText(m_selected.value()->name.c_str());
        hge::transformViewer(m_selected.value()->transform);
    }

    ImGui::End();

    if (m_window->input.devices.keyboardMouse()->buttonsPressed[input::devices::MouseButtons::Left]) {

        if (auto entity = getSwitch()) {
            m_switchHeld = entity;
        }

        if (auto entity = getDraggable()) {
            m_selected = entity;
            m_dragging = entity;
            m_dragStart = entity->position();
            m_dragOffset = state->dragPos.resize<3>() - entity->position();
        } else if (auto entity = getLead(nullptr)) {
            if (m_mode == Mode::AddWire) {
                auto wire = addWire(m_components);
                auto leads = getWireLeads(wire);
                leads.a->entity->transform.position = entity->position();
                connect(leads.a->entity, entity);
                m_dragging = leads.b->entity;
                m_dragStart = entity->position();
                m_dragOffset = Vec3::Zero();
            } else {
                m_dragging = entity;
                m_dragStart = entity->position();
                m_dragOffset = Vec3::Zero();
            }
        } else if (auto entity = getPin()) {
            auto wire = addWire(m_components);
            auto leads = getWireLeads(wire);
            leads.a->entity->transform.position = entity->position();
            connect(leads.a->entity, entity);
            m_dragging = leads.b->entity;
            m_dragStart = entity->position();
            m_dragOffset = Vec3::Zero();
        }
    }

    if (!m_window->input.devices.keyboardMouse()->buttons[input::devices::MouseButtons::Left]) {
        if (m_switchHeld.has_value()) {
            if (compareGridPos(state->dragPos, m_dragStart.resize<2>())) {
                m_switchHeld.value()->getComponent<SwitchComponent>()->toggle();
                m_circuit.notifyChange(ElectronicsFactory::GetInstance(m_switchHeld.value()->id()));
            }
        }

        if (m_dragging.has_value() && m_dragging.value()->hasComponent<LeadComponent>()) {
            hg::Entity* entity;
            if (entity = getPin()) {
                connect(m_dragging.value(), entity);
            } else if (entity = getLead(m_dragging.value())) {
                connect(m_dragging.value(), entity);
            } else {
                disconnect(m_dragging.value());
            }
        }
        m_dragging = std::nullopt;
        m_switchHeld = std::nullopt;
    }

    if (m_dragging.has_value()) {
        auto offset = state->dragPos - m_dragStart.resize<2>();
        m_dragging.value()->transform.position = m_dragStart + offset.resize<3>() - m_dragOffset;

        auto lead = m_dragging.value()->getComponent<LeadComponent>();

        for (const auto& child : m_dragging.value()->children()) {
            auto entity = static_cast<Entity*>(child);
            auto pin = entity->getComponent<PinComponent>();
            if (pin) {
                for (const auto& connection : pin->connectedTo) {
                    connection->transform.position = entity->position();
                }
            }
        }
    }
}

void Runtime::onAfterUpdate() {
    auto state = GameState::Get();
    auto windowSize = m_window->size().cast<float>();

    Vec2 mousePos = m_window->input.devices.keyboardMouse()->mousePosition();

    glViewport(0, 0, (int)windowSize.x(), (int)windowSize.y());

    auto rect = mapRectToBounds(GAME_SIZE.cast<float>(), windowSize);
    m_renderQuad.setSizeAndOffset(rect.size, rect.pos);
    m_renderMesh.update(&m_renderQuad);

    mousePos -= rect.pos;
    mousePos[1] = rect.size[1] - mousePos[1];
    mousePos = mousePos.prod(GAME_SIZE.cast<float>().div(rect.size));

    state->rawMousePos = mousePos;

    auto shader = getShader(TEXTURE_SHADER.name);
    shader->use();
    shader->setMat4("projection", Mat4::Identity());
    shader->setMat4("view", Mat4::Orthographic(0, windowSize.x(), windowSize.y(), 0, -10, 10));
    shader->setMat4("model", Mat4::Identity());

    getSystem<Renderer>()->m_renderPasses.get(RenderMode::Combined)->textures[0]->bind();
    m_renderMesh.render();
}