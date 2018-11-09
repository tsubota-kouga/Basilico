#include <iostream>
#include <regex>
#include <string>
#include <algorithm>

#include "PakuVim.hpp"

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

    QWebEngineView web;
    web.load(QUrl("https://www.google.co.jp"));
    // web.resize((width + 2)*8, (height + 5)*16);
    web.setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    glayout.addWidget(&web, 0, 1);

    glayout.setContentsMargins(0, 0, 0, 0);
    view.resize(width*8*1.91, (height*16 + 80));
    view.setLayout(&glayout);
    view.show();
    return app.exec();
}

