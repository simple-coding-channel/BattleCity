#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
ResourceManager::AnimatedSpritesMap ResourceManager::m_animatedSprites;
std::string ResourceManager::m_path;
std::vector<std::vector<std::string>> ResourceManager::m_levels;


void ResourceManager::unloadAllResources()
{
    m_shaderPrograms.clear();
    m_textures.clear();
    m_sprites.clear();
    m_animatedSprites.clear();
}

void ResourceManager::setExecutablePath(const std::string& executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
    std::ifstream f;
    f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
    if (!f.is_open())
    {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return std::string{};
    }

    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexString = getFileString(vertexPath);
    if (vertexString.empty())
    {
        std::cerr << "No vertex shader!" << std::endl;
        return nullptr;
    }

    std::string fragmentxString = getFileString(fragmentPath);
    if (fragmentxString.empty())
    {
        std::cerr << "No fragment shader!" << std::endl;
        return nullptr;
    }

    std::shared_ptr<RenderEngine::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentxString)).first->second;
    if (newShader->isCompiled())
    {
        return newShader;
    }

    std::cerr << "Can't load shader program:\n"
        << "Vertex: " << vertexPath << "\n"
        << "Fragment: " << fragmentPath << std::endl;

    return nullptr;
}


std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
    ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
    if (it != m_shaderPrograms.end())
    {
        return it->second;
    }
    std::cerr << "Can't find the shader program: " << shaderName << std::endl;
    return nullptr;
}


std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
    int channels = 0;
    int width = 0;
    int height = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

    if (!pixels)
    {
        std::cerr << "Can't load image: " << texturePath << std::endl;
        return nullptr;
    }

    std::shared_ptr<RenderEngine::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<RenderEngine::Texture2D>(width,
                                                                                                                            height,
                                                                                                                            pixels,
                                                                                                                            channels,
                                                                                                                            GL_NEAREST,
                                                                                                                            GL_CLAMP_TO_EDGE)).first->second;
    stbi_image_free(pixels);
    return newTexture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
    TexturesMap::const_iterator it = m_textures.find(textureName);
    if (it != m_textures.end())
    {
        return it->second;
    }
    std::cerr << "Can't find the texture: " << textureName << std::endl;
    return nullptr;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
                                                                  const std::string& textureName,
                                                                  const std::string& shaderName,
                                                                  const std::string& subTextureName)
{
    auto pTexture = getTexture(textureName);
    if (!pTexture)
    {
        std::cerr << "Can't find the texture: " << textureName << " for the sprite: " << spriteName << std::endl;
    }

    auto pShader = getShaderProgram(shaderName);
    if (!pShader)
    {
        std::cerr << "Can't find the shader: " << shaderName << " for the sprite: " << spriteName << std::endl;
    }

    std::shared_ptr<RenderEngine::Sprite> newSprite = m_sprites.emplace(spriteName, std::make_shared<RenderEngine::Sprite>(pTexture,
                                                                                                                           subTextureName,
                                                                                                                           pShader)).first->second;

    return newSprite;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::loadAnimatedSprite(const std::string& spriteName,
                                                                              const std::string& textureName,
                                                                              const std::string& shaderName,
                                                                              const std::string& subTextureName)
{
    auto pTexture = getTexture(textureName);
    if (!pTexture)
    {
        std::cerr << "Can't find the texture: " << textureName << " for the sprite: " << spriteName << std::endl;
    }

    auto pShader = getShaderProgram(shaderName);
    if (!pShader)
    {
        std::cerr << "Can't find the shader: " << shaderName << " for the sprite: " << spriteName << std::endl;
    }

    std::shared_ptr<RenderEngine::AnimatedSprite> newSprite = m_animatedSprites.emplace(spriteName, std::make_shared<RenderEngine::AnimatedSprite>(pTexture,
                                                                                                                                                  subTextureName,
                                                                                                                                                  pShader)).first->second;

    return newSprite;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
    SpritesMap::const_iterator it = m_sprites.find(spriteName);
    if (it != m_sprites.end())
    {
        return it->second;
    }
    std::cerr << "Can't find the sprite: " << spriteName << std::endl;
    return nullptr;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::getAnimatedSprite(const std::string& spriteName)
{
    auto it = m_animatedSprites.find(spriteName);
    if (it != m_animatedSprites.end())
    {
        return it->second;
    }
    std::cerr << "Can't find animated sprite: " << spriteName << std::endl;
    return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loatTextureAtlas(std::string textureName,
                                                                       std::string texturePath,
                                                                       std::vector<std::string> subTextures,
                                                                       const unsigned int subTextureWidth,
                                                                       const unsigned int subTextureHeight)
{
    auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));
    if (pTexture)
    {
        const unsigned int textureWidth = pTexture->width();
        const unsigned int textureHeight = pTexture->height();
        unsigned int currentTextureOffsetX = 0;
        unsigned int currentTextureOffsetY = textureHeight;
        for (auto& currentSubTextureName : subTextures)
        {
            glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,                 static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
            glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);
            pTexture->addSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTopUV);

            currentTextureOffsetX += subTextureWidth;
            if (currentTextureOffsetX >= textureWidth)
            {
                currentTextureOffsetX = 0;
                currentTextureOffsetY -= subTextureHeight;
            }
        }
    }
    return pTexture;
}

