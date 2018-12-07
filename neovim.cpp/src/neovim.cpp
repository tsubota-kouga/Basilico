#include "neovim.hpp"

static constexpr bool debug = true;

neovim::neovim(uint width, uint height, const Dictionary& options)
        :nvim_size_x{width},nvim_size_y{height},
        nvim_cursor_x{0},nvim_cursor_y{0}
{
    for(const auto& [key, val]: options)
    {
        ui_options[boost::get<String>(key)] = val;
    }

    try{ is_ext_linegrid = boost::get<bool>(ui_options.at("ext_linegrid")); }
    catch(std::out_of_range){ is_ext_linegrid = false; }

    nvim_screen.resize(height);
    for(auto& line: nvim_screen){ line.resize(width*2, ' '); }

    nvim_colors_map.resize(height);
    for(auto& line: nvim_colors_map){ line.set_default(width/2); }

    constexpr int num_colors = 400;
    nvim_hl_attr.resize(num_colors);

    nvim_grid_colors_map.resize(height);
    for(auto& line: nvim_grid_colors_map){ line.resize(width*2, 0); }

    need_update = true;
}

void neovim::connect_tcp(const String& host, const String& service, double timeout_sec)
{
    port = service;
    client_.connect_tcp(host, service, timeout_sec);
    ui_client_.connect_tcp(host, service, timeout_sec);
}

const Integer neovim::colors_map::_colors_map::get_rgb_fg() const
{
    try{ return boost::get<uInteger>(hl_map.at("foreground")); }
    catch(std::out_of_range){ return std::get<0>(colors_set); }
}
const Integer neovim::colors_map::_colors_map::get_rgb_bg() const
{
    try{ return boost::get<uInteger>(hl_map.at("background")); }
    catch(std::out_of_range){ return std::get<1>(colors_set); }
}
const tuple<int, int, int> neovim::colors_map::_colors_map::get_r_g_b_fg() const
{
    auto rgb_fg = get_rgb_fg();
    int rf = rgb_fg >> 16;
    int gf = (rgb_fg >> 8) - (rf << 8);
    int bf = rgb_fg - (rf << 16) - (gf << 8);
    return make_tuple(rf, gf, bf);
}
const tuple<int, int, int> neovim::colors_map::_colors_map::get_r_g_b_bg() const
{
    auto rgb_bg = get_rgb_bg();
    int rb = rgb_bg >> 16;
    int gb = (rgb_bg >> 8) - (rb << 8);
    int bb = rgb_bg - (rb << 16) - (gb << 8);
    return make_tuple(rb, gb, bb);
}
const tuple<int, int, int> neovim::colors_map::_colors_map::get_default_r_g_b_fg() const
{
    auto rgb_fg = std::get<0>(colors_set);
    int rf = rgb_fg >> 16;
    int gf = (rgb_fg >> 8) - (rf << 8);
    int bf = rgb_fg - (rf << 16) - (gf << 8);
    return make_tuple(rf, gf, bf);
}
const tuple<int, int, int> neovim::colors_map::_colors_map::get_default_r_g_b_bg() const
{
    auto rgb_bg = std::get<1>(colors_set);
    int rb = rgb_bg >> 16;
    int gb = (rgb_bg >> 8) - (rb << 8);
    int bb = rgb_bg - (rb << 16) - (gb << 8);
    return make_tuple(rb, gb, bb);
}
const Integer& neovim::colors_map::_colors_map::get_rgb_sp() const
{
    return std::get<1>(colors_set);
}
const Integer& neovim::colors_map::_colors_map::get_cterm_fg() const
{
    return std::get<3>(colors_set);
}
const Integer& neovim::colors_map::_colors_map::get_cterm_bg() const
{
    return std::get<4>(colors_set);
}
const pair<Integer, Integer>& neovim::colors_map::_colors_map::get_range() const
{
    return range;
}

void neovim::colors_map::_colors_map::set_rgb_fg(Integer rgb_fg)
{
    std::get<0>(colors_set) = rgb_fg;
}
void neovim::colors_map::_colors_map::set_rgb_bg(Integer rgb_bg)
{
    std::get<1>(colors_set) = rgb_bg;
}
void neovim::colors_map::_colors_map::set_rgb_sp(Integer rgb_sp)
{
    std::get<2>(colors_set) = rgb_sp;
}
void neovim::colors_map::_colors_map::set_cterm_fg(Integer cterm_fg)
{
    std::get<3>(colors_set) = cterm_fg;
}
void neovim::colors_map::_colors_map::set_cterm_bg(Integer cterm_bg)
{
    std::get<4>(colors_set) = cterm_bg;
}
void neovim::colors_map::_colors_map::set_range(Integer from, Integer to)
{
    range = std::make_pair(from, to);
}
void neovim::colors_map::_colors_map::set_range(pair<Integer, Integer> range_)
{
    range = range_;
}
bool neovim::colors_map::_colors_map::is_bold() const
{
    try{ return boost::get<bool>(hl_map.at("bold")); }
    catch(std::out_of_range){ return false; }
}
bool neovim::colors_map::_colors_map::is_italic() const
{
    try{ return boost::get<bool>(hl_map.at("italic")); }
    catch(std::out_of_range){ return false; }
}
bool neovim::colors_map::_colors_map::operator==(_colors_map c)
{
    return colors_set == c.colors_set and hl_map == c.hl_map;
}


