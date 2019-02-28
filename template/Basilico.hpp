#ifndef __Basilico_H_
#define __Basilico_H_

#include "NeoVim.hpp"
#include "BasilPlugin.hpp"

{% for name in message['plugin_name'] %}
#include "{{name}}.hpp"
{% endfor %}

#include <QtWidgets>
#include <QtGui>

class BasilPlugin;
{% for name in message['plugin_name'] %}
class {{name}};
{% endfor %}

class Basilico: public QMainWindow
{
    // timer
    int timer;

    // Plugins
    std::unordered_map<String, Vector<BasilPlugin*>> Plugins;

    // Integration widget
    QWidget neovim_integrate;

    QGridLayout basil_layout;

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
    void changeTabNeoVim(const deque<pair<Tabpage, String>>& tabs, Tabpage tabpage);

protected:
    virtual bool eventFilter(QObject* obj, QEvent* e) override;

    virtual void timerEvent(QTimerEvent* e) override;

    virtual void mousePressEvent(QMouseEvent* e) override;

    void addPlugin(String plugin_name, BasilPlugin* plugin);
};
#endif
