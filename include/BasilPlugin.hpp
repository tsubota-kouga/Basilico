#ifndef ___BasilPlugin_H_
#define ___BasilPlugin_H_

#include "neovim.hpp"
#include "NeoVim.hpp"
#include <QtWidgets>
class Basilico;

class BasilPlugin
{
public:
    BasilPlugin() = default;

    virtual void execute(Basilico* basil, Array args);

    virtual void timerExecute(Basilico* basil);

    virtual void keyPressedExecute(Basilico* basil);

    virtual void autocmdExecute(Basilico* basil, String autocmd);

    virtual std::tuple<int, int, int, int> splitPluginPosition(Tabpage tab);
};

#endif
