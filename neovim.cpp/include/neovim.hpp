
#ifndef ___NEOVIM_CLASS_H_
#define ___NEOVIM_CLASS_H_

#include <iostream>
#include <codecvt>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

#include "nvim_api.hpp"
#include "neovim_ui.hpp"
#include "msgpack.hpp"
#include "neovim_utils.hpp"

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

    struct screen_line
    {
        screen_line(String s, int recommend_size = 100)
            :line{}
        {
            line.reserve(recommend_size);
            for(uint i = 0;i < s.size();i++)
            {
                if((s.at(i) & utils::BIN1x2) == utils::BIN1x1) // 0b10xxxxxx
                {
                    line.at(line.size() - 1).push_back(s.at(i));
                }
                else
                {
                    line.push_back(String{ s.at(i) });
                }
            }
        }

        screen_line():line{}{}

    private:
        Vector<String> line;
    public:
        const String& at(int i) const;
        String& at(int i);

        bool overwrite(int start_pos, const String& s);

        void overwrite_with_exception(int start_pos, const String& s);

        bool overwrite_and_pushback(int start_pos, const String& s);

        String substr(int start, int num) const;

        size_t size() const;

        size_t length() const;

        void assign(const String& c, int start, int end);

        void assign(const String& c, int start=0);

        bool is_a_charactor(const String& c_) const;

        void resize(uint size, const String& s="");

        void reserve(uint size);

        void test() const;
    };

    friend std::ostream& operator<<(std::ostream& out, const screen_line& screen_line_);

    //ext_linegrid
    struct hl_attr
    {
        unordered_map<String, Object> rgb_attr;
        unordered_map<String, Object> cterm_attr;
        Array info;
    };

public:
    enum class Mode
    {
        normal = 0,
        visual,
        insert,
        replace,
        cmdline_normal,
        cmdline_insert,
        cmdline_replace,
        operator_,
        visual_select,
        cmdline_hover,
        statusline_hover,
        statusline_drag,
        vsep_hover,
        vsep_drag,
        more,
        more_lastline,
        showmatch
    };

    Integer nvim_size_x, nvim_size_y;

    Integer nvim_cursor_x, nvim_cursor_y; // [0, nvim_size_x], [0, nvim_size_y]

    Integer current_grid;

    Integer nvim_grid_width, nvim_grid_height;

    // Vector<String> nvim_screen;
    Vector<screen_line> nvim_screen;

    String nvim_title;

    String nvim_icon;

    String guifont;
    int font_size_px;

    bool cursor_style;

    unordered_map<String, unordered_map<String, Object>> ui_mode_info;

    unordered_map<String, Mode> current_mode;

    unordered_map<String, Object> ui_options;

    //[rgb_fg, rgb_bg, rgb_sp, cterm_fg, cterm_bg]
    //   U         U        S        U        U
    tuple<Integer, Integer, Integer, Integer, Integer> nvim_default_colors_set;

    unordered_map<String, Object> nvim_highlight_set;

    Vector<colors_map> nvim_colors_map;

//ext_linegrid option
    bool is_ext_linegrid;

    Vector<hl_attr> nvim_hl_attr;

    Vector<Vector<Integer>> nvim_grid_colors_map;  // color(x, y)

    bool need_update;

    bool incomplete_resize = false;

    bool is_busy = false;
public:

    neovim(uint width, uint height, const Dictionary& options);

    void redraw(Object ui_info);

    void redraw(Array ui_infos);

    bool operation(long timeout_millisec=1000);

    void nvim_ui_attach();

    virtual void nvim_buf_attach(Buffer buffer, bool send_buffer, const Dictionary& opts) override;

    void nvim_ui_try_resize(Integer width, Integer height) override;

    void connect_tcp(const String& host, const String& service, long timeout_sec=100) override;

    void set_ui_option(const String&& ui_option, bool on_or_off);

    bool get_ui_option(const String&& ui_option);

    virtual void set_font(String f){}

    virtual void set_font_size_px(int px){}

private:
    const tuple<Integer, Integer, Integer, Integer, Integer>& get_default_colors_set() const;

// ui_events
    void resize(Integer rows, Integer columns);

    void clear();

    void eol_clear();

    void cursor_goto(Integer row, Integer col);

    void mode_info_set(bool enabled, const Array& cursor_styles);

    void update_menu();

    void busy_start();

    void busy_stop();

    void mouse_on();

    void mouse_off();

    void mode_change(const String& mode, Mode mode_idx);

    void set_scroll_region(Integer top, Integer bot, Integer left, Integer right);

    void scroll(Integer count);

    void highlight_set(const Dictionary& attrs);

    void put(String str);

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

    virtual void popupmenu_show(const Array& items, Integer selected, Integer row, Integer col, Integer grid);

    virtual void popupmenu_hide();

    virtual void popupmenu_select(Integer selected);

    virtual void tabline_update(Tabpage current, const Array& tabs);

    virtual void cmdline_show(const cArray& content, Integer pos, const String& firstc, const String& prompt, Integer indent, Integer level);

    virtual void cmdline_pos(Integer pos, Integer level);

    virtual void cmdline_special_char(const String& c, bool shift, Integer level);

    virtual void cmdline_hide(Integer level);

    virtual void cmdline_block_show(const cArray& lines);

    virtual void cmdline_block_append(const cArray& lines);

    virtual void cmdline_block_hide();

    virtual void wildmenu_show(const cArray& items);

    virtual void wildmenu_select(Integer selected);

    virtual void wildmenu_hide();

    virtual void win_pos(Integer grid, Integer win, Integer start_row, Integer start_col, Integer width, Integer height){}

    virtual void win_hide(Integer grid){}

    virtual void win_scroll_over_start(){}

    virtual void win_scroll_over_reset(){}

    virtual void title_changed(){}

    virtual void icon_changed(){}

    virtual void mode_changed(){}

    virtual void call_plugin(Object func_and_args){}

    void hl_attr_define(Integer id, Dictionary& rgb_attr, Dictionary& cterm_attr, Array& info);

    void grid_line(Integer grid, Integer row, Integer col_start, Array cells);

    void grid_clear(Integer grid);

    void grid_destroy(Integer grid);

    void grid_resize(Integer grid, Integer row, Integer col);

    void grid_cursor_goto(Integer grid, Integer row, Integer col);

    void grid_scroll(Integer grid, Integer top, Integer bot, Integer left, Integer right, Integer rows, Integer cols);

};

#endif
