#include "Precompiled.h"
#include "LuaManager.h"
#include "Maths/Transform.h"
#include "Core/OS/Window.h"
#include "Core/VFS.h"
#include "Scene/Scene.h"
#include "Core/Application.h"
#include "Core/Engine.h"
#include "Core/OS/Input.h"
#include "Scene/SceneManager.h"
#include "LuaScriptComponent.h"
#include "Scene/SceneGraph.h"
#include "Graphics/Camera/ThirdPersonCamera.h"

#include "Scene/Component/Components.h"
#include "Graphics/Camera/Camera.h"
#include "Graphics/Camera/Camera2D.h"

#include "Graphics/Sprite.h"
#include "Graphics/Light.h"
#include "Graphics/API/Texture.h"
#include "Graphics/Model.h"
#include "Utilities/RandomNumberGenerator.h"
#include "Scene/Entity.h"
#include "Scene/EntityManager.h"
#include "Scene/EntityFactory.h"
#include "Physics/RazixPhysicsEngine/RazixPhysicsEngine.h"

#include "ImGuiLua.h"
#include "PhysicsLua.h"
#include "MathsLua.h"

#include <imgui/imgui.h>
#include <Tracy/TracyLua.hpp>

#ifdef CUSTOM_SMART_PTR
namespace sol
{
    template <typename T>
    struct unique_usertype_traits<Razix::Ref<T>>
    {
        typedef T type;
        typedef Razix::Ref<T> actual_type;
        static const bool value = true;

        static bool is_null(const actual_type& ptr)
        {
            return ptr == nullptr;
        }

        static type* get(const actual_type& ptr)
        {
            return ptr.get();
        }
    };

    template <typename T>
    struct unique_usertype_traits<Razix::UniqueRef<T>>
    {
        typedef T type;
        typedef Razix::UniqueRef<T> actual_type;
        static const bool value = true;

        static bool is_null(const actual_type& ptr)
        {
            return ptr == nullptr;
        }

        static type* get(const actual_type& ptr)
        {
            return ptr.get();
        }
    };

    template <typename T>
    struct unique_usertype_traits<Razix::WeakRef<T>>
    {
        typedef T type;
        typedef Razix::WeakRef<T> actual_type;
        static const bool value = true;

        static bool is_null(const actual_type& ptr)
        {
            return ptr == nullptr;
        }

        static type* get(const actual_type& ptr)
        {
            return ptr.get();
        }
    };
}

#endif

namespace Razix
{
    LuaManager::LuaManager()
        : m_State(nullptr)
    {
    }

    void LuaManager::OnInit()
    {
        Razix_PROFILE_FUNCTION();
        m_State.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table);
        tracy::LuaRegister(m_State.lua_state());

