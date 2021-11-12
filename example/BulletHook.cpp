#include "BulletHook.hpp"

#include <cstdio>
#include <memory>
#include <string>
#include <utility>

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btSimpleDynamicsWorld.h>
#include <LinearMath/btSerializer.h>

#include "Hry/Memory/Memory.hpp"
#include <Hry/Memory/Detour.hpp>

#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "LinearMath/btVector3.h"

#include "example.hpp"

using btDynamicsWorld_addRigidBody =
    void(__fastcall*)(btDiscreteDynamicsWorld*, btRigidBody* body, short group, short mask);

static std::unique_ptr<hry::Detour> detour;

void __fastcall new_byDynamicsWorld_addRigidBody(
    btDiscreteDynamicsWorld* thisa, btRigidBody* body, short group, short mask)
{
    detour->getOriginal<btDynamicsWorld_addRigidBody>()(thisa, body, group, mask);

    ExamplePlugin::world = thisa;

    auto* shape = body->getCollisionShape();
    ExamplePlugin::Logger->info("Shape: {}", shape->getShapeType());

    if (shape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
    {
        auto* com = dynamic_cast<btCompoundShape*>(shape);
        for (int i = 0; i < com->getNumChildShapes(); i++)
        {
            auto* innerShape = com->getChildShape(i);
            ExamplePlugin::Logger->info("- Shape: {}", innerShape->getShapeType());
        }
    }

    // if (ExamplePlugin::Serialize)

    static void* shapeUserPointer = nullptr;

    {
        if (body->getCollisionShape() != nullptr)
        {
            // btCollisionShape* shape = (btCollisionShape*)body->getCollisionShape();

            // if (shape->getShapeType() != COMPOUND_SHAPE_PROXYTYPE)
            // {
            //     ExamplePlugin::Logger->info(
            //         "Body: {:X} Shape: {:X}", (uint64_t)ExamplePlugin::userPointer,
            //         (uint64_t)shape->getUserPointer());
            //     if (ExamplePlugin::userPointer != body->getUserPointer())
            //     {
            //         ExamplePlugin::userPointer = body->getUserPointer();
            //     }

            //     if (shapeUserPointer != shape->getUserPointer())
            //     {
            //         shapeUserPointer = shape->getUserPointer();
            //         ExamplePlugin::Logger->info("Shape UP: {:X}", (uint64_t)shapeUserPointer);
            //     }

            //     ExamplePlugin::collision2 = shape;
            //     ExamplePlugin::Logger->info("set");
            // }

            // btCompoundShape* comp = (btCompoundShape*)body->getCollisionShape();

            // if (comp->getShapeType() == COMPOUND_SHAPE_PROXYTYPE && comp->getNumChildShapes() > 0 &&
            //     comp->getChildList() != nullptr && comp->getChildList()[0].m_childShape != nullptr)
            // {
            //     if (ExamplePlugin::userPointer != body->getUserPointer())
            //     {
            //         ExamplePlugin::userPointer = body->getUserPointer();
            //         ExamplePlugin::Logger->info(
            //             "User Pointer: {:X}", (uint64_t)ExamplePlugin::userPointer);

            //         // if (shapeUserPointer != shape->getUserPointer())
            //         // {
            //         //     shapeUserPointer = shape->getUserPointer();
            //         //     ExamplePlugin::Logger->info("Shape UP: {:X}", (uint64_t)shapeUserPointer);
            //         // }
            //     }
            //     ExamplePlugin::collision = comp;
            //     ExamplePlugin::Logger->info("set");
            // }
        }
        // ExamplePlugin::Logger->info(
        // "{:X} {:X}", (uintptr_t)body->m_extensionPointer, (uintptr_t)body->m_broadphaseHandle);
        // thisa->removeRigidBody(body);
        // ExamplePlugin::trackingBody = body;
        // ExamplePlugin::bodies.push_back(body);
    }
}

bool BulletHook::Install()
{
    auto addr =
        reinterpret_cast<btDynamicsWorld_addRigidBody>(hry::GetBaseAddress() + 0x00000001410C45F0);

    detour = std::make_unique<hry::Detour>(addr, &new_byDynamicsWorld_addRigidBody);

    if (detour->create() != hry::Detour::Status::Ok)
    {
        return false;
    }

    if (detour->enable() != hry::Detour::Status::Ok)
    {
        return false;
    }

    return true;
}

void BulletHook::Remove()
{
    detour.reset();
}
