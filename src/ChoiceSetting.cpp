#include <Geode/Geode.hpp>
#include "ChoiceSetting.hpp"

bool ChoiceSettingValue::load(matjson::Value const& json) {
    if (!json.is<int>()) return false;
    m_choice = json.as<int>();
    return true;
}

bool ChoiceSettingValue::save(matjson::Value& json) const {
    json = m_choice;
    return true;
}

SettingNode* ChoiceSettingValue::createNode(float width) {
    return ChoiceSettingNode::create(this, width);
}

void ChoiceSettingValue::setChoice(int choice) {
    m_choice = choice;
}

int ChoiceSettingValue::getChoice() const {
    return m_choice;
}

bool ChoiceSettingNode::init(ChoiceSettingValue* value, float width) {
    if (!SettingNode::init(value)) return false;

    m_currentChoice = value->getChoice();
    m_name = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("name");
    m_description = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("description");

    this->setContentSize({width, 40.f});
    auto name = CCLabelBMFont::create(m_name.c_str(), "bigFont.fnt");
    name->setAnchorPoint({0.f, 0.5f});
    name->setScale(0.5f);
    name->setPosition(20.f, this->getContentHeight() / 2.f);
    this->addChild(name);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(0.6f);
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr,
        this,
        menu_selector(ChoiceSettingNode::onInfo)
    );
    infoBtn->setPositionX(-15.f);
    auto menu = CCMenu::create();
    menu->setPosition(this->getContentSize() / 2);
    menu->addChild(infoBtn);
    menu->setContentSize(this->getContentSize());
    this->addChild(menu);

    auto dropdownBox = CCScale9Sprite::create("square.png");
    dropdownBox->setScale(0.3f);
    dropdownBox->setContentSize({500.f, 60.f});
    dropdownBox->setColor({74, 41, 25});
    auto dropdownMenu = CCMenuItemSpriteExtra::create(
        dropdownBox,
        this,
        menu_selector(ChoiceSettingNode::onDropdown)
    );
    dropdownMenu->useAnimationType(MenuAnimationType::Move);
    dropdownMenu->setAnchorPoint({1.f, 0.5f});
    dropdownMenu->setPosition(
        menu->getContentWidth() / 2.f - 6.f,
        0.f
    );
    m_dropdownLabel = CCLabelBMFont::create(
        m_choiceList.at(m_currentChoice).c_str(),
        "bigFont.fnt"
    );
    m_dropdownLabel->setScale(0.5f);
    m_dropdownLabel->setAnchorPoint({0.f, 0.5f});
    m_dropdownLabel->setPosition(
        2.f,
        dropdownMenu->getContentHeight() / 2.f
    );
    dropdownMenu->addChild(m_dropdownLabel);
    auto dropdownArrowBg = CCScale9Sprite::create("square.png");
    dropdownArrowBg->setScale(0.3f);
    dropdownArrowBg->setContentSize({60.f, 60.f});
    dropdownArrowBg->setColor({ 46, 25, 15 });
    dropdownArrowBg->setAnchorPoint({1.f, 0.f});
    dropdownArrowBg->setPositionX(dropdownMenu->getContentWidth());
    dropdownMenu->addChild(dropdownArrowBg);
    auto dropdownArrow = CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png");
    dropdownArrow->setPosition(dropdownArrowBg->getPosition() - CCPoint{
        dropdownArrowBg->getScaledContentSize().width / 2,
        -dropdownArrowBg->getScaledContentSize().height / 2
    });
    dropdownMenu->addChild(dropdownArrow);
    menu->addChild(dropdownMenu);

    m_dropdownList = CCMenu::create();
    m_dropdownList->ignoreAnchorPointForPosition(false);
    m_dropdownList->setPosition(
        dropdownMenu->getPosition() - dropdownMenu->getContentSize() / 2
    );
    m_dropdownList->setContentWidth(150.f);
    m_dropdownList->setAnchorPoint({0.5f, 1.f});
    m_dropdownList->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setGap(0.f)
    );
    menu->addChild(m_dropdownList);

    return true;
}

void ChoiceSettingNode::onDropdown(CCObject* sender) {
    if (m_dropdownList->getChildrenCount() > 0) {
        ChoiceSettingNode::clearDropdown();
    } else {
        this->setZOrder(1);
        for (int i = 0; i < m_choiceList.size(); i++) {
            if (i == m_currentChoice) continue;
            auto choiceBox = CCScale9Sprite::create("square.png");
            choiceBox->setScale(0.3f);
            choiceBox->setContentSize({500.f, 60.f});
            choiceBox->setColor({ 46, 25, 15 });
            auto choiceMenu = CCMenuItemSpriteExtra::create(
                choiceBox,
                this,
                menu_selector(ChoiceSettingNode::onClickChoice)
            );
            choiceMenu->setTag(i);
            choiceMenu->useAnimationType(MenuAnimationType::Move);
            auto choiceLabel = CCLabelBMFont::create(m_choiceList.at(i).c_str(), "bigFont.fnt");
            choiceLabel->setScale(0.5f);
            choiceLabel->setAnchorPoint({0.f, 0.5f});
            choiceLabel->setPosition(
                2.f,
                choiceMenu->getContentHeight() / 2.f
            );
            choiceMenu->addChild(choiceLabel);
            m_dropdownList->addChild(choiceMenu);
        }
        m_dropdownList->updateLayout();
    }
}

void ChoiceSettingNode::clearDropdown() {
    m_dropdownList->removeAllChildren();
    this->setZOrder(0);
}

void ChoiceSettingNode::onClickChoice(CCObject* sender) {
    m_currentChoice = sender->getTag();
    m_dropdownLabel->setString(m_choiceList.at(m_currentChoice).c_str());
    ChoiceSettingNode::clearDropdown();
    this->dispatchChanged();
}

void ChoiceSettingNode::commit() {
    static_cast<ChoiceSettingValue*>(m_value)->setChoice(m_currentChoice);
    this->dispatchCommitted();
}

bool ChoiceSettingNode::hasUncommittedChanges() {
    return m_currentChoice != static_cast<ChoiceSettingValue*>(m_value)->getChoice();
}

bool ChoiceSettingNode::hasNonDefaultValue() {
    return m_currentChoice != m_defaultChoice;
}

void ChoiceSettingNode::resetToDefault() {
    m_currentChoice = m_defaultChoice;
    m_dropdownLabel->setString(m_choiceList.at(m_currentChoice).c_str());
}

void ChoiceSettingNode::onInfo(CCObject* sender) {
    FLAlertLayer::create(
        m_name.c_str(),
        m_description.c_str(),
        "OK"
    )->show();
}

void ChoiceSettingNode::dispatchChanged() {
    SettingNode::dispatchChanged();
    log::info("Dispatch cahnged");
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