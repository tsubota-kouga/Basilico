#include "PakuVim.hpp"

void PakuVim::set_neovim_html()
{
    if(!need_update){ return; }
    QString screen;
    if(is_ext_newgrid)
    {
        auto [drf, dgf, dbf] = nvim_html::convert_rgb(
                boost::get<uInteger>(nvim_hl_attr.at(0).rgb_attr.at("foreground")));
        auto [drb, dgb, dbb] = nvim_html::convert_rgb(
                boost::get<uInteger>(nvim_hl_attr.at(0).rgb_attr.at("background")));
        screen.append(QString(
                    "<style>"
                    "body {"
                    "width: 100%;"
                    "color: rgb(%1,%2,%3);"
                    "background-color: rgb(%4,%5,%6);"
                    "font-size: 16px;"
                    "font-family: \"ubuntu mono\";"
                    "white-space: pre;}"
                    "</style>"
                    ).arg(drf).arg(dgf).arg(dbf).arg(drb).arg(dgb).arg(dbb));

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
                const char c = nvim_screen.at(i).at(j);
                color_part += c;
                constexpr short BIN1x2 = 0b11000000;
                constexpr short BIN1x3 = 0b11100000;
                if((c & BIN1x2) == BIN1x2 and (c & BIN1x3) != BIN1x3 and !pre_double)
                { pre_double = true;real_char_num++; }
                else if(!pre_double){ real_char_num++; }
                else { pre_double = false; }
                if(nvim_size_x <= real_char_num){ break; }

                current_color_id = next_color_id;
                current_is_bold = next_is_bold;
                current_is_italic = next_is_italic;
            }

            nvim_html::html_escape(color_part);
            screen.append(QString::fromStdString(color_part));
            if(i != nvim_screen.size() - 1)screen.append("<br>");
            if(!next_is_bold){ screen.append("</b>"); }
            if(!next_is_italic){ screen.append("</i>"); }
            screen.append("</font>");
        }
        screen.append("</body>");
    }
    else
    {
        auto [drb, dgb, dbb] = nvim_html::convert_rgb(std::get<1>(nvim_default_colors_set));

        screen.append(QString(
                    "<style>"
                    "body {"
                    "width: 100%;"
                    "background-color: rgb(%1,%2,%3);"
                    "font-size: 16px;"
                    "font-family: \"ubuntu mono\";"
                    "white-space: pre;}"
                    "</style>"
                    ).arg(drb).arg(dgb).arg(dbb));
        screen.append("<body>");
        for(int i = 0;i < nvim_screen.size();i++)
        {
            auto line = nvim_screen.at(i);
            auto line_color = nvim_colors_map.at(i);
            int drawn = -1;
            for(const auto& part: line_color._colors_map_list)
            {
                auto& [from, to] = part.range;
                auto& [rf, gf, bf] = part.get_r_g_b_fg();
                auto& [rb, gb, bb] = part.get_r_g_b_bg();
                auto& [each_of_drb, each_of_dgb, each_of_dbb] = part.get_default_r_g_b_bg();
                auto s_part = line.substr(from, to - from + 1);
                if(drawn < from)
                {
                    screen.append(QString(
                                "<font style=\"background-color:rgb(%1,%2,%3)\">"
                                ).arg(each_of_drb).arg(each_of_dgb).arg(each_of_dbb));
                    for(int j = drawn + 1;j < from;j++)
                    {
                        screen.append(' ');
                    }
                    screen.append("</font>");
                }
                screen.append(QString(
                            "<font style=\"color: rgb(%1,%2,%3);background-color: rgb(%4,%5,%6)\">"
                            ).arg(rf).arg(gf).arg(bf).arg(rb).arg(gb).arg(bb));

                if(part.is_bold()){ screen.append("<b>"); }
                if(part.is_italic()){ screen.append("<i>"); }

                nvim_html::html_escape(s_part);
                screen.append(QString::fromStdString(s_part));

                if(part.is_italic()){ screen.append("</i>"); }
                if(part.is_bold()){ screen.append("</b>"); }
                screen.append("</font>");
                drawn = to;
            }
            if(i < nvim_screen.size() - 1)screen.append("<br>");
        }
        screen.append("</body>");
    }
    setHtml(screen);
    // cursor move
    if(current_mode["screen"] == static_cast<Integer>(Mode::insert))
    {
        setCursorWidth(1);
    }
    else
    {
        setCursorWidth(8);
    }
    QTextCursor cursor(textCursor());
    cursor.setPosition(0);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, nvim_image_cursor_x);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, nvim_image_cursor_y);
    setTextCursor(cursor);
    need_update = false;
    return;
}

