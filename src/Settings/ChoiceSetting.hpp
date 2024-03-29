#pragma once
#include <Geode/loader/SettingNode.hpp>

struct ChoiceSettingStruct {
    int m_choice;
};

class ChoiceSettingValue : public geode::SettingValue {
protected:
    int m_choice;
    int const m_defaultChoice;
public:
    ChoiceSettingValue(std::string const& key, std::string const& modID, int choice)
      : SettingValue(key, modID), m_choice(choice), m_defaultChoice(choice) {}
    bool load(matjson::Value const& json) override;
    bool save(matjson::Value& json) const override;
    geode::SettingNode* createNode(float width) override;
    int getChoice();
    int getDefaultChoice() const;
    void setChoice(int choice);
};

class ChoiceSettingNode : public geode::SettingNode {
protected:
    std::string m_name;
    cocos2d::CCLabelBMFont* m_nameLabel;
    // Was worried it might be hard to click on phones
    #ifdef GEODE_IS_ANDROID
    cocos2d::CCSize const m_dropdownSize = {400.f, 90.f};
    float const m_dropdownLabelSize = 0.65f;
    #else
    cocos2d::CCSize const m_dropdownSize = {400.f, 60.f};
    float const m_dropdownLabelSize = 0.5f;
    #endif
    int m_currentChoice;
    matjson::Array m_choiceList;
    CCMenuItemSpriteExtra* m_resetBtn;
    std::optional<geode::CustomSetting> m_settingKeys;
    cocos2d::extension::CCScale9Sprite* m_dropdownBtnBg;
    cocos2d::extension::CCScale9Sprite* m_dropdownMenuBg;
    cocos2d::extension::CCScale9Sprite* m_thing;
    CCMenuItemSpriteExtra* m_dropdownBtn;
    cocos2d::CCLabelBMFont* m_dropdownLabel;
    float m_dropdownLabelWidth;
    float m_dropdownChoiceWidth;
    cocos2d::CCMenu* m_dropdownMenu;
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
struct geode::SettingValueSetter<ChoiceSettingStruct> {
    static ChoiceSettingStruct get(SettingValue* setting) {
        auto choiceSett = static_cast<ChoiceSettingValue*>(setting);
        struct ChoiceSettingStruct defaultStruct = {choiceSett->getChoice()};
        return defaultStruct;
    }
    static void set(ChoiceSettingValue* setting, ChoiceSettingStruct const& value) {
        setting->setChoice(value.m_choice);
    }
};