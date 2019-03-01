#include "NeoVim.hpp"
#include "Basilico.hpp"

NeoVim::NeoVim(uint width, uint height, Basilico* parent_, const Dictionary& options)
        :neovim{width, height, options},
         isKeyPressed{false},
         parent{parent_}
{
    timer = startTimer(30);

    // <default settings>
    font_size_px = 16;
    font = "ubuntu mono";
    // </default settings>

    setAttribute(Qt::WA_InputMethodEnabled);
    QWidget::resize(width*(font_size_px + cwi)/2 + cw, height*(font_size_px + chi));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setTextInteractionFlags(Qt::TextEditable | Qt::LinksAccessibleByMouse);
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
                    "line-height: 1;"
                    "color: rgb(%1,%2,%3);"
                    "background-color: rgb(%4,%5,%6);"
                    "font-size: %7px;"
                    "font-family: \"%8\";"
                    "white-space: pre;}"
                    "</style>"
                    ).arg(drf).arg(dgf).arg(dbf).arg(drb).arg(dgb).arg(dbb)
                     .arg(font_size_px).arg(
                         QString::fromStdString((guifont == "") ? font : guifont)));
        Integer special_color;

        screen.append("<body><tt>");
        for(int i = 0;i < nvim_screen.size();i++)
        {
            // COLORS
            constexpr int default_idx = 0;
            Integer current_color_id = nvim_grid_colors_map.at(i).at(0);
            String color_part;

            Object current_fg, current_bg;
            bool current_is_bold, current_is_italic, current_is_underline, current_is_undercurl, current_is_reverse;
            bool next_is_bold, next_is_italic, next_is_underline, next_is_undercurl, next_is_reverse;

            if(nvim_hl_attr.at(current_color_id).rgb_attr.count("foreground") == 1)
            { current_fg = nvim_hl_attr.at(current_color_id).rgb_attr.at("foreground"); }
            else { current_fg = nvim_hl_attr.at(default_idx).rgb_attr.at("foreground"); }

            if(nvim_hl_attr.at(current_color_id).rgb_attr.count("background") == 1)
            { current_bg = nvim_hl_attr.at(current_color_id).rgb_attr.at("background"); }
            else { current_bg = nvim_hl_attr.at(default_idx).rgb_attr.at("background"); }

            if(nvim_hl_attr.at(current_color_id).rgb_attr.count("reverse") == 1)
            { current_is_reverse = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("reverse")); }
            else { current_is_reverse = false; }
            if(current_is_reverse)
                /* swap */
            {
                auto tmp = current_bg;
                current_bg = current_fg;
                current_fg = tmp;
            }
            auto [frf, fgf, fbf] = nvim_html::convert_rgb(boost::get<uInteger>(current_fg));
            auto [frb, fgb, fbb] = nvim_html::convert_rgb(boost::get<uInteger>(current_bg));

            if(nvim_hl_attr.at(current_color_id).rgb_attr.count("bold") == 1)
            { current_is_bold = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("bold")); }
            else { current_is_bold = false; }

            if(nvim_hl_attr.at(current_color_id).rgb_attr.count("italic") == 1)
            { current_is_italic = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("italic")); }
            else { current_is_italic = false; }

            if(nvim_hl_attr.at(current_color_id).rgb_attr.count("underline") == 1)
            { current_is_underline = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("underline")); }
            else { current_is_underline = false; }

            if(nvim_hl_attr.at(current_color_id).rgb_attr.count("undercurl") == 1)
            { current_is_undercurl = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("undercurl")); }
            else { current_is_undercurl = false; }

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
                            Object next_fg, next_bg;

                            if(nvim_hl_attr.at(next_color_id).rgb_attr.count("foreground") == 1)
                            { next_fg = nvim_hl_attr.at(next_color_id).rgb_attr.at("foreground"); }
                            else
                            { next_fg = nvim_hl_attr.at(default_idx).rgb_attr.at("foreground"); }

                            if(nvim_hl_attr.at(next_color_id).rgb_attr.count("background") == 1)
                            { next_bg = nvim_hl_attr.at(next_color_id).rgb_attr.at("background"); }
                            else
                            { next_bg = nvim_hl_attr.at(default_idx).rgb_attr.at("background"); }

                            if(nvim_hl_attr.at(next_color_id).rgb_attr.count("bold") == 1)
                            { next_is_bold = boost::get<bool>(nvim_hl_attr.at(next_color_id).rgb_attr.at("bold")); }
                            else { next_is_bold = false; }

                            if(nvim_hl_attr.at(next_color_id).rgb_attr.count("italic") == 1)
                            { next_is_italic = boost::get<bool>(nvim_hl_attr.at(next_color_id).rgb_attr.at("italic")); }
                            else { next_is_italic = false; }

                            if(nvim_hl_attr.at(next_color_id).rgb_attr.count("underline") == 1)
                            { next_is_underline = boost::get<bool>(nvim_hl_attr.at(next_color_id).rgb_attr.at("underline")); }
                            else { next_is_underline = false; }

                            if(nvim_hl_attr.at(next_color_id).rgb_attr.count("undercurl") == 1)
                            { next_is_undercurl = boost::get<bool>(nvim_hl_attr.at(next_color_id).rgb_attr.at("undercurl")); }
                            else { next_is_undercurl = false; }

                            nvim_html::html_escape(color_part);
                            screen.append(QString::fromStdString(color_part));
                            color_part = "";
                            screen.append("</span>");

                            if(nvim_hl_attr.at(next_color_id).rgb_attr.count("reverse") == 1)
                            { next_is_reverse = boost::get<bool>(nvim_hl_attr.at(next_color_id).rgb_attr.at("reverse")); }
                            else { next_is_reverse = false; }
                            if(next_is_reverse)
                                /* swap */
                            {
                                auto tmp = next_bg;
                                next_bg = next_fg;
                                next_fg = tmp;
                            }
                            auto [rf, gf, bf] = nvim_html::convert_rgb(boost::get<uInteger>(next_fg));
                            auto [rb, gb, bb] = nvim_html::convert_rgb(boost::get<uInteger>(next_bg));

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
                            Object next_fg, next_bg;
                            if(nvim_hl_attr.at(next_color_id).rgb_attr.count("foreground") == 1)
                            { next_fg = nvim_hl_attr.at(next_color_id).rgb_attr.at("foreground"); }
                            else
                            { next_fg = nvim_hl_attr.at(default_idx).rgb_attr.at("foreground"); }

                            if(nvim_hl_attr.at(next_color_id).rgb_attr.count("background") == 1)
                            { next_bg = nvim_hl_attr.at(next_color_id).rgb_attr.at("background"); }
                            else
                            { next_bg = nvim_hl_attr.at(default_idx).rgb_attr.at("background"); }

                            auto [rf, gf, bf] = nvim_html::convert_rgb(boost::get<uInteger>(next_fg));
                            auto [rb, gb, bb] = nvim_html::convert_rgb(boost::get<uInteger>(next_bg));
                            nvim_html::html_escape(color_part);
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
                            nvim_html::html_escape(color_part);
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

                    nvim_html::html_escape(color_part);
                    screen.append(QString::fromStdString(color_part));
                    if(i != nvim_screen.size() - 1){ screen.append("\n"); }
                    screen.append("</span>");
        }
        screen.append("</tt></body>");
    }
    else
    {
        std::cerr << "Please Upgrade NeoVim" << std::endl;
        exit(1);
    }

    std::cout << qPrintable(screen) << std::endl;
    for(auto&& line:nvim_screen)
    {
        line.test();
    }
    setHtml(screen);
    cursor_shape_and_pos();
    need_update = false;
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
    if(e->button() == Qt::LeftButton)
    {
        mouseSend(e, "press", "left", 0);
    }
    else if(e->button() == Qt::RightButton)
    {
        mouseSend(e, "press", "right", 0);
    }
    else if(e->button() == Qt::MiddleButton)
    {
        mouseSend(e, "press", "middle", 0);
    }
    else if(e->button() == Qt::ForwardButton)
    {
    }
    else if(e->button() == Qt::BackButton)
    {
    }
}

