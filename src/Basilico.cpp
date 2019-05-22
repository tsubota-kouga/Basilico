
#include "Basilico.hpp"

Basilico::Basilico(String port, uint width, uint height, QApplication& app):
    QMainWindow{},
    neovim_integrate{},
    basil_layout{},
    tabline{},
    neovimsplinteg_tabplugins_integrate{},
    neovim_split_plugins_integrate{},
    neovim{width, height, this, { {"rgb", true},
                                {"ext_linegrid", true} },
           "localhost", port, 1000 },
    nvim_comp{&neovim},
    Plugins{}
{
    NeoVimSetting(port);

    if(neovim.get_ui_option("ext_tabline"))
    { tablineSetting(); }

    if(neovim.get_ui_option("ext_popupmenu"))
    { popupmenuSetting(); }
    else { nvim_comp.hide(); }

    BasilicoSetting();

    currentTabpage = neovim.nvim_get_current_tabpage();
}

void Basilico::open()
{
    QMainWindow::show();
}

void Basilico::NeoVimSetting(String port)
{
    neovim.nvim_set_client_info(Name,
                                {},
                                "ui",
                                {},
                                {{"logo", Object{""}}});
    // put func here which should be put in constructor
    // <begin>
    neovim.nvim_subscribe("NeoVim#plugin");
    neovim.nvim_subscribe("NeoVim#autocmd");

    try{
        auto is_ext_tabline = boost::get<bool>(neovim.nvim_get_var("basilico#ext_tabline"));
        neovim.set_ui_option("ext_tabline", is_ext_tabline);
    }
    catch(boost::bad_get& e){
        neovim.set_ui_option("ext_tabline", false);
    }
    try{
        auto is_ext_popupmenu = boost::get<bool>(neovim.nvim_get_var("basilico#ext_popupmenu"));
        neovim.set_ui_option("ext_popupmenu", is_ext_popupmenu);
    }
    catch(boost::bad_get& e){
        neovim.set_ui_option("ext_popupmenu", false);
    }
    try{
        auto is_ext_cmdline = boost::get<bool>(neovim.nvim_get_var("basilico#ext_cmdline"));
        neovim.set_ui_option("ext_cmdline", is_ext_cmdline);
    }
    catch(boost::bad_get& e){
        neovim.set_ui_option("ext_cmdline", false);
    }
    try{
        auto is_ext_wildmenu = boost::get<bool>(neovim.nvim_get_var("basilico#ext_wildmenu"));
        neovim.set_ui_option("ext_wildmenu", is_ext_wildmenu);
    }
    catch(boost::bad_get& e){
        neovim.set_ui_option("ext_wildmenu", false);
    }
#if NVIM_API_LEVEL >= 7
    try{
        auto is_ext_messages = boost::get<bool>(neovim.nvim_get_var("basilico#ext_messages"));
        neovim.set_ui_option("ext_messages", is_ext_messages);
    }
    catch(boost::bad_get& e){
        neovim.set_ui_option("ext_messages", false);
    }
#endif
    // <end>

    neovim.nvim_ui_attach();
    neovim.setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    neovim.installEventFilter(this);

    neovim.nvim_command("runtime! ginit.vim");
}

void Basilico::BasilicoSetting()
{
    neovim_index = neovimsplinteg_tabplugins_integrate.addWidget(
            &neovim_split_plugins_integrate);
    neovim_split_plugins_integrate.addWidget(&neovim);

    basil_layout.setContentsMargins(0, 0, 0, 0);
    basil_layout.setSpacing(0);
    basil_layout.addWidget(&neovimsplinteg_tabplugins_integrate, 1, 0);
    neovim_integrate.setLayout(&basil_layout);

    setContextMenuPolicy(Qt::NoContextMenu);
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
    tabline.setTabsClosable(true);
    tabline.setStyleSheet(QString::fromStdString(tablineStyleSheet));
    connect(&tabline, &QTabBar::tabCloseRequested, this,
            [&](int tabline_index){
            Integer tabpageIndex = -1;
            for(auto&& [t, plugin]: TabPluginId) {
                if(neovim.get_nvim_tabline().at(tabline_index).first == t) {
                    tabpageIndex = neovim.nvim_tabpage_get_number(t);
                    neovim.nvim_command("tabclose! " + std::to_string(tabpageIndex));
                    for(auto&& [tab, info] : TabPluginId) {
                        // on stackwidget [0][1][2][x][4][5]
                        //                [0][1][2]   [3][4]
                        if(tabline_index < info.second) {
                            --info.second;
                        }
                    }
                    TabPluginId.erase(t);
                    break;
                }
            }
            });

    // tab change
    connect(&tabline, &QTabBar::tabBarClicked, this,
            [&, this](int idx){ // neovim tabline index
                neovim.tabline_change(idx);
            });

    // tab move
    connect(&tabline, &QTabBar::tabMoved, this,
            [&, this](int from, int to){
                int idx = tabline.currentIndex();
                int movenum = to - from;
                if((from == idx) xor (movenum >= 0)) {
                    neovim.nvim_command("tabmove +" + std::to_string(-movenum));
                }
                else {
                    neovim.nvim_command("tabmove " + std::to_string(-movenum));
                }
            });
}

