#pragma once

class ShortcutsPopup : public geode::Popup<> {
public:
    static ShortcutsPopup* create();
protected:
    bool setup();
    void refreshPage();
    void onArrowNavigate(cocos2d::CCObject*);
    void onButtonNavigate(cocos2d::CCObject*);
protected:
    int m_currentPage{0};
    int const m_pageAmount{4};
    geode::Ref<cocos2d::CCArray> m_pageArray;
    std::vector<std::string> m_pageDescriptions;
    cocos2d::CCLayer* m_currentPageLayer = nullptr;
    cocos2d::CCMenu* m_navigateBtnMenu = nullptr;
};