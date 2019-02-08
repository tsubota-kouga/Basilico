#include "NeoVim.hpp"

void NeoVim::set_neovim_html()
{
    if(!need_update){ return; }
    QString screen;
    printf("AAAAAAAAAA\n");
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
                    "width: %7;"
                    "height: %7;"
                    "margin: 0px;"
                    "padding: 0px;"
                    "line-height: 100%;"
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

        screen.append("<body>");
        for(int i = 0;i < nvim_screen.size();i++)
        {
            constexpr int default_idx = 0;
            Integer current_color_id = nvim_grid_colors_map.at(i).at(0);
            String color_part;

            Object current_fg, current_bg;
            bool current_is_bold, current_is_italic, current_is_underline, current_is_undercurl, current_is_reverse;
            bool next_is_bold, next_is_italic, next_is_underline, next_is_undercurl, next_is_reverse;

            try{ current_fg = nvim_hl_attr.at(current_color_id).rgb_attr.at("foreground"); }
            catch(std::out_of_range)
            { current_fg = nvim_hl_attr.at(default_idx).rgb_attr.at("foreground"); }

            try{ current_bg = nvim_hl_attr.at(current_color_id).rgb_attr.at("background"); }
            catch(std::out_of_range)
            { current_bg = nvim_hl_attr.at(default_idx).rgb_attr.at("background"); }

            try{ current_is_reverse = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("reverse")); }
            catch(std::out_of_range){ current_is_reverse = false; }
            if(current_is_reverse)
                /* swap */
            {
                auto tmp = current_bg;
                current_bg = current_fg;
                current_fg = tmp;
            }
            auto [frf, fgf, fbf] = nvim_html::convert_rgb(boost::get<uInteger>(current_fg));
            auto [frb, fgb, fbb] = nvim_html::convert_rgb(boost::get<uInteger>(current_bg));

            try{ current_is_bold = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("bold")); }
            catch(std::out_of_range){ current_is_bold = false; }

            try{ current_is_italic = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("italic")); }
            catch(std::out_of_range){ current_is_italic = false; }

            try{ current_is_underline = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("underline")); }
            catch(std::out_of_range){ current_is_underline = false; }

            try{ current_is_undercurl = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("undercurl")); }
            catch(std::out_of_range){ current_is_undercurl = false; }

            if(current_is_underline or current_is_undercurl)
            try{
                special_color = boost::get<uInteger>(
                        nvim_hl_attr.at(current_color_id).rgb_attr.at("special"));
            }catch(std::out_of_range){}

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

                    try{ next_fg = nvim_hl_attr.at(next_color_id).rgb_attr.at("foreground"); }
                    catch(std::out_of_range)
                    { next_fg = nvim_hl_attr.at(default_idx).rgb_attr.at("foreground"); }

                    try{ next_bg = nvim_hl_attr.at(next_color_id).rgb_attr.at("background"); }
                    catch(std::out_of_range)
                    { next_bg = nvim_hl_attr.at(default_idx).rgb_attr.at("background"); }

                    try{ next_is_bold = boost::get<bool>(nvim_hl_attr.at(next_color_id).rgb_attr.at("bold")); }
                    catch(std::out_of_range){ next_is_bold = false; }

                    try{ next_is_italic = boost::get<bool>(nvim_hl_attr.at(next_color_id).rgb_attr.at("italic")); }
                    catch(std::out_of_range){ next_is_italic = false; }

                    try{ next_is_underline = boost::get<bool>(nvim_hl_attr.at(next_color_id).rgb_attr.at("underline")); }
                    catch(std::out_of_range){ next_is_underline = false; }

                    try{ next_is_undercurl = boost::get<bool>(nvim_hl_attr.at(next_color_id).rgb_attr.at("undercurl")); }
                    catch(std::out_of_range){ next_is_undercurl = false; }

                    nvim_html::html_escape(color_part);
                    screen.append(QString::fromStdString(color_part));
                    color_part = "";
                    screen.append("</span>");

                    try{ next_is_reverse = boost::get<bool>(nvim_hl_attr.at(next_color_id).rgb_attr.at("reverse")); }
                    catch(std::out_of_range){ next_is_reverse = false; }
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
                        try{
                            special_color = boost::get<uInteger>(
                                    nvim_hl_attr.at(next_color_id).rgb_attr.at("special"));
                        }catch(std::out_of_range){}
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

                bool cursor_positioned =
                    (j == nvim_cursor_x) and (i == nvim_cursor_y)
                    and (current_mode["screen"] == Mode::normal or
                         current_mode["screen"] == Mode::replace);

                if(cursor_positioned)
                {
                    Object next_fg, next_bg;
                    try{ next_fg = nvim_hl_attr.at(next_color_id).rgb_attr.at("foreground"); }
                    catch(std::out_of_range)
                    { next_fg = nvim_hl_attr.at(default_idx).rgb_attr.at("foreground"); }

                    try{ next_bg = nvim_hl_attr.at(next_color_id).rgb_attr.at("background"); }
                    catch(std::out_of_range)
                    { next_bg = nvim_hl_attr.at(default_idx).rgb_attr.at("background"); }

                    auto [rf, gf, bf] = nvim_html::convert_rgb(boost::get<uInteger>(next_fg));
                    auto [rb, gb, bb] = nvim_html::convert_rgb(boost::get<uInteger>(next_bg));
                    nvim_html::html_escape(color_part);
                    screen.append(QString::fromStdString(color_part));
                    color_part = "";
                    if(current_mode["screen"] == Mode::normal)
                    {
                        screen.append(QString(
                                    "<span style=\""
                                    "color: rgb(%1, %2, %3);"
                                    "background-color:rgb(%4, %5, %6);\">"
                                    ).arg(rb).arg(gb).arg(bb).arg(rf).arg(gf).arg(bf));
                    }
                    else if(current_mode["screen"] == Mode::replace)
                    {
                        screen.append(QString(
                                    "<span style=\""
                                    "text-decoration: underline;"
                                    "text-decoration-color:rgb(%1, %2, %3);\">"
                                    ).arg(rf).arg(gf).arg(bf));
                    }
                }
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
        screen.append("</body>");
    }
    else
    {
        std::cerr << "Please Upgrade NeoVim" << std::endl;
        exit(1);
    }

    // std::cout << qPrintable(screen) << std::endl;
    for(auto&& line:nvim_screen)
    {
        line.test();
    }
    setHtml(screen);
    cursor_shape();
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

        case Qt::Key_Backtab:
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

        case Qt::Key_Pause:
            break;

        case Qt::Key_Print:
            break;

        case Qt::Key_SysReq:
            break;

        case Qt::Key_Clear:
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

        case Qt::Key_Shift:
            break;

        case Qt::Key_Control:
            break;

        case Qt::Key_Meta:
            nvim_input("<Esc>");
            break;

        case Qt::Key_Alt:
            break;

        case Qt::Key_AltGr:
            break;

        case Qt::Key_CapsLock:
            break;

        case Qt::Key_NumLock:
            break;

        case Qt::Key_ScrollLock:
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

        case Qt::Key_Space:
            keySend(e, "Space");
            break;

        case Qt::Key_Exclam:
            nvim_input("!");
            break;

        case Qt::Key_QuoteDbl:
            nvim_input("\"");
            break;

        case Qt::Key_NumberSign:
            nvim_input("#");
            break;

        case Qt::Key_Dollar:
            nvim_input("$");
            break;

        case Qt::Key_Percent:
            nvim_input("%");
            break;

        case Qt::Key_Ampersand:
            nvim_input("&");
            break;

        case Qt::Key_Apostrophe:
            nvim_input("'");
            break;

        case Qt::Key_ParenLeft:
            nvim_input("(");
            break;

        case Qt::Key_ParenRight:
            nvim_input(")");
            break;

        case Qt::Key_Asterisk:
            nvim_input("*");
            break;

        case Qt::Key_Plus:
            nvim_input("+");
            break;

        case Qt::Key_Minus:
            nvim_input("-");
            break;

        case Qt::Key_Comma:
            keySend(e, ",");
            break;

        case Qt::Key_Period:
            keySend(e, ".");
            break;

        case Qt::Key_Slash:
            if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-_>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<M-/>"); }
            else { nvim_input("/"); }
            break;

        case Qt::Key_0:
            keySend(e, "0");
            break;

        case Qt::Key_1:
            keySend(e, "1");
            break;

        case Qt::Key_2:
            keySend(e, "2");
            break;

        case Qt::Key_3:
            keySend(e, "3");
            break;

        case Qt::Key_4:
            keySend(e, "4");
            break;

        case Qt::Key_5:
            keySend(e, "5");
            break;

        case Qt::Key_6:
            keySend(e, "6");
            break;

        case Qt::Key_7:
            keySend(e, "7");
            break;

        case Qt::Key_8:
            keySend(e, "8");
            break;

        case Qt::Key_9:
            keySend(e, "9");
            break;

        case Qt::Key_Colon:
            keySend(e, ":");
            break;

        case Qt::Key_Semicolon:
            keySend(e, ";");
            break;

        case Qt::Key_Less:
            keySend(e, "LT");
            break;

        case Qt::Key_Equal:
            keySend(e, "=");
            break;

        case Qt::Key_Greater:
            keySend(e, ">");
            break;

        case Qt::Key_Question:
            keySend(e, "?");
            break;

        case Qt::Key_At:
            keySend(e, "@");
            break;

        case Qt::Key_A:
            keySend(e, "a");
            break;

        case Qt::Key_B:
            keySend(e, "b");
            break;

        case Qt::Key_C:
            keySend(e, "c");
            break;

        case Qt::Key_D:
            keySend(e, "d");
            break;

        case Qt::Key_E:
            keySend(e, "e");
            break;

        case Qt::Key_F:
            keySend(e, "f");
            break;

        case Qt::Key_G:
            keySend(e, "g");
            break;

        case Qt::Key_H:
            keySend(e, "h");
            break;

        case Qt::Key_I:
            keySend(e, "i");
            break;

        case Qt::Key_J:
            keySend(e, "j");
            break;

        case Qt::Key_K:
            keySend(e, "k");
            break;

        case Qt::Key_L:
            keySend(e, "l");
            break;

        case Qt::Key_M:
            keySend(e, "m");
            break;

        case Qt::Key_N:
            keySend(e, "n");
            break;

        case Qt::Key_O:
            keySend(e, "o");
            break;

        case Qt::Key_P:
            keySend(e, "p");
            break;

        case Qt::Key_Q:
            keySend(e, "q");
            break;

        case Qt::Key_R:
            keySend(e, "r");
            break;

        case Qt::Key_S:
            keySend(e, "s");
            break;

        case Qt::Key_T:
            keySend(e, "t");
            break;

        case Qt::Key_U:
            keySend(e, "u");
            break;

        case Qt::Key_V:
            keySend(e, "v");
            break;

        case Qt::Key_W:
            keySend(e, "w");
            break;

        case Qt::Key_X:
            keySend(e, "x");
            break;

        case Qt::Key_Y:
            keySend(e, "y");
            break;

        case Qt::Key_Z:
            keySend(e, "z");
            break;

        case Qt::Key_BracketLeft:
            keySend(e, "[");
            break;

        case Qt::Key_BracketRight:
            keySend(e, "]");
            break;

        case Qt::Key_AsciiCircum:
            keySend(e, "^");
            break;

        case Qt::Key_Backslash:
            keySend(e, "\\");
            break;

        case Qt::Key_Underscore:
            keySend(e, "_");
            break;

        case Qt::Key_QuoteLeft:
            keySend(e, "`");
            break;

        case Qt::Key_BraceLeft:
            keySend(e, "{");
            break;

        case Qt::Key_Bar:
            keySend(e, "Bar");
            break;

        case Qt::Key_AsciiTilde:
            keySend(e, "~");
            break;

        case Qt::Key_BraceRight:
            keySend(e, "}");
            break;

        case Qt::Key_yen:
            keySend(e, "\\");
            break;

        case Qt::Key_ssharp:
            keySend(e, "#");
            break;

        case Qt::Key_division:
            keySend(e, "_");
            break;

        case Qt::Key_Multi_key:
            keySend(e, "*");
            break;

        case Qt::Key_Kanji:
            break;

        case Qt::Key_Muhenkan:
            break;

        case Qt::Key_Henkan:
            break;

        case Qt::Key_Romaji:
            break;

        case Qt::Key_Hiragana:
            break;

        case Qt::Key_Katakana:
            break;

        case Qt::Key_Hiragana_Katakana:
            break;

        case Qt::Key_Zenkaku:
            break;

        case Qt::Key_Hankaku:
            break;

        case Qt::Key_Zenkaku_Hankaku:
            break;

        default:
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
    //
    // if this does not exist, resize redraw will be very slow.
    nvim_input("<F36>");

    QTextEdit::resizeEvent(e);
}

