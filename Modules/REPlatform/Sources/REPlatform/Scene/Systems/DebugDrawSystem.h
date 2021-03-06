#pragma once

#include "REPlatform/Global/Constants.h"
#include "REPlatform/Scene/Systems/EditorSceneSystem.h"

#include <Base/BaseTypes.h>
#include <Entity/Component.h>
#include <Entity/SceneSystem.h>
#include <Functional/Function.h>
#include <Math/Color.h>
#include <Math/Matrix4.h>

namespace DAVA
{
class Scene;
class RenderBatch;
class DebugDrawSystem : public SceneSystem, public EditorSceneSystem
{
public:
    static const float32 HANGING_OBJECTS_DEFAULT_HEIGHT;

public:
    DebugDrawSystem(Scene* scene);
    virtual ~DebugDrawSystem();

    void AddEntity(Entity* entity) override;
    void RemoveEntity(Entity* entity) override;

    void RegisterComponent(Entity* entity, Component* component) override;
    void UnregisterComponent(Entity* entity, Component* component) override;

    void PrepareForRemove() override;

    void SetRequestedObjectType(ResourceEditor::eSceneObjectType objectType);
    ResourceEditor::eSceneObjectType GetRequestedObjectType() const;

    void EnableHangingObjectsMode(bool enabled);
    bool HangingObjectsModeEnabled() const;

    void SetHangingObjectsHeight(float32 height);
    float32 GetHangingObjectsHeight();

    //need be moved to testing tool
    DAVA_DEPRECATED(inline void EnableSwithcesWithDifferentLODsMode(bool enabled));
    DAVA_DEPRECATED(inline bool SwithcesWithDifferentLODsModeEnabled() const);

protected:
    void Draw() override;

private:
    void DrawComponent(const Type* type, const Function<void(Entity*)>& func);

    void DrawObjectBoxesByType(Entity* entity);
    void DrawLightNode(Entity* entity, bool isSelected);
    void DrawSoundNode(Entity* entity);
    void DrawSelectedSoundNode(Entity* entity);
    void DrawHangingObjects(Entity* entity);
    void DrawEntityBox(Entity* entity, const Color& color);
    void DrawSwitchesWithDifferentLods(Entity* entity);
    void DrawWindNode(Entity* entity);
    void DrawDebugOctTree(Entity* entity);
    void DrawDecals(Entity* entity);

    //hanging objects
    using RenderBatchWithTransform = std::pair<RenderBatch*, Matrix4>;
    using RenderBatchesWithTransforms = Vector<RenderBatchWithTransform>;

    void CollectRenderBatchesRecursively(Entity* entity, RenderBatchesWithTransforms& batches) const;
    float32 GetMinimalZ(const RenderBatchesWithTransforms& batches) const;
    void GetLowestVertexes(const RenderBatchesWithTransforms& batches, Vector<Vector3>& vertexes) const;
    Vector3 GetLandscapePointAtCoordinates(const Vector2& centerXY) const;
    bool IsObjectHanging(Entity* entity) const;

    Color objectTypeColor = Color::White;
    ResourceEditor::eSceneObjectType objectType = ResourceEditor::ESOT_NONE;
    bool hangingObjectsModeEnabled = false;
    bool switchesWithDifferentLodsEnabled = false;

    Vector<Entity*> entities;
    Map<const Type*, Vector<Entity*>> entitiesComponentMap;
    Map<const Type*, Function<void(Entity*)>> drawComponentFunctionsMap;

    float32 hangingObjectsHeight = HANGING_OBJECTS_DEFAULT_HEIGHT;
};

inline void DebugDrawSystem::SetHangingObjectsHeight(float32 height)
{
    hangingObjectsHeight = height;
}

inline float32 DebugDrawSystem::GetHangingObjectsHeight()
{
    return hangingObjectsHeight;
}

inline void DebugDrawSystem::EnableHangingObjectsMode(bool enabled)
{
    hangingObjectsModeEnabled = enabled;
}

inline bool DebugDrawSystem::HangingObjectsModeEnabled() const
{
    return hangingObjectsModeEnabled;
}

inline void DebugDrawSystem::EnableSwithcesWithDifferentLODsMode(bool enabled)
{
    switchesWithDifferentLodsEnabled = enabled;
}

inline bool DebugDrawSystem::SwithcesWithDifferentLODsModeEnabled() const
{
    return switchesWithDifferentLodsEnabled;
}
} // namespace DAVA