void Basilico::changeTabNeoVim(const deque<pair<Tabpage, String>>& tabs, Tabpage current)
{
    if(showtabline == 0){ return; }
    // if there is not tabline, make tabline
    if(basil_layout.indexOf(&tabline) == -1) {
        basil_layout.addWidget(&tabline, 0, 0);
    }

    // remove all tabs
    int num_tabs = tabline.count();
    for(int i = 0;i <= num_tabs;i++) {
        tabline.removeTab(0);
    }

    // make all tabs
    for(auto&& [tab, name]: tabs) {
        int idx = tabline.addTab(QString::fromStdString(name));
        if(tab == current){
            tabline.setCurrentIndex(idx);
        }
    }
    // if current is tab-plugin tab
    if(TabPluginId.count(current) == 1) {
        auto&& [name, index] = TabPluginId[current];
        neovimsplinteg_tabplugins_integrate.setCurrentIndex(index);
    }
    else { // else Neovim must be shown
        neovimsplinteg_tabplugins_integrate.setCurrentIndex(neovim_index);
    }

    // SplitPlugins
    // previous tabpage
    if(current == currentTabpage) {
        return;
    }
    // next pabpage
    {
        auto&& p = SplitPlugins.equal_range(current);
        for(auto&& it = p.first;it != p.second;++it) {
            it->second->show();
        }
    }
    {
        auto&& p = SplitPlugins.equal_range(currentTabpage);
        for(auto&& it = p.first;it != p.second;++it) {
            it->second->hide();
        }
    }

    currentTabpage = current;
}

void Basilico::popupmenuSetting()
{
    nvim_comp.hide();
}


Tabpage Basilico::makeTabForPlugin(String name)
{
    neovim.nvim_call_function("basilico_tabplugin#make_tab_for_plugin", {name});
    return neovim.nvim_get_current_tabpage();
}

bool Basilico::eventFilter(QObject* obj, QEvent* e)
{
    if(&neovim == obj) {
        switch (e->type()) {
            case QEvent::FocusOut:
                if(static_cast<QFocusEvent*>(e)->reason() == Qt::TabFocusReason) {
                    neovim.setFocus();
                    neovim.nvim_input("<Tab>");
                    return true;
                }
                else {
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
    // Plugin update
    if(e->timerId() == timer)
    {
        while(!neovim.plugin_req_deque.empty())
        {
            auto& args = neovim.plugin_req_deque.front();
            auto& plugin_name = boost::get<String>(args.at(0));
            if(plugin_name == "Basilico")
            {
                if(args.size() >= 1 and boost::get<String>(args.at(1)) == "set_input_flag") {
                    neovim.set_input_control_flag(true);
                }
                else if(args.size() >= 1 and
                        boost::get<String>(args.at(1)) == "reset_input_flag") {
                    neovim.set_input_control_flag(false);
                }
            }
            else {
#include "plugins.cpp"
            }
            neovim.plugin_req_deque.pop_front();
        }
        for(auto& [_, plugins]: Plugins) {
            for(auto&& plugin:plugins) {
                plugin->timerExecute(this);
            }
        }

        if(neovim.readIsKeyPressed()) {
            for(auto& [_, plugins]: Plugins) {
                for(auto&& plugin:plugins) {
                    plugin->keyPressedExecute(this);
                }
            }
        }
        getNeoVim().setFocus(Qt::ActiveWindowFocusReason);
    }
}

void Basilico::mousePressEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton) {

    }
    else if(e->button() == Qt::RightButton) {

    }
    else if(e->button() == Qt::MiddleButton) {

    }
}

void Basilico::closeEvent(QCloseEvent* e)
{
    e->accept();
}

void Basilico::addPlugin(String plugin_name, BasilPlugin* plugin)
{
    if(Plugins.count(plugin_name) == 0) {
        Plugins.emplace(plugin_name, Vector<BasilPlugin*>{plugin});
    }
    else {
        Plugins.at(plugin_name).push_back(plugin);
    }
}

