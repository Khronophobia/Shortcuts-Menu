#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ShortcutsLayer.hpp"
#include "Settings/ChoiceSetting.hpp"

using namespace geode::prelude;

bool ShortcutsLayer::setup() {
    m_noElasticity = GameManager::sharedState()->getGameVariable("0168"); // Fast Menu option

    // This spritesheet isn't loaded by default. Not sure if there's a better way to handle this.
    // Also, idk if I should deload this manually or if it'll deload automatically.
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SecretSheet.plist");
    this->setTitle("Shortcuts Menu", "bigFont.fnt");
    m_currentPage = 0;

    this->setID("ShortcutsLayer");
    m_closeBtn->setID("close-button"_spr);
    m_title->setID("title"_spr);
    m_buttonMenu->setID("popup-menu"_spr);

    auto pageBtnMenu = CCMenu::create();
    pageBtnMenu->ignoreAnchorPointForPosition(false);
    pageBtnMenu->setID("screenwide-menu"_spr);
    m_mainLayer->addChildAtPosition(pageBtnMenu, Anchor::Center);

    auto prevPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto prevPageBtn = CCMenuItemSpriteExtra::create(
        prevPageSpr, this, menu_selector(ShortcutsLayer::onChangePage)
    );
    prevPageBtn->setTag(-1);
    auto nextPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextPageSpr->setFlipX(true);
    auto nextPageBtn = CCMenuItemSpriteExtra::create(
        nextPageSpr, this, menu_selector(ShortcutsLayer::onChangePage)
    );
    nextPageBtn->setTag(1);

    pageBtnMenu->addChildAtPosition(
        prevPageBtn, Anchor::Left, {prevPageBtn->getContentWidth() / 2 + 4, 0}
    );
    pageBtnMenu->addChildAtPosition(
        nextPageBtn, Anchor::Right, {-nextPageBtn->getContentWidth() / 2 - 4, 0}
    );

    auto modSettingsBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png"),
        this,
        menu_selector(ShortcutsLayer::onModSettings)
    );
    modSettingsBtn->setID("mod-settings-button"_spr);
    m_buttonMenu->addChildAtPosition(modSettingsBtn, Anchor::TopRight, {-3.f, -3.f});

    m_pageDesc = CCLabelBMFont::create("Page Desc", "bigFont.fnt");
    m_pageDesc->setScale(0.5f);
    m_mainLayer->addChildAtPosition(
        m_pageDesc, Anchor::Bottom, {0, m_pageDesc->getContentHeight() / 2}
    );

    m_pageList = CCArray::create();
    if (!this->pagesSetup()) return false;
    m_pageLayers = CCLayerMultiplex::createWithArray(m_pageList);
    m_pageLayers->ignoreAnchorPointForPosition(false);
    m_pageLayers->setLayout(AnchorLayout::create());
    m_pageLayers->setID("current-page"_spr);
    m_mainLayer->addChildAtPosition(m_pageLayers, Anchor::Center);
    m_maxPage = m_pageList->count();

    m_navigationMenu = CCMenu::create();
    m_navigationMenu->ignoreAnchorPointForPosition(false);
    m_navigationMenu->setLayout(RowLayout::create());
    m_navigationMenu->setID("navigation-menu"_spr);
    m_mainLayer->addChildAtPosition(m_navigationMenu, Anchor::Bottom, {0.f, -12.f});

    for (int page = 0; page < m_maxPage; page++) {
        auto navBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("gj_navDotBtn_off_001.png"),
            this,
            menu_selector(ShortcutsLayer::onNavigate)
        );
        navBtn->setTag(page);
        m_navigationMenu->addChild(navBtn);
    }
    m_navigationMenu->updateLayout();
    m_navigateButtonList = m_navigationMenu->getChildren();

    this->refreshPage();
    return true;
}

