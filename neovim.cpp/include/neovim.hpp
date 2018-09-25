
#ifndef __NEOVIM_CLASS_H_
#define __NEOVIM_CLASS_H_

#include <iostream>
#include <codecvt>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "nvim_api.hpp"
#include "neovim_ui.hpp"
#include "msgpack.hpp"

using std::tuple;
using std::make_tuple;
using std::pair;
using std::make_pair;
using std::unordered_map;
using std::unordered_set;

using nvim::Integer;
using uInteger = uint64_t;
using nvim::Window;
using nvim::Buffer;
using nvim::Tabpage;
using nvim::String;
using nvim::Object;
using nvim::cObject;
using nvim::Map;
using nvim::Dictionary;
using nvim::Array;
using cArray = std::vector<msgpack::object>;


template<typename ...T>
using Tuple = msgpack::type::tuple<T...>;
using Resp = Tuple<Integer, Object, Object, Object>;
template<typename T>
using Vector = std::vector<T>;

class neovim: public nvim::nvim_api, public nvim::nvim_ui
{
    struct colors_map
    {
        struct _colors_map
        {
            _colors_map(tuple<Integer, Integer, Integer, Integer, Integer> t, unordered_map<String, Object> m, pair<Integer, Integer> p):colors_set{t},hl_map{m},range{p}{}

            tuple<Integer, Integer, Integer, Integer, Integer> colors_set;
            unordered_map<String, Object> hl_map;
            pair<Integer, Integer> range;

            const Integer get_rgb_fg() const;
            const Integer get_rgb_bg() const;
            const tuple<int, int, int> get_r_g_b_fg() const;
            const tuple<int, int, int> get_r_g_b_bg() const;
            const tuple<int, int, int> get_default_r_g_b_fg() const;
            const tuple<int, int, int> get_default_r_g_b_bg() const;
            const Integer& get_rgb_sp() const;
            const Integer& get_cterm_fg() const;
            const Integer& get_cterm_bg() const;
            const pair<Integer, Integer>& get_range() const;

            void set_rgb_fg(Integer rgb_fg);
            void set_rgb_bg(Integer rgb_bg);
            void set_rgb_sp(Integer rgb_sp);
            void set_cterm_fg(Integer cterm_fg);
            void set_cterm_bg(Integer cterm_bg);

            void set_range(Integer from, Integer to);
            void set_range(pair<Integer, Integer> range_);

            bool is_bold() const;
            bool is_italic() const;

            bool operator==(_colors_map c);
        };

    public:
        Vector<_colors_map> _colors_map_list;

        void set_default(Integer x);

        void overwrite(const _colors_map&& c_map);

        void print();
    };

    //ext_newgrid
    struct hl_attr
    {
        unordered_map<String, Object> rgb_attr;
        unordered_map<String, Object> cterm_attr;
        Array info;
    };

public:
    Integer nvim_size_x, nvim_size_y;

    Integer nvim_cursor_x, nvim_cursor_y; // [0, nvim_size_x], [0, nvim_size_y]
    Integer nvim_image_cursor_x,nvim_image_cursor_y;

    Integer current_grid;

    Integer nvim_grid_width, nvim_grid_height;

    Vector<String> nvim_screen;

    String nvim_title;

    bool cursor_style;

    unordered_map<String, Object> ui_mode_info;

    unordered_map<String, Integer> current_mode;

    unordered_map<String, Object> ui_options;

    //[rgb_fg, rgb_bg, rgb_sp, cterm_fg, cterm_bg]
    //   U         U        S        U        U
    tuple<Integer, Integer, Integer, Integer, Integer> nvim_default_colors_set;

    unordered_map<String, Object> nvim_highlight_set;

    Vector<colors_map> nvim_colors_map;

//ext_newgrid option
    bool is_ext_newgrid;

    Vector<hl_attr> nvim_hl_attr;

    Vector<Vector<Integer>> nvim_grid_colors_map;

    bool need_update;
public:
    enum class Mode
    {
        normal = (Integer)0,
        visual,
        insert
    };

    neovim(uint width, uint height, const Dictionary& options);

    void draw(Object ui_info);

    void draw(Array ui_infos);

    void draw(double timeout_millisec=1.0);

    void nvim_ui_attach();

    void connect_tcp(const String& host, const String& service, double timeout_sec=100) override;

private:
    const tuple<Integer, Integer, Integer, Integer, Integer>& get_default_colors_set() const;

    Integer real_x(Integer row, Integer col);
    Integer real_x_right(Integer row, Integer col);

// ui_events
    void resize(Integer rows, Integer columns);

    void clear();

    void eol_clear();

    void cursor_goto(Integer row, Integer col);

    void mode_info_set(bool enabled, const cArray& cursor_styles);

    void update_menu();

    void busy_start();

    void busy_stop();

    void mouse_on();

    void mouse_off();

    void mode_change(const String& mode, Integer mode_idx);

    void set_scroll_region(Integer top, Integer bot, Integer left, Integer right);

    void scroll(Integer count);

    void highlight_set(const Dictionary& attrs);

    Integer put(String str);

    void bell();

    void visual_bell();

    void flush();

    void update_fg(Integer fg);

    void update_bg(Integer bg);

    void update_sp(Integer sp);

    void default_colors_set(Integer rgb_fg, Integer rgb_bg, Integer rgb_sp, Integer cterm_fg, Integer cterm_bg);

    void suspend();

    void set_title(const String& title);

    void set_icon(const String& icon);

    void option_set(const unordered_map<String, Object>& opt);

    void popupmenu_show(const cArray& items, Integer selected, Integer row, Integer col);

    void popupmenu_hide();

    void popupmenu_select(Integer selected);

    void tabline_update(Tabpage current, const cArray& tabs);

    void cmdline_show(const cArray& content, Integer pos, const String& firstc, const String& prompt, Integer indent, Integer level);

    void cmdline_pos(Integer pos, Integer level);

    void cmdline_special_char(const String& c, bool shift, Integer level);

    void cmdline_hide(Integer level);

    void cmdline_block_show(const cArray& lines);

    void cmdline_block_append(const cArray& lines);

    void cmdline_block_hide();

    void wildmenu_show(const cArray& items);

    void wildmenu_select(Integer selected);

    void wildmenu_hide();

    void hl_attr_define(Integer id, Dictionary& rgb_attr, Dictionary& cterm_attr, Array& info);

    void grid_line(Integer grid, Integer row, Integer col_start, Array cells);

    void grid_clear(Integer grid);

    void grid_destroy(Integer grid);

    void grid_cursor_goto(Integer grid, Integer row, Integer col);

    void grid_scroll(Integer grid, Integer top, Integer bot, Integer left, Integer right, Integer rows, Integer cols);

};

#endif
