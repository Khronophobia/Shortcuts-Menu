#include <Geode/modify/GJShopLayer.hpp>
#include <Geode/modify/SecretRewardsLayer.hpp>
#include "GameManager.hpp"

using namespace geode::prelude;

class $modify(GJShopLayer) {
    void onBack(CCObject* sender) {
        auto gm = static_cast<CustomGameManager*>(GameManager::get());

        if (gm->m_fields->m_shortcutEntry) {
            gm->m_fields->m_shortcutEntry = false;
            CCTouchDispatcher::get()->unregisterForcePrio(this);

            CCDirector::get()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
            GameManager::get()->fadeInMenuMusic();
        } else {
            GJShopLayer::onBack(sender);
        }
    }
};

class $modify(SecretRewardsLayer) {
    void onBack(CCObject* sender) {
        auto gm = static_cast<CustomGameManager*>(GameManager::get());

        if (gm->m_fields->m_shortcutEntry) {
            gm->m_fields->m_shortcutEntry = false;

            CCDirector::get()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
            GameManager::get()->fadeInMenuMusic();
        } else {
            SecretRewardsLayer::onBack(sender);
        }
    }
};