void neovim::colors_map::set_default(Integer x)
{
    _colors_map_list.reserve(x);
}

void neovim::colors_map::overwrite(const _colors_map&& c_map)
{
    bool sw = true;
    const auto& [from, to] = c_map.range;
    if(from > to){ return; }
    Vector<_colors_map> tmp_c_map;
    for(auto& x:_colors_map_list)
    {
        auto& [x_, _x] = x.range;

        // x_ <= from <= _x <= to
        if(x_ <= from and (from <= _x and _x <= to))
        {
            _x = from - 1;
            sw = true;
        }
        // x_ < from <= to < _x
        else if(x_ < from and to < _x)
        {
            tmp_c_map.push_back(_colors_map(x.colors_set, x.hl_map, make_pair(to + 1, _x)));
            _x = from - 1;
        }
        // from <= x_ <= to <= _x
        else if((from <= x_ and x_ <= to) and to <= _x)
        {
            x_ = to + 1;
            sw = false;
        }
        // from <= x_ <= _x <= to
        else if(x_ <= from and to <= _x)
        {
            x.range = make_pair(-1, -1);
        }
        else if(sw and (from <= x_ and _x <= to))
        {
            x.range = make_pair(-1, -1);
        }

        if(x_ > _x)
        {
            x.range = make_pair(-1, -1);
        }
    }
    _colors_map_list.push_back(c_map);
    _colors_map_list.insert(_colors_map_list.cbegin(), tmp_c_map.cbegin(), tmp_c_map.cend());

    sort(_colors_map_list.begin(), _colors_map_list.end(),
            [](const _colors_map& p, const _colors_map& q)
            {
            if(p.range.first < 0) { return false; }
            else if(q.range.first < 0) { return true; }
            else { return p.range.first < q.range.first; }
            });
    for(auto it = _colors_map_list.crbegin();it != _colors_map_list.crend();it++)
    {
        if(it->range.first != -1)
        {
            break;
        }
        _colors_map_list.pop_back();
    }
}

void neovim::colors_map::print()
{
    for(auto x:_colors_map_list)
    {
        const auto& [x_, _x] = x.range;
        std::cout << "[" << x_ << "," << _x << "]";
    }
}


