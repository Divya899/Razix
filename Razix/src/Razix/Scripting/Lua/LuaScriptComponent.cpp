#include "Precompiled.h"
#include "LuaScriptComponent.h"
#include "LuaManager.h"
#include "Scene/Scene.h"
#include "Core/Engine.h"

namespace Razix
{
    LuaScriptComponent::LuaScriptComponent()
    {
        m_Scene = nullptr;
        m_FileName = "";
        m_Env = nullptr;
    }
    LuaScriptComponent::LuaScriptComponent(const std::string& fileName, Scene* scene)
    {
        m_Scene = scene;
        m_FileName = fileName;

        Init();
    }

    void LuaScriptComponent::Init()
    {
        LoadScript(m_FileName);
    }

    LuaScriptComponent::~LuaScriptComponent()
    {
        if(m_Env)
        {
            sol::protected_function releaseFunc = (*m_Env)["OnRelease"];
            if(releaseFunc.valid())
                releaseFunc.call();
        }
    }

    void LuaScriptComponent::LoadScript(const std::string& fileName)
    {
        m_FileName = fileName;
        std::string physicalPath;
        if(!VFS::Get()->ResolvePhysicalPath(fileName, physicalPath))
        {
            Razix_LOG_ERROR("Failed to Load Lua script {0}", fileName);
            m_Env = nullptr;
            return;
        }

        VFS::Get()->AbsoulePathToVFS(m_FileName, m_FileName);

        m_Env = CreateRef<sol::environment>(LuaManager::Get().GetState(), sol::create, LuaManager::Get().GetState().globals());

        auto loadFileResult = LuaManager::Get().GetState().script_file(physicalPath, *m_Env, sol::script_pass_on_error);
        if(!loadFileResult.valid())
        {
            sol::error err = loadFileResult;
            Razix_LOG_ERROR("Failed to Execute Lua script {0}", physicalPath);
            Razix_LOG_ERROR("Error : {0}", err.what());
            m_Errors.push_back(std::string(err.what()));
        }

        if(m_Scene)
            (*m_Env)["CurrentScene"] = m_Scene;

        m_OnInitFunc = CreateRef<sol::protected_function>((*m_Env)["OnInit"]);
        if(!m_OnInitFunc->valid())
            m_OnInitFunc.reset();

        m_UpdateFunc = CreateRef<sol::protected_function>((*m_Env)["OnUpdate"]);
        if(!m_UpdateFunc->valid())
            m_UpdateFunc.reset();

        m_Phys2DBeginFunc = CreateRef<sol::protected_function>((*m_Env)["OnCollision2DBegin"]);
        if(!m_Phys2DBeginFunc->valid())
            m_Phys2DBeginFunc.reset();

        m_Phys2DEndFunc = CreateRef<sol::protected_function>((*m_Env)["OnCollision2DEnd"]);
        if(!m_Phys2DEndFunc->valid())
            m_Phys2DEndFunc.reset();

        m_Phys3DBeginFunc = CreateRef<sol::protected_function>((*m_Env)["OnCollision3DBegin"]);
        if(!m_Phys3DBeginFunc->valid())
            m_Phys3DBeginFunc.reset();

        m_Phys3DEndFunc = CreateRef<sol::protected_function>((*m_Env)["OnCollision3DEnd"]);
        if(!m_Phys3DEndFunc->valid())
            m_Phys3DEndFunc.reset();

        LuaManager::Get().GetState().collect_garbage();
    }

    void LuaScriptComponent::OnInit()
    {
        if(m_OnInitFunc)
        {
            sol::protected_function_result result = m_OnInitFunc->call();
            if(!result.valid())
            {
                sol::error err = result;
                Razix_LOG_ERROR("Failed to Execute Script Lua Init function");
                Razix_LOG_ERROR("Error : {0}", err.what());
            }
        }
    }

    void LuaScriptComponent::OnUpdate(float dt)
    {
        if(m_UpdateFunc)
        {
            sol::protected_function_result result = m_UpdateFunc->call(dt);
            if(!result.valid())
            {
                sol::error err = result;
                Razix_LOG_ERROR("Failed to Execute Script Lua OnUpdate");
                Razix_LOG_ERROR("Error : {0}", err.what());
            }
        }
    }

    void LuaScriptComponent::Reload()
    {
        if(m_Env)
        {
            sol::protected_function releaseFunc = (*m_Env)["OnRelease"];
            if(releaseFunc.valid())
                releaseFunc.call();
        }

        Init();
    }

    void LuaScriptComponent::Load(const std::string& fileName)
    {
        if(m_Env)
        {
            sol::protected_function releaseFunc = (*m_Env)["OnRelease"];
            if(releaseFunc.valid())
                releaseFunc.call();
        }

        m_FileName = fileName;
        Init();
    }

    void LuaScriptComponent::OnCollision2DBegin()
    {
        if(m_Phys2DBeginFunc)
        {
            sol::protected_function_result result = m_Phys2DBeginFunc->call();
            if(!result.valid())
            {
                sol::error err = result;
                Razix_LOG_ERROR("Failed to Execute Script Lua OnCollision2DBegin");
                Razix_LOG_ERROR("Error : {0}", err.what());
            }
        }
    }

    void LuaScriptComponent::OnCollision2DEnd()
    {
        if(m_Phys2DEndFunc)
        {
            sol::protected_function_result result = m_Phys2DEndFunc->call();
            if(!result.valid())
            {
                sol::error err = result;
                Razix_LOG_ERROR("Failed to Execute Script Lua OnCollision2DEnd");
                Razix_LOG_ERROR("Error : {0}", err.what());
            }
        }
    }

    void LuaScriptComponent::OnCollision3DBegin()
    {
        if(m_Phys3DBeginFunc)
        {
            sol::protected_function_result result = m_Phys3DBeginFunc->call();
            if(!result.valid())
            {
                sol::error err = result;
                Razix_LOG_ERROR("Failed to Execute Script Lua OnCollision3DBegin");
                Razix_LOG_ERROR("Error : {0}", err.what());
            }
        }
    }

    void LuaScriptComponent::OnCollision3DEnd()
    {
        if(m_Phys3DEndFunc)
        {
            sol::protected_function_result result = m_Phys3DEndFunc->call();
            if(!result.valid())
            {
                sol::error err = result;
                Razix_LOG_ERROR("Failed to Execute Script Lua OnCollision3DEnd");
                Razix_LOG_ERROR("Error : {0}", err.what());
            }
        }
    }
}
