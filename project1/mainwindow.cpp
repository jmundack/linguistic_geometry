#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "twoD.h"
#include "trajectory.h"
#include <QTextStream>
#include <QFile>
#include <iostream>
#include <fstream>

using namespace std;

map<string, vector<pair<int,int> > > _GenericPieces;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->xCord->setValue(8);
    ui->yCord->setValue(8);
    ui->xCordCurrent->setValue(5);
    ui->yCordCurrent->setValue(1);
    ui->show->click();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_show_clicked()
{
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

    QString filename("temp");
    {
        QFile file(filename);
        cout << ui->obstacleList->toPlainText().toStdString();

        if ( file.open(QFile::WriteOnly) ) {
            cout << "file opened for writing" << endl;
            QTextStream outStream(&file);
            outStream << ui->obstacleList->toPlainText();
        }
    }
    vector<pair<int,int> > obstacles;
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

    obj.set_obstacles( obstacles.size(), obstacles);


    const string piece(ui->peiceList->currentText().toStdString());
//    obj.compute_distances(ui->peiceList->currentText().toStdString());

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
//            if ((row == ui->yCordCurrent->value()-1) && (col == ui->xCordCurrent->value()-1))
//                val = "*";
//            else if (num == 0)
//                val = QString();
            if (num == 999)
                val = "X";
            else
                val = QString::number(num);
            QTableWidgetItem *item = new QTableWidgetItem(val);
            ui->tableWidget->setItem(row,col,item);
        }
    Trajectory t(piece, obj, 5,8);
    t.GetShortestTrajectories();
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