void neovim::redraw(Object ui_info)
{
    Integer origin_cursor_x = nvim_cursor_x;
    Integer origin_cursor_y = nvim_cursor_y;
    Integer addtional_x = 0;
    Array dst_ary;

    try
    {
        dst_ary = boost::get<Array>(ui_info);
    }
    catch(boost::bad_get& e)
    {
        std::cout << e.what() << std::endl;
        return;
    }

    for(Object obj:dst_ary)
    {
        Array func;
        String func_name;
        try
        {
            func = boost::get<Array>(obj);
            func_name = boost::get<String>(func.at(0));
            if constexpr(debug)std::cout << func_name << std::endl;
        }
        catch(boost::bad_get& e)
        {
            std::cout << e.what() << std::endl;
            return;
        }

        if(func_name == "resize")
        {
            Array size = boost::get<Array>(func.at(1));

            Integer x = boost::get<uInteger>(size.at(0));

            Integer y = boost::get<uInteger>(size.at(1));

            resize(x, y);
            if constexpr(debug)std::cout << "call resize(" << x << "," << y << ")" << std::endl;
        }

        else if(func_name == "clear")
        {
            clear();
            if constexpr(debug)std::cout << "call clear()" << std::endl;
        }

        else if(func_name == "eol_clear")
        {
            eol_clear();
            if constexpr(debug)std::cout << "call eol_clear()" << std::endl;
        }

        else if(func_name == "cursor_goto")
        {
            Array size = boost::get<Array>(func.at(1));

            Integer x = boost::get<uInteger>(size.at(1));

            Integer y = boost::get<uInteger>(size.at(0));

            cursor_goto(x, y);
            if constexpr(debug)std::cout << "cursor_goto(" << x << "," << y << ")" << std::endl;
        }

        else if(func_name == "mode_info_set")
        {
            Array X = boost::get<Array>(obj);

            bool enabled;

            cArray cursor_styles;

            mode_info_set(enabled, cursor_styles);
            if constexpr(debug)std::cout << "call mode_info_set(" << enabled << ",cursor_styles" << ")" << std::endl;
        }

        else if(func_name == "update_menu")
        {
            update_menu();
            if constexpr(debug)std::cout << "call update_menu()" << std::endl;
        }

        else if(func_name == "busy_start")
        {
            busy_start();
            if constexpr(debug)std::cout << "call busy_start()" << std::endl;
        }

        else if(func_name == "busy_stop")
        {
            busy_stop();
            if constexpr(debug)std::cout << "call busy_stop()" << std::endl;
        }

        else if(func_name == "mouse_on")
        {
            mouse_on();
            if constexpr(debug)std::cout << "call mouse_on()" << std::endl;
        }

        else if(func_name == "mouse_off")
        {
            mouse_off();
            if constexpr(debug)std::cout << "call mouse_off()" << std::endl;
        }

        else if(func_name == "mode_change")
        {
            for(uint i = 1;i < func.size();i++)
            {
                auto mode_array = boost::get<Array>(func.at(i));
                auto mode = boost::get<String>(mode_array.at(0));
                auto mode_idx = boost::get<uInteger>(mode_array.at(1));
                mode_change(mode, mode_idx);
            }
            if constexpr(debug)std::cout << "call mode_change()" << std::endl;
        }

        else if(func_name == "set_scroll_region")
        {
            Array X = boost::get<Array>(obj);

            Integer top;

            Integer bot;

            Integer left;

            Integer right;

            set_scroll_region(top, bot, left, right);
            if constexpr(debug)std::cout << "call set_scroll_region(" << top << "," << bot << "," << left << "," << right << ")" << std::endl;
        }

        else if(func_name == "scroll")
        {
            Array X = boost::get<Array>(obj);

            Integer count;

            scroll(count);
            if constexpr(debug)std::cout << "call scroll(count)" << std::endl;
        }

        else if(func_name == "highlight_set")
        {
            Array hlight_dic = boost::get<Array>(func.at(1));

            Dictionary attrs = boost::get<Dictionary>(hlight_dic.at(0));

            highlight_set(attrs);
            if constexpr(debug)std::cout << "call highlight_set(attrs)" << std::endl;
        }

        else if(func_name == "put")
        {
            String str{};
            str.reserve(nvim_size_x);

            for(int i = 1;i < func.size();i++)
            {
                try
                {
                    Array ss = boost::get<Array>(func.at(i));
                    for(auto& x: ss)
                    {
                        str += boost::get<String>(x);
                    }
                }
                catch(boost::bad_get& e)
                {
                    std::cout << e.what() << std::endl;
                    break;
                }
            }

            if constexpr(debug)std::cout << "put" << str << std::endl;
            put(str);
        }

        else if(func_name == "bell")
        {
            bell();
            if constexpr(debug)std::cout << "call bell()" << std::endl;
        }

        else if(func_name == "visual_bell")
        {
            visual_bell();
            if constexpr(debug)std::cout << "call visual_bell()" << std::endl;
        }

        else if(func_name == "flush")
        {
            flush();
            if constexpr(debug)std::cout << "call flush()" << std::endl;
        }

        else if(func_name == "update_fg")
        {
            Array color = boost::get<Array>(func.at(1));

            Integer fg = boost::get<uInteger>(color.at(0));

            update_fg(fg);
            if constexpr(debug)std::cout << "call update_fg(" << fg << ")" << std::endl;
        }

        else if(func_name == "update_bg")
        {
            Array color = boost::get<Array>(func.at(1));

            Integer bg = boost::get<uInteger>(color.at(0));

            update_bg(bg);
            if constexpr(debug)std::cout << "call update_bg(" << bg << ")" << std::endl;
        }

        else if(func_name == "update_sp")
        {
            Array color = boost::get<Array>(func.at(1));

            Integer sp = boost::get<Integer>(color.at(0));

            update_sp(sp);
            if constexpr(debug)std::cout << "call update_sp(" << sp << ")" << std::endl;
        }

        else if(func_name == "default_colors_set")
        {
            Array colors = boost::get<Array>(func.at(1));

            Integer rgb_fg = boost::get<uInteger>(colors.at(0));

            Integer rgb_bg = boost::get<uInteger>(colors.at(1));

            Integer rgb_sp = boost::get<Integer>(colors.at(2));

            Integer cterm_fg = boost::get<uInteger>(colors.at(3));

            Integer cterm_bg = boost::get<uInteger>(colors.at(4));

            default_colors_set(rgb_fg, rgb_bg, rgb_sp, cterm_fg, cterm_bg);
            if constexpr(debug)std::cout << "call default_colors_set()" << std::endl;
        }

        else if(func_name == "suspend")
        {
            suspend();
            if constexpr(debug)std::cout << "call suspend()" << std::endl;
        }

        else if(func_name == "set_title")
        {
            Array X = boost::get<Array>(obj);

            String title;

            set_title(title);
            if constexpr(debug)std::cout << "call set_title()" << std::endl;
        }

        else if(func_name == "set_icon")
        {
            Array X = boost::get<Array>(obj);

            String icon;

            set_icon(icon);
            if constexpr(debug)std::cout << "call set_icon()" << std::endl;
        }

        else if(func_name == "option_set")
        {
            Array& opts = func;

            unordered_map<String, Object> opt;

            Array opt_pair;//[String, Object]

            for(int i = 1;i < opts.size();i++)
            {
                opt_pair = boost::get<Array>(opts.at(i));
                opt[boost::get<String>(opt_pair.at(0))] = opt_pair.at(1);
            }
            option_set(opt);
            if constexpr(debug)std::cout << "call option_set()" << std::endl;
        }

        else if(func_name == "popupmenu_show")
        {
            Array X = boost::get<Array>(obj);

            cArray items;

            Integer selected;

            Integer row;

            Integer col;

            popupmenu_show(items, selected, row, col);
            if constexpr(debug)std::cout << "call popupmenu_show()" << std::endl;
        }

        else if(func_name == "popupmenu_hide")
        {
            popupmenu_hide();
            if constexpr(debug)std::cout << "call popupmenu_hide()" << std::endl;
        }

        else if(func_name == "popupmenu_select")
        {
            Array X = boost::get<Array>(obj);

            Integer selected;

            popupmenu_select(selected);
            if constexpr(debug)std::cout << "call popupmenu_select()" << std::endl;
        }

        else if(func_name == "tabline_update")
        {
            Array X = boost::get<Array>(obj);

            Tabpage current;

            cArray tabs;

            tabline_update(current, tabs);
            if constexpr(debug)std::cout << "call tabline_update()" << std::endl;
        }

        else if(func_name == "cmdline_show")
        {
            Array X = boost::get<Array>(obj);

            cArray content;

            Integer pos;

            String firstc;

            String prompt;

            Integer indent;

            Integer level;

            cmdline_show(content, pos, firstc, prompt, indent, level);
            if constexpr(debug)std::cout << "call cmdline_show()" << std::endl;
        }

        else if(func_name == "cmdline_pos")
        {
            Array X = boost::get<Array>(obj);

            Integer pos;

            Integer level;

            cmdline_pos(pos, level);
            if constexpr(debug)std::cout << "call cmdline_pos()" << std::endl;
        }

        else if(func_name == "cmdline_special_char")
        {
            Array X = boost::get<Array>(obj);

            String c;

            bool shift;

            Integer level;

            cmdline_special_char(c, shift, level);
            if constexpr(debug)std::cout << "call cmdline_special_char()" << std::endl;
        }

        else if(func_name == "cmdline_hide")
        {
            Array X = boost::get<Array>(obj);

            Integer level;

            cmdline_hide(level);
            if constexpr(debug)std::cout << "call cmdline_hide()" << std::endl;
        }

        else if(func_name == "cmdline_block_show")
        {
            Array X = boost::get<Array>(obj);

            cArray lines;

            cmdline_block_show(lines);
            if constexpr(debug)std::cout << "call cmdline_block_show()" << std::endl;
        }

        else if(func_name == "cmdline_block_append")
        {
            Array X = boost::get<Array>(obj);

            cArray lines;

            cmdline_block_append(lines);
            if constexpr(debug)std::cout << "call cmdline_block_append()" << std::endl;
        }

        else if(func_name == "cmdline_block_hide")
        {
            cmdline_block_hide();
            if constexpr(debug)std::cout << "call cmdline_block_hide()" << std::endl;
        }

        else if(func_name == "wildmenu_show")
        {
            Array X = boost::get<Array>(obj);

            cArray items;

            wildmenu_show(items);
            if constexpr(debug)std::cout << "call wildmenu_show()" << std::endl;
        }

        else if(func_name == "wildmenu_select")
        {
            Array X = boost::get<Array>(obj);

            Integer selected;

            wildmenu_select(selected);
            if constexpr(debug)std::cout << "call wildmenu_select()" << std::endl;
        }

        else if(func_name == "wildmenu_hide")
        {
            wildmenu_hide();
            if constexpr(debug)std::cout << "call wildmenu_hide()" << std::endl;
        }

        else if(func_name == "hl_attr_define")
        {
            for(uint i = 1;i < func.size();i++)
            {
                Array arg = boost::get<Array>(func.at(i));

                Integer id = boost::get<uInteger>(arg.at(0));

                Dictionary rgb_attr = boost::get<Dictionary>(arg.at(1));

                Dictionary cterm_attr = boost::get<Dictionary>(arg.at(2));

                Array info = boost::get<Array>(arg.at(3));

                hl_attr_define(id, rgb_attr, cterm_attr, info);
            }
            if constexpr(debug)std::cout << "call hl_attr_define()" << std::endl;
        }

        else if(func_name == "grid_line")
        {
            for(uint i = 1;i < func.size();i++)
            {
                Array arg = boost::get<Array>(func.at(i));

                Integer grid = boost::get<uInteger>(arg.at(0));

                Integer row = boost::get<uInteger>(arg.at(1));

                Integer col_start = boost::get<uInteger>(arg.at(2));

                Array cells = boost::get<Array>(arg.at(3));

                grid_line(grid, row, col_start, cells);
            }
            if constexpr(debug)std::cout << "call grid_line()" << std::endl;
        }

        else if(func_name == "grid_clear")
        {
            Array arg = boost::get<Array>(func.at(1));

            Integer grid = boost::get<uInteger>(arg.at(0));

            grid_clear(grid);
            if constexpr(debug)std::cout << "call grid_clear()" << std::endl;
        }

        else if(func_name == "grid_destroy")
        {

            if constexpr(debug)std::cout << "call grid_destroy()" << std::endl;
        }

        else if(func_name == "grid_resize")
        {

            Array arg = boost::get<Array>(func.at(1));

            Integer grid = boost::get<uInteger>(arg.at(0));

            Integer row = boost::get<uInteger>(arg.at(1));

            Integer col = boost::get<uInteger>(arg.at(2));

            grid_resize(grid, row, col);
            if constexpr(debug)std::cout << "call grid_resize(" << grid << "," << row << "," << col << ")" << std::endl;
        }

        else if(func_name == "grid_cursor_goto")
        {
            Array arg = boost::get<Array>(func.at(1));

            Integer grid = boost::get<uInteger>(arg.at(0));

            Integer row = boost::get<uInteger>(arg.at(1));

            Integer col = boost::get<uInteger>(arg.at(2));

            int num_wchar = grid_cursor_goto(grid, row, col);
            nvim_image_cursor_y = row;
            nvim_image_cursor_x = col - num_wchar;
            if constexpr(debug)std::cout << "call grid_cursor_goto(" << grid << "," << row << "," << col << ")" << std::endl;
        }

        else if(func_name == "grid_scroll")
        {
            Array arg = boost::get<Array>(func.at(1));

            Integer grid = boost::get<uInteger>(arg.at(0));

            Integer top = boost::get<uInteger>(arg.at(1));

            Integer bot = boost::get<uInteger>(arg.at(2));

            Integer left = boost::get<uInteger>(arg.at(3));

            Integer right = boost::get<uInteger>(arg.at(4));

            Integer rows;
            if(arg.at(5).is_uint64_t()){ rows = boost::get<uInteger>(arg.at(5)); }
            else{ rows = boost::get<Integer>(arg.at(5)); }

            Integer cols = boost::get<uInteger>(arg.at(6));

            grid_scroll(grid, top, bot, left, right, rows, cols);
            if constexpr(debug)std::cout << "call grid_scroll(" << grid << "," << top << "," << bot << "," << left << "," << right << "," << rows << "," << cols << ")" << std::endl;
        }

    }
    return;
}

