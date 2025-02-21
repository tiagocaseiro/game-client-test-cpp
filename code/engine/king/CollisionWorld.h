// (C) king.com Ltd 2021
#pragma once

#include "BoxCollider.h"
#include "CircleCollider.h"
#include "RigidBody.h"

#include <glm/vec2.hpp>

#include <map>
#include <memory>
#include <vector>

namespace King
{
class CollisionWorld
{
public:
    class CollisionListener
    {
    public:
        virtual ~CollisionListener() = default;
        virtual void OnCollision(int l, int r) = 0;
    };

    struct CircleCastResult
    {
        int mId;
        glm::vec2 mNormal;
        glm::vec2 mPoint;
        float mDistance;
    };

    CollisionWorld();
    int AddBoxCollider(const glm::vec2& topLeft, const glm::vec2& size, unsigned int layer, unsigned int mask);
    int AddCircleCollider(const glm::vec2& position, float radius, unsigned int layer, unsigned int mask);
    RigidBody* AddRigidBody(const glm::vec2& movement, std::vector<int> colliders);

    bool CircleCast(const glm::vec2& origin, const glm::vec2& direction, float radius, float maxDistance,
                    CircleCastResult& result, unsigned int mask);
    // Returns nullptr if the box doesn't exist
    BoxCollider* GetBoxCollider(int id);
    CircleCollider* GetCircleCollider(int id);

    void RemoveBoxCollider(int boxId);
    void RemoveCircleCollider(int colliderId);
    void RemoveRigidBody(RigidBody* rigidbody);
    void ClearAll();

    void NotifyCollision(int l, int r);
    void AddCollisionListener(CollisionListener& listener);
    void RemoveCollisionListener(CollisionListener& listener);
    void ClearCollisionListeners();

    void Test();
    void ProcessCollisions(float dt);

private:
    int mNextColliderId = 1;
    enum class ColliderType
    {
        Circle,
        Box
    };

    struct ColliderInfo
    {
        ColliderType type;
        unsigned int mLayer = 0;
        unsigned int mMask = 0;
    };

    std::map<int, BoxCollider> mBoxColliders;
    std::map<int, CircleCollider> mCircleCollider;
    std::vector<std::unique_ptr<RigidBody>> mRigidBodies;
    std::vector<std::pair<int, ColliderInfo>> mAllColliders;

    std::vector<CollisionListener*> mListeners;
};
} // namespace King
