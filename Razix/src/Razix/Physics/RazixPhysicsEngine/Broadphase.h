#pragma once

#include "RigidBody3D.h"

namespace Razix
{

    struct Razix_EXPORT CollisionPair
    {
        RigidBody3D* pObjectA;
        RigidBody3D* pObjectB;
    };

    class Razix_EXPORT Broadphase
    {
    public:
        virtual ~Broadphase() = default;
        virtual void FindPotentialCollisionPairs(RigidBody3D** objects, uint32_t objectCount, std::vector<CollisionPair>& collisionPairs) = 0;
        virtual void DebugDraw() = 0;
    };
}
