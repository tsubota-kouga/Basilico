#include "NeoVim.hpp"
#include "Basilico.hpp"

NeoVim::NeoVim(uint width, uint height, Basilico* parent_, const Dictionary& options, String ip, String port, int timeout_millisec)
        :neovim{width, height, options},
         QTextEdit{},
         isKeyPressed{false},
         parent{parent_}
{
    connect_tcp(ip, port, timeout_millisec);
    timer = startTimer(30);

    // <default settings>
    _font_size_px = 16;
    wchar_font_size_px = _font_size_px - 1;
    _guifont = "ubuntu mono";
    // </default settings>

    setContextMenuPolicy(Qt::NoContextMenu);
    setAttribute(Qt::WA_InputMethodEnabled);
    QWidget::resize(width*(_font_size_px + cwi)/2 + cw, height*(_font_size_px + chi));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    keyModNumberMap = [](const String&){};
}

void NeoVim::set_neovim_html()
{
    if(!need_update){ return; }
    QString screen;
    if(is_ext_linegrid)
    {
        // default color
        auto [drf, dgf, dbf] = nvim_html::convert_rgb(
                boost::get<uInteger>(nvim_hl_attr.at(0).rgb_attr.at("foreground")));
        auto [drb, dgb, dbb] = nvim_html::convert_rgb(
                boost::get<uInteger>(nvim_hl_attr.at(0).rgb_attr.at("background")));
        screen.append(QString(
                    "<style>"
                    "* {"
                    "appearance: none;"
                    "margin: 0px;"
                    "padding: 0px;}"
                    "body {"
                    "width: 0;"
                    "height: 100%;"
                    "margin: 0px;"
                    "padding: 0px;"
                    "color: rgb(%1,%2,%3);"
                    "background-color: rgb(%4,%5,%6);"
                    "font-size: %7px;"
                    "font-family: \"%8\";"
                    "white-space: pre;}"
                    "</style>"
                    ).arg(drf).arg(dgf).arg(dbf).arg(drb).arg(dgb).arg(dbb)
                     .arg(_font_size_px).arg(
                         QString::fromStdString((guifont == "") ? _guifont : guifont)));
        screen.append("<body>\n");

        Integer special_color;
        for(int i = 0;i < nvim_screen.size();i++)
        {
            // COLORS
            Integer current_color_id = nvim_grid_colors_map.at(i).at(0);
            String color_part;

            uInteger current_fg, current_bg;
            bool current_is_bold, current_is_italic, current_is_underline, current_is_undercurl, current_is_reverse;
            bool next_is_bold, next_is_italic, next_is_underline, next_is_undercurl, next_is_reverse;

            current_fg = hl_attr_get<uInteger>(current_color_id, "foreground");
            current_bg = hl_attr_get<uInteger>(current_color_id, "background");

            current_is_reverse = hl_attr_get<bool>(current_color_id, "reverse");
            if(current_is_reverse)
                /* swap */
            {
                auto tmp = current_bg;
                current_bg = current_fg;
                current_fg = tmp;
            }
            auto [frf, fgf, fbf] = nvim_html::convert_rgb(current_fg);
            auto [frb, fgb, fbb] = nvim_html::convert_rgb(current_bg);

            current_is_bold = hl_attr_get<bool>(current_color_id, "bold");
            current_is_italic = hl_attr_get<bool>(current_color_id, "italic");
            current_is_underline = hl_attr_get<bool>(current_color_id, "underline");
            current_is_undercurl = hl_attr_get<bool>(current_color_id, "undercurl");

            if(current_is_underline or current_is_undercurl)
            {
                if(nvim_hl_attr.at(current_color_id).rgb_attr.count("special") == 1)
                {
                    special_color = boost::get<uInteger>(
                            nvim_hl_attr.at(current_color_id).rgb_attr.at("special"));
                }
                else if(auto special_orig =
                        nvim_hl_attr.at(default_idx).rgb_attr.at("special");
                        special_orig.is_uint64_t())
                {
                    special_color = boost::get<uInteger>(special_orig);
                }
                else
                {
                    special_color = boost::get<Integer>(special_orig);
                }
            }

            //<start>
            screen.append("<span style=\"");
            //</start>
            //<decoration>
            if(current_is_bold){ screen.append(
                    "font-weight: bold;"
                    ); }
            if(current_is_italic){ screen.append(
                    "font-style: italic;"
                    ); }
            if(current_is_underline){
                auto [sr, sg, sb] = nvim_html::convert_rgb(special_color);
                screen.append(QString(
                            "text-decoration: underline;"
                            "text-decoration-color: rgb(%1,%2,%3);"
                            ).arg(sr).arg(sg).arg(sb)); }
            if(current_is_undercurl){
                auto [sr, sg, sb] = nvim_html::convert_rgb(special_color);
                screen.append(QString(
                            "text-decoration: underline;"
                            "text-decoration-color: rgb(%1,%2,%3);"
                            ).arg(sr).arg(sg).arg(sb)); }
            //</decoration>
            //<end>
            screen.append(QString(
                        "color: rgb(%1, %2, %3);"
                        "background-color:rgb(%4, %5, %6);\">"
                        ).arg(frf).arg(fgf).arg(fbf).arg(frb).arg(fgb).arg(fbb));
            //</end>

            for(int j = 0;j < nvim_grid_colors_map.at(i).size();j++)
            {
                Integer next_color_id;
                next_color_id = nvim_grid_colors_map.at(i).at(j);

                if(current_color_id != next_color_id)
                {
                    uInteger next_fg, next_bg;

                    next_fg = hl_attr_get<uInteger>(next_color_id, "foreground");
                    next_bg = hl_attr_get<uInteger>(next_color_id, "background");

                    next_is_bold = hl_attr_get<bool>(next_color_id, "bold");
                    next_is_italic = hl_attr_get<bool>(next_color_id, "italic");
                    next_is_underline = hl_attr_get<bool>(next_color_id, "underline");
                    next_is_undercurl = hl_attr_get<bool>(next_color_id, "undercurl");

                    nvim_html::html_escape(color_part, wchar_font_size_px);
                    screen.append(QString::fromStdString(color_part));
                    screen.append("</span>");

                    color_part = "";
                    next_is_reverse = hl_attr_get<bool>(next_color_id, "reverse");
                    if(next_is_reverse)
                        /* swap */
                    {
                        auto tmp = next_bg;
                        next_bg = next_fg;
                        next_fg = tmp;
                    }
                    auto [rf, gf, bf] = nvim_html::convert_rgb(next_fg);
                    auto [rb, gb, bb] = nvim_html::convert_rgb(next_bg);

                    if(next_is_underline or next_is_undercurl)
                    {
                        if(nvim_hl_attr.at(next_color_id).rgb_attr.count("special") == 1)
                        {
                            special_color = boost::get<uInteger>(
                                    nvim_hl_attr.at(next_color_id).rgb_attr.at("special"));
                        }
                        else if(auto special_orig =
                                nvim_hl_attr.at(default_idx).rgb_attr.at("special");
                                special_orig.is_uint64_t())
                        {
                            special_color = boost::get<uInteger>(special_orig);
                        }
                        else
                        {
                            special_color = boost::get<Integer>(special_orig);
                        }
                    }
                    //<start>
                    screen.append("<span style=\"");
                    //</start>
                    //<decoration>
                    if(next_is_bold){ screen.append(
                            "font-weight: bold;"
                            ); }
                    if(next_is_italic){ screen.append(
                            "font-style: italic;"
                            ); }
                    if(next_is_underline){
                        auto [sr, sg, sb] = nvim_html::convert_rgb(special_color);
                        screen.append(QString(
                                    "text-decoration: underline;"
                                    "text-decoration-color: rgb(%1,%2,%3);"
                                    ).arg(sr).arg(sg).arg(sb)); }
                    if(next_is_undercurl){
                        auto [sr, sg, sb] = nvim_html::convert_rgb(special_color);
                        screen.append(QString(
                                    "text-decoration: underline;"
                                    "text-decoration-color: rgb(%1,%2,%3);"
                                    ).arg(sr).arg(sg).arg(sb)); }
                    //</decoration>
                    //<end>
                    screen.append(QString(
                                "color: rgb(%1, %2, %3);"
                                "background-color:rgb(%4, %5, %6);\">"
                                ).arg(rf).arg(gf).arg(bf).arg(rb).arg(gb).arg(bb));
                    //</end>
                }

                //<cursor setting>
                bool cursor_positioned =
                    (j == nvim_cursor_x) and (i == nvim_cursor_y)
                    and cursorShape != "vertical" and not is_busy;

                if(cursor_positioned)
                {
                    uInteger next_fg, next_bg;
                    next_fg = hl_attr_get<uInteger>(next_color_id, (next_is_reverse) ? "background" : "foreground");
                    next_bg = hl_attr_get<uInteger>(next_color_id, (next_is_reverse) ? "foreground" : "background");

                    auto [rf, gf, bf] = nvim_html::convert_rgb(next_fg);
                    auto [rb, gb, bb] = nvim_html::convert_rgb(next_bg);
                    nvim_html::html_escape(color_part, wchar_font_size_px);
                    screen.append(QString::fromStdString(color_part));
                    color_part = "";
                    if(cursorShape == "block")
                    {
                        screen.append(QString(
                                    "<span style=\""
                                    "color: rgb(%1, %2, %3);"
                                    "background-color:rgb(%4, %5, %6);\">"
                                    ).arg(rb).arg(gb).arg(bb).arg(rf).arg(gf).arg(bf));
                    }
                    else if(cursorShape == "horizontal")
                    {
                        screen.append(QString(
                                    "<span style=\""
                                    "text-decoration: underline;"
                                    "text-decoration-color:rgb(%1, %2, %3);\">"
                                    ).arg(rf).arg(gf).arg(bf));
                    }
                }
                //</cursor setting>

                color_part += nvim_screen.at(i).at(j);
                if(cursor_positioned)
                {
                    nvim_html::html_escape(color_part, wchar_font_size_px);
                    screen.append(QString::fromStdString(color_part));
                    screen.append("</span>");
                    color_part = "";
                    cursor_positioned = false;
                }

                current_color_id = next_color_id;
                current_is_bold = next_is_bold;
                current_is_italic = next_is_italic;
                current_is_underline = next_is_underline;
            }

            nvim_html::html_escape(color_part, wchar_font_size_px);
            screen.append(QString::fromStdString(color_part));
            screen.append("</span>");
            if(i != nvim_screen.size() - 1){ screen.append("\n"); }
        }
        screen.append("\n</body>");
    }
    else
    {
        std::cerr << "Please Upgrade NeoVim" << std::endl;
        exit(1);
    }

    // std::cout << qPrintable(screen) << std::endl;
    // for(auto&& line:nvim_screen)
    // {
    //     line.test();
    // }
    setHtml(screen);
    cursor_shape_and_pos();
    need_update = false;
}

