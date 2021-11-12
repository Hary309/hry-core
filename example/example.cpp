#include "example.hpp"

#include <fstream>
#include <ios>

#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <LinearMath/btDefaultMotionState.h>
#include <LinearMath/btIDebugDraw.h>
#include <LinearMath/btScalar.h>
#include <LinearMath/btSerializer.h>
#include <LinearMath/btVector3.h>
#include <corecrt_math_defines.h>
#include <fmt/core.h>

#include "Hry/Memory/Memory.hpp"
#include "Hry/System/Joystick.hpp"
#include <Hry/Config/Fields/BoolField.hpp>
#include <Hry/Config/Fields/NumericField.hpp>
#include <Hry/Config/Fields/SelectionField.hpp>
#include <Hry/Config/Fields/TextField.hpp>

#include "BulletHook.hpp"
#include "OrbitCameraHook.hpp"
#include "imgui.h"

struct DynamicActorVTable
{
};

struct DynamicActor
{
    DynamicActorVTable* vtable;
    char pad8[144];
    btRigidBody* body;
};

DynamicActor* __fastcall CreateDynamicActor()
{
    using CreateDynamicBody_t = DynamicActor*(__fastcall*)();
    auto* fun = (CreateDynamicBody_t)(0x0000000140C80F60 + hry::GetBaseAddress());

    return fun();
}

struct ActorVTable
{
    // DynamicActor(*createDynamicActor)
};

struct Actor
{
    ActorVTable* vtable;
};

struct World
{
    char pad0[3752]; // +0
    Actor* actorCreator;
    void* physics;

    static World* GetWorld() { return *(World**)(hry::GetBaseAddress() + 0x00000001417AA970); }
};

struct ConfigData
{
    bool renderWindow;
    float windowOpacity;
};

ExamplePlugin::ExamplePlugin()
{
    _pluginInfo.name = "hry-example";
    _pluginInfo.fullName = "Example Plugin";
    _pluginInfo.authorInfo = { "Foo Bar", "foo@bar.com" };
    _pluginInfo.website = "http://example.com";
    _pluginInfo.version = hry::Version{ 1, 0, 0 };
    _pluginInfo.desc = R"(
# About
Example plugin or sth
# Changelog
  * 1.0
    * First release
)";
}

ExamplePlugin::~ExamplePlugin()
{
    if (pupil)
    {
        world->removeRigidBody(pupil);
        delete pupil;
        delete pupilShape;
    }
}

ExamplePlugin::Result ExamplePlugin::init(const InitParams&& initParams)
{
    Logger = initParams.logger;
    Logger->info("Plugin initialized!");

    if (BulletHook::Install())
    {
        Logger->info("BulletHook installed!");
    }

    if (OrbitCameraHook::Install())
    {
        Logger->info("OribtCameraHook installed");
        OrbitCameraHook::OnOrbitCamera.connect<&ExamplePlugin::onOrbitCamera>(this);
    }

    Logger->info("Bullet version {}", btGetVersion());

    return ExamplePlugin::Result::Ok;
}

void ExamplePlugin::initConfig(hry::Config* config)
{
}

void ExamplePlugin::initKeyBinds(hry::KeyBinds* keyBinds)
{
    keyBinds->add(hry::KeyBindBuilder()
                      .setID("change_value")
                      .setLabel("Change value")
                      .setPressCallback(hry::Dlg<&ExamplePlugin::keyBindPress>(this))
                      .setReleaseCallback(hry::Dlg<&ExamplePlugin::keyBindPress>(this))
                      .setDefaultKey(hry::Keyboard::Key::Q)
                      .build());
}

void ExamplePlugin::initEvents(hry::EventDispatcher* eventDispatcher)
{
    eventDispatcher->system.onImGuiRender.connect<&ExamplePlugin::imguiRender>(this);
    eventDispatcher->game.onFrameEnd.connect<&ExamplePlugin::update>(this);
    eventDispatcher->system.onKeyPress.connect<&ExamplePlugin::onKeyPress>(this);
    eventDispatcher->system.onKeyRelease.connect<&ExamplePlugin::onKeyPress>(this);
    eventDispatcher->system.onMouseMove.connect<&ExamplePlugin::onMouseMove>(this);
    // eventDispatcher->system.onWindowResize.connect<&ExamplePlugin::onMouseMove>(this);
}

const hry::PluginInfo& ExamplePlugin::getPluginInfo() const
{
    return _pluginInfo;
}

