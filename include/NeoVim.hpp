#ifndef ___NeoVim_H_
#define ___NeoVim_H_

#include "neovim.hpp"
#include "neovim_utils.hpp"

#include <deque>
#include <queue>
#include <unordered_map>

#include <QtWidgets>

using std::queue;
using std::deque;

class Basilico;

class NeoVim: public QTextEdit, public neovim
{
    using neovim::neovim;
private:
    int timer;

    static constexpr double cw = 10;
    static constexpr double cwi = 0.1;
    static constexpr double chi = 1.1;

    Basilico* parent;

    deque<pair<Tabpage, String>> nvim_tabline;

    String font;
    int font_size_px;
    bool isKeyPressed;

    String cursorShape;
    Integer cursorColorId;
public:

    NeoVim(uint width, uint height, Basilico* parent_, const Dictionary& options);

    deque<Array> plugin_deque;

    void set_neovim_html();

    void set_font(String f){ font = f; }
    void set_font_size_px(int px){ font_size_px = px; }

    int width2WindowWidth(int editor_width){ return editor_width*(font_size_px + cwi)/2 + cw; }
    int height2WindowHeight(int editor_height){ return editor_height*(font_size_px + chi); }

    int windowWidth2Width(int window_width){ return (window_width - cw)*2/(font_size_px + cwi); }
    int windowHeight2Height(int window_height){ return window_height/(font_size_px + chi); }

    bool readIsKeyPressed()
    {
        bool b = isKeyPressed;
        isKeyPressed = false;
        return b;
    }

    void tabline_change(int idx);

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
    void html_escape(std::string& s);
    std::tuple<unsigned long, unsigned long, unsigned long> convert_rgb(unsigned long rgb);
}

#endif
