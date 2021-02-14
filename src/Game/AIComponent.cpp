#include "AIComponent.h"

#include "GameObjects/Tank.h"

AIComponent::AIComponent(Tank* pParentTank)
    : m_pParentTank(pParentTank)
{

}

void AIComponent::update(const double delta)
{
    m_pParentTank->fire();
}