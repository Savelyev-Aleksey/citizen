#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class UserQueueForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    UserQueueForm *queueForm;

    bool initConnection() const;

private slots:
    void tabChanged(int tabIndex);
};

#endif // MAINWINDOW_H
