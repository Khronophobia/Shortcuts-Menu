#pragma once

class ShortcutsPopup : public geode::Popup<> {
public:
    static ShortcutsPopup* create();
protected:
    bool setup();
    void refreshPage();
    void onArrowNavigate(cocos2d::CCObject*);
    void onButtonNavigate(cocos2d::CCObject*);
    void onModSettings(cocos2d::CCObject*);

    void onShortcutGarage(cocos2d::CCObject*);
    void onShortcutMainMenu(cocos2d::CCObject*);
    void onShortcutCreator(cocos2d::CCObject*);
    void onShortcutExit(cocos2d::CCObject*);
    void onShortcutRestart(cocos2d::CCObject*);
    void onShortcutSettings(cocos2d::CCObject*);
    void onShortcutGeode(cocos2d::CCObject*);
    void onShortcutShop(cocos2d::CCObject*);
protected:
    int m_currentPage{0};
    int const m_pageAmount{4};
    geode::cocos::CCArrayExt<cocos2d::CCLayer*> m_pageArray{cocos2d::CCArray::createWithCapacity(m_pageAmount)};
    std::vector<std::string> m_pageDescriptions;
    cocos2d::CCLayer* m_currentPageLayer{};
    cocos2d::CCMenu* m_navigateBtnMenu{};
};