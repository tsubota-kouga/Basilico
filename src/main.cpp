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
<<<<<<< HEAD
    constexpr int width = 80;
    constexpr int height = 24;
    Basilico basil(argv[1], width, height);
    basil.show();
=======
    QGraphicsView view;
    QGridLayout glayout;
    // QHBoxLayout glayout;
    constexpr int width = 101;
    constexpr int height = 56;
    PakuVim pakuvim(width, height, {{"rgb", true}, {"ext_linegrid", true}});
    pakuvim.connect_tcp("localhost", argv[1], 100);
    pakuvim.nvim_ui_attach();
    // pakuvim.QWidget::resize((width + 2)*8, (height + 5)*16);
    pakuvim.setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    // pakuvim.show();
    glayout.addWidget(&pakuvim, 0, 0);
>>>>>>> 0d09101b8e19cde01d798514fecaf52975df128c

    return app.exec();
}

