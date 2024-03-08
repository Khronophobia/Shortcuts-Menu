#include <Geode/Geode.hpp>
#include "ChoiceSetting.hpp"

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

int ChoiceSettingValue::getChoice() {
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

    auto constexpr height = 40.f;
    auto constexpr sidePad = 40.f;
    m_settingKeys = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>();
    m_choiceList = m_settingKeys->json->get<matjson::Array>("choices");
    m_currentChoice = value->getChoice();
    this->setContentSize({width, height});

    auto name = m_settingKeys->json->get<std::string>("name");
    m_nameLabel = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    m_nameLabel->setAnchorPoint({0.f, 0.5f});
    m_nameLabel->limitLabelWidth(width / 2 -50.f, 0.5f, 0.1f);
    m_nameLabel->setPosition(sidePad / 2, height / 2);
    this->addChild(m_nameLabel);

    auto menu = CCMenu::create();
    menu->setPosition({
        width - sidePad / 2.f,
        height / 2.f
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

    m_dropdownBg = CCScale9Sprite::create("square02_small.png");
    m_dropdownBg->setContentSize(m_dropdownSize);
    m_dropdownBg->setScale(0.3f);
    m_dropdownBg->setOpacity(127);
    m_dropdownBtn = CCMenuItemSpriteExtra::create(
        m_dropdownBg, this, menu_selector(ChoiceSettingNode::onDropdown)
    );
    m_dropdownBtn->setAnchorPoint({1.f, 0.5f});
    m_dropdownBg->setAnchorPoint({0.5f, 1.f});
    m_dropdownBg->setPositionY(m_dropdownSize.height * 0.3f);
    // Don't want the button to scale when holding, and setSizeMult didn't do it
    m_dropdownBtn->useAnimationType(MenuAnimationType::Move);
    menu->addChild(m_dropdownBtn);

    auto dropdownArrow = CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png");
    dropdownArrow->setAnchorPoint({1.f, 0.5f});
    dropdownArrow->setPosition({
        m_dropdownBtn->getScaledContentSize().width - 3.f,
        m_dropdownBtn->getScaledContentSize().height / 2
    });
    m_dropdownBtn->addChild(dropdownArrow);

    m_dropdownChoiceWidth =
        m_dropdownBtn->getScaledContentSize().width
          - 6.f;
    m_dropdownLabelWidth =
        m_dropdownChoiceWidth - dropdownArrow->getContentWidth() - 3.f;
    m_dropdownLabel = CCLabelBMFont::create(
        m_choiceList.at(m_currentChoice).as_string().c_str(), // I hope there's a better way than this
        "bigFont.fnt"
    );
    m_dropdownLabel->setAnchorPoint({0.f, 0.5f});
    m_dropdownLabel->setPosition(
        3.f,
        m_dropdownBtn->getScaledContentSize().height / 2
    );
    m_dropdownLabel->limitLabelWidth(m_dropdownLabelWidth, m_dropdownLabelSize, 0.1f);
    m_dropdownBtn->addChild(m_dropdownLabel);

    m_dropdownMenu = CCMenu::create();
    m_dropdownMenu->ignoreAnchorPointForPosition(false);
    m_dropdownMenu->setContentWidth(m_dropdownBtn->getScaledContentSize().width);
    m_dropdownMenu->setAnchorPoint({1.f, 1.f});
    m_dropdownMenu->setPosition(menu->getPosition() - CCPoint{
        0.f,
        m_dropdownBtn->getScaledContentSize().height / 2
    });
    m_dropdownMenu->setVisible(false);
    m_dropdownMenu->setTouchPriority(-5006);
    this->addChild(m_dropdownMenu);

    for (int i = 0; i < m_choiceList.size(); i++) {
        auto choiceBg = CCScale9Sprite::create("square02_001.png");
        choiceBg->setScale(0.3f);
        choiceBg->setContentSize(m_dropdownSize);
        if (i % 2 == 0) {
            choiceBg->setOpacity(63);
        } else {
            choiceBg->setOpacity(0);
        }
        auto choiceBtn = CCMenuItemSpriteExtra::create(
            choiceBg, this, menu_selector(ChoiceSettingNode::onSelectChoice)
        );
        choiceBtn->setAnchorPoint({0.f, 1.f});
        choiceBtn->useAnimationType(MenuAnimationType::Move);
        choiceBtn->setPositionY(
            m_dropdownMenu->getContentHeight()
              - i * m_dropdownBtn->getScaledContentSize().height
        );
        choiceBtn->setTag(i);
        auto choiceLabel = CCLabelBMFont::create(
            m_choiceList.at(i).as_string().c_str(),
            "bigFont.fnt"
        );
        choiceLabel->setAnchorPoint({0.f, 0.5f});
        choiceLabel->setPosition(
            3.f,
            m_dropdownBtn->getScaledContentSize().height / 2
        );
        choiceLabel->limitLabelWidth(m_dropdownChoiceWidth, m_dropdownLabelSize, 0.1f);
        choiceBtn->addChild(choiceLabel);
        m_dropdownMenu->addChild(choiceBtn);
    }

    return true;
}

void ChoiceSettingNode::onDescription(CCObject* sender) {
    FLAlertLayer::create(
        m_settingKeys->json->get<std::string>("name").c_str(),
        m_settingKeys->json->get<std::string>("description").c_str(),
        "OK"
    )->show();
}

void ChoiceSettingNode::onReset(CCObject* sender) {
    createQuickPopup(
        "Reset",
        "Are you sure you want to <cr>reset</c> <cl>"
          + m_settingKeys->json->get<std::string>("name")
          + "</c> to <cy>default</c>?",
        "Cancel", "Reset",
        [this](auto, bool reset) {
            if (reset) {
                this->resetToDefault();
            }
        }
    );
}

void ChoiceSettingNode::onDropdown(CCObject* sender) {
    if (m_dropdownMenu->isVisible()) {
        ChoiceSettingNode::clearDropdown();
        return;
    }
    m_dropdownBg->setContentSize({
        m_dropdownSize.width,
        m_dropdownSize.height * (m_choiceList.size() + 1)
    });
    auto savedChoice = static_cast<ChoiceSettingValue*>(m_value)->getChoice();
    for (int i = 0; i < m_choiceList.size(); i++) {
        auto choiceBtn = m_dropdownMenu->getChildByTag(i);
        auto choiceLabel = getChildOfType<CCLabelBMFont>(choiceBtn, 0);
        if (i == savedChoice) choiceLabel->setColor({ 127, 127, 127 });
        else choiceLabel->setColor({255, 255, 255});
    }
    m_dropdownMenu->setVisible(true);
    this->setZOrder(1);
}

void ChoiceSettingNode::onSelectChoice(CCObject* sender) {
    ChoiceSettingNode::selectChoice(sender->getTag());
    ChoiceSettingNode::clearDropdown();
    this->dispatchChanged();
}

void ChoiceSettingNode::selectChoice(int choice) {
    m_currentChoice = choice;
    m_dropdownLabel->setString(m_choiceList.at(m_currentChoice).as_string().c_str());
    m_dropdownLabel->limitLabelWidth(m_dropdownLabelWidth, m_dropdownLabelSize, 0.1f);
}

void ChoiceSettingNode::clearDropdown() {
    m_dropdownBg->setContentSize(m_dropdownSize);
    m_dropdownMenu->setVisible(false);
    this->setZOrder(0);
}

void ChoiceSettingNode::commit() {
    static_cast<ChoiceSettingValue*>(m_value)->setChoice(m_currentChoice);
    this->dispatchCommitted();
}

bool ChoiceSettingNode::hasUncommittedChanges() {
    return m_currentChoice != static_cast<ChoiceSettingValue*>(m_value)->getChoice();
}

bool ChoiceSettingNode::hasNonDefaultValue() {
    return m_currentChoice != static_cast<ChoiceSettingValue*>(m_value)->getDefaultChoice();
}

void ChoiceSettingNode::resetToDefault() {
    ChoiceSettingNode::selectChoice(static_cast<ChoiceSettingValue*>(m_value)->getDefaultChoice());
    this->dispatchChanged();
}

void ChoiceSettingNode::dispatchChanged() {
    SettingNode::dispatchChanged();
    m_resetBtn->setVisible(ChoiceSettingNode::hasNonDefaultValue());
    if (ChoiceSettingNode::hasUncommittedChanges()) m_nameLabel->setColor({0, 255, 0});
    else m_nameLabel->setColor({255, 255, 255});
}

void ChoiceSettingNode::dispatchCommitted() {
    SettingNode::dispatchCommitted();
    m_nameLabel->setColor({255, 255, 255});
    ChoiceSettingNode::clearDropdown();
}