bool ResourceManager::loadJSONResources(const std::string& JSONPath)
{
    const std::string JSONString = getFileString(JSONPath);
    if (JSONString.empty())
    {
        std::cerr << "No JSON resources file!" << std::endl;
        return false;
    }

    rapidjson::Document document;
    rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
    if (!parseResult)
    {
        std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl;
        std::cerr << "In JSON file: " << JSONPath << std::endl;
        return false;
    }

    auto shadersIt = document.FindMember("shaders");
    if (shadersIt != document.MemberEnd())
    {
        for (const auto& currentShader : shadersIt->value.GetArray())
        {
            const std::string name = currentShader["name"].GetString();
            const std::string filePath_v = currentShader["filePath_v"].GetString();
            const std::string filePath_f = currentShader["filePath_f"].GetString();
            loadShaders(name, filePath_v, filePath_f);
        }
    }

    auto textureAtlasesIt = document.FindMember("textureAtlases");
    if (textureAtlasesIt != document.MemberEnd())
    {
        for (const auto& currentTextrueAtlas : textureAtlasesIt->value.GetArray())
        {
            const std::string name = currentTextrueAtlas["name"].GetString();
            const std::string filePath = currentTextrueAtlas["filePath"].GetString();
            const unsigned int subTextureWidht = currentTextrueAtlas["subTextureWidth"].GetUint();
            const unsigned int subTextureHeight = currentTextrueAtlas["subTextureHeight"].GetUint();

            const auto subTexturesArray = currentTextrueAtlas["subTextures"].GetArray();
            std::vector<std::string> subTextures;
            subTextures.reserve(subTexturesArray.Size());
            for (const auto& currentSubTexture : subTexturesArray)
            {
                subTextures.emplace_back(currentSubTexture.GetString());
            }
            loatTextureAtlas(name, filePath, std::move(subTextures), subTextureWidht, subTextureHeight);
        }
    }

    auto spritesIt = document.FindMember("sprites");
    if (spritesIt != document.MemberEnd())
    {
        for (const auto& currentSprite : spritesIt->value.GetArray())
        {
            const std::string name = currentSprite["name"].GetString();
            const std::string textureAtlas = currentSprite["textureAtlas"].GetString();
            const std::string shader = currentSprite["shader"].GetString();
            const std::string subTexture = currentSprite["subTextureName"].GetString();

            auto pSprite = loadSprite(name, textureAtlas, shader, subTexture);
            if (!pSprite)
            {
                continue;
            }
        }
    }

    auto animatedSpritesIt = document.FindMember("animatedSprites");
    if (animatedSpritesIt != document.MemberEnd())
    {
        for (const auto& currentAnimatedSprite : animatedSpritesIt->value.GetArray())
        {
            const std::string name = currentAnimatedSprite["name"].GetString();
            const std::string textureAtlas = currentAnimatedSprite["textureAtlas"].GetString();
            const std::string shader = currentAnimatedSprite["shader"].GetString();
            const std::string initialSubTexture = currentAnimatedSprite["initialSubTexture"].GetString();

            auto pAnimatedSprite = loadAnimatedSprite(name, textureAtlas, shader, initialSubTexture);
            if (!pAnimatedSprite)
            {
                continue;
            }

            const auto statesArray = currentAnimatedSprite["states"].GetArray();
            for (const auto& currentState : statesArray)
            {
                const std::string stateName = currentState["stateName"].GetString();
                std::vector<std::pair<std::string, uint64_t>> frames;
                const auto framesArray = currentState["frames"].GetArray();
                frames.reserve(framesArray.Size());
                for (const auto& currentFrame : framesArray)
                {
                    const std::string subTexture = currentFrame["subTexture"].GetString();
                    const uint64_t duration = currentFrame["duration"].GetUint64();
                    frames.emplace_back(std::pair<std::string, uint64_t>(subTexture, duration));
                }
                pAnimatedSprite->insertState(stateName, std::move(frames));
            }
        }
    }
    auto levelsIt = document.FindMember("levels");
    if (levelsIt != document.MemberEnd())
    {
        for (const auto& currentLevel : levelsIt->value.GetArray())
        {
            const auto description = currentLevel["description"].GetArray();
            std::vector<std::string> levelRows;
            levelRows.reserve(description.Size());
            size_t maxLength = 0;
            for (const auto& currentRow : description)
            {
                levelRows.emplace_back(currentRow.GetString());
                if (maxLength < levelRows.back().length())
                {
                    maxLength = levelRows.back().length();
                }
            }

            for (auto& currentRow : levelRows)
            {
                while (currentRow.length() < maxLength)
                {
                    currentRow.append("D");
                }
            }
            m_levels.emplace_back(std::move(levelRows));
        }
    }
    return true;
}