bool ShortcutsLayer::pagesSetup() {
    auto showSpoilersValue = Mod::get()->getSettingValue<ChoiceSettingStruct>("show-spoilers").m_choice;

    // Utilities Page
    auto utilsMenu = CCMenu::create();
    utilsMenu->ignoreAnchorPointForPosition(false);
    utilsMenu->setContentSize(m_size - CCSize{40.f, 60.f});
    utilsMenu->setLayoutOptions(
        AnchorLayoutOptions::create()
            ->setAnchor(Anchor::Center)
            ->setOffset({0.f, -4.f})
    );
    utilsMenu->setLayout(
        AxisLayout::create()
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setGap(10.f)
    );
    utilsMenu->setID("utils-menu"_spr);

    auto mainMenuBtn = CCMenuItemSpriteExtra::create(
        CrossButtonSprite::createWithSpriteFrameName("menuBtn.png"_spr),
        this,
        menu_selector(ShortcutsLayer::onScene)
    );
    mainMenuBtn->setUserObject(MenuLayer::scene(false));
    mainMenuBtn->setID("main-menu-button"_spr);
    auto garageBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png"),
        this,
        menu_selector(LevelInfoLayer::onGarage)
    );
    garageBtn->setID("icon-kit-button"_spr);
    auto creatorBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_creatorBtn_001.png"),
        this,
        menu_selector(ShortcutsLayer::onScene)
    );
    creatorBtn->setUserObject(CreatorLayer::scene());
    creatorBtn->setLayoutOptions(
        AxisLayoutOptions::create()->setBreakLine(true)
    );
    creatorBtn->setID("editor-button"_spr);

    auto exitBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
        this,
        menu_selector(ShortcutsLayer::onQuit)
    );
    exitBtn->setID("close-button"_spr);
    auto restartBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"),
        this,
        menu_selector(ShortcutsLayer::onRestart)
    );
    restartBtn->setID("restart-button"_spr);
    auto geodeBtn = CCMenuItemSpriteExtra::create(
        CircleButtonSprite::createWithSpriteFrameName(
            "geode.loader/geode-logo-outline-gold.png"
        ),
        this,
        menu_selector(ShortcutsLayer::onModsList)
    );
    geodeBtn->setID("geode-button"_spr);
    auto settingsBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
        this,
        menu_selector(MenuLayer::onOptions)
    );
    settingsBtn->setID("settings-button"_spr);

    utilsMenu->addChild(garageBtn);
    utilsMenu->addChild(mainMenuBtn);
    utilsMenu->addChild(creatorBtn);
    // Line break
    utilsMenu->addChild(exitBtn);
    utilsMenu->addChild(restartBtn);
    utilsMenu->addChild(geodeBtn);
    utilsMenu->addChild(settingsBtn);
    utilsMenu->updateLayout();

    // Online Levels Page
    auto onlineMenu = CCMenu::create();
    onlineMenu->setContentSize(m_size - CCSize{40.f, 60.f});
    onlineMenu->ignoreAnchorPointForPosition(false);
    onlineMenu->setLayoutOptions(
        AnchorLayoutOptions::create()
            ->setAnchor(Anchor::Center)
            ->setOffset({0.f, -4.f})
    );
    onlineMenu->setLayout(
        AxisLayout::create()
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    onlineMenu->setID("online-menu"_spr);

    auto createdLvlBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_createBtn_001.png"),
        this,
        menu_selector(CreatorLayer::onMyLevels)
    );
    auto savedLvlBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_savedBtn_001.png"),
        this,
        menu_selector(CreatorLayer::onSavedLevels)
    );
    auto questBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_challengeBtn_001.png"),
        this,
        menu_selector(CreatorLayer::onChallenge)
    );
    auto dailyBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_dailyBtn_001.png"),
        this,
        menu_selector(CreatorLayer::onDailyLevel)
    );
    auto weeklyBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_weeklyBtn_001.png"),
        this,
        menu_selector(CreatorLayer::onWeeklyLevel)
    );
    auto featuredLvlBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_featuredBtn_001.png"),
        this,
        menu_selector(CreatorLayer::onFeaturedLevels)
    );
    auto searchLvlBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_searchBtn_001.png"),
        this,
        menu_selector(CreatorLayer::onOnlineLevels)
    );
    searchLvlBtn->setLayoutOptions(
        AxisLayoutOptions::create()->setBreakLine(true)  
    );

    CCSprite* treasureRoomSpr;
    if (GameManager::sharedState()->getUGV("5")) {
        treasureRoomSpr = CCSprite::createWithSpriteFrameName("secretDoorBtn_open_001.png");
    } else {
        treasureRoomSpr = CCSprite::createWithSpriteFrameName("secretDoorBtn_closed_001.png");
    }
    auto treasureRoomBtn = CCMenuItemSpriteExtra::create(
        treasureRoomSpr,
        this,
        menu_selector(CreatorLayer::onTreasureRoom)
    );
    treasureRoomBtn->setLayoutOptions(
        AxisLayoutOptions::create()->setMinScale(1.f)
    );

    onlineMenu->addChild(createdLvlBtn);
    onlineMenu->addChild(savedLvlBtn);
    onlineMenu->addChild(questBtn);
    onlineMenu->addChild(dailyBtn);
    onlineMenu->addChild(weeklyBtn);
    onlineMenu->addChild(featuredLvlBtn);
    onlineMenu->addChild(searchLvlBtn);
    // Line break
    onlineMenu->addChild(treasureRoomBtn);
    onlineMenu->updateLayout();

    // Vaults Page
    auto vaultMenu = CCMenu::create();
    vaultMenu->ignoreAnchorPointForPosition(false);
    vaultMenu->setContentSize({
        m_size.width - 60.f,
        60.f
    });
    vaultMenu->setLayoutOptions(
        AnchorLayoutOptions::create()
            ->setAnchor(Anchor::Center)
            ->setOffset({0.f, -4.f})
    );
    vaultMenu->setLayout(AxisLayout::create()->setGap(25.f));
    vaultMenu->setID("vaults-menu"_spr);

    // Vault
    if (
        // Check if collected user coins is less than 10
        // 10 user coins are needed to unlock the Vault
        (GameStatsManager::sharedState()->getStat("12") < 10)
        &&  showSpoilersValue != 0
    ) {
        log::info("Hide Spoilers enabled: Hiding Vault");
        if (showSpoilersValue == 1) {
            auto vault1Spr = CCSprite::createWithSpriteFrameName("vaultLocked.png"_spr);
            vaultMenu->addChild(vault1Spr);
        }
    } else {
        auto vault1Btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_secretLock_001.png"),
            this,
            menu_selector(OptionsLayer::onSecretVault)
        );
        vault1Btn->setID("vault1-button"_spr);
        vaultMenu->addChild(vault1Btn);
    }
    // Vault of Secrets
    if (
        // Check if total diamonds is less than 50
        // 50 diamonds are needed to unlock the Vault of Secrets
        (GameStatsManager::sharedState()->getStat("13") < 50)
        &&  showSpoilersValue != 0
    ) {
        log::info("Hide Spoilers enabled: Hiding Vault of Secrets");
        if (showSpoilersValue == 1) {
            auto vault2Spr = CCSprite::createWithSpriteFrameName("vaultLocked.png"_spr);
            vaultMenu->addChild(vault2Spr);
        }
    } else {
        auto vault2Spr = CCSprite::createWithSpriteFrameName("GJ_secretLock2_001.png");
        auto vault2Spr2 = CCSprite::createWithSpriteFrameName("GJ_secretLock2_2_001.png");
        vault2Spr2->setZOrder(-1);
        vault2Spr->addChild(vault2Spr2);
        auto vault2Btn = CCMenuItemSpriteExtra::create(
            vault2Spr,
            this,
            menu_selector(CreatorLayer::onSecretVault)
        );
        vault2Spr2->setPosition(vault2Spr->getPosition());
        vault2Btn->setID("vault2-button"_spr);
        vaultMenu->addChild(vault2Btn);
    }
    // Chamber of Time
    if (
        !GameManager::sharedState()->getUGV("8")
        &&  showSpoilersValue != 0
    ) {
        log::info("Hide Spoilers enabled: Hiding Chamber of Time");
        if (showSpoilersValue == 1) {
            auto vault3Spr = CCSprite::createWithSpriteFrameName("vaultLocked.png"_spr);
            vaultMenu->addChild(vault3Spr);
        }
    } else {
        auto vault3Btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_secretLock3_001.png"),
            this,
            menu_selector(LevelPage::onSecretDoor)
        );
        vault3Btn->setID("vault3-button"_spr);
        vaultMenu->addChild(vault3Btn);
    }

    if (vaultMenu->getChildrenCount() == 0) {
        auto encouragementText = CCLabelBMFont::create(
            "Nothing here yet...\nPlay the game to unlock more.",
            "goldFont.fnt",
            1000.f,
            CCTextAlignment::kCCTextAlignmentCenter
        );
        encouragementText->setID("encouragement-text"_spr);
        vaultMenu->addChild(encouragementText);
    }

    vaultMenu->updateLayout();

    // Shops Page
    /*
        Tags for SecretRewardsLayer::onShop
        1 = Scratch
        2 = Community
        3 = Mechanic
        4 = Diamond
    */
    auto shopMenu = CCMenu::create();
    shopMenu->ignoreAnchorPointForPosition(false);
    shopMenu->setContentSize(m_size - CCSize{40.f, 60.f});
    shopMenu->setLayoutOptions(
        AnchorLayoutOptions::create()
            ->setAnchor(Anchor::Center)
            ->setOffset({0.f, -4.f})
    );
    shopMenu->setLayout(
        AxisLayout::create()
            ->setGap(15.f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    shopMenu->setID("shops-menu"_spr);

    auto shopSpr = CCSprite::createWithSpriteFrameName("shopButton.png"_spr);
    shopSpr->setScale(1.25f);
    auto shopBtn = CCMenuItemSpriteExtra::create(
        shopSpr,
        this,
        menu_selector(GJGarageLayer::onShop)
    );
    shopBtn->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setBreakLine(true)
    );
    shopBtn->setID("shop-button"_spr);
    shopMenu->addChild(shopBtn);

    // Scratch
    if (
        !GameManager::sharedState()->getUGV("11") 
        &&  showSpoilersValue != 0
    ) {
        log::info("Hide Spoilers enabled: Hiding Scratch's shop");
        if (showSpoilersValue == 1) {
            auto secretShopSpr = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
            shopMenu->addChild(secretShopSpr);
        }
    } else {
        auto secretShop1Spr = CCSprite::createWithSpriteFrameName("secretShopButton1.png"_spr);
        auto secretShop1Btn = CCMenuItemSpriteExtra::create(
            secretShop1Spr,
            this,
            menu_selector(SecretRewardsLayer::onShop)
        );
        secretShop1Btn->setTag(1);
        secretShop1Btn->setID("secret-shop1-button"_spr);
        shopMenu->addChild(secretShop1Btn);
    }
    // Community
    if (
        !GameManager::sharedState()->getUGV("20")
        &&  showSpoilersValue != 0
    ) {
        log::info("Hide Spoilers enabled: Hiding Community shop");
        if (showSpoilersValue == 1) {
            auto secretShop2Spr = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
            shopMenu->addChild(secretShop2Spr);
        }
    } else {
        auto secretShop2Spr = CCSprite::createWithSpriteFrameName("secretShopButton2.png"_spr);
        auto secretShop2Btn = CCMenuItemSpriteExtra::create(
            secretShop2Spr,
            this,
            menu_selector(SecretRewardsLayer::onShop)
        );
        secretShop2Btn->setTag(2);
        secretShop2Btn->setID("secret-shop2-button"_spr);
        shopMenu->addChild(secretShop2Btn);
    }
    // Mechanic
    if (
        !GameManager::sharedState()->getUGV("35")
        &&  showSpoilersValue != 0
    ) {
        log::info("Hide Spoilers enabled: Hiding Mechanic's shop");
        if (showSpoilersValue == 1) {
            auto secretShop3Spr = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
            shopMenu->addChild(secretShop3Spr);
        }
    } else {
        auto secretShop3Spr = CCSprite::createWithSpriteFrameName("secretShopButton3.png"_spr);
        auto secretShop3Btn = CCMenuItemSpriteExtra::create(
            secretShop3Spr,
            this,
            menu_selector(SecretRewardsLayer::onShop)
        );
        secretShop3Btn->setTag(3);
        secretShop3Btn->setID("secret-shop3-button"_spr);
        shopMenu->addChild(secretShop3Btn);
    }
    // Diamond
    if (
        !GameManager::sharedState()->getUGV("34")
        &&  showSpoilersValue != 0
    ) {
        log::info("Hide Spoilers enabled: Hiding Diamond shop");
        if (showSpoilersValue == 1) {
            auto secretShop4Spr = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
            shopMenu->addChild(secretShop4Spr);
        }
    } else {
        auto secretShop4Spr = CCSprite::createWithSpriteFrameName("secretShopButton4.png"_spr);
        auto secretShop4Btn = CCMenuItemSpriteExtra::create(
            secretShop4Spr,
            this,
            menu_selector(SecretRewardsLayer::onShop)
        );
        secretShop4Btn->setTag(4);
        secretShop4Btn->setID("secret-shop4-button"_spr);
        shopMenu->addChild(secretShop4Btn);
    }

    shopMenu->updateLayout();

    this->addPage(utilsMenu, "Utilities");
    this->addPage(onlineMenu, "Online Page");
    this->addPage(vaultMenu, "Vaults");
    this->addPage(shopMenu, "Shops");

    return true;
}

