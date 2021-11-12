
#pragma once

#include <fstream>
#include <list>

#include <xmmintrin.h>

#include "Hry/Events/Event.hpp"
#include <Hry/Plugin.hpp>

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "OrbitCameraHook.hpp"

class ExamplePlugin : public hry::Plugin
{
public:
    inline static hry::Logger* Logger;
    inline static bool Serialize;
    inline static btRigidBody* trackingBody;
    inline static btRigidBody* pupil;
    inline static btDiscreteDynamicsWorld* world;

    inline static btCollisionShape* pupilShape;

    inline static btCompoundShape* collision;
    inline static btCollisionShape* collision2;
    inline static void* userPointer;

private:
    hry::PluginInfo _pluginInfo;

    bool _isKeyBindPressed{ false };

    bool _renderWindow{ false };
    float _windowOpacity{ 0.5f };

    bool _recordWorld = false;

    int _currentFrame = 0;

    double _lastSave = 0;

    bool _disableOribtCamera = false;

    Camera* _camera;

    float _yaw{};
    float _pitch{};
    float _roll{};

    int _recordCount = 0;

    std::ofstream _recordFile;

    bool _recordRotation = false;

    hry::Vec2u _windowSize{};

    hry::Vec3<float> _velocity{};

    bool buttonPress[5]{};

public:
    ExamplePlugin();
    virtual ~ExamplePlugin();

    Result init(const InitParams&& initParams) override;
    void initConfig(hry::Config* config) override;
    void initKeyBinds(hry::KeyBinds* keyBinds) override;
    void initEvents(hry::EventDispatcher* eventDispatcher) override;

    // TODO: replace with manifest.json
    [[nodiscard]] const hry::PluginInfo& getPluginInfo() const override;

    void saveColliders(const char* path);

private:
    bool onOrbitCamera(Camera* camera);
    void onWindowResize(const hry::ResizeEvent&& e);
    void onMouseMove(const hry::MouseMoveEvent&& e);
    void onKeyPress(const hry::KeyboardEvent&& event);

    void keyBindPress(hry::ButtonState);
    void imguiRender();

    void update(const hry::FrameEndEvent&& e);

    static void* getShapeUserPointer(btRigidBody* rb);
};
