#include "Precompiled.h"
#include "Model.h"
#include "Mesh.h"
#include "Core/StringUtilities.h"
#include "Core/VFS.h"

namespace Razix::Graphics
{
    Model::Model(const std::string& filePath)
        : m_FilePath(filePath)
        , m_PrimitiveType(PrimitiveType::File)
    {
        LoadModel(m_FilePath);
    }

    Model::Model(const Ref<Mesh>& mesh, PrimitiveType type)
        : m_FilePath("Primitive")
        , m_PrimitiveType(type)
    {
        m_Meshes.push_back(mesh);
    }

    Model::Model(PrimitiveType type)
        : m_FilePath("Primitive")
        , m_PrimitiveType(type)
    {
        m_Meshes.push_back(Ref<Mesh>(CreatePrimative(type)));
    }

    void Model::LoadModel(const std::string& path)
    {
        std::string physicalPath;
        if(!Razix::VFS::Get()->ResolvePhysicalPath(path, physicalPath))
        {
            Razix_LOG_INFO("Failed to load Model - {0}", path);
            return;
        }

        std::string resolvedPath = physicalPath;

        const std::string fileExtension = StringUtilities::GetFilePathExtension(path);

        if(fileExtension == "obj")
            LoadOBJ(resolvedPath);
        else if(fileExtension == "gltf" || fileExtension == "glb")
            LoadGLTF(resolvedPath);
        else if(fileExtension == "fbx" || fileExtension == "FBX")
            LoadFBX(resolvedPath);
        else
            Razix_LOG_ERROR("Unsupported File Type : {0}", fileExtension);

        Razix_LOG_INFO("Loaded Model - {0}", path);
    }
}
