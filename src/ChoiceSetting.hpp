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
    void setChoice(int choice);
    int getChoice() const;
};

class ChoiceSettingNode : public SettingNode {
protected:
    std::vector<std::string> const m_choiceList = {
        "Show",
        "As Silhouette",
        "Hide"
    };
    std::string m_name;
    std::string m_description;
    int m_currentChoice;
    int const m_defaultChoice = 2;
    CCMenu* m_dropdownList;
    CCLabelBMFont* m_dropdownLabel;
    bool init(ChoiceSettingValue* value, float width);
    void onInfo(CCObject*);
    void onDropdown(CCObject*);
    void onClickChoice(CCObject*);
    void clearDropdown();
    void dispatchChanged();
public:
    void commit() override;
    bool hasUncommittedChanges() override;
    bool hasNonDefaultValue() override;
    void resetToDefault() override;
    static ChoiceSettingNode* create(ChoiceSettingValue* value, float width);
};