void PakuVim::keyPressEvent(QKeyEvent* e)
{
    std::cout << e->key() << std::endl;

    switch(e->key())
    {
        case Qt::Key_Escape:
            nvim_input("<Esc>");
            break;

        case Qt::Key_Tab:
            nvim_input("<Tab>");
            break;

        case Qt::Key_Backtab:
            nvim_input("<S-Tab>");
            break;

        case Qt::Key_Backspace:
            nvim_input("<BS>");
            break;

        case Qt::Key_Return:
            nvim_input("<CR>");
            break;

        case Qt::Key_Enter:
            nvim_input("<CR>");
            break;

        case Qt::Key_Insert:
            nvim_input("<Insert>");
            break;

        case Qt::Key_Delete:
            nvim_input("<Del>");
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
            nvim_input("<Home>");
            break;

        case Qt::Key_End:
            nvim_input("<End>");
            break;

        case Qt::Key_Left:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<S-Left>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-Left>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-Left>"); }
            else { nvim_input("<Left>"); }
            break;

        case Qt::Key_Right:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<S-Right>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-Right>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-Right>"); }
            else { nvim_input("<Right>"); }
            break;

        case Qt::Key_Up:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<S-Up>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-Up>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-Up>"); }
            else { nvim_input("<Up>"); }
            break;

        case Qt::Key_Down:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<S-Down>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-Down>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-Down>"); }
            else { nvim_input("<Down>"); }
            break;

        case Qt::Key_PageUp:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<S-PageUp>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-PageUp>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-PageUp>"); }
            else { nvim_input("<PageUp>"); }
            break;

        case Qt::Key_PageDown:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<S-PageDown>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-PageDown>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-PageDown>"); }
            else { nvim_input("<PageDown>"); }
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
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F13>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F25>"); }
            else { nvim_input("<F1>"); }
            break;

        case Qt::Key_F2:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F14>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F26>"); }
            else { nvim_input("<F2>"); }
            break;

        case Qt::Key_F3:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F15>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F27>"); }
            else { nvim_input("<F3>"); }
            break;

        case Qt::Key_F4:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F16>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F28>"); }
            else { nvim_input("<F4>"); }
            break;

        case Qt::Key_F5:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F17>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F29>"); }
            else { nvim_input("<F5>"); }
            break;

        case Qt::Key_F6:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F18>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F30>"); }
            else { nvim_input("<F6>"); }
            break;

        case Qt::Key_F7:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F19>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F31>"); }
            else { nvim_input("<F7>"); }
            break;

        case Qt::Key_F8:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F20>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F32>"); }
            else { nvim_input("<F8>"); }
            break;

        case Qt::Key_F9:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F21>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F33>"); }
            else { nvim_input("<F9>"); }
            break;

        case Qt::Key_F10:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F22>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F34>"); }
            else { nvim_input("<F10>"); }
            break;

        case Qt::Key_F11:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F23>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F35>"); }
            else { nvim_input("<F11>"); }
            break;

        case Qt::Key_F12:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("<F24>"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<F36>"); }
            else { nvim_input("<F12>"); }
            break;

        case Qt::Key_Super_L:
            break;

        case Qt::Key_Super_R:
            break;

        case Qt::Key_Space:
            nvim_input("<Space>");
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
            nvim_input("<Plus>");
            break;

        case Qt::Key_Minus:
            nvim_input("<Minus>");
            break;

        case Qt::Key_Comma:
            nvim_input(",");
            break;

        case Qt::Key_Period:
            nvim_input(".");
            break;

        case Qt::Key_Slash:
            nvim_input("/");
            break;

        case Qt::Key_0:
            nvim_input("0");
            break;

        case Qt::Key_1:
            nvim_input("1");
            break;

        case Qt::Key_2:
            nvim_input("2");
            break;

        case Qt::Key_3:
            nvim_input("3");
            break;

        case Qt::Key_4:
            nvim_input("4");
            break;

        case Qt::Key_5:
            nvim_input("5");
            break;

        case Qt::Key_6:
            nvim_input("6");
            break;

        case Qt::Key_7:
            nvim_input("7");
            break;

        case Qt::Key_8:
            nvim_input("8");
            break;

        case Qt::Key_9:
            nvim_input("9");
            break;

        case Qt::Key_Colon:
            nvim_input(":");
            break;

        case Qt::Key_Semicolon:
            nvim_input(";");
            break;

        case Qt::Key_Less:
            nvim_input("<LT>");
            break;

        case Qt::Key_Equal:
            nvim_input("=");
            break;

        case Qt::Key_Greater:
            nvim_input(">");
            break;

        case Qt::Key_Question:
            nvim_input("?");
            break;

        case Qt::Key_At:
            nvim_input("@");
            break;

        case Qt::Key_A:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("A"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-a>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-a>"); }
            else { nvim_input("a"); }
            break;

        case Qt::Key_B:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("B"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-b>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-b>"); }
            else { nvim_input("b"); }
            break;

        case Qt::Key_C:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("C"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-c>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-c>"); }
            else { nvim_input("c"); }
            break;

        case Qt::Key_D:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("D"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-d>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-d>"); }
            else { nvim_input("d"); }
            break;

        case Qt::Key_E:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("E"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-e>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-e>"); }
            else { nvim_input("e"); }
            break;

        case Qt::Key_F:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("F"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-f>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-f>"); }
            else { nvim_input("f"); }
            break;

        case Qt::Key_G:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("G"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-g>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-g>"); }
            else { nvim_input("g"); }
            break;

        case Qt::Key_H:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("H"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-h>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-h>"); }
            else { nvim_input("h"); }
            break;

        case Qt::Key_I:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("I"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-i>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-i>"); }
            else { nvim_input("i"); }
            break;

        case Qt::Key_J:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("J"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-j>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-j>"); }
            else { nvim_input("j"); }
            break;

        case Qt::Key_K:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("K"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-k>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-k>"); }
            else { nvim_input("k"); }
            break;

        case Qt::Key_L:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("L"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-l>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-l>"); }
            else { nvim_input("l"); }
            break;

        case Qt::Key_M:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("M"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-m>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-m>"); }
            else { nvim_input("m"); }
            break;

        case Qt::Key_N:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("N"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-n>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-n>"); }
            else { nvim_input("n"); }
            break;

        case Qt::Key_O:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("O"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-o>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-o>"); }
            else { nvim_input("o"); }
            break;

        case Qt::Key_P:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("P"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-p>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-p>"); }
            else { nvim_input("p"); }
            break;

        case Qt::Key_Q:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("Q"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-q>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-q>"); }
            else { nvim_input("q"); }
            break;

        case Qt::Key_R:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("R"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-r>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-r>"); }
            else { nvim_input("r"); }
            break;

        case Qt::Key_S:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("S"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-s>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-s>"); }
            else { nvim_input("s"); }
            break;

        case Qt::Key_T:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("T"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-t>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-t>"); }
            else { nvim_input("t"); }
            break;

        case Qt::Key_U:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("U"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-u>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-u>"); }
            else { nvim_input("u"); }
            break;

        case Qt::Key_V:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("V"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-v>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-v>"); }
            else { nvim_input("v"); }
            break;

        case Qt::Key_W:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("W"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-w>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-w>"); }
            else { nvim_input("w"); }
            break;

        case Qt::Key_X:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("X"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-x>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-x>"); }
            else { nvim_input("x"); }
            break;

        case Qt::Key_Y:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("Y"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-y>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-y>"); }
            else { nvim_input("y"); }
            break;

        case Qt::Key_Z:
            if(e->modifiers() == Qt::ShiftModifier){ nvim_input("Z"); }
            else if(e->modifiers() == Qt::ControlModifier){ nvim_input("<C-z>"); }
            else if(e->modifiers() == Qt::AltModifier){ nvim_input("<A-z>"); }
            else { nvim_input("z"); }
            break;

        case Qt::Key_BracketLeft:
            nvim_input("[");
            break;

        case Qt::Key_BracketRight:
            nvim_input("]");
            break;

        case Qt::Key_Backslash:
            nvim_input("\\");
            break;

        case Qt::Key_Underscore:
            nvim_input("_");
            break;

        case Qt::Key_QuoteLeft:
            nvim_input("'");
            break;

        case Qt::Key_BraceLeft:
            nvim_input("{");
            break;

        case Qt::Key_Bar:
            nvim_input("<bar>");
            break;

        case Qt::Key_AsciiTilde:
            nvim_input("~");
            break;

        case Qt::Key_BraceRight:
            nvim_input("}");
            break;

        case Qt::Key_yen:
            nvim_input("\\");
            break;

        case Qt::Key_ssharp:
            nvim_input("#");
            break;

        case Qt::Key_division:
            nvim_input("/");
            break;

        case Qt::Key_Multi_key:
            nvim_input("*");
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

void PakuVim::timerEvent(QTimerEvent* e)
{
    if(e->timerId() == timer)
    {
        draw();
        set_neovim_html();
    }
}

void nvim_html::html_escape(std::string& s)
{
    std::unordered_map<std::string, std::string> special_char
    {
        {"<", "&lt;"},
        {">", "&gt;"},
        {"¦", "&brvbar;"},
        {" ", "&nbsp;"},
        // {"\"", "&quot;"},
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
