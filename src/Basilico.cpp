
#include "Basilico.hpp"

void Basilico::createMenus()
{
    fileMenu = menubar.addMenu(tr("File"));
    editMenu = menubar.addMenu(tr("Edit"));
    viewMenu = menubar.addMenu(tr("View"));
    searchMenu = menubar.addMenu(tr("Search"));
    helpMenu = menubar.addMenu(tr("Help"));
}

void Basilico::createActions()
{
    // Tools
    Run = new QAction(this);
    Run->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    toolbar.addAction(Run);
    connect(Run, &QAction::triggered, this, [&](){neovim.nvim_input("<Esc>:QuickRun\n");});

    // File Menu
    New = new QAction(tr("New"));
    fileMenu->addAction(New);

    Open = new QAction(tr("Open"), this);
    fileMenu->addAction(Open);
    connect(Open, &QAction::triggered, this, &Basilico::openFile);

    Save = new QAction(tr("Save"), this);
    fileMenu->addAction(Save);
    connect(Save, &QAction::triggered, this, [&](){neovim.nvim_input("<Esc>:write\n");});

    SaveAs = new QAction(tr("SaveAs"), this);
    fileMenu->addAction(SaveAs);
    connect(SaveAs, &QAction::triggered, this, &Basilico::saveAsFile);

    Exit = new QAction(tr("Exit"), this);
    fileMenu->addAction(Exit);
    connect(Exit, &QAction::triggered, this, [&](){neovim.nvim_input("<Esc>:quit\n");});

    // Edit Menu
    Normal = new QAction(tr("Normal"));
    editMenu->addAction(Normal);
    connect(Normal, &QAction::triggered, this, [&](){neovim.nvim_input("<Esc>");});

    Insert = new QAction(tr("Insert"));
    editMenu->addAction(Insert);
    connect(Insert, &QAction::triggered, this, [&](){neovim.nvim_input("<Esc>i");});

    Replace = new QAction(tr("Replace"));
    editMenu->addAction(Replace);
    connect(Replace, &QAction::triggered, this, [&](){neovim.nvim_input("<Esc><S-r>");});

    Undo = new QAction(tr("Undo"));
    editMenu->addAction(Undo);
    connect(Undo, &QAction::triggered, this, [&](){neovim.nvim_input("<Esc>:undo\n");});

    Redo = new QAction(tr("Redo"));
    editMenu->addAction(Redo);
    connect(Redo, &QAction::triggered, this, [&](){neovim.nvim_input("<Esc>:redo\n");});

}

void Basilico::openFile()
{
    auto Path = QFileDialog::getOpenFileName(this);
    neovim.nvim_input("<Esc>:edit " + Path.toStdString() + "\n");
}

void Basilico::saveFile()
{
    neovim.nvim_input("<Esc>:write\n");
}

void Basilico::saveAsFile()
{
    auto Path = QFileDialog::getSaveFileName(this);
    neovim.nvim_input("<Esc>:write " + Path.toStdString() + "\n");
}

void Basilico::timerEvent(QTimerEvent* e)
{
    if(e->timerId() == timer)
    {
        while(!neovim.plugin_queue.empty())
        {
            auto args = neovim.plugin_queue.front();
            auto plugin_name = boost::get<String>(args.at(0));

#include "plugins.cpp"

            neovim.plugin_queue.pop();
        }
        for(auto [_, plugins]: Plugins)
        {
            for(auto plugin:plugins)
            {
                plugin->timerExecute(this);
            }
        }

        if(neovim.readIsKeyPressed())
        {
            for(auto [_, plugins]: Plugins)
            {
                for(auto plugin:plugins)
                {
                    plugin->keyPressedExecute(this);
                }
            }
        }
    }
}

void Basilico::addPlugin(String plugin_name, BasilPlugin* plugin)
{
    if(Plugins.count(plugin_name) == 0)
    {
        Plugins.insert({plugin_name, Vector<BasilPlugin*>{plugin}});
    }
    else
    {
        Plugins.at(plugin_name).push_back(plugin);
    }
}