void NeoVim::set_font_size_px(int px)
{
    _font_size_px = px;
    int row = windowWidth2Width(size().width());
    int col = windowHeight2Height(size().height());
    nvim_ui_try_resize(row, col);
}

bool NeoVim::event(QEvent* e)
{
    switch (e->type())
    {
        default:
            return QTextEdit::event(e);
    }
}

void NeoVim::keyPressEvent(QKeyEvent* e)
{
    if(input_control_flag){ return; }
    std::cout << e->key() << std::endl;
    isKeyPressed = true;
    switch(e->key())
    {
        case Qt::Key_Escape:
            nvim_input("<Esc>");
            break;

        case Qt::Key_Tab:
            keySend(e, "Tab");
            break;
        case Qt::Key_Backspace:
            keySend(e, "BS");
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            keySend(e, "CR");
            break;

        case Qt::Key_Insert:
            keySend(e, "Ins");
            break;

        case Qt::Key_Delete:
            keySend(e, "Del");
            break;

        case Qt::Key_Home:
            keySend(e, "Home");
            break;

        case Qt::Key_End:
            keySend(e, "End");
            break;

        case Qt::Key_Left:
            keySend(e, "Left");
            break;

        case Qt::Key_Right:
            keySend(e, "Right");
            break;

        case Qt::Key_Up:
            keySend(e, "Up");
            break;

        case Qt::Key_Down:
            keySend(e, "Down");
            break;

        case Qt::Key_PageUp:
            keySend(e, "PageUp");
            break;

        case Qt::Key_PageDown:
            keySend(e, "PageDown");
            break;

        case Qt::Key_Space:
            keySend(e, "Space");
            break;

        case Qt::Key_F1:
            fkeySend(e, 1);
            break;

        case Qt::Key_F2:
            fkeySend(e, 2);
            break;

        case Qt::Key_F3:
            fkeySend(e, 3);
            break;

        case Qt::Key_F4:
            fkeySend(e, 4);
            break;

        case Qt::Key_F5:
            fkeySend(e, 5);
            break;

        case Qt::Key_F6:
            fkeySend(e, 6);
            break;

        case Qt::Key_F7:
            fkeySend(e, 7);
            break;

        case Qt::Key_F8:
            fkeySend(e, 8);
            break;

        case Qt::Key_F9:
            fkeySend(e, 9);
            break;

        case Qt::Key_F10:
            fkeySend(e, 10);
            break;

        case Qt::Key_F11:
            fkeySend(e, 11);
            break;

        case Qt::Key_F12:
            fkeySend(e, 12);
            break;

        case Qt::Key_Super_L:
            break;

        case Qt::Key_Super_R:
            break;

        case Qt::Key_0:
            keySend(e, 0);
            break;

        case Qt::Key_1:
            keySend(e, 1);
            break;

        case Qt::Key_2:
            keySend(e, 2);
            break;

        case Qt::Key_3:
            keySend(e, 3);
            break;

        case Qt::Key_4:
            keySend(e, 4);
            break;

        case Qt::Key_5:
            keySend(e, 5);
            break;

        case Qt::Key_6:
            keySend(e, 6);
            break;

        case Qt::Key_7:
            keySend(e, 7);
            break;

        case Qt::Key_8:
            keySend(e, 8);
            break;

        case Qt::Key_9:
            keySend(e, 9);
            break;

        default:
            if(e->text().size() == 1 and e->text().at(0).isPrint())
            {
                keySend(e,
                        e->text().toUtf8().data(),
                        e->text().size() == 1 and !e->text().at(0).isLetterOrNumber());
            }
            else if(e->text().size() == 1 and 0x20 < e->key() and e->key() < 0x80)
            {
                keySend(e,
                        String{ static_cast<char>(e->key()) },
                        e->text().size() == 1 and !e->text().at(0).isLetterOrNumber());
            }
            break;
    }
}

