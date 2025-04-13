#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ShortcutsPopup.hpp"
#include "GameManager.hpp"

using namespace geode::prelude;

ShortcutsPopup* ShortcutsPopup::create() {
    auto ret = new (std::nothrow) ShortcutsPopup();
    if (ret && ret->initAnchored(400.f, 250.f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

namespace ShowSpoilers {
    constexpr std::string_view Hide = "Hide";
    constexpr std::string_view Silhouette = "As Silhouette";
    constexpr std::string_view Show = "Show";
};

bool ShortcutsPopup::setup() {
    m_noElasticity = GameManager::sharedState()->getGameVariable("0168"); // Fast Menu option
    m_currentPage = Mod::get()->getSavedValue<int>("current-page", 0);
    CCSpriteFrameCache::get()->addSpriteFramesWithFile("SecretSheet.plist");
    setTitle("Shortcuts Menu");

    std::string showSpoilersChoice = Mod::get()->getSettingValue<std::string>("show-spoilers");

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

    auto modSettingsBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png"),
        this, menu_selector(ShortcutsPopup::onModSettings)
    );
    m_buttonMenu->addChildAtPosition(modSettingsBtn, Anchor::TopRight, ccp(-3.f, -3.f));

    // ------
    // PAGE 1
    // ------
    auto page1Layer = CCLayer::create();
    page1Layer->ignoreAnchorPointForPosition(false);

    auto page1Menu = CCMenu::create();
    page1Menu->ignoreAnchorPointForPosition(false);
    page1Menu->setContentSize(m_size - ccp(40.f, 50.f));
    page1Menu->setLayout(
        AxisLayout::create()
            ->setGrowCrossAxis(true)
    );
    page1Layer->addChildAtPosition(page1Menu, Anchor::Center, ccp(0.f, -8.f));

    auto garageBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png"),
        this, menu_selector(ShortcutsPopup::onShortcutGarage)
    );
    page1Menu->addChild(garageBtn);
    auto mainMenuBtn = CCMenuItemSpriteExtra::create(
        CrossButtonSprite::createWithSpriteFrameName("menuBtn.png"_spr, 0.9f),
        this, menu_selector(ShortcutsPopup::onShortcutMainMenu)
    );
    page1Menu->addChild(mainMenuBtn);
    auto creatorBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_creatorBtn_001.png"),
        this, menu_selector(ShortcutsPopup::onShortcutCreator)
    );
    creatorBtn->setLayoutOptions(
        AxisLayoutOptions::create()->setBreakLine(true),
        false
    );
    page1Menu->addChild(creatorBtn);

    auto exitGameBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
        this, menu_selector(ShortcutsPopup::onShortcutExit)
    );
    page1Menu->addChild(exitGameBtn);
    auto restartGameSpr = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
    restartGameSpr->setScale(0.75f);
    auto restartGameBtn = CCMenuItemSpriteExtra::create(
        restartGameSpr,
        this, menu_selector(ShortcutsPopup::onShortcutRestart)
    );
    page1Menu->addChild(restartGameBtn);
    auto settingsBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
        this, menu_selector(ShortcutsPopup::onShortcutSettings)
    );
    page1Menu->addChild(settingsBtn);

    auto dailyChestBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_dailyRewardBtn_001.png"),
        this, menu_selector(MenuLayer::onDaily)
    );
    page1Menu->addChild(dailyChestBtn);

    page1Menu->updateLayout();

    // ------
    // PAGE 2
    // ------
    auto page2Layer = CCLayer::create();
    page2Layer->ignoreAnchorPointForPosition(false);

    auto onlinePageMenu = CCMenu::create();
    onlinePageMenu->ignoreAnchorPointForPosition(false);
    onlinePageMenu->setContentSize(m_size - ccp(40.f, 50.f));
    onlinePageMenu->setLayout(
        AxisLayout::create()
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    page2Layer->addChildAtPosition(onlinePageMenu, Anchor::Center, ccp(0.f, -8.f));

    auto createdLvlBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_createBtn_001.png"),
        this, menu_selector(CreatorLayer::onMyLevels)
    );
    onlinePageMenu->addChild(createdLvlBtn);
    auto savedLvlBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_savedBtn_001.png"),
        this, menu_selector(CreatorLayer::onSavedLevels)
    );
    onlinePageMenu->addChild(savedLvlBtn);
    auto questBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_challengeBtn_001.png"),
        this, menu_selector(CreatorLayer::onChallenge)
    );
    onlinePageMenu->addChild(questBtn);
    auto dailyLvlBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_dailyBtn_001.png"),
        this, menu_selector(CreatorLayer::onDailyLevel)
    );
    onlinePageMenu->addChild(dailyLvlBtn);
    auto weeklyBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_weeklyBtn_001.png"),
        this,
        menu_selector(CreatorLayer::onWeeklyLevel)
    );
    onlinePageMenu->addChild(weeklyBtn);
    auto featuredLvlBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_featuredBtn_001.png"),
        this,
        menu_selector(CreatorLayer::onFeaturedLevels)
    );
    onlinePageMenu->addChild(featuredLvlBtn);
    auto searchLvlBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_searchBtn_001.png"),
        this,
        menu_selector(CreatorLayer::onOnlineLevels)
    );
    searchLvlBtn->setLayoutOptions(
        AxisLayoutOptions::create()->setBreakLine(true)  
    );
    onlinePageMenu->addChild(searchLvlBtn);

    if (
        isTreasureRoomUnlocked()
        || showSpoilersChoice != ShowSpoilers::Hide
    ) {
        char const* treasureRoomSprName;
        if (isTreasureRoomUnlocked()) {
            treasureRoomSprName = "secretDoorBtn_open_001.png";
        } else {
            treasureRoomSprName = "secretDoorBtn_closed_001.png";
        }

        auto treasureRoomBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName(treasureRoomSprName),
            this, menu_selector(ShortcutsPopup::onShortcutTreasureRoom)
        );
        treasureRoomBtn->setLayoutOptions(
            AxisLayoutOptions::create()->setScaleLimits(1.f, 1.f)
        );
        onlinePageMenu->addChild(treasureRoomBtn);
    }

    onlinePageMenu->updateLayout();

    // ------
    // PAGE 3
    // ------
    auto page3Layer = CCLayer::create();
    page3Layer->ignoreAnchorPointForPosition(false);

    auto vaultMenu = CCMenu::create();
    vaultMenu->ignoreAnchorPointForPosition(false);
    vaultMenu->setContentSize(m_size - ccp(40.f, 50.f));
    vaultMenu->setLayout(AxisLayout::create()->setGap(20.f));
    page3Layer->addChildAtPosition(vaultMenu, Anchor::Center, ccp(0.f, -8.f));

    // Vault
    if (
        // Check if collected user coins is less than 10
        // 10 user coins are needed to unlock the Vault
        (GameStatsManager::sharedState()->getStat("12") < 10)
        && showSpoilersChoice != ShowSpoilers::Show
    ) {
        if (showSpoilersChoice == ShowSpoilers::Silhouette) {
            auto vault = CCSprite::createWithSpriteFrameName("vaultLocked.png"_spr);
            vaultMenu->addChild(vault);
        }
    } else {
        auto vault1 = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_secretLock_001.png"),
            this, menu_selector(OptionsLayer::onSecretVault)
        );
        // TODO: Create a custom callback for this
        vaultMenu->addChild(vault1);
    }
    // Vault of Secrets
    if (
        // Check if total diamonds is less than 50
        // 50 diamonds are needed to unlock the Vault of Secrets
        (GameStatsManager::sharedState()->getStat("13") < 50)
        && showSpoilersChoice != ShowSpoilers::Show
    ) {
        if (showSpoilersChoice == ShowSpoilers::Silhouette) {
            auto vault = CCSprite::createWithSpriteFrameName("vaultLocked.png"_spr);
            vaultMenu->addChild(vault);
        }
    } else {
        auto vault2 = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_secretLock2_001.png"),
            this, menu_selector(CreatorLayer::onSecretVault)
        );
        // TODO: Create a custom callback for this
        auto lockSpr = CCSprite::createWithSpriteFrameName("GJ_secretLock2_2_001.png");
        lockSpr->setZOrder(-1);
        lockSpr->setPosition(vault2->getNormalImage()->getPosition());
        vault2->getNormalImage()->addChild(lockSpr);

        vaultMenu->addChild(vault2);
    }
    // Chamber of Time
    if (
        !GameManager::sharedState()->getUGV("8")
        && showSpoilersChoice != ShowSpoilers::Show
    ) {
        if (showSpoilersChoice == ShowSpoilers::Silhouette) {
            auto vault = CCSprite::createWithSpriteFrameName("vaultLocked.png"_spr);
            vaultMenu->addChild(vault);
        }
    } else {
        auto vault3 = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_secretLock3_001.png"),
            this, menu_selector(LevelPage::onSecretDoor)
        );
        // TODO: Create a custom callback for this
        vaultMenu->addChild(vault3);
    }

    vaultMenu->updateLayout();
    if (vaultMenu->getChildrenCount() == 0) {
        auto encouragementText = CCLabelBMFont::create(
            "Nothing here yet...\nPlay the game to unlock more.", "goldFont.fnt",
            350.f, CCTextAlignment::kCCTextAlignmentCenter
        );
        encouragementText->setScale(0.75f);
        page3Layer->addChildAtPosition(encouragementText, Anchor::Center);
    }

    // ------
    // PAGE 4
    // ------
    auto page4Layer = CCLayer::create();
    page4Layer->ignoreAnchorPointForPosition(false);

    auto shopMenu = CCMenu::create();
    shopMenu->ignoreAnchorPointForPosition(false);
    shopMenu->setContentSize(m_size - ccp(40.f, 50.f));
    shopMenu->setLayout(
        AxisLayout::create()
            ->setGap(15.f)
            ->setGrowCrossAxis(true)
    );
    page4Layer->addChildAtPosition(shopMenu, Anchor::Center, ccp(0.f, -8.f));

    auto shopSpr = CCSprite::createWithSpriteFrameName("shopButton.png"_spr);
    shopSpr->setScale(1.25f);
    auto shopBtn = CCMenuItemSpriteExtra::create(
        shopSpr,
        this, menu_selector(ShortcutsPopup::onShortcutShop)
    );
    shopBtn->setTag(static_cast<int>(ShopType::Normal));
    shopBtn->setLayoutOptions(AxisLayoutOptions::create()->setBreakLine(true));
    shopMenu->addChild(shopBtn);

    // Scratch
    if (
        !isShopUnlocked(ShopType::Secret)
        && showSpoilersChoice != ShowSpoilers::Show
    ) {
        if (showSpoilersChoice == ShowSpoilers::Silhouette) {
            auto lockedShop = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
            shopMenu->addChild(lockedShop);
        }
    } else {
        auto shop = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("secretShopButton1.png"_spr),
            this, menu_selector(ShortcutsPopup::onShortcutShop)
        );
        shop->setTag(static_cast<int>(ShopType::Secret));
        shopMenu->addChild(shop);
    }
    // Community Shop
    if (
        !isShopUnlocked(ShopType::Community)
        && showSpoilersChoice != ShowSpoilers::Show
    ) {
        if (showSpoilersChoice == ShowSpoilers::Silhouette) {
            auto lockedShop = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
            shopMenu->addChild(lockedShop);
        }
    } else {
        auto shop = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("secretShopButton2.png"_spr),
            this, menu_selector(ShortcutsPopup::onShortcutShop)
        );
        shop->setTag(static_cast<int>(ShopType::Community));
        shopMenu->addChild(shop);
    }
    // Mechanic
    if (
        !isShopUnlocked(ShopType::Mechanic)
        && showSpoilersChoice != ShowSpoilers::Show
    ) {
        if (showSpoilersChoice == ShowSpoilers::Silhouette) {
            auto lockedShop = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
            shopMenu->addChild(lockedShop);
        }
    } else {
        auto shop = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("secretShopButton3.png"_spr),
            this, menu_selector(ShortcutsPopup::onShortcutShop)
        );
        shop->setTag(static_cast<int>(ShopType::Mechanic));
        shopMenu->addChild(shop);
    }
    // Diamond
    if (
        !isShopUnlocked(ShopType::Diamond)
        && showSpoilersChoice != ShowSpoilers::Show
    ) {
        if (showSpoilersChoice == ShowSpoilers::Silhouette) {
            auto lockedShop = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
            shopMenu->addChild(lockedShop);
        }
    } else {
        auto shop = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("secretShopButton4.png"_spr),
            this, menu_selector(ShortcutsPopup::onShortcutShop)
        );
        shop->setTag(static_cast<int>(ShopType::Diamond));
        shopMenu->addChild(shop);
    }

    shopMenu->updateLayout();

    m_pageArray.push_back(page1Layer);
    m_pageArray.push_back(page2Layer);
    m_pageArray.push_back(page3Layer);
    m_pageArray.push_back(page4Layer);

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

    static_cast<CopySizeLayout*>(m_mainLayer->getLayout())
        ->add(page1Layer)
        ->add(page2Layer)
        ->add(page3Layer)
        ->add(page4Layer);
    m_mainLayer->updateLayout();

    refreshPage();
    return true;
}