void neovim::redraw(Array ui_infos)
{
    for(auto ui_info: ui_infos)
    {
        redraw(ui_info);
    }
}

void neovim::operation(double timeout_millisec)
{
    while(true)
    {
        try
        {
            if(available() == 0){ return; }
            auto [funcs, infoes] = ui_client_.ui_read_info(timeout_millisec);
            Array redraw_infoes;
            for(int i = 0;i < funcs.size();i++)
            {
                std::cout << funcs.at(i) << std::endl;
                if(funcs.at(i) == "redraw")
                {
                    redraw_infoes.push_back(infoes.at(i));
                }
                else if(funcs.at(i) == "plugin")
                {
                    if constexpr(true)std::cout << "call plugin" << std::endl;
                    call_plugin(infoes.at(i));
                }
            }
            redraw(redraw_infoes);
        }
        catch(boost::system::system_error& e)
        {
            if constexpr(debug)std::cout << "can't read" << e.what() << std::endl;
            return;
        }
        catch(boost::bad_get& e)
        {
            if constexpr(debug)std::cout << "can't read" << e.what() << std::endl;
            return;
        }
    }
}

void neovim::nvim_ui_attach()
{
    ui_client_.no_read_do_call("nvim_ui_attach", nvim_size_x, nvim_size_y, ui_options);
    operation();
}

