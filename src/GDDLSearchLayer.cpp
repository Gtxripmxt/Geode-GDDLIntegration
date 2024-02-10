#include "GDDLSearchLayer.h"


#include "RatingsManager.h"
bool GDDLSearchLayer::init() {
    if (!FLAlertLayer::init(75))
        return false;

    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    // background
    const auto bg = CCScale9Sprite::create("GJ_square01.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(popupSize);
    bg->setPosition({winSize.width / 2, winSize.height / 2});
    bg->setID("gddl-demon-search-popup"_spr);
    m_mainLayer->addChild(bg, -1);
    // menu with the main layout
    m_buttonMenu = CCMenu::create();
    m_buttonMenu->setContentSize({winSize.width, winSize.height});
    m_buttonMenu->setPosition({74.5f, 295.0f});
    m_buttonMenu->setID("gddl-demon-search-menu"_spr);
    m_mainLayer->addChild(m_buttonMenu, 0);
    // close button
    const auto closeButtonSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    const auto closeButton = CCMenuItemSpriteExtra::create(closeButtonSprite, this, menu_selector(GDDLSearchLayer::onClose));
    m_buttonMenu->addChild(closeButton, 1);
    closeButton->setPosition({0.0f, 0.0f});
    // info button
    const auto infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    const auto infoButton = CCMenuItemSpriteExtra::create(infoButtonSprite, this, menu_selector(GDDLSearchLayer::onInfo));
    m_buttonMenu->addChild(infoButton);
    m_buttonMenu->reorderChild(infoButton, 1);
    infoButton->setPosition({413.0f, -7.0f});
    // title
    const auto title = CCLabelBMFont::create("GDDL Search", "goldFont.fnt");
    m_mainLayer->addChild(title, 1);
    title->setPosition({284.5f, 287.0f});
    title->setScale(0.7f);
    loadPage();
    // title
    // const auto titleContainer = CCMenu::create();
    // titleContainer->setLayout(RowLayout::create());
    // titleContainer->setContentSize({200.0f, 30.0f});
    // const auto title = CCLabelBMFont::create("GDDL Search", "goldFont.fnt");
    // title->setID("gddl-demon-search-title"_spr);
    // titleContainer->addChild(title);
    // titleContainer->reorderChild(title, 0);
    // // the (i) button
    // const auto iButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    // iButtonSprite->setScale(0.5f);
    // const auto iButton = CCMenuItemSpriteExtra::create(iButtonSprite, this, menu_selector(GDDLSearchLayer::onInfo));
    // iButton->setScale(0.4f);
    // iButton->setContentSize({10.0f, 10.0f});
    // titleContainer->addChild(iButton);
    // titleContainer->updateLayout();
    // m_buttonMenu->addChild(titleContainer);
    // m_buttonMenu->reorderChild(titleContainer, 0);
    // // tier buttons
    // for (int row = 0; row < 5; row++) {
    //     const auto rowNode = CCMenu::create();
    //     rowNode->setLayout(RowLayout::create()->setGap(5.0f));
    //     rowNode->setContentSize({popupSize.x - 10.0f, 20.0f});
    //     for (int column = 0; column < 7; column++) {
    //         const auto tierNode = createTierNode(row + 1 + column * 5);
    //         rowNode->addChild(tierNode);
    //     }
    //     rowNode->updateLayout();
    //     m_buttonMenu->addChild(rowNode);
    //     m_buttonMenu->reorderChild(rowNode, row + 1);
    // }
    // // togglers - container
    // const auto rowNode = CCMenu::create();
    // rowNode->setLayout(RowLayout::create()->setGap(5.0f));
    // rowNode->setContentSize({popupSize.x - 10.0f, 35.0f});
    // // unknown tier
    // const auto unknownButton = createTierNode(-1);
    // rowNode->addChild(unknownButton);
    // // togglers - checkboxes
    // const std::string ids[2] = {"completed", "uncompleted"};
    // const std::string labels[2] = {"Completed", "Uncompleted"};
    // for (int i = 0; i < 2; i++) {
    //     const auto togglerNode = createCheckboxNode(ids[i], labels[i]);
    //     rowNode->addChild(togglerNode);
    //     rowNode->reorderChild(togglerNode, i);
    // }
    // rowNode->updateLayout();
    // m_buttonMenu->addChild(rowNode);
    // m_buttonMenu->reorderChild(rowNode, 9);
    // // ok button
    // const auto spr = ButtonSprite::create("OK");
    // const auto okButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(GDDLSearchLayer::onClose));
    // okButton->setID("gddl-demon-split-ok"_spr);
    // m_buttonMenu->addChild(okButton);
    // m_buttonMenu->reorderChild(okButton, 10);
    // m_buttonMenu->updateLayout();
    //
    // // fix the iButton :tm: placement
    // iButton->setPosition({iButton->getPositionX(), 15.0f});
    return true;
}

void GDDLSearchLayer::loadPage() {
    createLabel(m_buttonMenu, "bigFont.fnt", "Name", 0.6f, {75.0f, -52.5f});
    createTextInputNode(m_buttonMenu, nameTextfield, "bigFont.fnt", "", {80.0f, 25.0f}, {60.0f, -77.5f});
    createCheckbox(m_buttonMenu, nameExactMatchToggler, "Ex. match", 17.5f, 0.9f, {117.0f, -77.5f}, menu_selector(GDDLSearchLayer::onToggleExactMatch));
    createLabel(m_buttonMenu, "bigFont.fnt", "Creator", 0.6f, {75.0f, -112.5f});
    createTextInputNode(m_buttonMenu, creatorTextfield, "bigFont.fnt", "", {110.0f, 25.0f}, {75.0f, -137.5f});
    createLabel(m_buttonMenu, "bigFont.fnt", "Song name", 0.6f, {75.0f, -172.5f});
    createTextInputNode(m_buttonMenu, songTextfield, "bigFont.fnt", "", {110.0f, 25.0f}, {75.0f, -197.5f});
    createLabel(m_buttonMenu, "bigFont.fnt", "Difficulty", 0.6f, {75.0f, -232.5f});
    createTextInputNode(m_buttonMenu, difficultyTextfield, "bigFont.fnt", "", {110.0f, 25.0f}, {75.0f, -257.5f});
    createLeftRightButtonsAround(difficultyTextfield, {15.0f, 22.0f}, menu_selector(GDDLSearchLayer::onInGameRatingLeft), menu_selector(GDDLSearchLayer::onInGameRatingRight));
}

void GDDLSearchLayer::onClose(CCObject *sender) {
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void GDDLSearchLayer::onInfo(CCObject *sender) {
    FLAlertLayer::create("GDDL Search", "Select the <cg>completed</c>/<cr>uncompleted</c> checkboxes and click a <co>button</c> to search for a corresponding <cy>tier</c>, click the <cp>?</c> button to search for levels that are <cb>not yet rated</c>", "OK")->show();
}

void GDDLSearchLayer::onSearchOptionSelected(CCObject *sender) {} // do nothing lmao

void GDDLSearchLayer::onTierSearch(CCObject *sender) {
    auto *senderNode = dynamic_cast<CCNode *>(sender);
    const std::string tierStr = senderNode->getID();
    const int tierNumber = std::stoi(tierStr.substr(12, tierStr.size() - 10));
    const bool searchForCompleted =
            dynamic_cast<CCMenuItemToggler *>(m_buttonMenu->getChildByIDRecursive("toggler-completed"))->isToggled();
    const bool searchForUncompleted =
            dynamic_cast<CCMenuItemToggler *>(m_buttonMenu->getChildByIDRecursive("toggler-uncompleted"))->isToggled();
    TierSearchType searchType = ANY;
    if (searchForCompleted != searchForUncompleted) {
        searchType = searchForCompleted ? COMPLETED : UNCOMPLETED;
    }
    RatingsManager::setupSearch(tierNumber, searchType);
    const auto listLayer = LevelBrowserLayer::create(RatingsManager::getSearchPage(1));
    cocos::switchToScene(listLayer);
}

void GDDLSearchLayer::createLabel(CCLayer *parent, std::string font, std::string text, float scale, CCPoint position,
                                  int zOrder) {
    const auto label = CCLabelBMFont::create(text.c_str(), font.c_str());
    parent->addChild(label, zOrder);
    label->setPosition(position);
    label->setScale(scale);
}
void GDDLSearchLayer::createTextInputNode(CCLayer *parent, CCTextInputNode *&textfield, std::string font,
                                          std::string placeholder, CCPoint bgSize, CCPoint position,
                                          int zOrder) {
    const auto bg = CCScale9Sprite::create("square02_small.png");
    parent->addChild(bg, zOrder);
    bg->setContentSize(bgSize);
    bg->setScale(0.5f);
    bg->setContentSize(bg->getContentSize() / 0.5f);
    bg->setPosition(position);
    bg->setOpacity(100);
    textfield = CCTextInputNode::create(bgSize.x, bgSize.y, placeholder.c_str(), font.c_str());
    parent->addChild(textfield, zOrder + 1);
    textfield->setPosition(position);
    textfield->setMaxLabelLength(32);
    textfield->setMaxLabelScale(0.7f);
}
void GDDLSearchLayer::createLeftRightButtonsAround(CCLayer *object, CCPoint size, SEL_MenuHandler leftCallback,
                                                   SEL_MenuHandler rightCallback, int zOrder) {
    // left
    const CCPoint positionLeft = object->getPosition() - CCPoint(object->getContentSize().width / 2 + size.x / 2, 0.0f);
    const auto leftButtonSprite = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
    const auto leftButton = CCMenuItemSpriteExtra::create(leftButtonSprite, this, leftCallback);
    object->getParent()->addChild(leftButton, zOrder);
    leftButton->setPosition(positionLeft);
    leftButton->setContentSize(size);
    // right
    const CCPoint positionRight =
            positionLeft + CCPoint(object->getContentSize().width + leftButton->getContentSize().width + 0.5f,
                                   0.0f); // why is this not symmetrical wtf
    const auto rightButtonSprite = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
    const auto rightButton = CCMenuItemSpriteExtra::create(rightButtonSprite, this, rightCallback);
    object->getParent()->addChild(rightButton, zOrder);
    rightButton->setPosition(positionRight);
    rightButton->setContentSize(size);
}

void GDDLSearchLayer::createCheckbox(CCLayer *parent, CCMenuItemToggler *&toggler, std::string label, float labelOffset, float scale,
                                     CCPoint position, SEL_MenuHandler callback, int zOrder) {
    toggler = CCMenuItemToggler::createWithStandardSprites(this, callback, scale);
    parent->addChild(toggler, zOrder);
    toggler->setPosition(position);
    const auto toggleLabel = CCLabelBMFont::create(label.c_str(), "bigFont.fnt");
    parent->addChild(toggleLabel, zOrder);
    toggleLabel->setPosition({toggler->getPositionX(), toggler->getPositionY()-labelOffset});
    toggleLabel->setScale(0.3f);
}

CCMenuItemSpriteExtra *GDDLSearchLayer::createTierNode(int tier) {
    // tier sprite
    const std::string tierString = tier != -1 ? std::to_string(tier) : "unrated";
    const std::string tierSpriteName = "tier_" + tierString + ".png";
    const auto textureName = Mod::get()->expandSpriteName(tierSpriteName.c_str());
    const auto tierSprite = CCSprite::create(textureName);
    tierSprite->setScale(0.15f);
    tierSprite->setContentSize({20.0f, 20.0f});
    const auto tierButton = CCMenuItemSpriteExtra::create(tierSprite, this, menu_selector(GDDLSearchLayer::onTierSearch));
    tierButton->setContentSize({20.0f, 20.0f});
    tierButton->setID("button-tier-" + std::to_string(tier));
    return tierButton;
}

CCMenu *GDDLSearchLayer::createCheckboxNode(const std::string &idSuffix, const std::string &name) {
    const auto menu = CCMenu::create();
    menu->setLayout(RowLayout::create()->setGap(3.0f)->setAutoScale(true));
    // checkbox
    const auto toggler = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(GDDLSearchLayer::onSearchOptionSelected), 0.8f);
    toggler->setID("toggler-" + idSuffix);
    menu->addChild(toggler);
    menu->reorderChild(toggler, 0);
    // label
    const auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setContentSize({110.0f, 35.0f});
    menu->addChild(label);
    menu->reorderChild(label, 1);
    menu->setContentSize({150.0f, 35.0f});
    menu->updateLayout();
    label->setScale(110.0f / label->getContentSize().width);
    return menu;
}

void GDDLSearchLayer::onToggleExactMatch(CCObject *sender) {}

void GDDLSearchLayer::onInGameRatingLeft(CCObject *sender) {
    FLAlertLayer::create("GDDL", "You touched the left button", "OK")->show();
}

void GDDLSearchLayer::onInGameRatingRight(CCObject *sender) {
    FLAlertLayer::create("GDDL", "You touched the left button", "OK")->show();
}

GDDLSearchLayer *GDDLSearchLayer::create() {
    const auto newLayer = new GDDLSearchLayer();
    if(newLayer != nullptr && newLayer->init()) {
        newLayer->autorelease();
        return newLayer;
    } else {
        delete newLayer;
        return nullptr;
    }
}

void GDDLSearchLayer::show() {
    FLAlertLayer::show();
    cocos::handleTouchPriority(this);
}

void GDDLSearchLayer::loadSettings() {

}

void GDDLSearchLayer::saveSettings() {

}
