#pragma once
#include <string>

enum ActionKey
{
    NONE,
    JUMP,
    RIGHT,
    LEFT,
    DOWN,
    UP,
    INTERACT,
    PRIMARY,
    SECONDARY,
    ROLL,
    CROUCH,
    PAUSE
};

class Action
{
    ActionKey m_key = NONE;
    bool      m_isStartType = true;

public:

    Action() {};

    Action(ActionKey m_key, const bool isStartType) :
        m_key(m_key),
        m_isStartType(isStartType)
    {}

    const ActionKey getKey() const;
    const bool isStartType() const;
    const std::string toString() const;
};