void neovim::nvim_ui_try_resize(Integer width, Integer height)
{
    nvim_size_x = width;
    nvim_size_y = height;

    // resize members
    nvim_screen.resize(height);
    for(auto& line: nvim_screen){ line.resize(width*2, ' '); }
    nvim_colors_map.resize(height);
    for(auto& line: nvim_colors_map){ line.set_default(width/2); }
    nvim_grid_colors_map.resize(height);
    for(auto& line: nvim_grid_colors_map){ line.resize(width*2, 0); }

    ui_client_.no_read_do_call("nvim_ui_try_resize", width, height);
    operation();
}

const tuple<Integer, Integer, Integer, Integer, Integer>& neovim::get_default_colors_set() const
{
    return nvim_default_colors_set;
}

Integer neovim::real_x(Integer row, Integer col)
{
    Integer count = 0;
    Integer position_x = 0;
    if(count == col)
    {
        return position_x;
    }
    for(uint i = 0;i < nvim_screen.at(row).size();i++)
    {
        constexpr short BIN1x2 = 0b11000000;
        const char& c = nvim_screen.at(row).at(i);

        if((c & BIN1x2) == BIN1x2){ position_x += 2, i++; }
        else{ position_x++; }
        count++;
        if(count == col)
        {
            return position_x;
        }
    }
    return position_x;
}

// ui_events
void neovim::resize(Integer rows, Integer columns)
{
    nvim_size_x = rows;
    nvim_size_y = columns;
    return;
}

void neovim::clear()
{
    try
    {
        nvim_screen.at(nvim_cursor_y).at(nvim_cursor_x) = ' ';
    }
    catch(std::out_of_range& e)
    {
        std::cout << "out_of_range:" << nvim_screen.at(nvim_cursor_y).at(nvim_cursor_x) << std::endl;
    }
    nvim_colors_map.at(nvim_cursor_y).overwrite(colors_map::_colors_map(
            nvim_default_colors_set,
            unordered_map<String, Object>(),
            make_pair(nvim_cursor_x, nvim_cursor_x)));
    return;
}

void neovim::eol_clear()
{
    try
    {
        for(uint i = nvim_cursor_x;i < nvim_screen.at(nvim_cursor_y).size();i++)
        {
            nvim_screen.at(nvim_cursor_y).at(i) = ' ';
        }
    }
    catch(std::out_of_range& e)
    {
        std::cout << "out_of_range:" << nvim_screen.at(nvim_cursor_y) << std::endl;
    }
    nvim_colors_map.at(nvim_cursor_y).overwrite(colors_map::_colors_map(
            nvim_default_colors_set,
            unordered_map<String, Object>(),
            make_pair(nvim_cursor_x, nvim_screen.at(nvim_cursor_y).size() - 1)));
    return;
}

void neovim::cursor_goto(Integer x, Integer y)
{
    if constexpr(debug)std::cout << "x:" << x << std::endl;
    nvim_cursor_y = y;
    Integer count = 0;
    Integer position_x = 0;
    if(count == x)
    {
        nvim_cursor_x = position_x;
        if constexpr(debug)std::cout << "nvim_cursor_x:" << nvim_cursor_x << std::endl;
        return;
    }
    for(uint i = 0;i < nvim_screen.at(y).size();i++)
    {
        constexpr short BIN1x2 = 0b11000000;
        const char& c = nvim_screen.at(y).at(i);

        if((c & BIN1x2) == BIN1x2){ position_x += 2, i++; }
        else{ position_x++; }
        count++;
        if(count == x)
        {
            nvim_cursor_x = position_x;
            break;
        }
    }
    if constexpr(debug)std::cout << "nvim_cursor_x:" << nvim_cursor_x << std::endl;
    return;
}

