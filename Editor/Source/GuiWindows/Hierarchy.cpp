#include "GuiWindows/Hierarchy.h"

#include <Oniun/Core/Engine.h>
#include <Oniun/Scene/SceneLayer.h>

namespace Oniun
{
    Hierarchy::Hierarchy()
        : IImGuiWindow("Hierarchy")
    {
    }

    void Hierarchy::Draw()
    {
        SceneLayer* layer = Engine::GetLayer<SceneLayer>();

        for (Scene& scene : layer->GetLoadedScenes())
        {
            if (ImGui::CollapsingHeader(*scene.GetTitle(), ImGuiTreeNodeFlags_SpanFullWidth))
            {
                for (auto&[uuid, entry] : scene)
                {
                    if (entry.Parent == NO_POS)
                        EntitySetDrag(uuid, entry, &scene);
                }
            }
        }
    }

    void Hierarchy::EntitySetDrag(const UUID& entity, const EntityEntry& entry, Scene* scene)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen;

        // Setting name
        char name[EntityEntry::MaxFullNameSize];
        if (entry.NameId > 0)
            Crt::Format(name, EntityEntry::MaxFullNameSize, "%s%llu", entry.Name.Data(), entry.NameId);
        else
            Crt::Copy(name, entry.Name.Data(), entry.Name.Count());

        // Tree
        ImGui::PushID(entity);
        if (ImGui::TreeNodeEx(name, flags))
        {
            Entity child = Entity(entity, scene).GetFirstChild();
            while (child != Entity::Invalid)
            {
                EntitySetDrag(child.GetId(), *child.GetEntry(), scene);
                child = child.GetNextSibling();
            }

            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}
