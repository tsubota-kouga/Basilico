
#include "BasilPlugin.hpp"

void BasilPlugin::on_init(){};

void BasilPlugin::execute(Basilico* basil, Array args){}

void BasilPlugin::timerExecute(Basilico* basil){}

void BasilPlugin::keyPressedExecute(Basilico* basil){}

void BasilPlugin::autocmdExecute(Basilico* basil, String autocmd){}

std::tuple<int, int, int, int> BasilPlugin::splitPluginPosition(Tabpage tab)
{ return std::make_tuple(0, 0, 1, 1); }
