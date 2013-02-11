#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <fstream>
#include <QtDebug>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

namespace
{
   void _InitScript()
   {
      {
         ofstream out("plot.sh");
         out << "#!/bin/bash" << endl;
         out << "echo \"Running gnuplot\"" << endl;
         out << "echo \"set term jpeg\" > profile.gnu" << endl;
         out << "echo \"set out \'$1\'\" >> profile.gnu" << endl;
         out << "echo \"splot \'$2\' with lines title \'move1\'\" >> profile.gnu" << endl;
         out << "gnuplot < profile.gnu" << endl;
         out << "echo \"Done running gnuplot\"" << endl;
      }
      chmod("plot.sh", S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
   }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _InitScript();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Reload_clicked()
{
    static bool temp(false);
    string inputFilename("input.dat");
    if (ui->move->toPlainText().isEmpty())
    {
        ui->trajectories->setText("Please input data below");
        return;
    }
    ui->trajectories->setText("Loading");
    {
        cout << "1" << endl;
        QString filename(inputFilename.c_str());
        QFile file(filename);
        cout << ui->move->toPlainText().toStdString();

        if ( file.open(QFile::WriteOnly) ) {
            cout << "file opened" << endl;
            QTextStream outStream(&file);
            outStream << ui->move->toPlainText();
        }
        file.close();
    }
    temp = !temp;
    string outFilename(temp? "one.jpg" : "two.jpg");
    cout << "3" << endl;
    string cmd = "./plot.sh " + outFilename + " " + inputFilename;
    system(cmd.c_str());
    cout << "4" << endl;
    sleep(1);
    QPixmap p(outFilename.c_str());

    ui->trajectories->setPixmap(p);
    ui->trajectories->resize(p.size());
}
