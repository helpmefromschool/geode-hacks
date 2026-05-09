#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCLayer.hpp>
#include <Geode/ui/Layout.hpp>

using namespace geode::prelude;

// --- GLOBAL HACK SETTINGS ---
bool g_noclip = false;
bool g_instantRestart = false;
bool g_showLayout = false;
bool g_noDeathEffect = false;
float g_speedhack = 1.0f;

// --- HACK LOGIC (THE HOOKS) ---
class $modify(MyPlayLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        if (!g_noclip) PlayLayer::destroyPlayer(p0, p1);
    }
    
    void update(float dt) {
        PlayLayer::update(dt * g_speedhack);
    }

    void resetLevel() {
        PlayLayer::resetLevel();
    }
};

// --- THE PC-STYLE MENU UI ---
class PCStyleHackMenu : public CCLayer {
public:
    static PCStyleHackMenu* create() {
        auto ret = new PCStyleHackMenu();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() override {
        if (!CCLayer::init()) return false;

        auto winSize = CCDirector::get()->getWinSize();

        // 1. Dark Overlay
        auto bg = CCLayerColor::create({ 0, 0, 0, 180 });
        this->addChild(bg);

        // 2. Main Container (Row Layout)
        auto mainContainer = CCMenu::create();
        mainContainer->setLayout(
            RowLayout::create()
                ->setGap(15.f)
                ->setAxisAlignment(AxisAlignment::Start)
        );
        mainContainer->setContentSize({winSize.width - 60, winSize.height - 60});
        mainContainer->setPosition(winSize / 2);

        // 3. Create Columns
        mainContainer->addChild(createColumn("PLAYER", {
            {"Noclip", &g_noclip},
            {"Instant Restart", &g_instantRestart}
        }));

        mainContainer->addChild(createColumn("LEVEL", {
            {"Show Layout", &g_showLayout},
            {"No Death Effect", &g_noDeathEffect}
        }));

        mainContainer->addChild(createColumn("GLOBAL", {
            {"Speedhack x2", nullptr}
        }));

        mainContainer->updateLayout();
        this->addChild(mainContainer);

        return true;
    }

    CCNode* createColumn(std::string title, std::vector<std::pair<std::string, bool*>> features) {
        auto col = CCMenu::create();
        col->setContentSize({140, 260});
        col->setLayout(ColumnLayout::create()->setGap(8.f)->setAxisAlignment(AxisAlignment::Start));
        
        auto label = CCLabelBMFont::create(title.c_str(), "goldFont.fnt");
        label->setScale(0.6f);
        col->addChild(label);

        for (auto& feat : features) {
            auto rowMenu = CCMenu::create();
            rowMenu->setLayout(RowLayout::create()->setGap(5.f)->setAxisAlignment(AxisAlignment::Start));
            rowMenu->setContentSize({130, 25});

            auto toggle = CCMenuItemToggler::createWithStandardSprites(
                this, menu_selector(PCStyleHackMenu::onToggle), 0.5f
            );

            auto featLabel = CCLabelBMFont::create(feat.first.c_str(), "bigFont.fnt");
            featLabel->setScale(0.4f);

            rowMenu->addChild(toggle);
            rowMenu->addChild(featLabel);
            rowMenu->updateLayout();
            col->addChild(rowMenu);
        }

        col->updateLayout();
        return col;
    }

    void onToggle(CCObject* sender) {
        FLAlertLayer::create("Info", "Hack toggled!", "OK")->show();
    }

    void toggle() {
        bool isVisible = !this->isVisible();
        this->setVisible(isVisible);
        
        if (isVisible) {
            this->stopAllActions();
            this->setScale(0.85f);
            this->runAction(CCEaseBackOut::create(CCScaleTo::create(0.2f, 1.0f)));
        }
    }
};

// --- THE TAB KEY CONTROLLER ---
PCStyleHackMenu* g_menuInstance = nullptr;

class $modify(MyCCLayer, CCLayer) {
    void onEnter() {
        CCLayer::onEnter();
        if (!g_menuInstance) {
            g_menuInstance = PCStyleHackMenu::create();
            g_menuInstance->setVisible(false);
            CCScene::get()->addChild(g_menuInstance, 100000);
        }
    }

    void keyDown(enum p0) {
        if (p0 == enum::KEY_Tab) {
            if (g_menuInstance) g_menuInstance->toggle();
        }
        CCLayer::keyDown(p0);
    }
};
