//--
// This file is part of Sonic Pi: http://sonic-pi.net
// Full project source: https://github.com/samaaron/sonic-pi
// License: https://github.com/samaaron/sonic-pi/blob/master/LICENSE.md
//
// Copyright 2013, 2014 by Sam Aaron (http://sam.aaron.name).
// All rights reserved.
//
// Permission is granted for use, copying, modification, distribution,
// and distribution of modified versions of this work as long as this
// notice is included.
//++

#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QBitmap>

#include <iostream>
#include "mainwindow.h"
#include "parsingfunctions.hpp"

#include <iostream>
using namespace std;

int game_init(int argc, char *argv[])
{
    create_apih_from_wrapper();

    QApplication *app = new QApplication(argc, argv);
    app->setStyle("windows");
    app->setAttribute(Qt::AA_NativeWindows, true);

    MainWindow * mainWin = new MainWindow();

    mainWin->show();
    int ret = app->exec();

    //Engine::set_game_window(mainWin);

    delete mainWin;
    delete app;

    return ret;
}
