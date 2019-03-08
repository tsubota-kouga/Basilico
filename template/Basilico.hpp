#ifndef __Basilico_H_
#define __Basilico_H_

#include "NeoVim.hpp"
#include "BasilPlugin.hpp"

{% for name in message['plugin_name'] %}
#include "{{name}}.hpp"
{% endfor %}

#include <QtWidgets>
#include <QtGui>

#include <set>

class BasilPlugin;
{% for name in message['plugin_name'] %}
class {{name}};
{% endfor %}
// -Basilico---------------------------------------------------------
// |-neovim_integrate(layout:basil_layout)--------------------------|
// || -tabline(QTabBar)------------------------------------------- ||
// || |NeoVim-tab TabPlugin-tab                                  | ||
// || ------------------------------------------------------------ ||
// || -neovimsplinteg_tabplugins_integrate(QStackedWidget)-------- ||
// || |                                                          | ||
// || |  -neovim_split_plugins_integrate------  -tabplugins---   | ||
// || |  |   (layout:neovim_layout)          |  |            |-  | ||
// || |  | -neovim(NeoVim)-- -splitplugins   |  |            ||  | ||
// || |  | |               | |           |-  |  |            ||  | ||
// || |  | |               | |           ||  |  |            ||  | ||
// || |  | |               | |           ||  |  |            ||  | ||
// || |  | |               | |           ||  |  |            ||  | ||
// || |  | |               | |           ||  |  |            ||  | ||
// || |  | |               | |           ||  |  |            ||  | ||
// || |  | ----------------- -------------|  |  |            ||  | ||
// || |  |                    -------------  |  |            ||  | ||
// || |  -------------------------------------  --------------|  | ||
// || |                                          --------------  | ||
// || ------------------------------------------------------------ ||
// |----------------------------------------------------------------|
// -----------------------------------------------------------------
class Basilico: public QMainWindow
{
    // timer
    int timer;

    // Plugins
    std::unordered_map<String, Vector<BasilPlugin*>> Plugins;

    // CentralWidget : Integration of widgets
    QWidget neovim_integrate;
    // Layout for CentralWidget
    QGridLayout basil_layout;

    // Tabs
    QTabBar tabline;

    // Integration of neovim-splitplugin-integ and tab-plugin
    QStackedWidget neovimsplinteg_tabplugins_integrate;
    int neovim_index; // this must be 0
    std::map<Tabpage, std::pair<String, int>> TabPluginId; // tab, [name, stack index]
    std::multimap<Tabpage, std::tuple<QWidget*, int , int, int, int>> SplitPlugins;

    // options
    Integer showtabline;
    String tablineStyleSheet;

    QWidget neovim_split_plugins_integrate;
    QGridLayout neovim_layout;

    // currentTabpage Handler
    Tabpage currentTabpage;

    const String Name = "Basilico";
    NeoVim neovim;


public:

    Basilico(String port, uint width, uint height, QApplication& app);
    Basilico() = delete;
    Basilico operator=(QMainWindow) = delete;

    void open();

    void NeoVimSetting(String port);

    void BasilicoSetting();

    void tablineSetting();
    void changeTabNeoVim(const deque<pair<Tabpage, String>>& tabs, Tabpage current);

    void popupmenuSetting();

    const String& getName() const { return Name; }
    NeoVim& getNeoVim()& { return neovim; }

    QListWidget nvim_comp;
    QListWidget& get_nvim_comp() { return nvim_comp; };

    template<typename T>
    void addTabPluginId(T* plugin, String name, Tabpage tab)
    {
        auto* integrate = new QWidget{};
        auto* layout = new QGridLayout{integrate};

        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(plugin, 1, 0);
        integrate->setLayout(layout);

        // stackwidget index
        Integer index = neovimsplinteg_tabplugins_integrate.addWidget(integrate);
        TabPluginId[tab] = std::make_pair(name, index);
    }

    const std::multimap<Tabpage, std::tuple<QWidget*, int , int, int, int>>& getSplitPlugins() const
    {
        return SplitPlugins;
    }

    Tabpage makeTabForPlugin(String name);

protected:
    virtual bool eventFilter(QObject* obj, QEvent* e) override;

    virtual void timerEvent(QTimerEvent* e) override;

    virtual void mousePressEvent(QMouseEvent* e) override;

    void addPlugin(String plugin_name, BasilPlugin* plugin);
};
#endif
