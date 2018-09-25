
#ifndef __NEOVIM_CLASS_H_
#define __NEOVIM_CLASS_H_

#include <iostream>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "nvim_api.hpp"
#include "msgpack.hpp"

using std::tuple;
using std::make_tuple;
using std::get;
using std::unordered_map;
using std::unordered_set;

using nvim::Integer;
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
using std::pair;

template<typename ...T>
using Tuple = msgpack::type::tuple<T...>;
using Resp = Tuple<Integer, Object, Object, Object>;
template<typename T>
using Vector = std::vector<T>;

class neovim: public nvim::nvim_api
{

public:
    Integer nvim_size_x, nvim_size_y;

    Integer nvim_cursor_x, nvim_cursor_y;

    Vector<String> nvim_code;

    String nvim_title;

    bool cursor_style;

    unordered_map<String, Object> ui_mode_info;

    unordered_map<String, Object> ui_options;
    //[rgb_fg, rgb_bg, rgb_sp, cterm_fg, cterm_bg]
    tuple<Integer, Integer, Integer, Integer, Integer> nvim_default_colors_set;

public:
    void redraw(Object res);
    void redraw();

    void nvim_ui_attach (Integer width, Integer height, const Dictionary& options)override;

private:
{% for event in ui_events%}
    void {{event.name}}({% for arg in event.parameters%}{{arg.type}} {{arg.name}}{% if not loop.last %}, {% endif %}{% endfor%});
{% endfor %}
};


#endif
