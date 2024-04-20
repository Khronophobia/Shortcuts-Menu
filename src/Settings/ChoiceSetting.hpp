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
    int getChoice() const;
    int getDefaultChoice() const;
    void setChoice(int choice);
};

class ChoiceSettingNode : public geode::SettingNode {
protected:
    std::optional<geode::CustomSetting> m_settingKeys;
    std::string m_name;
    float m_height;
    std::vector<std::string> m_choiceList;
    int m_currentChoice;
    cocos2d::CCLabelBMFont* m_nameLabel;
    CCMenuItemSpriteExtra* m_resetBtn;
    geode::Ref<cocos2d::CCArray> m_choiceButtons = cocos2d::CCArray::create();

    bool init(ChoiceSettingValue* value, float width);
    float setupHeight() const;
    void setCurrentChoice(int choice);
    void onDescription(CCObject*);
    void onReset(CCObject*);
    void onSelectChoice(CCObject*);
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