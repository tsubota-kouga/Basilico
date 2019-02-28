
#include "Basilico.hpp"

Basilico::Basilico(String port, uint width, uint height, QApplication& app):
    QMainWindow{},
    basil_layout{},
    neovim_integrate{},
    neovim{width, height, this, { {"rgb", true},
                                {"ext_linegrid", true},
                                {"ext_tabline", true} } },
    tabline{}
{
    NeoVimSetting(port);

    tablineSetting();

    BasilicoSetting();
}

void Basilico::open()
{
    QMainWindow::show();
}

void Basilico::NeoVimSetting(String port)
{
    neovim.connect_tcp("localhost", port, 1000);
    neovim.nvim_set_client_info(Name,
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

    neovim.nvim_command("runtime! ginit.vim");
}

void Basilico::BasilicoSetting()
{
    basil_layout.setContentsMargins(0, 0, 0, 0);
    basil_layout.addWidget(&neovim, 1, 0);
    neovim_integrate.setLayout(&basil_layout);

    setCentralWidget(&neovim_integrate);
    setWindowTitle("Basilico");
    resize(neovim.size());

    timer = startTimer(100);
}

void Basilico::tablineSetting()
{
    try{
        showtabline = boost::get<uInteger>(neovim.nvim_get_option("showtabline"));
    }
    catch(boost::bad_get& e){
        showtabline = 0;
    }
    try{
        tablineStyleSheet = boost::get<String>(neovim.nvim_get_var("basilico#tabline_style_sheet"));
    }
    catch(boost::bad_get& e){
        tablineStyleSheet = "";
    }
    tabline.setMovable(true);
    tabline.setAutoHide((showtabline == 2) ? false: true);
    tabline.setExpanding(false);
    tabline.setStyleSheet(QString::fromStdString(tablineStyleSheet));
    connect(&tabline, &QTabBar::tabBarClicked, this,
            [&, this](int idx){
                neovim.tabline_change(idx);
            });
    connect(&tabline, &QTabBar::tabMoved, this,
            [&, this](int from, int to){
                int idx = tabline.currentIndex();
                int movenum = to - from;
                if((from == idx) xor (movenum >= 0))
                {
                    neovim.nvim_command("tabm +" + std::to_string(-movenum));
                }
                else
                {
                    neovim.nvim_command("tabm " + std::to_string(-movenum));
                }
            });
}

void Basilico::changeTabNeoVim(const deque<pair<Tabpage, String>>& tabs, Tabpage tabpage)
{
    if(showtabline == 0){ return; }
    if(basil_layout.indexOf(&tabline) == -1)
    {
        basil_layout.addWidget(&tabline, 0, 0);
    }
    int num_tabs = tabline.count();
    for(int i = 0;i <= num_tabs;i++)
    {
        tabline.removeTab(0);
    }
    for(auto&& [tab, name]: tabs)
    {
        int idx = tabline.addTab(QString::fromStdString(name));
        if(tab == tabpage){ tabline.setCurrentIndex(idx); }
    }
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
                break;
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
        while(!neovim.plugin_deque.empty())
        {
            auto& args = neovim.plugin_deque.front();
            auto& plugin_name = boost::get<String>(args.at(0));

#include "plugins.cpp"

            neovim.plugin_deque.pop_front();
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

