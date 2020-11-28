#include "StartScreen.h"

#include <iostream>

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"


std::shared_ptr<RenderEngine::Sprite> getSpriteForDescription(const char description)
{
    switch (description)
    {
    case '0':
        return ResourceManager::getSprite("brickWall_All");
    case '1':
        return ResourceManager::getSprite("brickWall_TopLeft");
    case '2':
        return ResourceManager::getSprite("brickWall_TopRight");
    case '3':
        return ResourceManager::getSprite("brickWall_Top");
    case '4':
        return ResourceManager::getSprite("brickWall_BottomLeft");
    case '5':
        return ResourceManager::getSprite("brickWall_Left");
    case '6':
        return ResourceManager::getSprite("brickWall_TopRight_BottomLeft");
    case '7':
        return ResourceManager::getSprite("brickWall_Top_BottomLeft");
    case '8':
        return ResourceManager::getSprite("brickWall_BottomRight");
    case '9':
        return ResourceManager::getSprite("brickWall_TopLeft_BottomRight");
    case 'A':
        return ResourceManager::getSprite("brickWall_Right");
    case 'B':
        return ResourceManager::getSprite("brickWall_Top_BottomRight");
    case 'C':
        return ResourceManager::getSprite("brickWall_Bottom");
    case 'D':
        return ResourceManager::getSprite("brickWall_TopLeft_Bottom");
    case 'E':
        return ResourceManager::getSprite("brickWall_TopRight_Bottom");
    case 'F':
        return nullptr;
    default:
        std::cerr << "Unknown Start Level description: " << description << std::endl;
    }
    return nullptr;
}

StartScreen::StartScreen(const std::vector<std::string>& startScreenDescription)
{
    if (startScreenDescription.empty())
    {
        std::cerr << "Empty start screen description!" << std::endl;
    }

    auto leftOffsetPixels = 2 * BLOCK_SIZE;
    auto bottomOffset  = STARTSCREEN_HEIGHT - 2 * BLOCK_SIZE;



    unsigned int currentBottomOffset = bottomOffset;
    for (const std::string& currentRow : startScreenDescription)
    {
        unsigned int currentLeftOffset = leftOffsetPixels;
        for (const char currentElement : currentRow)
        {
            m_sprites.emplace_back(std::make_pair(getSpriteForDescription(currentElement), glm::vec2(currentLeftOffset, currentBottomOffset)));
            currentLeftOffset += BLOCK_SIZE;
        }
        currentBottomOffset -= BLOCK_SIZE;
    }


}

unsigned int StartScreen::getStateWidth() const
{
    return STARTSCREEN_WIDTH;
}

unsigned int StartScreen::getStateHeight() const
{
    return STARTSCREEN_HEIGHT;
}

void StartScreen::render() const
{
    for (const auto& current : m_sprites)
    {
        if (current.first)
        {
            current.first->render(current.second, glm::vec2(BLOCK_SIZE), 0.f);
        }
    }
}

void StartScreen::update(const double delta)
{

}