void NeoVim::timerEvent(QTimerEvent* e)
{
    if(e->timerId() == timer)
    {
        while(!input_deque.empty())
        {
            auto&& input = input_deque.front();
            nvim_input(input);
            input_deque.pop_front();
            if(input == "<CR>" and current_mode["screen"] == Mode::cmdline_normal){
                input_deque.clear();
            }
        }
        operation();
        if(nvim_hl_attr.size() > 0)
        {
            set_neovim_html();
        }
    }
}

void NeoVim::inputMethodEvent(QInputMethodEvent* e)
{
    QWidget::inputMethodEvent(e);
    nvim_input(e->commitString().toUtf8().constData());
}

void NeoVim::resizeEvent(QResizeEvent* e)
{
    int row = windowWidth2Width(e->size().width());
    int col = windowHeight2Height(e->size().height());
    nvim_ui_try_resize(row, col);

    QTextEdit::resizeEvent(e);
}

void NeoVim::mousePressEvent(QMouseEvent* e)
{
    if(e->button() & Qt::LeftButton)
    {
        mouseSend(e, "press", "left", 0);
    }
    else if(e->button() & Qt::RightButton)
    {
        mouseSend(e, "press", "right", 0);
    }
    else if(e->button() & Qt::MiddleButton)
    {
        mouseSend(e, "press", "middle", 0);
    }
    else if(e->button() & Qt::ForwardButton)
    {
    }
    else if(e->button() & Qt::BackButton)
    {
    }
}

