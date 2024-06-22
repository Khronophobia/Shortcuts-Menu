#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "ShortcutsPopup.hpp"

using namespace geode::prelude;

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (Mod::get()->getSettingValue<bool>("enable-menulayer")) {
            auto shortcutButton = CCMenuItemExt::createSpriteExtra(
                CircleButtonSprite::createWithSpriteFrameName(
                    "shortcutIcon.png"_spr, 1.f,
                    CircleBaseColor::Green, CircleBaseSize::MediumAlt
                ),
                [](auto) {
                    ShortcutsPopup::create()->show();
                }
            );
            auto menu = this->getChildByID("bottom-menu");
            menu->addChild(shortcutButton);
            menu->updateLayout();
        }

        return true;
    }
};

class $modify(EditLevelLayer) {
    bool init(GJGameLevel* level) {
        if (!EditLevelLayer::init(level)) return false;

        if (Mod::get()->getSettingValue<bool>("enable-editlevellayer")) {
            auto shortcutButton = CCMenuItemExt::createSpriteExtra(
                CircleButtonSprite::createWithSpriteFrameName(
                    "shortcutIcon.png"_spr, 1.f,
                    CircleBaseColor::Green, CircleBaseSize::Medium
                ),
                [](auto) {
                    ShortcutsPopup::create()->show();
                }
            );
            auto menu = this->getChildByID("level-actions-menu");
            menu->addChild(shortcutButton);
            menu->updateLayout();
        }

        return true;
    }
};

class $modify(CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        if (Mod::get()->getSettingValue<bool>("enable-creatorlayer")) {
            auto shortcutButton = CCMenuItemExt::createSpriteExtra(
                CircleButtonSprite::createWithSpriteFrameName(
                    "shortcutIcon.png"_spr, 1.f,
                    CircleBaseColor::Green, CircleBaseSize::Small
                ),
                [](auto) {
                    ShortcutsPopup::create()->show();
                }
            );
            auto menu = this->getChildByID("bottom-right-menu");
            menu->addChild(shortcutButton);
            menu->updateLayout();
        }

        return true;
    }
};

class $modify(LevelBrowserLayer) {
    bool init(GJSearchObject* p0) {
        if (!LevelBrowserLayer::init(p0)) return false;

        if (Mod::get()->getSettingValue<bool>("enable-levelbrowserlayer")) {
            auto shortcutButton = CCMenuItemExt::createSpriteExtra(
                CircleButtonSprite::createWithSpriteFrameName(
                    "shortcutIcon.png"_spr, 1.f,
                    CircleBaseColor::Green, CircleBaseSize::Small
                ),
                [](auto) {
                    ShortcutsPopup::create()->show();
                }
            );
            auto menu = this->getChildByID("back-menu");
            auto backButton = menu->getChildByID("back-button");
            menu->insertAfter(shortcutButton, backButton);
            menu->updateLayout();
        }

        return true;
    }
};

class $modify(LevelSearchLayer) {
    bool init(int p0) {
        if (!LevelSearchLayer::init(p0)) return false;

        if (Mod::get()->getSettingValue<bool>("enable-levelsearchlayer")) {
            auto shortcutButton = CCMenuItemExt::createSpriteExtra(
                CircleButtonSprite::createWithSpriteFrameName(
                    "shortcutIcon.png"_spr, 1.f,
                    CircleBaseColor::Green, CircleBaseSize::Small
                ),
                [](auto) {
                    ShortcutsPopup::create()->show();
                }
            );
            auto menu = this->getChildByID("bottom-left-menu");
            menu->addChild(shortcutButton);
            menu->updateLayout();
        }

        return true;
    }
};

class $modify(LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        if (Mod::get()->getSettingValue<bool>("enable-levelinfolayer")) {
            auto shortcutButton = CCMenuItemExt::createSpriteExtra(
                CircleButtonSprite::createWithSpriteFrameName(
                    "shortcutIcon.png"_spr, 1.f,
                    CircleBaseColor::Green, CircleBaseSize::Small
                ),
                [](auto) {
                    ShortcutsPopup::create()->show();
                }
            );
            auto menu = this->getChildByID("back-menu");
            auto backButton = menu->getChildByID("back-button");
            menu->insertAfter(shortcutButton, backButton);
            menu->updateLayout();
        }

        return true;
    }
};