void NeoVim::mouseReleaseEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton)
    {
        mouseSend(e, "release", "left", 0);
    }
    else if(e->button() == Qt::RightButton)
    {
        mouseSend(e, "release", "right", 0);
    }
    else if(e->button() == Qt::MiddleButton)
    {
        mouseSend(e, "release", "middle", 0);
    }
    else if(e->button() == Qt::ForwardButton)
    {
    }
    else if(e->button() == Qt::BackButton)
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
    parent->setWindowTitle(QString::fromStdString(nvim_title + "<" + parent->Name + ">"));
}

void NeoVim::mode_changed()
{
    cursor_shape(current_mode["screen"]);
}

void NeoVim::call_plugin(Object func_and_args)
{
    Array info = boost::get<Array>(func_and_args);
    plugin_deque.push_back(info);
}

void NeoVim::cursor_shape_and_pos()
{
    QTextCursor cursor(textCursor());
    cursor.setPosition(0);

    auto num_wchar = 0;
    for(uint i = 0;i < nvim_screen.at(nvim_cursor_y).size();i++)
    {
        if(nvim_screen.at(nvim_cursor_y).at(i).size() >= 3){ ++num_wchar; }
    }
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, nvim_cursor_x - num_wchar);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, nvim_cursor_y);
    setTextCursor(cursor);
    return;
}


