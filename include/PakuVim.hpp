#ifndef __PAKUVIM_H_
#define __PAKUVIM_H_

#include "neovim.hpp"

#include <unordered_map>
#include <thread>
#include <chrono>

#include <QtWidgets>

class PakuVim: public QTextEdit, public neovim
{
    using neovim::neovim;
private:
    int timer;

public:

    PakuVim(uint width, uint height, const Dictionary& options)
        :neovim{width, height, options}
    {
        timer = startTimer(30);
    }

    void set_neovim_html();

protected:
    virtual void keyPressEvent(QKeyEvent* e) override;

    virtual void timerEvent(QTimerEvent* e) override;

    void update();
};

namespace nvim_html
{
    void html_escape(std::string& s);
    std::tuple<long, long, long> convert_rgb(long rgb);
}

#endif