void NeoVim::mouseReleaseEvent(QMouseEvent* e)
{
    if(e->button() & Qt::LeftButton)
    {
        mouseSend(e, "release", "left", 0);
    }
    else if(e->button() & Qt::RightButton)
    {
        mouseSend(e, "release", "right", 0);
    }
    else if(e->button() & Qt::MiddleButton)
    {
        mouseSend(e, "release", "middle", 0);
    }
    else if(e->button() & Qt::ForwardButton)
    {
    }
    else if(e->button() & Qt::BackButton)
    {
    }
}

void NeoVim::mouseMoveEvent(QMouseEvent* e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        mouseSend(e, "drag", "left", 0);
    }
    else if(e->buttons() & Qt::RightButton)
    {
        mouseSend(e, "drag", "right", 0);
    }
    else if(e->buttons() & Qt::MiddleButton)
    {
        mouseSend(e, "drag", "middle", 0);
    }
    else if(e->buttons() & Qt::ForwardButton)
    {
    }
    else if(e->buttons() & Qt::BackButton)
    {
    }
}

void NeoVim::wheelEvent(QWheelEvent* e)
{
    auto numDegrees = e->angleDelta() / 8;
    if(!numDegrees.isNull())
    {
        if(numDegrees.ry() > 0)
        {
            mouseSend(e, "up", "wheel", 0);
        }
        else if(numDegrees.ry() < 0)
        {
            mouseSend(e, "down", "wheel", 0);
        }
    }
}

