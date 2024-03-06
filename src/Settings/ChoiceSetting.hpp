#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>

using namespace geode::prelude;

class ChoiceSettingValue : public SettingValue {
public:
    int m_choice;
    int const m_defaultChoice;
    ChoiceSettingValue(std::string const& key, std::string const& modID, int choice)
      : SettingValue(key, modID), m_choice(choice), m_defaultChoice(choice) {}
    bool load(matjson::Value const& json) override;
    bool save(matjson::Value& json) const override;
    SettingNode* createNode(float width) override;
};

class ChoiceSettingNode : public SettingNode {
protected:
    int m_currentChoice;
    bool init(ChoiceSettingValue* value, float width);
public:
    void commit() override;
    bool hasUncommittedChanges() override;
    bool hasNonDefaultValue() override;
    void resetToDefault() override;
    static ChoiceSettingNode* create(ChoiceSettingValue* value, float width);
};