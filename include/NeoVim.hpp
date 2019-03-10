#ifndef ___NeoVim_H_
#define ___NeoVim_H_

#include "neovim.hpp"
#include "neovim_utils.hpp"

#include <deque>
#include <unordered_map>
#include <cmath>

#include <QtWidgets>

using std::deque;

class Basilico;

class NeoVim: public QTextEdit, public neovim
{
    using neovim::neovim;
    int timer;

    static constexpr double cw = 10;
    static constexpr double cwi = 0.1;
    static constexpr double chi = 1.1;
    static constexpr double eps_h = 2;
    static constexpr double eps_w = 6;

    static constexpr int default_idx = 0;

    Basilico* parent;

    deque<pair<Tabpage, String>> nvim_tabline;  // tab, name

    String _guifont;
    int _font_size_px;
    int wchar_font_size_px;
    bool isKeyPressed;

    Integer api_level;

    String cursorShape;
    Integer cursorColorId;

    std::vector<std::tuple<String, String, String, String>> completion_info;
    Integer popupmenu_selected;
    std::pair<Integer, Integer> popupmenu_pos;

public:

    NeoVim(uint width, uint height, Basilico* parent_, const Dictionary& options, String ip, String port, int timeout_millisec);
    NeoVim() = delete;
    NeoVim operator=(neovim) = delete;
    NeoVim operator=(QTextEdit) = delete;

    deque<Array> plugin_req_deque;

    void set_neovim_html();

    virtual void set_font(String f) override { _guifont = f; }
    virtual void set_font_size_px(int px) override;

    int width2WindowWidth(int editor_width){ return editor_width*(_font_size_px + cwi)/2 + cw; }
    int height2WindowHeight(int editor_height){ return editor_height*(_font_size_px + chi); }

    int windowWidth2Width(int window_width){ return (window_width - cw)*2/(_font_size_px + cwi); }
    int windowHeight2Height(int window_height){ return window_height/(_font_size_px + chi); }

    bool readIsKeyPressed()
    {
        bool b = isKeyPressed;
        isKeyPressed = false;
        return b;
    }

    void tabline_change(int idx);

    deque<pair<Tabpage, String>>& get_nvim_tabline(){ return nvim_tabline; }

    template<typename T>
    T hl_attr_get(Integer id, const String&& key)
    {
        if(nvim_hl_attr.at(id).rgb_attr.count(key) == 1)
        { return boost::get<T>(nvim_hl_attr.at(id).rgb_attr.at(key)); }
        else if(nvim_hl_attr.at(default_idx).rgb_attr.count(key) == 1)  // has default key
        { return boost::get<T>(nvim_hl_attr.at(default_idx).rgb_attr.at(key)); }
        else  // has no default key
        { return T{}; }
    }

protected:
    virtual bool event(QEvent* e) override;

    virtual void keyPressEvent(QKeyEvent* e) override;

    virtual void timerEvent(QTimerEvent* e) override;

    virtual void inputMethodEvent(QInputMethodEvent* e) override;

    virtual void resizeEvent(QResizeEvent* e) override;

    virtual void mousePressEvent(QMouseEvent* e) override;

    virtual void mouseReleaseEvent(QMouseEvent* e) override;

    virtual void mouseMoveEvent(QMouseEvent* e) override;

    virtual void wheelEvent(QWheelEvent* e) override;

    virtual void dropEvent(QDropEvent* e) override;

    virtual void dragEnterEvent(QDragEnterEvent* e) override;

    virtual void dragLeaveEvent(QDragLeaveEvent* e) override;

    virtual void dragMoveEvent(QDragMoveEvent* e) override;

    virtual void tabline_update(Tabpage current, const Array& tabs) override;

    virtual void title_changed() override;

    virtual void mode_changed() override;

    void update();

    virtual void call_plugin(Object func_and_args) override;

    virtual void cursor_shape_and_pos();

    virtual void popupmenu_show(const Array& items, Integer selected, Integer row, Integer col, Integer grid) override;

    virtual void popupmenu_hide() override;

    virtual void popupmenu_select(Integer selected) override;

private:
    void fkeySend(QKeyEvent* e, Integer key);

    void keySend(QInputEvent* e, const String& key, bool no_shift=false);

    void mouseSend(QPoint pos, const String& modifiers, const String& action, const String& button, Integer grid);

    void mouseSend(QMouseEvent* e, const String& action, const String& button, Integer grid);

    void mouseSend(QWheelEvent* e, const String& action, const String& button, Integer grid);

    void cursor_shape(Mode m);
};

namespace nvim_html
{
    void html_escape(std::string& s, int wchar_size);
    std::tuple<unsigned long, unsigned long, unsigned long> convert_rgb(unsigned long rgb);
    int how_many_div_by_exp2(unsigned char c);
    std::pair<unsigned, unsigned> utf8_ord(const std::string& str, int& i);
}

#endif
