#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/CCTransitionFade.hpp>
#include "ShortcutsLayer.hpp"

using namespace geode::prelude;

CCMenuItemSpriteExtra* addShortcutButton(
	CCObject* target,
	float scale = 1.f,
	CircleBaseColor color = CircleBaseColor::Green,
	CircleBaseSize size = CircleBaseSize::Medium) {
	auto shortcutButton = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSpriteFrameName("shortcutIcon.png"_spr, scale, color, size),
			target,
			menu_selector(ShortcutsLayer::onShortcut)
		);
	shortcutButton->setID("shortcut-button"_spr);
	return shortcutButton;
}

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		if (Mod::get()->getSettingValue<bool>("enable-menulayer")) {
			auto shortcutButton = addShortcutButton(this, 1.2f, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
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
			auto shortcutButton = addShortcutButton(this, 1.1f);
			shortcutButton->setID("shortcut-button"_spr);
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
			auto shortcutButton = addShortcutButton(this, 0.9f, CircleBaseColor::Green, CircleBaseSize::Small);
			shortcutButton->setID("shortcut-button"_spr);
			auto menu = this->getChildByID("bottom-right-menu");
			menu->addChild(shortcutButton);
			menu->updateLayout();
		}

		return true;
	}
};