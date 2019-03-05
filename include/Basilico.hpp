#ifndef __Basilico_H_
#define __Basilico_H_

#include "NeoVim.hpp"
#include "BasilPlugin.hpp"


#include "RuntimeView.hpp"

#include "WebView.hpp"

#include "PDFView.hpp"


#include <QtWidgets>
#include <QtGui>
#include <type_traits>

class BasilPlugin;

class RuntimeView;

class WebView;

class PDFView;


class Basilico: public QMainWindow
{
    // timer
    int timer;

    // Plugins
    std::unordered_map<String, Vector<BasilPlugin*>> Plugins;

    // Integration widget
    QWidget neovim_integrate;

    QGridLayout basil_layout;

    // Integration neovim tab and plugin tabs
    QStackedWidget neovim_tabplugins_integrate;
    int neovim_index; // this must be 0
    std::map<Tabpage, std::pair<String, int>> TabPluginId;

    // Tabs
    QTabBar tabline;

    // options
    Integer showtabline;
    String tablineStyleSheet;
public:
    String Name = "Basilico";

    NeoVim neovim;

    Basilico(String port, uint width, uint height, QApplication& app);
    Basilico() = delete;

    void open();

    void NeoVimSetting(String port);

    void BasilicoSetting();

    void tablineSetting();
    void changeTabNeoVim(const deque<pair<Tabpage, String>>& tabs, Tabpage current);

    template<typename T>
    void addTabPluginId(T* plugin, String name, Tabpage tab)
    {
        auto index = neovim_tabplugins_integrate.addWidget(plugin);
        TabPluginId[tab] = std::make_pair(name, index);
    }

    Tabpage makeTabForPlugin(String name);

protected:
    virtual bool eventFilter(QObject* obj, QEvent* e) override;

    virtual void timerEvent(QTimerEvent* e) override;

    virtual void mousePressEvent(QMouseEvent* e) override;

    void addPlugin(String plugin_name, BasilPlugin* plugin);
};
#endif
