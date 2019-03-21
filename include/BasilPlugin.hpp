#ifndef ___BasilPlugin_H_
#define ___BasilPlugin_H_

#include "nvim_api.hpp"
#include "neovim.hpp"
#include "NeoVim.hpp"
#include <QtWidgets>
class Basilico;

class BasilPlugin
{
private:
    bool init = false;

public:
    BasilPlugin() = default;
    BasilPlugin(const String& port);

    // TODO Future support
    virtual void on_init();

    virtual void execute(Basilico* basil, Array args);

    virtual void timerExecute(Basilico* basil);

    virtual void keyPressedExecute(Basilico* basil);

    virtual void autocmdExecute(Basilico* basil, String autocmd);

    virtual std::tuple<int, int, int, int> splitPluginPosition(Tabpage tab);
};

#endif