bool ExamplePlugin::onOrbitCamera(Camera* camera)
{
    if (_camera != camera)
    {
        Logger->info("Camera: {:x}", (uint64_t)camera);
        _camera = camera;
    }

    float cy = cosf(_yaw * 0.5f);
    float sy = sinf(_yaw * 0.5f);
    float cp = cosf(_pitch * 0.5f);
    float sp = sinf(_pitch * 0.5f);
    float cr = cosf(_roll * 0.5f);
    float sr = sinf(_roll * 0.5f);

    _camera->w = cr * cp * cy + sr * sp * sy;
    _camera->x = sr * cp * cy - cr * sp * sy;
    _camera->y = cr * sp * cy + sr * cp * sy;
    _camera->z = cr * cp * sy - sr * sp * cy;

    if (pupil != nullptr && pupil != nullptr)
    {
        _velocity = {};
        int forward = 0;
        int side = 0;

        if (buttonPress[0])
        {
            forward = 1;
        }
        else if (buttonPress[1])
        {
            forward = -1;
        }

        if (buttonPress[2])
        {
            side = -1;
        }
        else if (buttonPress[3])
        {
            side = 1;
        }

        const float POWER = 20.f;

        if (forward)
        {
            _velocity.x = forward * cosf(_pitch + (float)M_PI_2) * POWER;
            _velocity.z = forward * -sinf(_pitch + (float)M_PI_2) * POWER;
        }

        if (side)
        {
            _velocity.x += side * cosf(_pitch) * POWER;
            _velocity.z += side * -sinf(_pitch) * POWER;
        }

        if (buttonPress[4])
        {
            _velocity.y = POWER / 10;
        }

        auto vel = pupil->getLinearVelocity();

        pupil->setLinearVelocity({ 0, vel.y(), 0 });
        pupil->applyCentralForce({ _velocity.x, _velocity.y, _velocity.z });

        // if (buttonPress[4])
        // {
        //     pupil->clearForces();
        // }

        auto dif = pupil->getWorldTransform().getOrigin();

        _camera->pos = { fmodf((float)dif.x(), 512.f), fmodf((float)dif.y(), 512.f),
                         fmodf((float)dif.z(), 512.f) };
    }

    return !_disableOribtCamera;
}

void ExamplePlugin::onWindowResize(const hry::ResizeEvent&& e)
{
    _windowSize = e.size;
}

void ExamplePlugin::onMouseMove(const hry::MouseMoveEvent&& e)
{
    _pitch -= (float)e.offset.x / 500.f;
    _roll -= (float)e.offset.y / 500.f;
}

void ExamplePlugin::onKeyPress(const hry::KeyboardEvent&& event)
{
    if (_camera == nullptr)
    {
        return;
    }

    bool press = event.state == hry::ButtonState::Pressed;

    switch (event.key)
    {
        case hry::Keyboard::Key::Numpad8: buttonPress[0] = press; break;
        case hry::Keyboard::Key::Numpad2: buttonPress[1] = press; break;
        case hry::Keyboard::Key::Numpad4: buttonPress[2] = press; break;
        case hry::Keyboard::Key::Numpad6: buttonPress[3] = press; break;
        case hry::Keyboard::Key::Numpad5: buttonPress[4] = press; break;
    }
}

void ExamplePlugin::saveColliders(const char* path)
{
    if (world != nullptr)
    {
        auto* serializer = new btDefaultSerializer();
        world->serialize(serializer);

        FILE* file = fopen(path, "wb");
        fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, file);
        fclose(file);
    }
}

void ExamplePlugin::keyBindPress(hry::ButtonState state)
{
    if (state == hry::ButtonState::Pressed)
    {
        _disableOribtCamera = !_disableOribtCamera;
        // Serialize = !Serialize;

        // Logger->info("Tracking: {}", Serialize);
    }
}

struct btBasicCollisionShape
{
    void* vtable;
};

