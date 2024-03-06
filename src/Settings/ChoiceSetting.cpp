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

ChoiceSettingNode* ChoiceSettingNode::create(ChoiceSettingValue* value, float width) {
    auto ret = new ChoiceSettingNode();
    if (ret && ret->init(value, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChoiceSettingNode::init(ChoiceSettingValue* value, float width) {
    if (!SettingNode::init(value)) return false;

    this->setContentSize({width, 40.f});

    return true;
}

void ChoiceSettingNode::commit() {}

bool ChoiceSettingNode::hasUncommittedChanges() {
    return false;
}

bool ChoiceSettingNode::hasNonDefaultValue() {
    return false;
}

void ChoiceSettingNode::resetToDefault() {
    m_currentChoice = static_cast<ChoiceSettingValue*>(m_value)->m_defaultChoice;
}