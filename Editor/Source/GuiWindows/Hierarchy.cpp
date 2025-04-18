#include "GuiWindows/Hierarchy.h"

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/KeyCode.h"
#include "Oniun/Scene/SceneLayer.h"

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
        uint64 selectedIndex = m_SelectedEntities.Find(entity);
        if (selectedIndex != NO_POS)
            flags |= ImGuiTreeNodeFlags_Selected;

        // Setting name
        char name[EntityEntry::MaxFullNameSize];
        if (entry.NameId > 0)
            Crt::Format(name, EntityEntry::MaxFullNameSize, "%s%llu", entry.Name.Data(), entry.NameId);
        else
            Crt::Copy(name, entry.Name.Data(), entry.Name.Count());

        // Check if entity has any children
        Entity child = Entity(entity, scene).GetFirstChild();
        if (child == Entity::Invalid)
            flags |= ImGuiTreeNodeFlags_Leaf;

        // Tree
        ImGui::PushID(entity);
        if (ImGui::TreeNodeEx(name, flags))
        {
            if (ImGui::IsItemClicked((int32)MouseButton::Left))
            {
                if (ImGui::IsMouseDoubleClicked((int32)MouseButton::Left))
                {
                    if (selectedIndex != NO_POS)
                        m_SelectedEntities.RemoveAt(selectedIndex);
                    else
                        m_SelectedEntities.Add(entity);
                }
            }

            // Iterate over all children and set as draggable recursively
            while (child != Entity::Invalid)
            {
                EntitySetDrag(child.GetId(), *child.GetEntry(), scene);
                child = child.GetNextSibling();
            }

            // TODO: Make draggable
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}
