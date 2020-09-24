#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString GetCmd(bool showmsg, QString command, QStringList arg, bool showmsg_error,bool write_errors_only);
    void SetThumb(QString name);
    void ClickThumb(QString name);
    void ReadJSON(QString json);
    void CreateShortcut(QString json);
    QString AppendHomePath(QString path);
private slots:
    void click_button();
    void click_start();
    void indexChanged(int index);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