void NeoVim::dropEvent(QDropEvent* e)
{
    const auto mime = e->mimeData();
    if(mime->hasFormat("text/*"))
    {
        nvim_input(mime->text().toUtf8().data());
    }
}

void NeoVim::dragEnterEvent(QDragEnterEvent* e)
{
    std::cout << "DragEnter" << std::endl;
}

void NeoVim::dragLeaveEvent(QDragLeaveEvent* e)
{
    std::cout << "DragLeave" << std::endl;
}

void NeoVim::dragMoveEvent(QDragMoveEvent* e)
{
    std::cout << "DragMove" << std::endl;
}

void NeoVim::tabline_update(Tabpage current, const Array& tabs)
{
    int tabline_size = nvim_tabline.size();
    for(auto&& c: tabs)
    {
        auto tab_dict = boost::get<Dictionary>(c);
        auto&& itr_tab = tab_dict.find("tab");
        auto&& tab = boost::get<Tabpage>(itr_tab->second);

        auto&& itr_name = tab_dict.find("name");
        auto&& name = boost::get<String>(itr_name->second);

        nvim_tabline.push_back(make_pair(tab, name));
    }
    for(int i = 0;i < tabline_size;i++)
    {
        nvim_tabline.pop_front();
    }
    parent->changeTabNeoVim(nvim_tabline, current);
}

void NeoVim::title_changed()
{
    parent->setWindowTitle(QString::fromStdString(nvim_title + "<" + parent->getName() + ">"));
}

void NeoVim::mode_changed()
{
    cursor_shape(current_mode["screen"]);
}

void NeoVim::call_plugin(Object func_and_args)
{
    Array info = boost::get<Array>(func_and_args);
    plugin_req_deque.push_back(info);
}

void NeoVim::cursor_shape_and_pos()
{
    QTextCursor cursor(document());
    cursor.setPosition(0);

    std::cout << nvim_cursor_x << std::endl;
    std::cout << nvim_cursor_y << std::endl;
    auto num_wchar = 0;
    auto pos_x = 0;
    for(uint i = 0;i < nvim_cursor_x;i++)  // nvim_cursor_x include empty char:[]
    {
        if(nvim_screen.at(nvim_cursor_y).at(i) != "")
        {
            pos_x++;
        }
    }
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, nvim_cursor_y);
    auto pos = cursor.position() + pos_x;
    cursor.setPosition(pos);
    setTextCursor(cursor);
    return;
}

