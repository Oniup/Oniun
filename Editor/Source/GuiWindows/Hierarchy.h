#pragma once

#include "Oniun/Renderer/IImGuiWindow.h"
#include "Oniun/Scene/Entity.h"

namespace Oniun
{
    class Hierarchy : public IImGuiWindow
    {
    public:
        Hierarchy();

    public:
        FORCE_INLINE Entity GetSelectedEntity() const
        {
            return m_SelectedEntity;
        }

        FORCE_INLINE Scene* GetSelectedScene() const
        {
            return m_SelectedScene;
        }

        void Draw() override;

    private:
        void EntitySetDrag(const UUID& entity, const EntityEntry& entry, Scene* scene);

    private:
        Entity m_SelectedEntity;
        Scene* m_SelectedScene;
        Array<UUID> m_SelectedEntities;
    };
}