void NeoVim::fkeySend(QKeyEvent* e, Integer key)
{
    if(e->modifiers() == Qt::ShiftModifier){ key += 12; }
    if(e->modifiers() == Qt::ControlModifier){ key += 24; }
    nvim_input("<F" + std::to_string(key) + ">");
}

void NeoVim::keySend(QInputEvent* e, const String& key, bool no_shift)
{
    if(e->modifiers() == Qt::NoModifier)
    {
        (key.size() == 1) ? nvim_input(key) : nvim_input("<" + key + ">");
        return;
    }
    String k = "";
    if(e->modifiers() == Qt::ShiftModifier and !no_shift){ k += "S-"; }
    if(e->modifiers() == Qt::ControlModifier){ k += "C-"; }
    if(e->modifiers() == Qt::AltModifier){ k += "A-"; }

    if(key == "<") { k += "LT"; }
    else if(key == "\\") { k += "Bslash"; }
    else { k += key; }

    if(k == "C-/"){ k = "C-_"; }

    (k == key and key.size() == 1) ? nvim_input(k) : nvim_input("<" + k + ">");
}

void NeoVim::mouseSend(QPoint pos, const String& modifiers, const String& action, const String& button, Integer grid)
{
    QTextCursor c = cursorForPosition(pos);
    int row = c.position() / nvim_screen.at(0).size();
    int col = c.position() % nvim_screen.at(0).size() - row;
    nvim_input_mouse(button, action, modifiers, grid, row, col);
}

void NeoVim::mouseSend(QMouseEvent* e, const String& action, const String& button, Integer grid)
{
    String modifiers = "";
    if(e->modifiers() == Qt::ShiftModifier){ modifiers += "S"; }
    if(e->modifiers() == Qt::ControlModifier){ modifiers += "C"; }
    if(e->modifiers() == Qt::AltModifier){ modifiers += "A"; }
    mouseSend(e->pos(), modifiers, action, button, grid);
}

void NeoVim::mouseSend(QWheelEvent* e, const String& action, const String& button, Integer grid)
{
    String modifiers = "";
    if(e->modifiers() == Qt::ShiftModifier){ modifiers += "S"; }
    if(e->modifiers() == Qt::ControlModifier){ modifiers += "C"; }
    if(e->modifiers() == Qt::AltModifier){ modifiers += "A"; }
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
                 (m == Mode::shortmatch) ? "shortmatch" : "???";

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

void nvim_html::html_escape(std::string& s)
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

    // bool prev_len_is_1 = true;
    // bool tag_open = false;
    // for(int i = 0;i < s.size();i++)
    // {
    //     if((s.at(i) & utils::BIN1x2) == utils::BIN1x2)  // top of unicode char and not ascii
    //     {
    //         if(prev_len_is_1){
    //             const std::string html = "<span style=\"font-size: 80%;\">";
    //             s.insert(i, html);
    //             i += html.size();
    //             prev_len_is_1 = false;
    //             tag_open = true;
    //         }
    //     }
    //     else if((s.at(i) & utils::BIN1x1) == 0)  // ascii code
    //     {
    //         if(!prev_len_is_1){
    //             std::string html = "</span>";
    //             s.insert(i, html);
    //             i += html.size();
    //             tag_open = false;
    //         }
    //         prev_len_is_1 = true;
    //     }
    // }
    // if(tag_open)
    // {
    //     std::string html = "</span>";
    //     s.append(html);
    // }
}

std::tuple<unsigned long, unsigned long, unsigned long> nvim_html::convert_rgb(unsigned long rgb)
{
    unsigned long r = rgb >> 16;
    unsigned long g = (rgb >> 8) - (r << 8);
    unsigned long b = rgb - (r << 16) - (g << 8);
    return std::make_tuple(r, g, b);
}
