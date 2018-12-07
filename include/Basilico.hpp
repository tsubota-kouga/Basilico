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

    Basilico(String port, uint width, uint height):
        basil_view{},
        basil_layout{},
        neovim{width, height, { {"rgb", true}, {"ext_linegrid", true} } },
        menubar(this),
        toolbar(this)
    {
        neovim.connect_tcp("localhost", port, 1000);
        // put func here which should be put in constructor
        // <begin>
        neovim.nvim_subscribe("plugin");

        // <end>
        neovim.nvim_ui_attach();
        neovim.setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

        basil_layout.setContentsMargins(0, 0, 0, 0);
        basil_layout.addWidget(&neovim, 0, 0);

        menubar.setStyleSheet("color: #FFFFFF;"
                              "background-color: #444444;"
                              "font-weight: 8");

        toolbar.setStyleSheet("color: #FFFFFF;"
                              "background-color: #444444;"
                              "font-weight: 8");

        addToolBar(&toolbar);
        setMenuBar(&menubar);
        createMenus();
        createActions();

        setCentralWidget(&neovim);
        setWindowTitle("Basilico");
        resize(neovim.size());

        timer = startTimer(100);
    }

    void createMenus();
    void createActions();

    void openFile();
    void saveFile();
    void saveAsFile();

protected:
    virtual void timerEvent(QTimerEvent* e) override;

    void addPlugin(String plugin_name, BasilPlugin* plugin);
};
#endif
