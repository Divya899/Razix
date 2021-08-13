#pragma once

#include "Constraint.h"

namespace Razix
{
    namespace Maths
    {
        class Vector3;
    }

    class Quaternion;
    class RigidBody3D;

    enum class Axes
    {
        X,
        Y,
        Z,
        XY,
        XZ,
        YZ,
        XYZ
    };

    class Razix_EXPORT AxisConstraint : public Constraint
    {
    public:
        AxisConstraint(RigidBody3D* obj1, Axes axes);

        virtual void ApplyImpulse() override;
        virtual void DebugDraw() const override;

    protected:
        RigidBody3D* m_pObj1;
        Axes m_Axes;
    };
}
