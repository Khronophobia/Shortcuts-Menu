#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>

using namespace geode::prelude;

class ChoiceSettingValue : public SettingValue {
protected:
    int m_choice;
public:
    ChoiceSettingValue(std::string const& key, std::string const& modID, int const& choice)
        : SettingValue(key, modID), m_choice(choice) {}
    bool load(matjson::Value const& json) override;
    bool save(matjson::Value& json) const override;
    SettingNode* createNode(float width) override;
};

class ChoiceSettingNode : public SettingNode {
protected:
    std::vector<std::string> m_choices;
    std::string m_name;
    std::string m_description;
    bool init(ChoiceSettingValue* value, float width);
public:
    void commit() override;
    bool hasUncommittedChanges() override;
    bool hasNonDefaultValue() override;
    void resetToDefault() override;
    void onInfo(CCObject*);
    static ChoiceSettingNode* create(ChoiceSettingValue* value, float width);
};