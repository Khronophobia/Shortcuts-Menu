#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ChoiceSettingValue : public SettingValue {
    std::string m_choice;
public:
    bool load(matjson::Value const& json) override;
    bool save(matjson::Value& json) const override;
    SettingNode* createNode(float width) override;
};

class ChoiceSettingNode : public SettingNode {
protected:
    bool init(ChoiceSettingValue* value, float width);
public:
    void commit() override;
    bool hasUncommittedChanges() override;
    bool hasNonDefaultValue() override;
    static ChoiceSettingNode* create(ChoiceSettingValue* value, float width);
};