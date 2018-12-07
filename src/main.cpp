#include <iostream>
#include <regex>
#include <string>
#include <algorithm>

#include "NeoVim.hpp"
#include "Basilico.hpp"

#include <QtWidgets>
#include <QtGui>
#include <QtWebEngineWidgets>
#include <QTimer>

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::unordered_map;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    constexpr int width = 80;
    constexpr int height = 24;
    Basilico basil(argv[1], width, height);
    basil.show();

    return app.exec();
}