void NeoVim::mousePressEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton)
    {
        keySend(e, "LeftMouse");
    }
    else if(e->button() == Qt::RightButton)
    {
        keySend(e, "RightMouse");
    }
    else if(e->button() == Qt::MiddleButton)
    {
        keySend(e, "MiddleMouse");
    }
    else if(e->button() == Qt::ForwardButton)
    {
    }
    else if(e->button() == Qt::BackButton)
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
            keySend(e, "ScrollWheelUp");
        }
        else if(numDegrees.ry() < 0)
        {
            keySend(e, "ScrollWheelDown");
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

void NeoVim::call_plugin(Object func_and_args)
{
    Array info = boost::get<Array>(func_and_args);
    plugin_queue.push(info);
}

void NeoVim::cursor_shape()
{
    if(current_mode["screen"] == Mode::insert)
    {
        setReadOnly(false); // display textCursor
        setCursorWidth(1);
    }
    else if(current_mode["screen"] == Mode::normal or
            current_mode["screen"] == Mode::replace)
    {
        setReadOnly(true); // not display textCursor
        setCursorWidth(0);
    }
    else
    {
        setReadOnly(true);
        setCursorWidth(0);
    }

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

void NeoVim::keySend(QInputEvent* e, const String& key)
{
    if(e->modifiers() == Qt::NoModifier)
    {
        (key.size() == 1) ? nvim_input(key) : nvim_input("<" + key + ">");
        return;
    }
    String k = key;
    if(e->modifiers() == Qt::ShiftModifier){ k = "S-" + k; }
    if(e->modifiers() == Qt::ControlModifier){ k = "C-" + k; }
    if(e->modifiers() == Qt::AltModifier){ k = "M-" + k; }
    nvim_input("<" + k + ">");
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
}

std::tuple<unsigned long, unsigned long, unsigned long> nvim_html::convert_rgb(unsigned long rgb)
{
    unsigned long r = rgb >> 16;
    unsigned long g = (rgb >> 8) - (r << 8);
    unsigned long b = rgb - (r << 16) - (g << 8);
    return std::make_tuple(r, g, b);
}