bool ShortcutsPopup::isShopUnlocked(ShopType type) {
    switch (type) {
        case ShopType::Normal: return true;
        case ShopType::Secret: return GameManager::get()->getUGV("11");
        case ShopType::Community: return GameManager::get()->getUGV("20");
        case ShopType::Mechanic: return GameManager::get()->getUGV("35");
        case ShopType::Diamond: return GameManager::get()->getUGV("34");
    }
}

bool ShortcutsPopup::isTreasureRoomUnlocked() {
    return GameManager::get()->getUGV("5");
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

void ShortcutsPopup::onModSettings(CCObject*) {
    geode::openSettingsPopup(Mod::get());
}

void ShortcutsPopup::refreshPage() {
    Mod::get()->setSavedValue("current-page", m_currentPage);
    if (m_currentPageLayer) m_currentPageLayer->removeFromParent();

    m_currentPageLayer = m_pageArray[m_currentPage];
    m_currentPageLayer->setID("current-page");
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

void ShortcutsPopup::onShortcutGarage(CCObject*) {
    CCDirector::get()->pushScene(
        CCTransitionMoveInT::create(0.5f, GJGarageLayer::scene())
    );
    GameManager::get()->m_ropeGarageEnter = true;
}

void ShortcutsPopup::onShortcutMainMenu(CCObject*) {
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, MenuLayer::scene(false)));
}

