//
// Created by henry on 9/9/26.
//

#ifndef CIRCUITSIM_RUNTIME_H
#define CIRCUITSIM_RUNTIME_H

#include <hagame/core/scene.h>
#include <hagame/graphics/window.h>
#include <hagame/graphics/primitives/quad.h>
#include <hagame/utils/history.h>
#include <hge/components/fileBrowser.h>
#include "../core/circuit.h"

ENUM(Mode)
ENUM_VALUE(Mode, Drag)
ENUM_VALUE(Mode, AddWire)

class Runtime : public hg::Scene {
public:

    Runtime(hg::graphics::Window* window);

protected:

    void onActivate() override;
    void onInit() override;
    void onUpdate(double dt) override;
    void onAfterUpdate() override;

private:

    CircuitRunner m_circuit;
    bool m_running = true;

    hg::utils::History m_history;

    hg::graphics::Window* m_window;
    hg::graphics::primitives::Quad m_renderQuad;
    hg::graphics::MeshInstance m_renderMesh;
    hg::Entity* m_components;

    std::optional<hg::Entity*> m_selected;
    std::optional<hg::Entity*> m_dragging;
    std::optional<hg::Entity*> m_switchHeld;
    hg::Vec3 m_dragStart;
    hg::Vec3 m_dragOffset;

    hge::FileBrowser m_fileBrowser;

    Mode::type m_mode = Mode::Drag;

    hg::Entity* getSwitch();
    hg::Entity* getDraggable();
    hg::Entity* getLead(hg::Entity* ignore = nullptr);
    hg::Entity* getPin();

    void connect(hg::Entity* leadEntity, hg::Entity* connection);
    void disconnect(hg::Entity* leadEntity);

    nlohmann::json saveCircuit();
    void loadCircuit(nlohmann::json const& json);

};

#endif //CIRCUITSIM_RUNTIME_H
