#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ShortcutsLayer.hpp"

using namespace geode::prelude;

bool ShortcutsLayer::setup() {
    // This spritesheet isn't loaded by default. Not sure if there's a better way to handle this.
    CCSpriteFrameCache::get()->addSpriteFramesWithFile("SecretSheet.plist");
    m_currentPage = 1;
    m_maxPage = 3;
    this->setTitle("Shortcuts Menu", "bigFont.fnt");
    auto screenSize = CCDirector::sharedDirector()->getWinSize();

    auto prevPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto prevPageBtn = CCMenuItemSpriteExtra::create(
        prevPageSpr, this, menu_selector(ShortcutsLayer::onChangePage)
    );
    prevPageBtn->setTag(-1);
    prevPageBtn->setPosition({
        -screenSize.width / 2
            + prevPageBtn->getContentSize().width / 2
            + 4,
        0
    });
    auto nextPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextPageSpr->setFlipX(true);
    auto nextPageBtn = CCMenuItemSpriteExtra::create(
        nextPageSpr, this, menu_selector(ShortcutsLayer::onChangePage)
    );
    nextPageBtn->setTag(1);
    nextPageBtn->setPosition({
        screenSize.width / 2
            - nextPageBtn->getContentSize().width / 2
            - 4,
        0
    });
    m_buttonMenu->addChild(prevPageBtn);
    m_buttonMenu->addChild(nextPageBtn);

    auto modSettingsBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png"),
        this,
        menu_selector(ShortcutsLayer::onModSettings)
    );
    modSettingsBtn->setPosition(m_size / 2 - CCPoint{3, 3});
    m_buttonMenu->addChild(modSettingsBtn);

    pageDesc = CCLabelBMFont::create("Page Desc", "bigFont.fnt");
    pageDesc->setScale(0.5f);
    pageDesc->setPosition(screenSize / 2 - CCPoint{
        0,
        m_bgSprite->getContentSize().height / 2
            - pageDesc->getContentSize().height / 2
    });
    m_mainLayer->addChild(pageDesc);

    // Page Layers
    page1Layer = CCLayer::create();
    page1Layer->setID("page-1"_spr);
    page2Layer = CCLayer::create();
    page2Layer->setID("page-2"_spr);
    page3Layer = CCLayer::create();
    page3Layer->setID("page-3"_spr);

    m_mainLayer->addChild(page1Layer);
    m_mainLayer->addChild(page2Layer);
    m_mainLayer->addChild(page3Layer);

    // Page 1 (Menus)
    auto placeMenu = CCMenu::create();
    placeMenu->ignoreAnchorPointForPosition(false);
    placeMenu->setContentSize(m_size - CCSize{60.f, 70.f});
    placeMenu->setPositionY(screenSize.height / 2 - 5.f);
    placeMenu->setLayout(
        AxisLayout::create()
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setGap(10.f)
    );
    page1Layer->addChild(placeMenu);

    auto mainMenuBtn = CCMenuItemSpriteExtra::create(
        CrossButtonSprite::createWithSpriteFrameName("menuArrow.png"_spr, 0.6f),
        this,
        menu_selector(ShortcutsLayer::onScene)
    );
    mainMenuBtn->setUserObject(MenuLayer::scene(false));
    auto garageBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png"),
        this,
        menu_selector(LevelInfoLayer::onGarage)
    );
    auto creatorBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_creatorBtn_001.png"),
        this,
        menu_selector(ShortcutsLayer::onScene)
    );
    creatorBtn->setUserObject(CreatorLayer::scene());
    creatorBtn->setLayoutOptions(
        AxisLayoutOptions::create()->setBreakLine(true)
    );

    auto exitBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
        this,
        menu_selector(ShortcutsLayer::onQuit)
    );
    auto restartBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"),
        this,
        menu_selector(ShortcutsLayer::onRestart)
    );
    auto geodeBtn = CCMenuItemSpriteExtra::create(
        CircleButtonSprite::createWithSpriteFrameName(
            "geode.loader/geode-logo-outline-gold.png"
        ),
        this,
        menu_selector(ShortcutsLayer::onModsList)
    );
    auto settingsBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
        this,
        menu_selector(MenuLayer::onOptions)
    );

    placeMenu->addChild(garageBtn);
    placeMenu->addChild(mainMenuBtn);
    placeMenu->addChild(creatorBtn);
    // Line break
    placeMenu->addChild(exitBtn);
    placeMenu->addChild(restartBtn);
    placeMenu->addChild(geodeBtn);
    placeMenu->addChild(settingsBtn);
    placeMenu->updateLayout();

    // Page 2 (Vault Shortcuts)
    auto vaultMenu = CCMenu::create();
    vaultMenu->ignoreAnchorPointForPosition(false);
    vaultMenu->setContentSize({
        m_size.width - 60.f,
        60.f
    });
    vaultMenu->setLayout(AxisLayout::create()->setGap(25.f));
    page2Layer->addChild(vaultMenu);

    // Vault
    if (
        (GameStatsManager::sharedState()->getStat("12") < 10) &&
        Mod::get()->getSettingValue<bool>("hide-spoilers")
    ) {
        log::info("Hide Spoilers enabled: Hiding Vault");
        auto vault1Spr = CCSprite::createWithSpriteFrameName("vaultLocked.png"_spr);
        vaultMenu->addChild(vault1Spr);
    } else {
        auto vault1Btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_secretLock_001.png"),
            this,
            menu_selector(OptionsLayer::onSecretVault)
        );
        vaultMenu->addChild(vault1Btn);
    }
    // Vault of Secrets
    if (
        (GameStatsManager::sharedState()->getStat("13") < 50) &&
        Mod::get()->getSettingValue<bool>("hide-spoilers")
    ) {
        log::info("Hide Spoilers enabled: Hiding Vault of Secrets");
        auto vault2Spr = CCSprite::createWithSpriteFrameName("vaultLocked.png"_spr);
        vaultMenu->addChild(vault2Spr);
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
        vaultMenu->addChild(vault2Btn);
    }
    // Chamber of Time
    if (
        !GameManager::sharedState()->getUGV("8") &&
        Mod::get()->getSettingValue<bool>("hide-spoilers")
    ) {
        log::info("Hide Spoilers enabled: Hiding Chamber of Time");
    } else {
        auto vault3Btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_secretLock3_001.png"),
            this,
            menu_selector(LevelPage::onSecretDoor)
        );
        vaultMenu->addChild(vault3Btn);
    }

    vaultMenu->updateLayout();

    // Page 3 (Shop Shortcuts)
    /*
        Tags for SecretRewardsLayer::onShop
        1 = Scratch
        2 = Community
        3 = Mechanic
        4 = Diamond
    */
    auto shopMenu = CCMenu::create();
    shopMenu->ignoreAnchorPointForPosition(false);
    shopMenu->setContentSize(m_size - CCSize{60.f, 70.f});
    shopMenu->setLayout(
        AxisLayout::create()
            ->setGap(15.f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    page3Layer->addChild(shopMenu);

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
    shopMenu->addChild(shopBtn);

    // Scratch
    if (
        !GameManager::sharedState()->getUGV("11") && 
        Mod::get()->getSettingValue<bool>("hide-spoilers")
    ) {
        log::info("Hide Spoilers enabled: Hiding Scratch's shop");
        auto secretShopSpr = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
        shopMenu->addChild(secretShopSpr);
    } else {
        auto secretShop1Spr = CCSprite::createWithSpriteFrameName("secretShopButton1.png"_spr);
        auto secretShop1Btn = CCMenuItemSpriteExtra::create(
            secretShop1Spr,
            this,
            menu_selector(SecretRewardsLayer::onShop)
        );
        secretShop1Btn->setTag(1);
        shopMenu->addChild(secretShop1Btn);
    }
    // Community
    if (
        !GameManager::sharedState()->getUGV("20") &&
        Mod::get()->getSettingValue<bool>("hide-spoilers")
    ) {
        log::info("Hide Spoilers enabled: Hiding Community shop");
        auto secretShop2Spr = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
        shopMenu->addChild(secretShop2Spr);
    } else {
        auto secretShop2Spr = CCSprite::createWithSpriteFrameName("secretShopButton2.png"_spr);
        auto secretShop2Btn = CCMenuItemSpriteExtra::create(
            secretShop2Spr,
            this,
            menu_selector(SecretRewardsLayer::onShop)
        );
        secretShop2Btn->setTag(2);
        shopMenu->addChild(secretShop2Btn);
    }
    // Mechanic
    if (
        !GameManager::sharedState()->getUGV("35") &&
        Mod::get()->getSettingValue<bool>("hide-spoilers")
    ) {
        log::info("Hide Spoilers enabled: Hiding Mechanic's shop");
        auto secretShop3Spr = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
        shopMenu->addChild(secretShop3Spr);
    } else {
        auto secretShop3Spr = CCSprite::createWithSpriteFrameName("secretShopButton3.png"_spr);
        auto secretShop3Btn = CCMenuItemSpriteExtra::create(
            secretShop3Spr,
            this,
            menu_selector(SecretRewardsLayer::onShop)
        );
        secretShop3Btn->setTag(3);
        shopMenu->addChild(secretShop3Btn);
    }
    // Diamond
    if (
        !GameManager::sharedState()->getUGV("34") &&
        Mod::get()->getSettingValue<bool>("hide-spoilers")
    ) {
        log::info("Hide Spoilers enabled: Hiding Diamond shop");
        auto secretShop4Spr = CCSprite::createWithSpriteFrameName("secretShopButtonLocked.png"_spr);
        shopMenu->addChild(secretShop4Spr);
    } else {
        auto secretShop4Spr = CCSprite::createWithSpriteFrameName("secretShopButton4.png"_spr);
        auto secretShop4Btn = CCMenuItemSpriteExtra::create(
            secretShop4Spr,
            this,
            menu_selector(SecretRewardsLayer::onShop)
        );
        secretShop4Btn->setTag(4);
        shopMenu->addChild(secretShop4Btn);
    }

    shopMenu->updateLayout();

    ShortcutsLayer::refreshPage();
    return true;
}

ShortcutsLayer* ShortcutsLayer::create() {
    auto ret = new ShortcutsLayer();

    if (ret && ret->init(400.f, 260.f)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
};

void ShortcutsLayer::onShortcut(CCObject* sender) {
    ShortcutsLayer::create()->show();
}

void ShortcutsLayer::onChangePage(CCObject* sender) {
    m_currentPage += sender->getTag();
    if (m_currentPage < 1) {
        m_currentPage = m_maxPage;
    } else if (m_currentPage > m_maxPage) {
        m_currentPage = 1;
    }
    ShortcutsLayer::refreshPage();
}

void ShortcutsLayer::refreshPage() {
    std::string pageTitle;
    switch(m_currentPage) {
        case 1:
            pageTitle = "Menus";
            page1Layer->setVisible(true);
            page2Layer->setVisible(false);
            page3Layer->setVisible(false);
            break;
        case 2:
            pageTitle = "Vaults";
            page1Layer->setVisible(false);
            page2Layer->setVisible(true);
            page3Layer->setVisible(false);
            break;
        case 3:
            pageTitle = "Shops";
            page1Layer->setVisible(false);
            page2Layer->setVisible(false);
            page3Layer->setVisible(true);
            break;
    }
    pageDesc->setString(fmt::format("Page {} / {}", m_currentPage, m_maxPage).c_str());
}

void ShortcutsLayer::onScene(CCObject* sender) {
    auto obj = static_cast<CCNode*>(sender)->getUserObject();
    auto scene = static_cast<CCScene*>(obj);
    if (scene == nullptr) {
        log::info("Scene not set.");
        return;
    }

    CCDirector::sharedDirector()->replaceScene(
        CCTransitionFade::create(0.5f, scene)
    );
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