void NeoVim::popupmenu_show(const Array& items, Integer selected, Integer row, Integer col, Integer gird)
{
    completion_info.resize(items.size());
    popupmenu_pos = std::make_pair(row, col);
    popupmenu_selected = selected;
    for(int i = 0;i < items.size();i++)
    {
        auto&& item = boost::get<Array>(items.at(i));
        completion_info.at(i) = std::make_tuple(
                boost::get<String>(item.at(0)),
                boost::get<String>(item.at(1)),
                boost::get<String>(item.at(2)),
                boost::get<String>(item.at(3))
                );
    }

    parent->get_nvim_comp().clear();
    for(auto&& [word, kind, menu, info]: completion_info)
    {
        parent->get_nvim_comp().addItem(QString::fromStdString(word));
    }

    parent->get_nvim_comp().move((col + 1)*(_font_size_px) / 2 + eps_w,
                                 (row + 1)*(_font_size_px + 1 + eps_h));
    parent->get_nvim_comp().setCurrentRow(selected);
    parent->get_nvim_comp().show();
}

void NeoVim::popupmenu_select(Integer selected)
{
    popupmenu_selected = selected;
}

void NeoVim::popupmenu_hide()
{
    parent->get_nvim_comp().hide();
}

void NeoVim::fkeySend(QKeyEvent* e, Integer key)
{
    if(e->modifiers() == Qt::ShiftModifier){ key += 12; }
    if(e->modifiers() == Qt::ControlModifier){ key += 24; }
    // nvim_input("<F" + std::to_string(key) + ">");
    input_deque.push_back("<F" + std::to_string(key) + ">");
}

void NeoVim::keySend(QInputEvent* e, const String& key, bool no_shift)
{
    if(input_control_flag){ return; }
    if(e->modifiers() == Qt::NoModifier)
    {
        // (key.size() == 1) ? nvim_input(key) : nvim_input("<" + key + ">");
        (key.size() == 1) ? input_deque.push_back(key) : input_deque.push_back("<" + key + ">");
    }
    else
    {
        String k = "";
        if(e->modifiers() == Qt::ShiftModifier and !no_shift){ k += "S-"; }
        if(e->modifiers() == Qt::ControlModifier){ k += "C-"; }
        if(e->modifiers() == Qt::AltModifier){ k += "A-"; }

        if(key == "<") { k += "LT"; }
        else if(key == "\\") { k += "Bslash"; }
        else if(key == "," or key == "." or key == ";" or key == ":"){ k = key; }
        else { k += key; }

        if(k == "C-/"){ k = "C-_"; }

        // (k == key and key.size() == 1) ? nvim_input(k) : nvim_input("<" + k + ">");
        (k == key and key.size() == 1) ? input_deque.push_back(k) : input_deque.push_back("<" + k + ">");
    }
}

void NeoVim::keySend(QInputEvent* e, const Integer& key)
{
    if(e->modifiers() == Qt::NoModifier)
    {
        // nvim_input(std::to_string(key));
        input_deque.push_back(std::to_string(key));
        return;
    }
    String k = "";
    if(e->modifiers() & Qt::ShiftModifier){ k += "S-"; }
    if(e->modifiers() & Qt::ControlModifier){ k += "C-"; }
    if(e->modifiers() & Qt::AltModifier){ k += "A-"; }
    if(e->modifiers() & Qt::KeypadModifier)
    {
        // nvim_input(k + "k" + std::to_string(key));
        input_deque.push_back(k + "k" + std::to_string(key));
        return;
    }
    else
    {
        keyModNumberMap(k + std::to_string(key));
    }
}

