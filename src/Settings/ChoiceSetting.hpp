#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>

using namespace geode::prelude;

class ChoiceSettingValue : public SettingValue {
protected:
    int m_choice;
    int const m_defaultChoice;
public:
    ChoiceSettingValue(std::string const& key, std::string const& modID, int choice)
      : SettingValue(key, modID), m_choice(choice), m_defaultChoice(choice) {}
    bool load(matjson::Value const& json) override;
    bool save(matjson::Value& json) const override;
    SettingNode* createNode(float width) override;
    int getChoice();
    int getDefaultChoice() const;
    void setChoice(int choice);
};

class ChoiceSettingNode : public SettingNode {
protected:
    CCLabelBMFont* m_nameLabel;
    CCSize const m_dropdownSize = {400.f, 60.f};
    int m_currentChoice;
    matjson::Array m_choiceList;
    CCMenuItemSpriteExtra* m_resetBtn;
    std::optional<CustomSetting> m_settingKeys;
    CCScale9Sprite* m_dropdownBg;
    CCMenuItemSpriteExtra* m_dropdownBtn;
    CCLabelBMFont* m_dropdownLabel;
    float m_dropdownLabelWidth;
    CCMenu* m_dropdownMenu;
    bool init(ChoiceSettingValue* value, float width);
    void onDescription(CCObject*);
    void onReset(CCObject*);
    void onDropdown(CCObject*);
    void onSelectChoice(CCObject*);
    void selectChoice(int choice);
    void clearDropdown();
public:
    void commit() override;
    bool hasUncommittedChanges() override;
    bool hasNonDefaultValue() override;
    void resetToDefault() override;
    void dispatchChanged();
    void dispatchCommitted();
    static ChoiceSettingNode* create(ChoiceSettingValue* value, float width);
};

template<>
struct SettingValueSetter<int> {

};