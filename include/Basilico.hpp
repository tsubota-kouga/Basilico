#ifndef __Basilico_H_
#define __Basilico_H_

#include "NeoVim.hpp"
#include "BasilPlugin.hpp"



#include <QtWidgets>
#include <QtGui>

class BasilPlugin;


class Basilico: public QMainWindow
{
    QGraphicsView basil_view;
    QGridLayout basil_layout;

    // Tools
    QToolBar toolbar;

    QAction* Run;

    // Menus
    QMenuBar menubar;

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* viewMenu;
    QMenu* searchMenu;
    QMenu* helpMenu;

    // File Menu
    QAction* New;
    QAction* Open;
    QAction* Save;
    QAction* SaveAs;
    QAction* Exit;

    // Edit Menu
    QAction* Normal;
    QAction* Insert;
    QAction* Replace;
    QAction* Undo;
    QAction* Redo;

    // timer
    int timer;

    // Plugins
    std::unordered_map<String, Vector<BasilPlugin*>> Plugins;

public:
    NeoVim neovim;

    Basilico(String port, uint width, uint height);

    void createMenus();
    void createActions();

    void openFile();
    void saveFile();
    void saveAsFile();

protected:
    virtual bool eventFilter(QObject* obj, QEvent* e) override;

    virtual void timerEvent(QTimerEvent* e) override;

    virtual void mousePressEvent(QMouseEvent* e) override;

    void addPlugin(String plugin_name, BasilPlugin* plugin);
};
#endif