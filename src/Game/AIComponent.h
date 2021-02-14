#pragma once

class Tank;

class AIComponent
{
public:
    AIComponent(Tank* pParentTank);

    void update(const double delta);

private:
    Tank* m_pParentTank;
};