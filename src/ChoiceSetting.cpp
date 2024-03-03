#include <Geode/Geode.hpp>
#include "ChoiceSetting.hpp"

bool ChoiceSettingValue::load(matjson::Value const& json) {
    return true;
}

bool ChoiceSettingValue::save(matjson::Value& json) const {
    return true;
}

SettingNode* ChoiceSettingValue::createNode(float width) {
    return ChoiceSettingNode::create(this, width);
}

bool ChoiceSettingNode::init(ChoiceSettingValue* value, float width) {
    if (!SettingNode::init(value)) return false;

    this->setContentSize({width, 40.f});
    m_name = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("name");
    m_description = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("description");
    auto name = CCLabelBMFont::create(m_name.c_str(), "bigFont.fnt");
    name->setAnchorPoint({0.f, 0.5f});
    name->setScale(0.5f);
    name->setPosition(20.f, this->getContentHeight() / 2.f);
    this->addChild(name);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(0.6f);
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr,
        this,
        menu_selector(ChoiceSettingNode::onInfo)
    );
    auto infoMenu = CCMenu::create();
    infoMenu->setPosition(
        name->getContentWidth() / 2.f + 35.f,
        this->getContentHeight() / 2
    );
    infoMenu->addChild(infoBtn);
    this->addChild(infoMenu);

    return true;
}

void ChoiceSettingNode::commit() {}

bool ChoiceSettingNode::hasUncommittedChanges() {
    return false;
}

bool ChoiceSettingNode::hasNonDefaultValue() {
    return true;
}

void ChoiceSettingNode::resetToDefault() {}

void ChoiceSettingNode::onInfo(CCObject* sender) {
    FLAlertLayer::create(
        m_name.c_str(),
        m_description.c_str(),
        "OK"
    )->show();
}

ChoiceSettingNode* ChoiceSettingNode::create(ChoiceSettingValue* value, float width) {
    auto ret = new ChoiceSettingNode();
    if (ret && ret->init(value, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}