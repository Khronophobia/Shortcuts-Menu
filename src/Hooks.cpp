#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "ShortcutsPopup.hpp"

using namespace geode::prelude;

CCMenuItemSpriteExtra* createShorcutsButton(CCObject* target, SEL_MenuHandler selector, CircleBaseSize size) {
    auto button = CCMenuItemSpriteExtra::create(
        CircleButtonSprite::createWithSpriteFrameName(
            "shortcutIcon.png"_spr, 1.f,
            CircleBaseColor::Green, size
        ), target, selector
    );
    button->setID("shortcuts-menu-button"_spr);
    return button;
}

class $modify(SMMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (Mod::get()->getSettingValue<bool>("enable-menulayer")) {
            auto shortcutButton = createShorcutsButton(this, menu_selector(SMMenuLayer::onShortcutsMenu), CircleBaseSize::MediumAlt);
            auto menu = this->getChildByID("bottom-menu");
            menu->addChild(shortcutButton);
            menu->updateLayout();
        }

        return true;
    }

    void onShortcutsMenu(CCObject*) {
        ShortcutsPopup::create()->show();
    }
};

class $modify(SMEditLevelLayer, EditLevelLayer) {
    bool init(GJGameLevel* level) {
        if (!EditLevelLayer::init(level)) return false;

        if (Mod::get()->getSettingValue<bool>("enable-editlevellayer")) {
            auto shortcutButton = createShorcutsButton(this, menu_selector(SMEditLevelLayer::onShortcutsMenu), CircleBaseSize::Medium);
            auto menu = this->getChildByID("level-actions-menu");
            menu->addChild(shortcutButton);
            menu->updateLayout();
        }

        return true;
    }

    void onShortcutsMenu(CCObject*) {
        ShortcutsPopup::create()->show();
    }
};

class $modify(SMCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        if (Mod::get()->getSettingValue<bool>("enable-creatorlayer")) {
            auto shortcutButton = createShorcutsButton(this, menu_selector(SMCreatorLayer::onShortcutsMenu), CircleBaseSize::Small);
            auto menu = this->getChildByID("bottom-right-menu");
            menu->addChild(shortcutButton);
            menu->updateLayout();
        }

        return true;
    }

    void onShortcutsMenu(CCObject*) {
        ShortcutsPopup::create()->show();
    }
};

class $modify(SMLevelBrowserLayer, LevelBrowserLayer) {
    bool init(GJSearchObject* p0) {
        if (!LevelBrowserLayer::init(p0)) return false;

        if (Mod::get()->getSettingValue<bool>("enable-levelbrowserlayer")) {
            auto shortcutButton = createShorcutsButton(this, menu_selector(SMLevelBrowserLayer::onShortcutsMenu), CircleBaseSize::Small);
            auto menu = this->getChildByID("back-menu");
            auto backButton = menu->getChildByID("back-button");
            menu->insertAfter(shortcutButton, backButton);
            menu->updateLayout();
        }

        return true;
    }

    void onShortcutsMenu(CCObject*) {
        ShortcutsPopup::create()->show();
    }
};

class $modify(SMLevelSearchLayer, LevelSearchLayer) {
    bool init(int p0) {
        if (!LevelSearchLayer::init(p0)) return false;

        if (Mod::get()->getSettingValue<bool>("enable-levelsearchlayer")) {
            auto shortcutButton = createShorcutsButton(this, menu_selector(SMLevelSearchLayer::onShortcutsMenu), CircleBaseSize::Small);
            auto menu = this->getChildByID("bottom-left-menu");
            menu->addChild(shortcutButton);
            menu->updateLayout();
        }

        return true;
    }

    void onShortcutsMenu(CCObject*) {
        ShortcutsPopup::create()->show();
    }
};

class $modify(SMLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        if (Mod::get()->getSettingValue<bool>("enable-levelinfolayer")) {
            auto shortcutButton = createShorcutsButton(this, menu_selector(SMLevelInfoLayer::onShortcutsMenu), CircleBaseSize::Small);
            auto menu = this->getChildByID("back-menu");
            auto backButton = menu->getChildByID("back-button");
            menu->insertAfter(shortcutButton, backButton);
            menu->updateLayout();
        }

        return true;
    }

    void onShortcutsMenu(CCObject*) {
        ShortcutsPopup::create()->show();
    }
};