void NeoVim::mouseSend(QPoint pos, const String& modifiers, const String& action, const String& button, Integer grid)
{
    QTextCursor c = cursorForPosition(pos);
    int row = 0, col = 0, num_char = 0;
    for(row = 0;row < nvim_screen.size();row++)
    {
        for(col = 0;col < nvim_screen.at(row).size();col++)
        {
            if(nvim_screen.at(row).at(col) != "")
            {
                num_char++;
            }
            if(num_char == c.position())
            {
                // TODO
                std::cout << row << ", " << col << std::endl;
                std::cout << num_char << ", " << c.position() << std::endl;
                goto send;
            }
        }
        num_char++;
    }
send:
#if NVIM_API_LEVEL >= 6
    nvim_input_mouse(button, action, modifiers, grid, row, col);
#else
    auto b = button;
    b.at(0) += 0x20;
    auto a = action;
    a = (a == "press") ? "Mouse" :
        (a == "release") ? "Release" :
        (a == "drag") ? "Drag" :
        (a == "up") ? "Up":
        (a == "down") ? "Down" : "";
    if(button != "wheel")
    {
        // nvim_input("<" + modifiers + button + a + "><"
        //         + std::to_string(col) + "," + std::to_string(row) + ">");
        input_deque.push_back("<" + modifiers + button + a + "><"
                + std::to_string(col) + "," + std::to_string(row) + ">");
    }
    else
    {
        // nvim_input("<" + modifiers + "ScrollWheel" + a + "><"
        //         + std::to_string(col) + "," + std::to_string(row) + ">");
        input_deque.push_back("<" + modifiers + "ScrollWheel" + a + "><"
                + std::to_string(col) + "," + std::to_string(row) + ">");
    }
#endif
}

void NeoVim::mouseSend(QMouseEvent* e, const String& action, const String& button, Integer grid)
{
    String modifiers = "";
    if(e->modifiers() & Qt::ShiftModifier){ modifiers += "S-"; }
    if(e->modifiers() & Qt::ControlModifier){ modifiers += "C-"; }
    if(e->modifiers() & Qt::AltModifier){ modifiers += "A-"; }
    mouseSend(e->pos(), modifiers, action, button, grid);
}

void NeoVim::mouseSend(QWheelEvent* e, const String& action, const String& button, Integer grid)
{
    String modifiers = "";
    if(e->modifiers() & Qt::ShiftModifier){ modifiers += "S-"; }
    if(e->modifiers() & Qt::ControlModifier){ modifiers += "C-"; }
    if(e->modifiers() & Qt::AltModifier){ modifiers += "A-"; }
    mouseSend(e->pos(), modifiers, action, button, grid);
}

void NeoVim::cursor_shape(Mode m)
{
    String key = (m == Mode::normal) ? "normal" :
                 (m == Mode::insert) ? "insert" :
                 (m == Mode::replace) ? "replace" :
                 (m == Mode::visual) ? "visual" :
                 (m == Mode::cmdline_normal) ? "cmdline_normal" :
                 (m == Mode::cmdline_insert) ? "cmdline_insert" :
                 (m == Mode::cmdline_replace) ? "cmdline_replace" :
                 (m == Mode::operator_) ? "operator" :
                 (m == Mode::visual_select) ? "visual_select" :
                 (m == Mode::cmdline_hover) ? "cmdline_hover" :
                 (m == Mode::statusline_hover) ? "statusline_hover" :
                 (m == Mode::statusline_drag) ? "statusline_drag" :
                 (m == Mode::vsep_hover) ? "vsep_hover" :
                 (m == Mode::vsep_drag) ? "vsep_drag" :
                 (m == Mode::more) ? "more" :
                 (m == Mode::more_lastline) ? "more_lastline" :
                 (m == Mode::showmatch) ? "showmatch" : "???";

    cursorShape = boost::get<String>(ui_mode_info[key]["cursor_shape"]);
    if(cursorShape == "vertical")
    {
        auto percent = boost::get<uInteger>(ui_mode_info[key]["cell_percentage"]);
        setReadOnly(percent == 0);setCursorWidth(1);
    }
    else
    {
        setReadOnly(true);setCursorWidth(0);
    }
    cursorColorId = boost::get<uInteger>(ui_mode_info[key]["attr_id"]);
}

void NeoVim::tabline_change(int idx)
{
    if(idx < 0){ return; }
    nvim_set_current_tabpage(nvim_tabline.at(idx).first);
}

