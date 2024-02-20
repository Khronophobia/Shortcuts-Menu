#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ShortcutsLayer : public Popup<> {
protected:
    int m_currentPage;
    int m_maxPage;
    bool setup();
    void refreshPage();
    CCLayer* page1Layer;
    CCLayer* page2Layer;
    CCLayer* page3Layer;
    CCLabelBMFont* pageDesc;
public:
    static ShortcutsLayer* create();
    void onShortcut(CCObject*);
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