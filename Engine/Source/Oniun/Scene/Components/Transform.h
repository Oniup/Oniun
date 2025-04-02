#pragma once

namespace Oniun
{
    struct TransformComponent
    {
        glm::vec3 Position;
        glm::vec3 Scale;
        glm::vec3 Rotation;

        TransformComponent()
            : Position(0.0f), Scale(1.0f), Rotation(0.0f)
        {
        }
    };
}