void nvim_html::html_escape(std::string& s, int wchar_size)
{
    std::unordered_map<std::string, std::string> special_char
    {
        {"<", "&lt;"},
        {">", "&gt;"},
        {"¦", "&brvbar;"},
        {" ", "&nbsp;"},
        {"　", "&emsp;"},
        // {"\"", "&quot;"},
        {"&", "&amp;"},
    };

    for(const auto& [from, to]: special_char)
    {
        std::string::size_type pos = 0;
        while((pos = s.find(from, pos)) != std::string::npos)
        {
            s.replace(pos, from.size(), to);
            pos += to.size();
        }
    }

    bool is_prev_ascii = true;
    bool tag_open = false;
    auto append_begin_tag = [&](auto& s, auto& i)
    {
        if(is_prev_ascii){
            const std::string html = "<span style=\"font-size: " + std::to_string(wchar_size) + "px;\">";
            s.insert(i, html);
            i += html.size();
            is_prev_ascii = false;
            tag_open = true;
        }
    };
    auto append_end_tag = [&](auto& s, auto& i)
    {
        if(!is_prev_ascii){
            const std::string html = "</span>";
            s.insert(i, html);
            i += html.size();
            tag_open = false;
        }
        is_prev_ascii = true;
    };

    for(int i = 0;i < s.size();)
    {
        auto&& [codepoint, size] = utf8_ord(s, i);
        if((0x00A2 <= codepoint and codepoint <= 0x00F7) or
           (0x0391 <= codepoint and codepoint <= 0x03C9) or
           (0x0401 <= codepoint and codepoint <= 0x0451) or
           (0x2010 <= codepoint and codepoint <= 0x2312) or
           (0x2500 <= codepoint and codepoint <= 0x254B) or
           (0x25A0 <= codepoint and codepoint <= 0x266F) or
           (0x3000 <= codepoint and codepoint <= 0x301C) or
           (0x3041 <= codepoint and codepoint <= 0x3093) or
           (0x30A1 <= codepoint and codepoint <= 0x30F6) or
           (0x4E00 <= codepoint and codepoint <= 0x9FA0) or
           codepoint == 0x309B or
           codepoint == 0x309C or
           codepoint == 0x309D or // ゝ
           codepoint == 0x309E or // ゞ
           codepoint == 0x30FB or // ・
           codepoint == 0x30FC or // ー
           codepoint == 0x30FD or // ヽ
           codepoint == 0x30FE)   // ヾ
        {
            append_begin_tag(s, i);
        }
        else
        {
            append_end_tag(s, i);
        }
        i += size;
    }
    if(tag_open)
    {
        std::string html = "</span>";
        s.append(html);
    }
}

std::tuple<unsigned long, unsigned long, unsigned long> nvim_html::convert_rgb(unsigned long rgb)
{
    unsigned long r = rgb >> 16;
    unsigned long g = (rgb >> 8) - (r << 8);
    unsigned long b = rgb - (r << 16) - (g << 8);
    return std::make_tuple(r, g, b);
}

int nvim_html::how_many_div_by_exp2(unsigned char c)
{
    int i = sizeof(c)*8 - 1;
    int ans = 0;
    for(int e = static_cast<unsigned char>(std::exp2(i));
            c >= e;c -= e, e = static_cast<unsigned char>(std::exp2(--i)))
    {
        ++ans;
    }
    return ans;
}

std::pair<unsigned, unsigned> nvim_html::utf8_ord(const std::string& str, int& i)
{
    const unsigned char& c = str.at(i);
    unsigned codepoint = 0;
    unsigned size = 0;
    if (c < 0x80)
    {
        codepoint = str.at(i);
        size = 1;
    }
    else if (c < 0xE0)
    {
        codepoint = ((str.at(i) & 0x1F) << 6) | (str.at(i + 1) & 0x3F);
        size = 2;
    }
    else if (c < 0xF0)
    {
        codepoint = ((str.at(i) &  0xF) << 12) | ((str.at(i + 1) & 0x3F) <<  6) | (str[i + 2] & 0x3F);
        size = 3;
    }
    else
    {
        codepoint = ((str.at(i) &  0x7) << 18) | ((str.at(i + 1) & 0x3F) << 12) | ((str.at(i + 2) & 0x3F) <<  6) | (str.at(i + 3)  & 0x3F);
        size = 4;
    }
    return std::make_pair(codepoint, size);
}