void neovim::mode_info_set(bool enabled, const cArray& cursor_styles)
{
    cursor_style = enabled;
    msgpack::type::tuple<String, Object> dict;
    for(cObject obj:cursor_styles)
    {
        obj.convert(dict);
        ui_mode_info[dict.get<0>()] = dict.get<1>();
    }
    return;
}

void neovim::update_menu()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::busy_start()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::busy_stop()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::mouse_on()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::mouse_off()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::mode_change(const String& mode, Integer mode_idx)
{
    String cmd_pre = "cmdline_";
    if(mode.size() >= cmd_pre.size() and
            std::equal(std::begin(cmd_pre), std::end(cmd_pre), std::begin(mode)))
    {
        current_mode["cmdline"] = mode_idx;
    }
    else
    {
        current_mode["screen"] = mode_idx;
    }
    return;
}

void neovim::set_scroll_region(Integer top, Integer bot, Integer left, Integer right)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::scroll(Integer count)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::highlight_set(const Dictionary& attrs)
{
    nvim_highlight_set.clear();
    for(auto& [key, val]: attrs)
    {
        nvim_highlight_set[boost::get<String>(key)] = val;
    }
    return;
}

Integer neovim::put(String str)
{
    try
    {
        std::cout << nvim_cursor_y << ":" << nvim_cursor_x << std::endl;
        nvim_screen.at(nvim_cursor_y).replace(nvim_cursor_x, str.size(), str);
    }
    catch(std::out_of_range& e)
    {
        std::cout << e.what() << nvim_screen.at(nvim_cursor_y) << std::endl;
        exit(1);
    }

    int str_size = 0;
    for(uint i = 0;i < str.size();i++)
    {
        constexpr short BIN1x2 = 0b11000000;
        const char& c = str.at(i);
        if((c & BIN1x2) == BIN1x2){ str_size++;i++; }
        str_size++;
    }
    nvim_colors_map.at(nvim_cursor_y).overwrite(colors_map::_colors_map(
            nvim_default_colors_set,
            nvim_highlight_set,
            make_pair(nvim_cursor_x, nvim_cursor_x + str_size - 1)));
    nvim_cursor_x += str_size;
    return str_size;
}

void neovim::bell()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::visual_bell()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::flush()
{
    need_update = true;
    if constexpr(debug)std::cout << "flush" << std::endl;
    return;
}

void neovim::update_fg(Integer fg)
{
    std::get<0>(nvim_default_colors_set) = fg;
    return;
}

void neovim::update_bg(Integer bg)
{
    std::get<1>(nvim_default_colors_set) = bg;
    return;
}

void neovim::update_sp(Integer sp)
{
    std::get<2>(nvim_default_colors_set) = sp;
    return;
}

void neovim::default_colors_set(Integer rgb_fg, Integer rgb_bg, Integer rgb_sp, Integer cterm_fg, Integer cterm_bg)
{
    nvim_default_colors_set = make_tuple(rgb_fg, rgb_bg, rgb_sp, cterm_fg, cterm_bg);
    nvim_hl_attr.at(0).rgb_attr["foreground"] = Object(rgb_fg);
    nvim_hl_attr.at(0).rgb_attr["background"] = Object(rgb_bg);
    nvim_hl_attr.at(0).cterm_attr["foreground"] = Object(cterm_fg);
    nvim_hl_attr.at(0).cterm_attr["background"] = Object(cterm_bg);
    return;
}

void neovim::suspend()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::set_title(const String& title)
{
    nvim_title = title;
    return;
}

void neovim::set_icon(const String& icon)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::option_set(const unordered_map<String, Object>& opt)
{
    for(auto& [key, val]: opt)
    {
        ui_options[key] = val;
    }
    if constexpr(debug)std::cout << "option_set" << std::endl;
    return;
}

void neovim::popupmenu_show(const cArray& items, Integer selected, Integer row, Integer col)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::popupmenu_hide()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::popupmenu_select(Integer selected)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::tabline_update(Tabpage current, const cArray& tabs)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::cmdline_show(const cArray& content, Integer pos, const String& firstc, const String& prompt, Integer indent, Integer level)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::cmdline_pos(Integer pos, Integer level)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::cmdline_special_char(const String& c, bool shift, Integer level)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::cmdline_hide(Integer level)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::cmdline_block_show(const cArray& lines)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::cmdline_block_append(const cArray& lines)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::cmdline_block_hide()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::wildmenu_show(const cArray& items)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::wildmenu_select(Integer selected)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::wildmenu_hide()
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
    return;
}

void neovim::hl_attr_define(Integer id, Dictionary& rgb_attr, Dictionary& cterm_attr, Array& info)
{
    for(const auto& [key, val]:rgb_attr)
    {
        nvim_hl_attr.at(id).rgb_attr[boost::get<String>(key)] = val;
    }
    for(const auto& [key, val]:cterm_attr)
    {
        nvim_hl_attr.at(id).cterm_attr[boost::get<String>(key)] = val;
    }
    nvim_hl_attr.at(id).info = info;
}


