#pragma once

#include <Oniun/RHI/IImGuiWindow.h>
#include <Oniun/Scene/Entity.h>

namespace Oniun
{
    class Hierarchy : public IImGuiWindow
    {
    public:
        Hierarchy();

        void Draw() override;

    private:
        void EntitySetDrag(const UUID& entity, const EntityEntry& entry, Scene* scene);

    private:
        UUID m_SelectedEntity;
    };
}
