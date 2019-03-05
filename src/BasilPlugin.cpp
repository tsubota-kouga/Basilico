
#include "BasilPlugin.hpp"

void BasilPlugin::execute(Basilico* Basilico, Array args){}

void BasilPlugin::timerExecute(Basilico* Basilico){}

void BasilPlugin::keyPressedExecute(Basilico* Basilico){}

void BasilPlugin::autocmdExecute(Basilico* basil, String autocmd){}

std::tuple<int, int, int, int> BasilPlugin::splitPluginPosition(Tabpage tab)
{ return std::make_tuple(0, 0, 1, 1); }
