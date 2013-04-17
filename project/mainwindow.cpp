#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "twoD.h"
#include "trajectory.h"
#include "plot.h"
#include "grammarofzones.h"
#include <QTextStream>
#include <QFile>
#include <iostream>
#include <fstream>
#include "searches.h"

using namespace std;

map<string, vector<pair<int,int> > > _GenericPieces;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->xCord->setValue(8);
    ui->yCord->setValue(8);
#if 1
    ui->xCordCurrent->setValue(7);
    ui->yCordCurrent->setValue(3);
    ui->xCordEnd->setValue(5);
    ui->yCordEnd->setValue(2);
#endif
    ui->xCordCurrent->hide();
    ui->yCordCurrent->hide();
    ui->xCordEnd->hide();
    ui->yCordEnd->hide();
    ui->peiceList->hide();

    Searches s;
    s.Start();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_show_clicked()
{
    vector<pair<int,int> > obstacles;
    if (ui->show->text() == "Mark Reachability")
    {
        ui->show->setText("Save");
        ui->tableWidget->setRowCount(ui->yCord->value());
        ui->tableWidget->setColumnCount(ui->xCord->value());
        for (int i = 0; i < ui->yCord->value(); i++)
           ui->tableWidget->setColumnWidth(i,ui->tableWidget->rowHeight(0));

        for (int row = 0; row < ui->xCord->value(); row++)
            for (int col = 0; col < ui->yCord->value(); col++)
            {
                QString v;
                if (ui->xCordCurrent->value()-1 == row && ui->yCordCurrent->value()-1 == col)
                    v = "*";
                QTableWidgetItem *item = new QTableWidgetItem(v);
                ui->tableWidget->setItem(row,col,item);
            }
   }
    else if (ui->show->text() == "Save")
    {
        vector<pair<int, int> > reachability;
        reachability.push_back(make_pair(ui->xCordCurrent->value()-1, ui->yCordCurrent->value()-1));
        for (int row = 0; row < ui->tableWidget->rowCount(); row++)
            for (int col = 0; col < ui->tableWidget->columnCount(); col++)
            {
                if (ui->tableWidget->item(row,col)->text() == "1")
                {
                    reachability.push_back(make_pair(row,col));
                }
            }
        stringstream s;
        s << "Generic Peice " << _GenericPieces.size() +1;
        _GenericPieces[s.str()] = reachability;
        ui->peiceList->addItem(QString(s.str().c_str()));
        ui->show->setText("Show");
    }
    else
    {
       twoD obj (ui->yCord->value(), ui->xCord->value(), ui->xCordCurrent->value(), ui->yCordCurrent->value(), _GenericPieces);

       {
           QString filename("obstacles");
           {
               QFile file(filename);
               cout << ui->obstacleList->toPlainText().toStdString();

               if ( file.open(QFile::WriteOnly) ) {
                   cout << "file opened for writing" << endl;
                   QTextStream outStream(&file);
                   outStream << ui->obstacleList->toPlainText();
               }
           }
           {
               ifstream file(filename);
               cout << "file opened for reading" << endl;
               while(file.good() && !file.eof())
               {
                   int x,y;
                   file >> x;
                   if (file.good())
                   {
                       file >> y;
                       obstacles.push_back(make_pair(x,y));
                       cout << "Added obstacle at " << x << "," << y << endl;
                   }
               }
           }
       }

       obj.set_obstacles( obstacles.size(), obstacles);


       const string piece(ui->peiceList->currentText().toStdString());

       ui->tableWidget->setRowCount(ui->yCord->value());
       ui->tableWidget->setColumnCount(ui->xCord->value());
       for (int i = 0; i < ui->yCord->value(); i++)
          ui->tableWidget->setColumnWidth(i,ui->tableWidget->rowHeight(0));

       const twoD::Array2D &data(obj.GetData());
       for (size_t row = 0; row < data.size(); row++)
          for (size_t col = 0; col < data.at(row).size(); col++)
          {
             const size_t num(data.at(row).at(col));
             QString val;
             if (num == 999)
                val = "X";
             else
                val = QString::number(num);
             QTableWidgetItem *item = new QTableWidgetItem(val);
             ui->tableWidget->setItem(row,col,item);
          }
       ui->tableWidget->hide();
       cout << "Generating Trajectories" << endl;
       cout << "*** generating zone: " << endl;
       vector<char> pieces(ui->xCord->value() * ui->yCord->value(),' ');
       U u(0,0,0);

       {
           QString filename("piece_list");
           {
               QFile file(filename);
               cout << ui->listOfPieces->toPlainText().toStdString();

               if ( file.open(QFile::WriteOnly) ) {
                   cout << "file opened for writing" << endl;
                   QTextStream outStream(&file);
                   outStream << ui->listOfPieces->toPlainText();
               }
           }
           {
               ifstream file(filename);
               cout << "file opened for reading" << endl;
               int counter(0);
               while(file.good() && !file.eof())
               {
                   int x,y;
                   char c;
                   file >> c;
                   if (!file.good()) break;
                   file >> x;
                   if (!file.good()) break;
                   file >> y;
                   int val = (y-1) * ui->xCord->value() + (x -1);
                   pieces.at(val) = c;
                   counter ++;
                   cout << "Added piece " << c << "at " << val << endl;
                   if (counter == 1)
                      u.x = val;
                   else if (counter == 2)
                      u.y = val;
               }
           }
       }

       for (size_t i = 0; i < obstacles.size(); i++)
       {
           int val = (obstacles.at(i).second -1) * ui->xCord->value() + (obstacles.at(i).first -1);
           pieces.at(val) = 'X';
       }
       GrammarOfZones gz(u,pieces,ui->xCord->value(), obstacles);
       vector<string> zones = gz.GetZone();
       cout << "Zones : " ;
       for (size_t i = 0; i < zones.size(); i++)
          cout << zones.at(i) << "-->";
       cout << endl;
       cout << "*** DONE generating zone: " << endl;
    }
}


void MainWindow::on_peiceList_currentIndexChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        ui->show->setText("Mark Reachability");
    }
    else
        ui->show->setText("Show");
}

