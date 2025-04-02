#include "GuiWindows/Inspector.h"

#include "Oniun/Scene/Components/Transform.h"

namespace Oniun::Editor
{
    Inspector::Inspector(Hierarchy* hierarchy)
        : IImGuiWindow("Inspector"), m_Hierarchy(hierarchy)
    {
    }

    void Inspector::Draw()
    {
        Entity entity = m_Hierarchy->GetSelectedEntity();
        if (entity == Entity::Invalid || !entity.IsAlive())
            return;

        ImGui::Text("Entity: %s", *entity.GetFullName());

        Array<Pair<UUID, byte*>> components(entity.GetScene()->GetAllEntitiesComponents(entity));
        for (const auto&[uuid, compData] : components)
        {
            switch (uuid)
            {
            case TypeInfo::GetFastId<TransformComponent>():
                if (ImGui::CollapsingHeader(*TypeInfo::GetName<TransformComponent>()))
                {
                    TransformComponent* transform = (TransformComponent*)compData;
                    ImGui::DragFloat3("Position", &transform->Position[0]);
                    ImGui::DragFloat3("Scale", &transform->Scale[0]);
                    ImGui::DragFloat3("Rotation", &transform->Rotation[0]);
                }
                break;
            default:
                break;
            }
        }
    }
}