void ExamplePlugin::imguiRender()
{
    if (ImGui::Begin("hry-example window"))
    {
        if (trackingBody != nullptr)
        {
            // trackingBody->serializeSingleObject(nullptr);

            // auto* serializer = new btDefaultSerializer();
            // btChunk* chunk = serializer->allocate(trackingBody->calculateSerializeBufferSize(), 1);
            // const char* structType = trackingBody->serialize(chunk->m_oldPtr, serializer);
            // btRigidBodyData* rbd = (btRigidBodyData*)chunk->m_oldPtr;

            // serializer->finalizeChunk(chunk, structType, BT_RIGIDBODY_CODE, (void*)this);
            // auto trans = trackingBody->getWorldTransform().getOrigin();
            auto trans = trackingBody->getCenterOfMassPosition();
            // trackingBody->applyCent({ 0, 1, 0 });
            // ImGui::Text("Ptr: %llx", (uint64_t)(trackingBody));
            ImGui::Text("X: %f", trans.x());
            ImGui::Text("Y: %f", trans.y());
            ImGui::Text("Z: %f", trans.z());

            if (ImGui::Button("velocity"))
            {
                trackingBody->setLinearVelocity({ 0, 10, 0 });
                trackingBody->setInterpolationLinearVelocity({ 0, 10, 0 });
            }

            // ;
        }

        if (_camera != nullptr)
        {
            ImGui::DragFloat("pitch", &_pitch, 0.01f);
            ImGui::DragFloat("yaw", &_yaw, 0.01f);
            ImGui::DragFloat("roll", &_roll, 0.01f);

            // ImGui::DragFloat("Un1", &_camera->un1, 0.0001f);
            // ImGui::DragFloat("Un2", &_camera->un2, 0.0001f);
            // ImGui::DragFloat("Un3", &_camera->un3, 0.0001f);
            // ImGui::DragFloat("Un4", &_camera->un4, 0.0001f);

            if (ImGui::Checkbox("Save rotation", &_recordRotation))
            {
                if (_recordRotation)
                {
                    _recordFile = std::ofstream(
                        fmt::format("posrec/record-{}.txt", _recordCount++), std::ios_base::trunc);
                }
                else
                {
                    _recordFile.close();
                }
            }
        }

        if (pupil != nullptr)
        {
            ImGui::Text("Pupil");
            auto trans = pupil->getCenterOfMassPosition();
            ImGui::Text("X: %f", trans.x());
            ImGui::Text("Y: %f", trans.y());
            ImGui::Text("Z: %f", trans.z());

            ImGui::Text("Velocity:");
            ImGui::Text("%f", pupil->getLinearVelocity().x());
            ImGui::Text("%f", pupil->getLinearVelocity().y());
            ImGui::Text("%f", pupil->getLinearVelocity().z());

            ImGui::Text("Force:");
            ImGui::Text("%f", pupil->getTotalForce().x());
            ImGui::Text("%f", pupil->getTotalForce().y());
            ImGui::Text("%f", pupil->getTotalForce().z());

            ImGui::Text("Activate: %d", pupil->getActivationState());

            if (ImGui::Button("Activate"))
            {
                pupil->activate();
            }

            if (ImGui::Button("TP"))
            {
                auto& origin = pupil->getWorldTransform().getOrigin();

                btVector3 vec = { 2640, 50, -1938 };
                for (int i = 0; i < world->getNumCollisionObjects(); i++)
                {
                    auto* obj = world->getCollisionObjectArray()[i];

                    if (obj != nullptr)
                    {
                        vec = obj->getWorldTransform().getOrigin();
                        break;
                    }
                }

                vec.setY(vec.getY() + 20.f);
                origin = vec;
            }
            // ;
        }

        if (world != nullptr)
        {
            auto* truck = world->getCollisionObjectArray()[0];
            double a, b, c;
            truck->getWorldTransform().getBasis().getEulerYPR(a, b, c);
            ImGui::Text("Rot X: %f", a);
            ImGui::Text("Rot Y: %f", b);
            ImGui::Text("Rot Z: %f", c);

            // ImGui::Text("Gravity: %f %f %f", gravity.x(), gravity.y(), gravity.z());

            ImGui::Text("Count: %d", world->getNumCollisionObjects());

            if (ImGui::Button("List bodies"))
            {
                Logger->info("Listing objects:");
                for (int i = 0; i < world->getNumCollisionObjects(); i++)
                {
                    auto* obj = (btRigidBody*)world->getCollisionObjectArray()[0];

                    if (obj == nullptr)
                    {
                        continue;
                    }

                    double mass = obj->getInvMass();

                    auto* shape = obj->getCollisionShape();

                    int shapeCount = 1;

                    if (shape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
                    {
                        btCompoundShape* com = (btCompoundShape*)shape;
                        shapeCount = com->getNumChildShapes();
                    }

                    bool active = obj->isActive();
                    bool isKinematic = obj->isKinematicObject();
                    bool isStatic = obj->isStaticObject();

                    if (!isStatic)
                    {
                        mass = 1.0 / mass;
                    }

                    Logger->info(
                        "[{}] M:{:.3f} SC:{} A:{} K:{} S:{}", i, mass, shapeCount, active,
                        isKinematic, isStatic);
                }
            }

            if (ImGui::Button("Create shape"))
            {
                auto* truck = world->getCollisionObjectArray()[0];

                auto* playerShape = new btCompoundShape(true); // moze false

                btTransform localTrans;
                localTrans.setIdentity();

                auto* sphere = new btCapsuleShape(1.0, 2.0);
                sphere->setUserPointer(getShapeUserPointer((btRigidBody*)truck));
                playerShape->addChildShape(localTrans, sphere);
                pupilShape = playerShape;

                // auto* originalShape = (btBasicCollisionShape*)collision;
                // auto* newShape = (btBasicCollisionShape*)compound;
                // newShape->vtable = originalShape->vtable;
            }

            if (ImGui::Button("Create rigid body"))
            {
                auto* truck = world->getCollisionObjectArray()[0];

                btScalar mass(0.1f);
                btVector3 localInertia(0, 0, 0);

                pupilShape->calculateLocalInertia(mass, localInertia);

                btRigidBody::btRigidBodyConstructionInfo rbInfo(
                    mass, nullptr, pupilShape, localInertia);
                auto* body = new btRigidBody(rbInfo);
                // auto* body = new btKinematicCharacterController();

                btVector3 vec = truck->getWorldTransform().getOrigin();

                double a, b, c;
                truck->getWorldTransform().getBasis().getEulerYPR(a, b, c);

                float offsetX = cos(b);
                float offsetZ = sin(b);

                vec.setX(vec.x() + offsetX * 2.0);
                vec.setY(vec.y() + 2.0);
                vec.setZ(vec.z() + offsetZ * 2.0);

                body->setAngularFactor({ 0, 0, 0 });

                body->setGravity({ 0, -9, 0 });
                body->getWorldTransform().setOrigin(vec);
                world->addRigidBody(body, 1, -1);
                body->setUserPointer(truck->getUserPointer());
                pupil = body;
            }

            if (ImGui::Button("Spawn rigid body 2"))
            {
                using CreateDynamicBody_t = DynamicActor*(__fastcall*)();
                auto* func = (CreateDynamicBody_t)(0x0000000140C80F60 + hry::GetBaseAddress());
                DynamicActor* actor = func();

                if (actor != nullptr)
                {
                    btScalar mass(1.f);
                    btVector3 localInertia(0, 0, 0);
                    pupilShape->calculateLocalInertia(mass, localInertia);

                    actor->body->setCollisionShape(pupilShape);
                    actor->body->setMassProps(mass, localInertia);
                    pupil = actor->body;

                    using AddToWorld_t = void(__fastcall*)(DynamicActor*, void*);

                    auto* func2 = (AddToWorld_t)(0x0000000140C9E450 + hry::GetBaseAddress());
                    func2(actor, World::GetWorld()->physics);
                }
            }

            if (ImGui::Button("Save colliders to file"))
            {
                auto* serializer = new btDefaultSerializer();
                world->serialize(serializer);

                FILE* file = fopen("ets2.bullet", "wb");
                fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, file);
                fclose(file);
            }

            if (ImGui::Button("Record coliders"))
            {
                _recordWorld = !_recordWorld;
                _currentFrame = 0;
            }

            ImGui::SameLine();
            ImGui::Text("%s", _recordWorld ? "true" : "false");
        }
    }

    ImGui::End();
}

void ExamplePlugin::update(const hry::FrameEndEvent&& e)
{
    _lastSave += e.deltaTime;

    if (_recordWorld && _lastSave > 0.10)
    {
        _lastSave = 0;
        std::string path = fmt::format("record/ets2-{}.bullet", _currentFrame++);
        saveColliders(path.c_str());

        Logger->info("Saved to {}", path);
    }
}

void* ExamplePlugin::getShapeUserPointer(btRigidBody* rb)
{
    auto* shape = rb->getCollisionShape();

    if (shape->getShapeType() != COMPOUND_SHAPE_PROXYTYPE)
    {
        return shape->getUserPointer();
    }

    auto* compoundShape = (btCompoundShape*)shape;

    for (int i = 0; i < compoundShape->getNumChildShapes(); i++)
    {
        auto* shape = compoundShape->getChildShape(i);

        if (shape != nullptr && shape->getUserPointer() != nullptr)
        {
            return shape->getUserPointer();
        }
    }
}

INIT_PLUGIN(ExamplePlugin)