void neovim::grid_line(Integer grid, Integer row, Integer col_start, Array cells)
{
    try{
    constexpr short BIN1x1 = 0b10000000;
    constexpr short BIN1x2 = 0b11000000;
    constexpr short BIN1x3 = 0b11100000;
    constexpr short BIN1x4 = 0b11110000;

    // wwchar converter :to use wwchar
    int gap = 0; // number of wwchar
    int num_char = 0; // あ:+2, a:+1, ¦:+1
    for(int i = 0;num_char < col_start;i++)
    {
        if((nvim_screen.at(row).at(i) & BIN1x4) == BIN1x3){ num_char += 2; gap++; }
        else if((nvim_screen.at(row).at(i) & BIN1x2) == BIN1x1){  }
        else { num_char++; }
    }
    col_start -= gap;


    auto string_chr_num = [](const String& s)
    {
        int num = 0;
        for(const auto& c:s)
        {
            if((c & BIN1x1) == 0){ num++; } // 0b0xxxxxxx
            else if((c & BIN1x2) == BIN1x1){  } // 0b10xxxxxx
            else if((c & BIN1x3) == BIN1x2){ num++; } // 0b110xxxxx
            else if((c & BIN1x4) == BIN1x3){ num++; } // 0b1110xxxx
        }
        return num;
    };

    auto count_byte = [](const String& s, int num)
    {
        int n = 0;
        int ret = 0;
        if(num == 0){ return ret; }
        for(int i = 0;i < s.size();i++)
        {
            if((s.at(i) & BIN1x2) != BIN1x1){ n++; }

            if(n == num)
            {
                if((s.at(i) & BIN1x1) == 0){ ret++; } // 0b0xxxxxxx
                else if((s.at(i) & BIN1x3) == BIN1x2){ ret += 2; } // 0b110xxxxx
                else if((s.at(i) & BIN1x4) == BIN1x3){ ret += 3; } // 0b1110xxxx
                return ret;
            }
            ret++;
        }
        return ret;
    };

    int start_idx = count_byte(nvim_screen.at(row), col_start);
    String input = "";
    Vector<Integer> input_color;
    input_color.reserve(nvim_screen.at(row).size());
    Integer color_id = 0;
    for(auto cell:cells)
    {
        Integer times = 1;
        auto args = boost::get<Array>(cell);
        auto charactor = boost::get<String>(args.at(0)); // charactor is may not 1byte
        if(args.size() > 1)
        {
            color_id = boost::get<uInteger>(args.at(1));
            if(args.size() > 2)
            {
                times = boost::get<uInteger>(args.at(2));
            }
        }
        for(int i = 0;i < times;i++)
        {
            for(auto c: charactor)
            {
                input += c;
                input_color.push_back(color_id);
            }
        }
    }

    int input_char_num = string_chr_num(input);
    int last_part_start_idx_current = count_byte(nvim_screen.at(row), col_start + input_char_num);

    // current comment is [0:start_idx-1][removed][last_part_start_idx_current:]
    // next component is [0:start_idx-1][input][start_idx+input_size:prev_line_size]
    // get from original position
    auto first_part = nvim_screen.at(row).substr(0, start_idx);
    auto last_part = nvim_screen.at(row).substr(last_part_start_idx_current);

    nvim_screen.at(row) = first_part + input;
    nvim_screen.at(row).resize(nvim_grid_colors_map.at(row).size(), ' ');

    // combine last_part
    {
        int i = 0, j = 0;
        for(i = first_part.size() + input.size();
                i < nvim_screen.at(row).size() and j < last_part.size();i++, j++)
        {
            nvim_screen.at(row).at(i) =  last_part.at(j);
        }
        while(i < nvim_screen.at(row).size()){ nvim_screen.at(row).at(i++) = ' '; }
    }

    Vector<Integer> first_part_color;
    first_part_color.reserve(nvim_screen.at(row).size());

    Vector<Integer> last_part_color;
    last_part_color.reserve(nvim_screen.at(row).size());

    for(int i = 0;i < start_idx;i++)
    { first_part_color.push_back(nvim_grid_colors_map.at(row).at(i)); }
    for(int i = last_part_start_idx_current;i < nvim_grid_colors_map.at(row).size();i++)
    { last_part_color.push_back(nvim_grid_colors_map.at(row).at(i)); }

    int idx_color_map = 0;

    for(auto fc:first_part_color)
    { nvim_grid_colors_map.at(row).at(idx_color_map++) = fc; }

    for(auto ic:input_color)
    { nvim_grid_colors_map.at(row).at(idx_color_map++) = ic; }

    for(auto lc:last_part_color)
    {
        if(idx_color_map < nvim_grid_colors_map.at(row).size())
        {
            nvim_grid_colors_map.at(row).at(idx_color_map++) = lc;
        }
    }
    }
    catch(std::out_of_range){ return; }
}

void neovim::grid_clear(Integer grid)
{
    for(auto& line:nvim_screen)
    {
        line.replace(0, line.size(), line.size(), ' ');
    }
    for(auto& line:nvim_grid_colors_map)
    {
        for(auto& ch:line)
        {
            ch = 0;
        }
    }
}

void neovim::grid_destroy(Integer grid)
{
    if constexpr(debug)std::cout << "not defined" << std::endl;
}