        BindAppLua(m_State);
        BindInputLua(m_State);
        BindMathsLua(m_State);
        BindImGuiLua(m_State);
        BindECSLua(m_State);
        BindLogLua(m_State);
        BindSceneLua(m_State);
        BindPhysicsLua(m_State);
    }

    LuaManager::~LuaManager()
    {
    }

    void LuaManager::OnInit(Scene* scene)
    {
        Razix_PROFILE_FUNCTION();
        auto& registry = scene->GetRegistry();

        auto view = registry.view<LuaScriptComponent>();

        if(view.empty())
            return;

        for(auto entity : view)
        {
            auto& luaScript = registry.get<LuaScriptComponent>(entity);
            luaScript.OnInit();
        }
    }

    void LuaManager::OnUpdate(Scene* scene)
    {
        Razix_PROFILE_FUNCTION();
        auto& registry = scene->GetRegistry();

        auto view = registry.view<LuaScriptComponent>();

        if(view.empty())
            return;

        float dt = Engine::Get().GetTimeStep().GetElapsedMillis();

        for(auto entity : view)
        {
            auto& luaScript = registry.get<LuaScriptComponent>(entity);
            luaScript.OnUpdate(dt);
        }
    }

    entt::entity GetEntityByName(entt::registry& registry, const std::string& name)
    {
        Razix_PROFILE_FUNCTION();
        entt::entity e = entt::null;
        registry.view<NameComponent>().each([&](const entt::entity& entity, const NameComponent& component)
            {
                if(name == component.name)
                {
                    e = entity;
                }
            });

        return e;
    }

    void LuaManager::BindLogLua(sol::state& state)
    {
        Razix_PROFILE_FUNCTION();
        auto log = state.create_table("Log");

        log.set_function("Trace", [&](sol::this_state s, std::string_view message)
            { Razix_LOG_TRACE(message); });

        log.set_function("Info", [&](sol::this_state s, std::string_view message)
            { Razix_LOG_TRACE(message); });

        log.set_function("Warn", [&](sol::this_state s, std::string_view message)
            { Razix_LOG_WARN(message); });

        log.set_function("Error", [&](sol::this_state s, std::string_view message)
            { Razix_LOG_ERROR(message); });

        log.set_function("Critical", [&](sol::this_state s, std::string_view message)
            { Razix_LOG_CRITICAL(message); });
    }

    void LuaManager::BindInputLua(sol::state& state)
    {
        Razix_PROFILE_FUNCTION();
        auto input = state["Input"].get_or_create<sol::table>();

        input.set_function("GetKeyPressed", [](Razix::InputCode::Key key) -> bool
            { return Input::Get().GetKeyPressed(key); });

        input.set_function("GetKeyHeld", [](Razix::InputCode::Key key) -> bool
            { return Input::Get().GetKeyHeld(key); });

        input.set_function("GetMouseClicked", [](Razix::InputCode::MouseKey key) -> bool
            { return Input::Get().GetMouseClicked(key); });

        input.set_function("GetMouseHeld", [](Razix::InputCode::MouseKey key) -> bool
            { return Input::Get().GetMouseHeld(key); });

        input.set_function("GetMousePosition", []() -> Maths::Vector2
            { return Input::Get().GetMousePosition(); });

        input.set_function("GetScrollOffset", []() -> float
            { return Input::Get().GetScrollOffset(); });

        std::initializer_list<std::pair<sol::string_view, Razix::InputCode::Key>> keyItems = {
            { "A", Razix::InputCode::Key::A },
            { "B", Razix::InputCode::Key::B },
            { "C", Razix::InputCode::Key::C },
            { "D", Razix::InputCode::Key::D },
            { "E", Razix::InputCode::Key::E },
            { "F", Razix::InputCode::Key::F },
            { "H", Razix::InputCode::Key::G },
            { "G", Razix::InputCode::Key::H },
            { "I", Razix::InputCode::Key::I },
            { "J", Razix::InputCode::Key::J },
            { "K", Razix::InputCode::Key::K },
            { "L", Razix::InputCode::Key::L },
            { "M", Razix::InputCode::Key::M },
            { "N", Razix::InputCode::Key::N },
            { "O", Razix::InputCode::Key::O },
            { "P", Razix::InputCode::Key::P },
            { "Q", Razix::InputCode::Key::Q },
            { "R", Razix::InputCode::Key::R },
            { "S", Razix::InputCode::Key::S },
            { "T", Razix::InputCode::Key::T },
            { "U", Razix::InputCode::Key::U },
            { "V", Razix::InputCode::Key::V },
            { "W", Razix::InputCode::Key::W },
            { "X", Razix::InputCode::Key::X },
            { "Y", Razix::InputCode::Key::Y },
            { "Z", Razix::InputCode::Key::Z },
            //{ "UNKOWN", Razix::InputCode::Key::Unknown },
            { "Space", Razix::InputCode::Key::Space },
            { "Escape", Razix::InputCode::Key::Escape },
            { "APOSTROPHE", Razix::InputCode::Key::Apostrophe },
            { "Comma", Razix::InputCode::Key::Comma },
            { "MINUS", Razix::InputCode::Key::Minus },
            { "PERIOD", Razix::InputCode::Key::Period },
            { "SLASH", Razix::InputCode::Key::Slash },
            { "SEMICOLON", Razix::InputCode::Key::Semicolon },
            { "EQUAL", Razix::InputCode::Key::Equal },
            { "LEFT_BRACKET", Razix::InputCode::Key::LeftBracket },
            { "BACKSLASH", Razix::InputCode::Key::Backslash },
            { "RIGHT_BRACKET", Razix::InputCode::Key::RightBracket },
            //{ "BACK_TICK", Razix::InputCode::Key::BackTick },
            { "Enter", Razix::InputCode::Key::Enter },
            { "Tab", Razix::InputCode::Key::Tab },
            { "Backspace", Razix::InputCode::Key::Backspace },
            { "Insert", Razix::InputCode::Key::Insert },
            { "Delete", Razix::InputCode::Key::Delete },
            { "Right", Razix::InputCode::Key::Right },
            { "Left", Razix::InputCode::Key::Left },
            { "Down", Razix::InputCode::Key::Down },
            { "Up", Razix::InputCode::Key::Up },
            { "PageUp", Razix::InputCode::Key::PageUp },
            { "PageDown", Razix::InputCode::Key::PageDown },
            { "Home", Razix::InputCode::Key::Home },
            { "End", Razix::InputCode::Key::End },
            { "CAPS_LOCK", Razix::InputCode::Key::CapsLock },
            { "SCROLL_LOCK", Razix::InputCode::Key::ScrollLock },
            { "NumLock", Razix::InputCode::Key::NumLock },
            { "PrintScreen", Razix::InputCode::Key::PrintScreen },
            { "Pasue", Razix::InputCode::Key::Pause },
            { "LeftShift", Razix::InputCode::Key::LeftShift },
            { "LeftControl", Razix::InputCode::Key::LeftControl },
            { "LEFT_ALT", Razix::InputCode::Key::LeftAlt },
            { "LEFT_SUPER", Razix::InputCode::Key::LeftSuper },
            { "RightShift", Razix::InputCode::Key::RightShift },
            { "RightControl", Razix::InputCode::Key::RightControl },
            { "RIGHT_ALT", Razix::InputCode::Key::RightAlt },
            { "RIGHT_SUPER", Razix::InputCode::Key::RightSuper },
            { "Menu", Razix::InputCode::Key::Menu },
            { "F1", Razix::InputCode::Key::F1 },
            { "F2", Razix::InputCode::Key::F2 },
            { "F3", Razix::InputCode::Key::F3 },
            { "F4", Razix::InputCode::Key::F4 },
            { "F5", Razix::InputCode::Key::F5 },
            { "F6", Razix::InputCode::Key::F6 },
            { "F7", Razix::InputCode::Key::F7 },
            { "F8", Razix::InputCode::Key::F8 },
            { "F9", Razix::InputCode::Key::F9 },
            { "F10", Razix::InputCode::Key::F10 },
            { "F11", Razix::InputCode::Key::F11 },
            { "F12", Razix::InputCode::Key::F12 },
            { "Keypad0", Razix::InputCode::Key::D0 },
            { "Keypad1", Razix::InputCode::Key::D1 },
            { "Keypad2", Razix::InputCode::Key::D2 },
            { "Keypad3", Razix::InputCode::Key::D3 },
            { "Keypad4", Razix::InputCode::Key::D4 },
            { "Keypad5", Razix::InputCode::Key::D5 },
            { "Keypad6", Razix::InputCode::Key::D6 },
            { "Keypad7", Razix::InputCode::Key::D7 },
            { "Keypad8", Razix::InputCode::Key::D8 },
            { "Keypad9", Razix::InputCode::Key::D9 },
            { "Decimal", Razix::InputCode::Key::Period },
            { "Divide", Razix::InputCode::Key::Slash },
            { "Multiply", Razix::InputCode::Key::KPMultiply },
            { "Subtract", Razix::InputCode::Key::Minus },
            { "Add", Razix::InputCode::Key::KPAdd },
            { "KP_EQUAL", Razix::InputCode::Key::KPEqual }
        };
        state.new_enum<Razix::InputCode::Key, false>("Key", keyItems); // false makes it read/write in Lua, but its faster

        std::initializer_list<std::pair<sol::string_view, Razix::InputCode::MouseKey>> mouseItems = {
            { "Left", Razix::InputCode::MouseKey::ButtonLeft },
            { "Right", Razix::InputCode::MouseKey::ButtonRight },
            { "Middle", Razix::InputCode::MouseKey::ButtonMiddle },
        };
        state.new_enum<Razix::InputCode::MouseKey, false>("MouseButton", mouseItems);
    }

    Ref<Graphics::Texture2D> LoadTexture(const std::string& name, const std::string& path)
    {
        Razix_PROFILE_FUNCTION();
        return Ref<Graphics::Texture2D>(Graphics::Texture2D::CreateFromFile(name, path));
    }

    Ref<Graphics::Texture2D> LoadTextureWithParams(const std::string& name, const std::string& path, Razix::Graphics::TextureFilter filter, Razix::Graphics::TextureWrap wrapMode)
    {
        Razix_PROFILE_FUNCTION();
        return Ref<Graphics::Texture2D>(Graphics::Texture2D::CreateFromFile(name, path, Graphics::TextureParameters(filter, filter, wrapMode)));
    }

    void LuaManager::BindECSLua(sol::state& state)
    {
        Razix_PROFILE_FUNCTION();

        sol::usertype<entt::registry> enttRegistry = state.new_usertype<entt::registry>("enttRegistry");

        sol::usertype<Entity> entityType = state.new_usertype<Entity>("Entity", sol::constructors<sol::types<entt::entity, Scene*>>());
        sol::usertype<EntityManager> entityManagerType = state.new_usertype<EntityManager>("EntityManager");
        entityManagerType.set_function("Create", static_cast<Entity (EntityManager::*)()>(&EntityManager::Create));
        entityManagerType.set_function("GetRegistry", &EntityManager::GetRegistry);

        entityType.set_function("Valid", &Entity::Valid);
        entityType.set_function("Destroy", &Entity::Destroy);
        entityType.set_function("SetParent", &Entity::SetParent);
        entityType.set_function("GetParent", &Entity::GetParent);
        entityType.set_function("IsParent", &Entity::IsParent);
        entityType.set_function("GetChildren", &Entity::GetChildren);
        entityType.set_function("SetActive", &Entity::SetActive);
        entityType.set_function("Active", &Entity::Active);

        state.set_function("GetEntityByName", &GetEntityByName);

        state.set_function("AddPyramidEntity", &EntityFactory::AddPyramid);
        state.set_function("AddSphereEntity", &EntityFactory::AddSphere);
        state.set_function("AddLightCubeEntity", &EntityFactory::AddLightCube);

        sol::usertype<NameComponent> nameComponent_type = state.new_usertype<NameComponent>("NameComponent");
        nameComponent_type["name"] = &NameComponent::name;
        REGISTER_COMPONENT_WITH_ECS(state, NameComponent, static_cast<NameComponent& (Entity::*)()>(&Entity::AddComponent<NameComponent>));

        sol::usertype<LuaScriptComponent> script_type = state.new_usertype<LuaScriptComponent>("LuaScriptComponent", sol::constructors<sol::types<std::string, Scene*>>());
        REGISTER_COMPONENT_WITH_ECS(state, LuaScriptComponent, static_cast<LuaScriptComponent& (Entity::*)(std::string&&, Scene * &&)>(&Entity::AddComponent<LuaScriptComponent, std::string, Scene*>));

        using namespace Maths;
        REGISTER_COMPONENT_WITH_ECS(state, Transform, static_cast<Transform& (Entity::*)()>(&Entity::AddComponent<Transform>));

        using namespace Graphics;
        sol::usertype<Sprite> sprite_type = state.new_usertype<Sprite>("Sprite", sol::constructors<sol::types<Maths::Vector2, Maths::Vector2, Maths::Vector4>, Sprite(const Ref<Graphics::Texture2D>&, const Maths::Vector2&, const Maths::Vector2&, const Maths::Vector4&)>());
        sprite_type.set_function("SetTexture", &Sprite::SetTexture);

        REGISTER_COMPONENT_WITH_ECS(state, Sprite, static_cast<Sprite& (Entity::*)(const Vector2&, const Vector2&, const Vector4&)>(&Entity::AddComponent<Sprite, const Vector2&, const Vector2&, const Vector4&>));

        REGISTER_COMPONENT_WITH_ECS(state, Light, static_cast<Light& (Entity::*)()>(&Entity::AddComponent<Light>));

        std::initializer_list<std::pair<sol::string_view, Razix::Graphics::PrimitiveType>> primitives = {
            { "Cube", Razix::Graphics::PrimitiveType::Cube },
            { "Plane", Razix::Graphics::PrimitiveType::Plane },
            { "Quad", Razix::Graphics::PrimitiveType::Quad },
            { "Pyramid", Razix::Graphics::PrimitiveType::Pyramid },
            { "Sphere", Razix::Graphics::PrimitiveType::Sphere },
            { "Capsule", Razix::Graphics::PrimitiveType::Capsule },
            { "Cylinder", Razix::Graphics::PrimitiveType::Cylinder },
            { "Terrain", Razix::Graphics::PrimitiveType::Terrain },
        };

        state.new_enum<Razix::Graphics::PrimitiveType, false>("PrimitiveType", primitives);

        state.new_usertype<Model>("Model", sol::constructors<Model(const std::string&), Model(Razix::Graphics::PrimitiveType)>());
        REGISTER_COMPONENT_WITH_ECS(state, Model, static_cast<Model& (Entity::*)(const std::string&)>(&Entity::AddComponent<Model, const std::string&>));

        sol::usertype<Camera> camera_type = state.new_usertype<Camera>("Camera", sol::constructors<Camera(float, float, float, float), Camera(float, float)>());
        camera_type["fov"] = &Camera::GetFOV;
        camera_type["aspectRatio"] = &Camera::GetAspectRatio;
        camera_type["nearPlane"] = &Camera::GetNear;
        camera_type["farPlane"] = &Camera::GetFar;
        camera_type["SetIsOrthographic"] = &Camera::SetIsOrthographic;
        camera_type["SetNearPlane"] = &Camera::SetNear;
        camera_type["SetFarPlane"] = &Camera::SetFar;

        REGISTER_COMPONENT_WITH_ECS(state, Camera, static_cast<Camera& (Entity::*)(const float&, const float&)>(&Entity::AddComponent<Camera, const float&, const float&>));

        sol::usertype<Physics3DComponent> physics3DComponent_type = state.new_usertype<Physics3DComponent>("Physics3DComponent", sol::constructors<sol::types<const Ref<RigidBody3D>&>>());
        physics3DComponent_type.set_function("GetRigidBody", &Physics3DComponent::GetRigidBody);
        REGISTER_COMPONENT_WITH_ECS(state, Physics3DComponent, static_cast<Physics3DComponent& (Entity::*)(Ref<RigidBody3D>&)>(&Entity::AddComponent<Physics3DComponent, Ref<RigidBody3D>&>));

        sol::usertype<Physics2DComponent> physics2DComponent_type = state.new_usertype<Physics2DComponent>("Physics2DComponent", sol::constructors<sol::types<const RigidBodyParameters&>>());
        physics2DComponent_type.set_function("GetRigidBody", &Physics2DComponent::GetRigidBody);

        REGISTER_COMPONENT_WITH_ECS(state, Physics2DComponent, static_cast<Physics2DComponent& (Entity::*)(const RigidBodyParameters&)>(&Entity::AddComponent<Physics2DComponent, const RigidBodyParameters&>));

        REGISTER_COMPONENT_WITH_ECS(state, SoundComponent, static_cast<SoundComponent& (Entity::*)()>(&Entity::AddComponent<SoundComponent>));

        //state.set_function("LoadMesh", &ModelLoader::LoadModel);
        //TODO MODEL
        sol::usertype<Graphics::Mesh> mesh_type = state.new_usertype<Graphics::Mesh>("Mesh");

        std::initializer_list<std::pair<sol::string_view, Razix::Graphics::TextureFilter>> textureFilter = {
            { "None", Razix::Graphics::TextureFilter::NONE },
            { "Linear", Razix::Graphics::TextureFilter::LINEAR },
            { "Nearest", Razix::Graphics::TextureFilter::NEAREST }
        };

        std::initializer_list<std::pair<sol::string_view, Razix::Graphics::TextureWrap>> textureWrap = {
            { "None", Razix::Graphics::TextureWrap::NONE },
            { "Repeat", Razix::Graphics::TextureWrap::REPEAT },
            { "Clamp", Razix::Graphics::TextureWrap::CLAMP },
            { "MirroredRepeat", Razix::Graphics::TextureWrap::MIRRORED_REPEAT },
            { "ClampToEdge", Razix::Graphics::TextureWrap::CLAMP_TO_EDGE },
            { "ClampToBorder", Razix::Graphics::TextureWrap::CLAMP_TO_BORDER }
        };

        state.set_function("LoadMesh", &CreatePrimative);

        state.new_enum<Razix::Graphics::TextureWrap, false>("TextureWrap", textureWrap);
        state.new_enum<Razix::Graphics::TextureFilter, false>("TextureFilter", textureFilter);

        state.set_function("LoadTexture", &LoadTexture);
        state.set_function("LoadTextureWithParams", &LoadTextureWithParams);
    }

    static float LuaRand(float a, float b)
    {
        return RandomNumberGenerator32::Rand(a, b);
    }

    void LuaManager::BindSceneLua(sol::state& state)
    {
        sol::usertype<Scene> scene_type = state.new_usertype<Scene>("Scene");
        scene_type.set_function("GetRegistry", &Scene::GetRegistry);
        scene_type.set_function("GetEntityManager", &Scene::GetEntityManager);

        sol::usertype<Graphics::Texture2D> texture2D_type = state.new_usertype<Graphics::Texture2D>("Texture2D");
        texture2D_type.set_function("CreateFromFile", &Graphics::Texture2D::CreateFromFile);

        state.set_function("Rand", &LuaRand);
    }

    static void SwitchSceneByIndex(int index)
    {
        Application::Get().GetSceneManager()->SwitchScene(index);
    }

    static void SwitchScene()
    {
        Application::Get().GetSceneManager()->SwitchScene();
    }

    static void SwitchSceneByName(const std::string& name)
    {
        Application::Get().GetSceneManager()->SwitchScene(name);
    }

    static void SetPhysicsDebugFlags(int flags)
    {
        Application::Get().GetSystem<RazixPhysicsEngine>()->SetDebugDrawFlags(flags);
    }

    void LuaManager::BindAppLua(sol::state& state)
    {
        sol::usertype<Application> app_type = state.new_usertype<Application>("Application");
        state.set_function("SwitchSceneByIndex", &SwitchSceneByIndex);
        state.set_function("SwitchSceneByName", &SwitchSceneByName);
        state.set_function("SwitchScene", &SwitchScene);
        state.set_function("SetPhysicsDebugFlags", &SetPhysicsDebugFlags);

        std::initializer_list<std::pair<sol::string_view, Razix::PhysicsDebugFlags>> physicsDebugFlags = {
            { "CONSTRAINT", Razix::PhysicsDebugFlags::CONSTRAINT },
            { "MANIFOLD", Razix::PhysicsDebugFlags::MANIFOLD },
            { "COLLISIONVOLUMES", Razix::PhysicsDebugFlags::COLLISIONVOLUMES },
            { "COLLISIONNORMALS", Razix::PhysicsDebugFlags::COLLISIONNORMALS },
            { "AABB", Razix::PhysicsDebugFlags::AABB },
            { "LINEARVELOCITY", Razix::PhysicsDebugFlags::LINEARVELOCITY },
            { "LINEARFORCE", Razix::PhysicsDebugFlags::LINEARFORCE },
            { "BROADPHASE", Razix::PhysicsDebugFlags::BROADPHASE },
            { "BROADPHASE_PAIRS", Razix::PhysicsDebugFlags::BROADPHASE_PAIRS },
            { "BOUNDING_RADIUS", Razix::PhysicsDebugFlags::BOUNDING_RADIUS },
        };

        state.new_enum<PhysicsDebugFlags, false>("PhysicsDebugFlags", physicsDebugFlags);

        app_type.set_function("GetWindowSize", &Application::GetWindowSize);
        state.set_function("GetAppInstance", &Application::Get);
    }
}