ShortcutsLayer* ShortcutsLayer::create() {
    auto ret = new ShortcutsLayer();

    if (ret && ret->initAnchored(400.f, 260.f)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
};

void ShortcutsLayer::onShortcut(CCObject* sender) {
    ShortcutsLayer::create()->show();
}

void ShortcutsLayer::addPage(CCNode* node, std::string pageDesc) {
    m_pageList->addObject(node);
    m_pageDescList.push_back(pageDesc);
}

void ShortcutsLayer::onChangePage(CCObject* sender) {
    m_currentPage += sender->getTag();
    if (m_currentPage < 0) {
        m_currentPage = m_maxPage - 1;
    } else if (m_currentPage > m_maxPage - 1) {
        m_currentPage = 0;
    }
    ShortcutsLayer::refreshPage();
}

void ShortcutsLayer::onNavigate(CCObject* sender) {
    m_currentPage = sender->getTag();
    ShortcutsLayer::refreshPage();
}

void ShortcutsLayer::refreshPage() {
    m_pageLayers->switchTo(m_currentPage);
    m_pageDesc->setString(m_pageDescList.at(m_currentPage).c_str());
    m_pageLayers->updateLayout();

    for (auto btn : CCArrayExt<CCMenuItemSpriteExtra*>(m_navigateButtonList)) {
        if (btn->getTag() == m_currentPage)
            btn->setNormalImage(CCSprite::createWithSpriteFrameName("gj_navDotBtn_on_001.png"));
        else
            btn->setNormalImage(CCSprite::createWithSpriteFrameName("gj_navDotBtn_off_001.png"));
    }
}

void ShortcutsLayer::onScene(CCObject* sender) {
    auto obj = static_cast<CCNode*>(sender)->getUserObject();
    if (auto scene = typeinfo_cast<CCScene*>(obj)) {
        CCDirector::sharedDirector()->replaceScene(
            CCTransitionFade::create(0.5f, scene)
        );
        return;
    }
    log::error("Not a valid scene");
}

void ShortcutsLayer::onRestart(CCObject* sender) {
    geode::createQuickPopup(
        "Restart Game",
        "Are you sure you want to <cr>restart</c>?",
        "Cancel", "Yes",
        [](auto, bool confirm) {
            if (confirm) {
                utils::game::restart();
            }
        }
    );
}

void ShortcutsLayer::onQuit(CCObject* sender) {
    geode::createQuickPopup(
        "Quit Game",
        "Are you sure you want to <cr>quit?</c>",
        "Cancel", "Yes",
        [](auto, bool confirm) {
            if (confirm) {
                utils::game::exit();
            }
        }
    );
}

void ShortcutsLayer::onModsList(CCObject* sender) {
    geode::openModsList();
}

void ShortcutsLayer::onModSettings(CCObject* sender) {
    geode::openSettingsPopup(Mod::get());
}

CCMenuItemSpriteExtra* ShortcutsLayer::addShortcutButton(
	CCObject* target,
	float scale,
	CircleBaseColor color,
	CircleBaseSize size) {
	auto shortcutButton = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSpriteFrameName("shortcutIcon.png"_spr, scale, color, size),
			target,
			menu_selector(ShortcutsLayer::onShortcut)
		);
	shortcutButton->setID("shortcut-button"_spr);
	return shortcutButton;
}