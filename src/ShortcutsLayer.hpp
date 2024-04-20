#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ShortcutsLayer : public Popup<> {
protected:
    int m_currentPage;
    int m_maxPage;
    bool setup();
    bool pagesSetup();
    void refreshPage();
    CCLabelBMFont* m_pageDesc;
    CCLayerMultiplex* m_pageLayers;
    CCArray* m_pageList;
    CCMenu* m_navigationMenu;
    CCArray* m_navigateButtonList;
    std::deque<std::string> m_pageDescList;
    void setScene(CCScene* scene);
public:
    static ShortcutsLayer* create();
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
        CircleBaseColor color = CircleBaseColor::Green,
        CircleBaseSize size = CircleBaseSize::Medium
    );
};