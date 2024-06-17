#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ShortcutsPopup.hpp"
#include "Settings/ChoiceSetting.hpp"

using namespace geode::prelude;

ShortcutsPopup* ShortcutsPopup::create() {
    auto ret = new (std::nothrow) ShortcutsPopup();
    if (ret && ret->initAnchored(400.f, 250.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ShortcutsPopup::setup() {
    m_noElasticity = GameManager::sharedState()->getGameVariable("0168"); // Fast Menu option
    setTitle("Shortcuts Menu");

    auto screenMenu = CCMenu::create();
    screenMenu->ignoreAnchorPointForPosition(false);

    m_mainLayer->addChildAtPosition(screenMenu, Anchor::Center);
    auto prevPageBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this, menu_selector(ShortcutsPopup::onArrowNavigate)
    );
    prevPageBtn->setTag(-1);
    screenMenu->addChildAtPosition(
        prevPageBtn, Anchor::Left, ccp(prevPageBtn->getContentWidth() / 2 + 4.f, 0)
    );

    auto nextPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextPageSpr->setFlipX(true);
    auto nextPageBtn = CCMenuItemSpriteExtra::create(
        nextPageSpr,
        this, menu_selector(ShortcutsPopup::onArrowNavigate)
    );
    nextPageBtn->setTag(1);
    screenMenu->addChildAtPosition(
        nextPageBtn, Anchor::Right, ccp(-prevPageBtn->getContentWidth() / 2 - 4.f, 0)
    );

    m_pageArray = CCArray::createWithCapacity(m_pageAmount);

    // ------
    // PAGE 1
    // ------
    auto page1Layer = CCLayer::create();
    page1Layer->ignoreAnchorPointForPosition(false);
    page1Layer->setContentSize(m_size);
    auto page1Menu = CCMenu::create();
    page1Menu->ignoreAnchorPointForPosition(false);
    page1Menu->setContentSize(m_size - ccp(40.f, 50.f));
    page1Layer->addChildAtPosition(page1Menu, Anchor::Center, ccp(0.f, -10.f));

    // ------
    // PAGE 2
    // ------
    auto page2Layer = CCLayer::create();
    page2Layer->ignoreAnchorPointForPosition(false);
    page2Layer->setContentSize(m_size);

    // ------
    // PAGE 3
    // ------
    auto page3Layer = CCLayer::create();
    page3Layer->ignoreAnchorPointForPosition(false);
    page3Layer->setContentSize(m_size);

    // ------
    // PAGE 4
    // ------
    auto page4Layer = CCLayer::create();
    page4Layer->ignoreAnchorPointForPosition(false);
    page4Layer->setContentSize(m_size);

    m_pageArray->addObject(page1Layer);
    m_pageArray->addObject(page2Layer);
    m_pageArray->addObject(page3Layer);
    m_pageArray->addObject(page4Layer);

    m_navigateBtnMenu = CCMenu::create();
    m_navigateBtnMenu->ignoreAnchorPointForPosition(false);
    m_navigateBtnMenu->setLayout(RowLayout::create());
    m_mainLayer->addChildAtPosition(m_navigateBtnMenu, Anchor::Bottom, ccp(0.f, -12.f));

    for (int i = 0; i < m_pageAmount; ++i) {
        auto navBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("gj_navDotBtn_off_001.png"),
            this, menu_selector(ShortcutsPopup::onButtonNavigate)
        );
        navBtn->setTag(i);
        m_navigateBtnMenu->addChild(navBtn);
    }
    m_navigateBtnMenu->updateLayout();

    refreshPage();
    return true;
}

void ShortcutsPopup::onArrowNavigate(CCObject* sender) {
    m_currentPage += sender->getTag();
    if (m_currentPage >= m_pageAmount) {
        m_currentPage = 0;
    } else if (m_currentPage < 0) {
        m_currentPage = m_pageAmount - 1;
    }
    refreshPage();
}

void ShortcutsPopup::onButtonNavigate(CCObject* sender) {
    m_currentPage = sender->getTag();
    refreshPage();
}

void ShortcutsPopup::refreshPage() {
    if (m_currentPageLayer) m_currentPageLayer->removeFromParent();

    m_currentPageLayer = static_cast<CCLayer*>(m_pageArray->objectAtIndex(m_currentPage));
    m_mainLayer->addChildAtPosition(m_currentPageLayer, Anchor::Center);

    for (int i = 0; i < m_pageAmount; ++i) {
        auto btn = static_cast<CCMenuItemSpriteExtra*>(m_navigateBtnMenu->getChildByTag(i));

        if (i == m_currentPage) {
            btn->setSprite(CCSprite::createWithSpriteFrameName("gj_navDotBtn_on_001.png"));
        } else {
            btn->setSprite(CCSprite::createWithSpriteFrameName("gj_navDotBtn_off_001.png"));
        }
    }
}