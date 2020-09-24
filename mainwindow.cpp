#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStyle"

#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"

#include "QMessageBox"
#include "QProcess"
#include "QPixmap"
#include "QDesktopServices"
#include "QUrl"
#include "QFile"
#include "QTextStream"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle("Non Steam Game Shortcut Maker");
    this->setWindowIcon(QIcon(":/ShortCutMaker.png"));

    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),this->rect())
        );

    //SetThumb("uplay");
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(click_button()));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(indexChanged(int)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(click_start()));

    QStringList a;
    a<<"-lj";
    QString msg = GetCmd(false,"/usr/games/lutris",a,false,false);
    ReadJSON(msg);
}

QString MainWindow::AppendHomePath(QString path)
{
    QString p = "/home/";

    QString name = qgetenv("USER");
        if (name.isEmpty())
            name = qgetenv("USERNAME");

     p+=name;
     p+="/";
     p+=path;

    return p;
}

void MainWindow::CreateShortcut(QString json)
{
    QString val = "{";
    val+=json;
    val+="}";

    QString data ="#!/bin/bash\n";
    data+="#used \"lutris -d lutris:rungame/";
    data+=this->ui->comboBox->currentText();
    data+="\" to retrieve env variables\n";
    data+="\n\n\n";

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

    //QJsonObject obj_D = d.;
    QJsonObject obj = d["env"].toObject();
    QJsonArray array = d["command"].toArray();
    //QMessageBox::information(this,"test",obj["WINEARCH"].toString());

    foreach(const QString & key, obj.keys())
    {
        data+="export ";
        data+=key;
        data+="=\"";
        data+=obj[key].toString();
        data+="\"\n";
    }

    if(array.count()>=2)
    {
        data+="LUTRIS_WINE=\"";
        data+=array[0].toString();
        data+="\"\n";
        data+="GAME_EXE=\"";
        data+=array[1].toString();
        data+="\"\n";

        if(array.count()==3)
        {
            data+="GAME_ARGS=\"";
            data+=array[2].toString();
            data+="\"\n";
        }

        data+="\n\n";
        data+="\"$LUTRIS_WINE\" \"$GAME_EXE\"";

        if(array.count()==3)
        {
            data+=" \"$GAME_ARGS\"";
        }

    }

    //QMessageBox::information(this,"test",data);

    QFile file(AppendHomePath(this->ui->comboBox->currentText()+".sh"));

    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stre(&file);
        stre << data;
        //stre<<"Shit";
    }

    QMessageBox::information(this,"Finished!","Shortcut is inside your home directory.");
}

void MainWindow::click_start()
{
    this->ui->pushButton->setText("Wait");

    QString cmd = "lutris:rungame/";
    cmd+=this->ui->comboBox->currentText();

    QStringList args;
    args<<"-d"<<cmd;
    //args<<cmd;

    QString result = GetCmd(false,"/usr/games/lutris",args,false,true);
    //QMessageBox::warning(this,"Error",result);
    QString env = result.split(":{")[1];
    env = env.split("}\nDEBUG")[0];

    //QMessageBox::warning(this,"Error",env);

    CreateShortcut(env);

    /*
    QFile file("/home/john/TEST.txt");

    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stre(&file);
        stre << env;
        //stre<<"Shit";
    }*/

    this->ui->pushButton->setText("Create");
}

void MainWindow::click_button()
{
    ClickThumb(this->ui->comboBox->currentText());
}

void MainWindow::indexChanged(int index)
{
    //QMessageBox::information(this,"",this->ui->comboBox->currentText());
    SetThumb(this->ui->comboBox->currentText());
}

void MainWindow::ClickThumb(QString name)
{
    QString url = "https://lutris.net/games/";
    url += name;
    url+="/";

    bool result = QDesktopServices::openUrl ( QUrl(url) );
//QMessageBox::warning(this,"Error",QString::number(result));
    if(!result)
        QMessageBox::warning(this,"Error","Invalid Game URL");
}

void MainWindow::ReadJSON(QString json)
{
      QString val = json;
      //qWarning() << val;
      QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

      QJsonArray array = d.array();
      //QJsonArray array = obj[""].toArray();

      foreach (const QJsonValue & value, array) {
             QJsonObject obj = value.toObject();
             this->ui->comboBox->addItem(obj["slug"].toString());
             //QMessageBox::warning(this,"Error",obj["slug"].toString());
             //propertyNames.append(obj["PropertyName"].toString());
             //propertyKeys.append(obj["key"].toString());
      }

}

void MainWindow::SetThumb(QString name)
{    
    QString banner = AppendHomePath(".local/share/lutris/banners/");
    banner+=name;
    banner+=".jpg";
    QPixmap pix;
    bool msg = pix.load(banner);
    QImage newImage = QImage(banner);

    QIcon ButtonIcon = QIcon(pix);

    //QImageReader reader = QImage(banner);
    //reader.setAutoTransform(true);

    //QMessageBox::information(this,QString::number(test),banner);

    this->ui->pushButton_2->setIcon(ButtonIcon);
    this->ui->pushButton_2->setIconSize(pix.rect().size());

    if(!msg)
    {
        QMessageBox::warning(this,"Error","Invalid Image Path");
    }
}

QString MainWindow::GetCmd(bool showmsg, QString command, QStringList arg, bool showmsg_error,bool write_errors_only)
{
        QProcess OProcess;
        QString Command;    //Contains the command to be executed
        QStringList args;   //Contains arguments of the command

        Command = command;
        //this->showMinimized();
        args=arg;

        OProcess.start(Command,args,QIODevice::ReadOnly); //Starts execution of command        
        OProcess.waitForFinished();                       //Waits for execution to complete

        QString StdOut      =   OProcess.readAllStandardOutput();  //Reads standard output
        QString StdError    =   OProcess.readAllStandardError();   //Reads standard error

        if(showmsg)
        {
            QMessageBox::information(this,command,StdOut);// + "\n######Errors######\n" + StdError);            
        }

        if(showmsg_error)
            QMessageBox::information(this,"ERRORs","\n######Errors######\n" + StdError);

        if(write_errors_only)
            return StdError;

        return  StdOut;
}

MainWindow::~MainWindow()
{
    delete ui;
}

