#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ShortcutsLayer : public Popup<> {
protected:
    int m_currentPage = 0;
    int m_maxPage;
    bool setup();
    bool initPages();
    void refreshPage();
    CCLabelBMFont* m_pageDesc;
    CCLayerMultiplex* m_pageLayers;
    CCArray* m_pageList;
    std::deque<std::string> m_pageDescList;
public:
    static ShortcutsLayer* create();
    void onShortcut(CCObject*);
    void addPage(CCNode* node, int index, std::string pageDesc = "");
    void addPage(CCNode* node, std::string pageDesc = "");
    void onChangePage(CCObject*);
    void onScene(CCObject*);
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