#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ShortcutsLayer : public Popup<> {
protected:
    int m_currentPage;
    int m_maxPage;
    bool setup();
    void refreshPage();
    CCLabelBMFont* m_pageDesc;
    CCLayerMultiplex* m_pageLayers;
    CCArray* m_pageArrays;
public:
    static ShortcutsLayer* create();
    void onShortcut(CCObject*);
    void addPage(CCLayer* layer);
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