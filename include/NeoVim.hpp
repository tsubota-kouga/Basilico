#ifndef __NeoVim_H_
#define __NeoVim_H_

#include "neovim.hpp"

#include <queue>

#include <QtWidgets>

using std::queue;

class NeoVim: public QTextEdit, public neovim
{
    using neovim::neovim;
private:
    int timer;

public:

    NeoVim(uint width, uint height, const Dictionary& options)
        :neovim{width, height, options},
         isKeyPressed{false}
    {
        timer = startTimer(30);

        // <default settings>
        font_size_px = 16;
        font = "ubuntu mono";
        // </default settings>

        setAttribute(Qt::WA_InputMethodEnabled);

        QWidget::resize(width*font_size_px/2 + 10, height*(font_size_px + 1.2));

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        setTextInteractionFlags(Qt::TextEditable | Qt::LinksAccessibleByMouse);

    }

    String font;
    int font_size_px;
    bool isKeyPressed;
    queue<Array> plugin_queue;

    void set_neovim_html();

    void set_font(String f){ font = f; }
    void set_font_size_px(int px){ font_size_px = px; }

    int width2WindowWidth(int editor_width){ return editor_width*font_size_px/2 + 10; }
    int height2WindowHeight(int editor_height){ return editor_height*(font_size_px + 1.2); }

    int windowWidth2Width(int window_width){ return (window_width - 10)*2/font_size_px; }
    int windowHeight2Height(int window_height){ return window_height/(font_size_px + 1.2); }

    bool readIsKeyPressed()
    {
        bool b = isKeyPressed;
        isKeyPressed = false;
        return b;
    }

protected:
    virtual void keyPressEvent(QKeyEvent* e) override;

    virtual void timerEvent(QTimerEvent* e) override;

    virtual void inputMethodEvent(QInputMethodEvent* e) override;

    virtual void resizeEvent(QResizeEvent* e) override;

    virtual QVariant inputMethodQuery(Qt::InputMethodQuery q) const override;

    void update();

    virtual void call_plugin(Object func_and_args) override;

    virtual void cursor_shape();

    void fkeySend(QKeyEvent* e, Integer key);

    void keySend(QKeyEvent* e, const String& key);
};

namespace nvim_html
{
    void html_escape(std::string& s);
    std::tuple<long, long, long> convert_rgb(long rgb);
}

#endif
