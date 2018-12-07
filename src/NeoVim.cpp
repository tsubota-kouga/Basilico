#include "NeoVim.hpp"

void NeoVim::set_neovim_html()
{
    if(!need_update){ return; }
    QString screen;
    if(is_ext_linegrid)
    {
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
                     .arg(font_size_px).arg(QString::fromStdString(font)));

        screen.append("<body>");
        for(int i = 0;i < nvim_screen.size();i++)
        {
            Integer real_char_num = 0;
            bool pre_double = false;

            constexpr int default_idx = 0;
            Integer current_color_id = nvim_grid_colors_map.at(i).at(0);
            String color_part;

            Object current_fg, current_bg;
            bool current_is_bold, current_is_italic;
            bool next_is_bold, next_is_italic;

            try{ current_fg = nvim_hl_attr.at(current_color_id).rgb_attr.at("foreground"); }
            catch(std::out_of_range)
            { current_fg = nvim_hl_attr.at(default_idx).rgb_attr.at("foreground"); }

            try{ current_bg = nvim_hl_attr.at(current_color_id).rgb_attr.at("background"); }
            catch(std::out_of_range)
            { current_bg = nvim_hl_attr.at(default_idx).rgb_attr.at("background"); }
            auto [frf, fgf, fbf] = nvim_html::convert_rgb(boost::get<uInteger>(current_fg));
            auto [frb, fgb, fbb] = nvim_html::convert_rgb(boost::get<uInteger>(current_bg));

            try{ current_is_bold = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("bold")); }
            catch(std::out_of_range){ current_is_bold = false; }

            try{ current_is_italic = boost::get<bool>(nvim_hl_attr.at(current_color_id).rgb_attr.at("italic")); }
            catch(std::out_of_range){ current_is_italic = false; }

            screen.append(QString(
                        "<font style=\"color:rgb(%1,%2,%3);"
                        "background-color:rgb(%4,%5,%6)\">"
                        ).arg(frf).arg(fgf).arg(fbf).arg(frb).arg(fgb).arg(fbb));
            if(current_is_bold){ screen.append("<b>"); }
            if(current_is_italic){ screen.append("<i>"); }

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

                    nvim_html::html_escape(color_part);
                    screen.append(QString::fromStdString(color_part));
                    color_part = "";
                    screen.append("</font>");
                    auto [rf, gf, bf] = nvim_html::convert_rgb(boost::get<uInteger>(next_fg));
                    auto [rb, gb, bb] = nvim_html::convert_rgb(boost::get<uInteger>(next_bg));
                    screen.append(QString(
                                "<font style=\"color:rgb(%1,%2,%3);"
                                "background-color:rgb(%4,%5,%6)\">"
                                ).arg(rf).arg(gf).arg(bf).arg(rb).arg(gb).arg(bb));
                    if(next_is_bold){ screen.append("<b>"); }
                    if(next_is_italic){ screen.append("<i>"); }

                }
                constexpr short BIN1x1 = 0b10000000;
                constexpr short BIN1x2 = 0b11000000;
                constexpr short BIN1x3 = 0b11100000;
                const char c = nvim_screen.at(i).at(j);

//TODO fix bug
                // // c == 0b110xxxxx: 'あ'
                // if((c & BIN1x3) == BIN1x2)
                // {
                //     color_part += nvim_screen.at(i).at(j);
                //     j++;
                //     color_part += nvim_screen.at(i).at(j);
                //     j++;
                //     color_part += nvim_screen.at(i).at(j);
                // }
                // // c == 0b10xxxxxx: '¦'
                // else if((c & BIN1x2) == BIN1x1)
                // {
                //     color_part += nvim_screen.at(i).at(j);
                //     j++;
                //     color_part += nvim_screen.at(i).at(j);
                // }
                // // c == 0b0xxxxxxx: 'a'
                // else if((c & BIN1x1) == 0)
                // {
                //     color_part += nvim_screen.at(i).at(j);
                // }
                // // c == 0b111xxxxx
                // else {}
                // if(nvim_size_x <= real_char_num){ break; }

                color_part += c;
                // if((c & BIN1x2) == BIN1x1 and !pre_double) // c == 0b10xxxxxx: '¦'
                // { pre_double = true;#<{(| real_char_num++; |)}># }
                // else if((c & BIN1x2) == BIN1x2 and !pre_double) // c == 0b11xxxxxx: 'あ'
                // { pre_double = true;#<{(| real_char_num++; |)}># }
                // else if(!pre_double){ #<{(| real_char_num++; |)}># }
                // else { pre_double = false; }
                // // if(nvim_size_x <= real_char_num){ break; }

                current_color_id = next_color_id;
                current_is_bold = next_is_bold;
                current_is_italic = next_is_italic;
            }

            nvim_html::html_escape(color_part);
            screen.append(QString::fromStdString(color_part));
            if(i != nvim_screen.size() - 1)screen.append("\n");
            if(!next_is_bold){ screen.append("</b>"); }
            if(!next_is_italic){ screen.append("</i>"); }
            screen.append("</font>");
        }
        screen.append("</body>");
    }
    else
    {
        std::cout << "Please Upgrade NeoVim" << std::endl;
        exit(1);
    }
    // std::cout << qPrintable(screen) << std::endl;
    setHtml(screen);

    cursor_shape();
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
        set_neovim_html();
    }
}

void NeoVim::inputMethodEvent(QInputMethodEvent* e)
{
    QWidget::inputMethodEvent(e);
    nvim_input(e->commitString().toUtf8().constData());
}

QVariant NeoVim::inputMethodQuery(Qt::InputMethodQuery q) const
{
    return QTextEdit::inputMethodQuery(q);
}

void NeoVim::resizeEvent(QResizeEvent* e)
{
    int row = windowWidth2Width(width());
    int col = windowHeight2Height(height());
    nvim_ui_try_resize(row, col);
    QTextEdit::resizeEvent(e);
}

void NeoVim::call_plugin(Object func_and_args)
{
    Array info = boost::get<Array>(func_and_args);
    plugin_queue.push(info);
}

void NeoVim::cursor_shape()
{
    if(current_mode["screen"] == static_cast<Integer>(Mode::insert))
    {
        setCursorWidth(1);
    }
    else
    {
        setCursorWidth(font_size_px/2);
    }
    QTextCursor cursor(textCursor());
    cursor.setPosition(0);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, nvim_image_cursor_x);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, nvim_image_cursor_y);
    setTextCursor(cursor);
    need_update = false;
    return;
}


void NeoVim::fkeySend(QKeyEvent* e, Integer key)
{
    if(e->modifiers() == Qt::ShiftModifier){ key += 12; }
    if(e->modifiers() == Qt::ControlModifier){ key += 24; }
    nvim_input("<F" + std::to_string(key) + ">");
}

void NeoVim::keySend(QKeyEvent* e, const String& key)
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

std::tuple<long, long, long> nvim_html::convert_rgb(long rgb)
{
    long r = rgb >> 16;
    long g = (rgb >> 8) - (r << 8);
    long b = rgb - (r << 16) - (g << 8);
    return std::make_tuple(r, g, b);
}