void neovim::grid_resize(Integer grid, Integer row, Integer col)
{
    nvim_size_x = row;
    nvim_size_y = col;

    // resize members
    nvim_screen.resize(col);
    for(auto& line: nvim_screen){ line.resize(row*2, ' '); }
    nvim_colors_map.resize(col);
    for(auto& line: nvim_colors_map){ line.set_default(row/2); }
    nvim_grid_colors_map.resize(col);
    for(auto& line: nvim_grid_colors_map){ line.resize(row*2, 0); }
}

int neovim::grid_cursor_goto(Integer grid, Integer row, Integer col)
{
    int num_wchar = 0;
    current_grid = grid;
    nvim_cursor_y = row;
    Integer count = 0;
    Integer position_x = 0;
    if(count == col)
    {
        nvim_cursor_x = position_x;
        if constexpr(debug)std::cout << "nvim_cursor_x:" << nvim_cursor_x << std::endl;
        return num_wchar;
    }
    for(uint i = 0;i < nvim_screen.at(row).size();i++)
    {
        constexpr short BIN1x1 = 0b10000000;
        constexpr short BIN1x2 = 0b11000000;
        constexpr short BIN1x3 = 0b11100000;
        const char& c = nvim_screen.at(row).at(i);

        // c == 0b110xxxxx: 'あ'
        if((c & BIN1x3) == BIN1x2){ position_x += 2, i++; }
        // c == 0b10xxxxxx: '¦'
        else if((c & BIN1x2) == BIN1x1){ position_x++;i++; }
        // c == 0b0xxxxxxx: 'a'
        else if((c & BIN1x1) == 0){position_x++;}
        // c == 0b111xxxxx
        else{ num_wchar++;position_x++; }
        count++;
        if(count == col)
        {
            nvim_cursor_x = position_x;
            break;
        }
    }
    return num_wchar;
}

void neovim::grid_scroll(Integer grid, Integer top, Integer bot, Integer left, Integer right, Integer rows, Integer cols)
{
    Integer src_top = top;
    Integer src_bot = bot;
    Integer dst_top = top - rows;
    Integer dst_bot = bot - rows;
    Integer start = (rows >= 0) ? src_top : src_bot - 1;
    Integer end = (rows >= 0) ? src_bot : src_top;
    auto to_next = [&](Integer& src_i)
    {
        if(rows >= 0){ src_i++; }
        else { src_i--; }
    };
    auto cond_next = [&](const Integer& src_i)
    {
        if(rows >= 0){ return src_i < end; }
        else { return src_i >= end; }
    };

    for(Integer src_i = start;cond_next(src_i);to_next(src_i))
    {
        Integer dst_i = src_i - rows;

        if(src_top <= dst_i and dst_i < src_bot)
        {
            Integer src_right = real_x(src_i, right);
            Integer src_left = real_x(src_i, left);
            Integer dst_right = real_x(dst_i, right);
            Integer dst_left = real_x(dst_i, left);

            String tmp = nvim_screen.at(src_i).substr(src_left, src_right - src_left + 1); //move
            tmp += (nvim_screen.at(dst_i).substr(dst_right + 1)); //not move
            for(uint idx = dst_right;idx < nvim_screen.at(dst_i).size();idx++)
            {
                nvim_screen.at(dst_i).at(idx) = ' ';
            }

            Vector<Integer> tmp_color;
            tmp_color.resize(nvim_grid_colors_map.at(dst_i).size(), 0);
            uint cidx = 0;
            for(uint idx = src_left;idx < src_right;idx++, cidx++)
            {
                tmp_color.at(cidx) = nvim_grid_colors_map.at(src_i).at(idx);
            }
            for(uint idx = dst_right;idx < nvim_grid_colors_map.at(dst_i).size() and cidx < tmp_color.size();idx++, cidx++)
            {
                tmp_color.at(cidx) = nvim_grid_colors_map.at(dst_i).at(idx);
            }
            for(uint idx = dst_left, cidx = 0;idx < nvim_grid_colors_map.at(dst_i).size() and cidx < tmp_color.size();idx++, cidx++)
            {
                nvim_grid_colors_map.at(dst_i).at(idx) = tmp_color.at(cidx);
            }

            nvim_screen.at(dst_i).replace(dst_left, tmp.size(), tmp);
        }
    }

    Integer clear_start = (rows >= 0) ? dst_bot : src_top;
    Integer clear_end = (rows >= 0) ? src_bot : dst_top;
    for(Integer i = clear_start;i < clear_end;i++)
    {
        Integer src_right = real_x(i, right);
        Integer src_left = real_x(i, left);
        String tmp = nvim_screen.at(i).substr(src_right);
        Vector<Integer> tmp_color;
        tmp_color.resize(nvim_grid_colors_map.at(i).size(), 0);
        for(Integer idx = src_right, cidx = 0;idx < nvim_grid_colors_map.at(i).size();idx++, cidx++)
        {
            tmp_color.at(cidx) = nvim_grid_colors_map.at(i).at(idx);
        }

        nvim_screen.at(i).replace(src_left, src_right - src_left, src_right - src_left, ' ');
        for(Integer idx = src_left;idx < src_right;idx++)
        {
            nvim_grid_colors_map.at(i).at(idx) = 0;
        }

        src_right = real_x(i, right);
        nvim_screen.at(i).replace(src_right, tmp.size(), tmp);
        for(Integer idx = src_right, cidx = 0;idx < nvim_grid_colors_map.at(i).size();idx++, cidx++)
        {
            nvim_grid_colors_map.at(i).at(idx) = tmp_color.at(cidx);
        }
    }
}
