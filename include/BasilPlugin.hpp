#ifndef ___BasilPlugin_H_
#define ___BasilPlugin_H_

#include "neovim.hpp"
class Basilico;

class BasilPlugin
{
public:
    BasilPlugin() = default;

    virtual void execute(Basilico* basil, Array args);

    virtual void timerExecute(Basilico* basil);

    virtual void keyPressedExecute(Basilico* basil);
};
#endif
