#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace RenderEngine
{
    class ShaderProgram;
    class Texture2D;
    class Sprite;
}


class ResourceManager {
public:
    static void setExecutablePath(const std::string& executablePath);
    static void unloadAllResources();

    ~ResourceManager() = delete;
    ResourceManager() = delete;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;
    ResourceManager(ResourceManager&&) = delete;

    static std::shared_ptr<RenderEngine::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
    static std::shared_ptr<RenderEngine::ShaderProgram> getShaderProgram(const std::string& shaderName);
    static std::shared_ptr<RenderEngine::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
    static std::shared_ptr<RenderEngine::Texture2D> getTexture(const std::string& textureName);

    static std::shared_ptr<RenderEngine::Sprite> loadSprite(const std::string& spriteName,
                                                        const std::string& textureName,
                                                        const std::string& shaderName,
                                                        const std::string& subTextureName = "default");
    static std::shared_ptr<RenderEngine::Sprite> getSprite(const std::string& spriteName);

    static std::shared_ptr<RenderEngine::Texture2D> loatTextureAtlas(std::string textureName,
                                                                 std::string texturePath,
                                                                 std::vector<std::string> subTextures,
                                                                 const unsigned int subTextureWidth,
                                                                 const unsigned int subTextureHeight);

    static bool loadJSONResources(const std::string& JSONPath);

    static const std::vector<std::vector<std::string>>& getLevels() { return m_levels; }
    static const std::vector<std::string>& getStartScreen() { return m_startScreen; }

private:
    static std::string getFileString(const std::string& relativeFilePath);

    typedef std::map<const std::string, std::shared_ptr<RenderEngine::ShaderProgram>> ShaderProgramsMap;
    static ShaderProgramsMap m_shaderPrograms;

    typedef std::map<const std::string, std::shared_ptr<RenderEngine::Texture2D>> TexturesMap;
    static TexturesMap m_textures;

    typedef std::map<const std::string, std::shared_ptr<RenderEngine::Sprite>> SpritesMap;
    static SpritesMap m_sprites;

    static std::vector<std::vector<std::string>> m_levels;
    static std::vector<std::string> m_startScreen;

    static std::string m_path;
};