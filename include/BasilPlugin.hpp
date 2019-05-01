#ifndef ___BasilPlugin_H_
#define ___BasilPlugin_H_

#include "nvim_api.hpp"
#include "neovim.hpp"
#include "NeoVim.hpp"

#include <QtWidgets>
#include <type_traits>
#include <typeinfo>

class Basilico;

enum class PluginType {
    tab,
    split,
    dock
};

class BasilPlugin
{
private:
    PluginType pluginType;

public:
    BasilPlugin() = default;
    BasilPlugin(const String& port);

    void setPluginType(PluginType type){ pluginType = type; }
    PluginType getPluginType(){ return pluginType; }

    void suicide(Basilico* basil);

    virtual void on_init();

    virtual void execute(Basilico* basil, Array args);

    virtual void timerExecute(Basilico* basil);

    virtual void keyPressedExecute(Basilico* basil);

    virtual void autocmdExecute(Basilico* basil, String autocmd);

    virtual std::tuple<int, int, int, int> splitPluginPosition(Basilico* basil, Tabpage tab);
};

#endif
