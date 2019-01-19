
#include "Basilico.hpp"

Basilico::Basilico(String port, uint width, uint height):
    basil_view{},
    basil_layout{},
    neovim{width, height, { {"rgb", true}, {"ext_linegrid", true} } }
    // menubar(this),
    // toolbar(this)
{
    neovim.connect_tcp("localhost", port, 1000);
    neovim.nvim_set_client_info("Basilico",
                                {},
                                "ui",
                                {},
                                {{"logo", Object{""}}});
    // put func here which should be put in constructor
    // <begin>
    neovim.nvim_subscribe("plugin");
    // <end>

    neovim.nvim_ui_attach();
    neovim.setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    neovim.installEventFilter(this);

    basil_layout.setContentsMargins(0, 0, 0, 0);
    basil_layout.addWidget(&neovim, 0, 0);

    // menubar.setStyleSheet("color: #FFFFFF;"
    //         "background-color: #444444;"
    //         "font-weight: 8");
    //
    // toolbar.setStyleSheet("color: #FFFFFF;"
    //         "background-color: #444444;"
    //         "font-weight: 8");
    //
    // addToolBar(&toolbar);
    // setMenuBar(&menubar);
    // createMenus();
    // createActions();

    setCentralWidget(&neovim);
    setWindowTitle("Basilico");
    resize(neovim.size());

    timer = startTimer(100);
}

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
    connect(Run, &QAction::triggered, this, [&, this](){neovim.nvim_input("<Esc>:QuickRun\n");});

    // File Menu
    New = new QAction(tr("New"));
    fileMenu->addAction(New);

    Open = new QAction(tr("Open"), this);
    fileMenu->addAction(Open);
    connect(Open, &QAction::triggered, this, &Basilico::openFile);

    Save = new QAction(tr("Save"), this);
    fileMenu->addAction(Save);
    connect(Save, &QAction::triggered, this, [&, this](){neovim.nvim_input("<Esc>:write\n");});

    SaveAs = new QAction(tr("SaveAs"), this);
    fileMenu->addAction(SaveAs);
    connect(SaveAs, &QAction::triggered, this, &Basilico::saveAsFile);

    Exit = new QAction(tr("Exit"), this);
    fileMenu->addAction(Exit);
    connect(Exit, &QAction::triggered, this, [&, this](){neovim.nvim_input("<Esc>:quit\n");});

    // Edit Menu
    Normal = new QAction(tr("Normal"));
    editMenu->addAction(Normal);
    connect(Normal, &QAction::triggered, this, [&, this](){neovim.nvim_input("<Esc>");});

    Insert = new QAction(tr("Insert"));
    editMenu->addAction(Insert);
    connect(Insert, &QAction::triggered, this, [&, this](){neovim.nvim_input("<Esc>i");});

    Replace = new QAction(tr("Replace"));
    editMenu->addAction(Replace);
    connect(Replace, &QAction::triggered, this, [&, this](){neovim.nvim_input("<Esc><S-r>");});

    Undo = new QAction(tr("Undo"));
    editMenu->addAction(Undo);
    connect(Undo, &QAction::triggered, this, [&, this](){neovim.nvim_input("<Esc>:undo\n");});

    Redo = new QAction(tr("Redo"));
    editMenu->addAction(Redo);
    connect(Redo, &QAction::triggered, this, [&, this](){neovim.nvim_input("<Esc>:redo\n");});

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

bool Basilico::eventFilter(QObject* obj, QEvent* e)
{
    if(&neovim == obj)
    {
        switch (e->type())
        {
            case QEvent::FocusOut:
                if(static_cast<QFocusEvent*>(e)->reason() == Qt::TabFocusReason)
                {
                    neovim.setFocus();
                    neovim.nvim_input("<Tab>");
                    return true;
                }
                else
                {
                    return false;
                }
            default:
                return false;
        }
    }
    else { return false; }
}

void Basilico::timerEvent(QTimerEvent* e)
{
    if(e->timerId() == timer)
    {
        while(!neovim.plugin_queue.empty())
        {
            auto& args = neovim.plugin_queue.front();
            auto& plugin_name = boost::get<String>(args.at(0));

#include "plugins.cpp"

            neovim.plugin_queue.pop();
        }
        for(auto& [_, plugins]: Plugins)
        {
            for(auto plugin:plugins)
            {
                plugin->timerExecute(this);
            }
        }

        if(neovim.readIsKeyPressed())
        {
            for(auto& [_, plugins]: Plugins)
            {
                for(auto plugin:plugins)
                {
                    plugin->keyPressedExecute(this);
                }
            }
        }
    }
}

void Basilico::mousePressEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton)
    {

    }
    else if(e->button() == Qt::RightButton)
    {

    }
    else if(e->button() == Qt::MiddleButton)
    {

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
