#pragma once
#include <Geode/Geode.hpp>

class ShortcutsPopup : public geode::Popup<> {
protected:
    int m_currentPage;
    int m_maxPage;
    bool setup();
    bool pagesSetup();
    void refreshPage();
    cocos2d::CCLabelBMFont* m_pageDesc;
    cocos2d::CCLayerMultiplex* m_pageLayers;
    cocos2d::CCArray* m_pageList;
    cocos2d::CCMenu* m_navigationMenu;
    cocos2d::CCArray* m_navigateButtonList;
    std::deque<std::string> m_pageDescList;
    void setScene(cocos2d::CCScene* scene);
public:
    static ShortcutsPopup* create();
    void onShortcut(CCObject*);
    void addPage(CCNode* node, std::string pageDesc = "");
    void onChangePage(CCObject*);
    void onNavigate(CCObject*);
    void onMainMenu(CCObject*);
    void onCreatorLayer(CCObject*);
    void onRestart(CCObject*);
    void onQuit(CCObject*);
    void onModSettings(CCObject*);
    void onModsList(CCObject*);
    static CCMenuItemSpriteExtra* addShortcutButton(
        CCObject* target,
        float scale = 1.f,
        geode::CircleBaseColor color = geode::CircleBaseColor::Green,
        geode::CircleBaseSize size = geode::CircleBaseSize::Medium
    );

    ~ShortcutsPopup();
};