#include <Geode/Geode.hpp>
#include <matjson/stl_serialize.hpp>
#include "ChoiceSetting.hpp"

using namespace geode::prelude;
using ChoiceVector = std::vector<std::string>;

auto constexpr CHECKBOX_GAP = 24.f;

bool ChoiceSettingValue::load(matjson::Value const& json) {
    if (!json.is<int>()) return false;
    m_choice = json.as_int();
    return true;
}

bool ChoiceSettingValue::save(matjson::Value& json) const {
    json = m_choice;
    return true;
}

SettingNode* ChoiceSettingValue::createNode(float width) {
    return ChoiceSettingNode::create(this, width);
}

int ChoiceSettingValue::getChoice() const {
    return m_choice;
}

int ChoiceSettingValue::getDefaultChoice() const {
    return m_defaultChoice;
}

void ChoiceSettingValue::setChoice(int choice) {
    m_choice = choice;
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
    m_settingKeys = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>();
    m_choiceList = m_settingKeys->json->get<ChoiceVector>("choices");
    m_height = setupHeight();
    m_currentChoice = value->getChoice();

    auto constexpr sidePad = 40.f;
    this->setContentSize({width, m_height});

    if (m_settingKeys->json->contains("name"))
        m_name = m_settingKeys->json->get<std::string>("name");
    else
        m_name = value->getKey();
    m_nameLabel = CCLabelBMFont::create(m_name.c_str(), "bigFont.fnt");
    m_nameLabel->setAnchorPoint({0.f, 0.5f});
    m_nameLabel->limitLabelWidth(width / 2 -50.f, 0.5f, 0.1f);
    m_nameLabel->setPosition(sidePad / 2, m_height / 2);
    this->addChild(m_nameLabel);

    auto menu = CCMenu::create();
    menu->setPosition({
        width - sidePad / 2.f,
        m_height / 2.f
    });
    this->addChild(menu);

    auto btnPos = 15.f;

    if (m_settingKeys->json->contains("description")) {
        auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        infoSpr->setScale(0.6f);
        auto infoBtn = CCMenuItemSpriteExtra::create(
            infoSpr,
            this,
            menu_selector(ChoiceSettingNode::onDescription)
        );
        infoBtn->setPosition(
            -width + sidePad + m_nameLabel->getScaledContentSize().width + btnPos,
            0.f
        );
        menu->addChild(infoBtn);

        btnPos += 20.f;
    }

    auto resetBtnSpr = CCSprite::createWithSpriteFrameName(
        "geode.loader/reset-gold.png"
    );
    resetBtnSpr->setScale(0.5f);
    m_resetBtn = CCMenuItemSpriteExtra::create(
        resetBtnSpr,
        this,
        menu_selector(ChoiceSettingNode::onReset)
    );
    m_resetBtn->setPosition(
        -width + sidePad + m_nameLabel->getScaledContentSize().width + btnPos,
        0.f
    );
    m_resetBtn->setVisible(ChoiceSettingNode::hasNonDefaultValue());
    menu->addChild(m_resetBtn);

    for (size_t i = 0; i < m_choiceList.size(); i++) {
        float yPos = (i * -CHECKBOX_GAP) + ((m_choiceList.size() - 1) / 2.f * CHECKBOX_GAP);

        auto checkbox = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(ChoiceSettingNode::onSelectChoice), 0.6f);
        checkbox->setPosition(ccp(-120.f, yPos));
        checkbox->setTag(i);
        m_choiceButtons->addObject(checkbox);
        menu->addChild(checkbox);

        if (i == m_currentChoice) {
            checkbox->setEnabled(false);
            checkbox->toggle(true);
        }

        auto label = CCLabelBMFont::create(m_choiceList[i].c_str(), "bigFont.fnt");
        label->setAnchorPoint(ccp(0.f, 0.5f));
        label->setPosition(ccp(-105.f, yPos));
        label->limitLabelWidth(110.f, 0.5f, 0.1f);
        menu->addChild(label);
    }

    return true;
}

float ChoiceSettingNode::setupHeight() const {
    return std::max(40.f, m_choiceList.size() * CHECKBOX_GAP + 20.f);
}

void ChoiceSettingNode::setCurrentChoice(int choice) {
    for (auto checkbox : CCArrayExt<CCMenuItemToggler*>(m_choiceButtons)) {
        if (checkbox->getTag() == choice) {
            checkbox->setEnabled(false);
            checkbox->toggle(true);
            continue;
        };

        checkbox->setEnabled(true);
        checkbox->toggle(false);
    }
}

void ChoiceSettingNode::onDescription(CCObject* sender) {
    FLAlertLayer::create(
        m_name.c_str(),
        m_settingKeys->json->get<std::string>("description").c_str(),
        "OK"
    )->show();
}

void ChoiceSettingNode::onReset(CCObject* sender) {
    createQuickPopup(
        "Reset",
        "Are you sure you want to <cr>reset</c> <cl>"
          + m_name
          + "</c> to <cy>default</c>?",
        "Cancel", "Reset",
        [this](auto, bool reset) {
            if (reset) {
                this->resetToDefault();
            }
        }
    );
}

void ChoiceSettingNode::onSelectChoice(CCObject* sender) {
    m_currentChoice = sender->getTag();
    setCurrentChoice(m_currentChoice);
    this->dispatchChanged();
}

void ChoiceSettingNode::commit() {
    static_cast<ChoiceSettingValue*>(this->m_value)->setChoice(m_currentChoice);
    this->dispatchCommitted();
}

bool ChoiceSettingNode::hasUncommittedChanges() {
    return m_currentChoice != static_cast<ChoiceSettingValue*>(this->m_value)->getChoice();
}

bool ChoiceSettingNode::hasNonDefaultValue() {
    return m_currentChoice != static_cast<ChoiceSettingValue*>(this->m_value)->getDefaultChoice();
}

void ChoiceSettingNode::resetToDefault() {
    m_currentChoice = static_cast<ChoiceSettingValue*>(this->m_value)->getDefaultChoice();
    setCurrentChoice(m_currentChoice);
    this->dispatchChanged();
}

void ChoiceSettingNode::dispatchChanged() {
    SettingNode::dispatchChanged();
    m_resetBtn->setVisible(ChoiceSettingNode::hasNonDefaultValue());
    if (ChoiceSettingNode::hasUncommittedChanges()) m_nameLabel->setColor(cc3x(0x1d0));
    else m_nameLabel->setColor(cc3x(0xfff));
}

void ChoiceSettingNode::dispatchCommitted() {
    SettingNode::dispatchCommitted();
    m_nameLabel->setColor(cc3x(0xfff));
}