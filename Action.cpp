#include "Action.h"

const ActionKey Action::getKey() const
{
    return m_key;
}

const bool Action::isStartType() const
{
    return m_isStartType;
}

const std::string Action::toString() const
{
    return std::to_string(m_key) + " " + std::to_string(m_isStartType);
}