void ShortcutsPopup::onShortcutCreator(CCObject*) {
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, CreatorLayer::scene()));
}

void ShortcutsPopup::onShortcutExit(CCObject*) {
    createQuickPopup(
        "Quit Game",
        "Are you sure you want to <cr>quit</c>?",
        "Cancel", "Yes",
        [](auto, bool btn2) {
            if (btn2) {
                utils::game::exit();
            }
        }
    );
}

void ShortcutsPopup::onShortcutRestart(CCObject*) {
    createQuickPopup(
        "Restart Game",
        "Are you sure you want to <cy>restart</c>?",
        "Cancel", "Yes",
        [](auto, bool btn2) {
            if (btn2) {
                utils::game::restart();
            }
        }
    );
}

void ShortcutsPopup::onShortcutSettings(CCObject*) {
    auto optionsLayer = OptionsLayer::create();
    optionsLayer->setZOrder(100);
    CCDirector::get()->getRunningScene()->addChild(optionsLayer);
    optionsLayer->enterLayer();
    // It likes to show up behind so I'll just close the shortcuts popup
    this->onClose(nullptr);
}

void ShortcutsPopup::onShortcutGeode(CCObject*) {
    openModsList();
}

void ShortcutsPopup::onShortcutShop(CCObject* sender) {
    auto shopType = static_cast<ShopType>(sender->getTag());
#ifndef SHORTCUTSMENU_DEBUG
    if (!isShopUnlocked(shopType)) {
        FLAlertLayer::create("No", "Unlock it first.", "Ok")->show();
        return;
    }
#endif

    static_cast<CustomGameManager*>(GameManager::get())->m_fields->m_shortcutEntry = true;
    auto scene = GJShopLayer::scene(shopType);

    CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
}

void ShortcutsPopup::onShortcutTreasureRoom(CCObject*) {
#ifndef SHORTCUTSMENU_DEBUG
    if (!isTreasureRoomUnlocked()) {
        FLAlertLayer::create("No", "Unlock it first.", "Ok")->show();
        return;
    }
#endif

    static_cast<CustomGameManager*>(GameManager::get())->m_fields->m_shortcutEntry = true;
    auto scene = SecretRewardsLayer::scene(false);
    CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
}