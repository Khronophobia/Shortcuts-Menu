#pragma once
#include <Geode/modify/GameManager.hpp>

class $modify(CustomGameManager, GameManager) {
    struct Fields {
        bool m_shortcutEntry = false;
    };
};