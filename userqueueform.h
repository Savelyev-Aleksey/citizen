#ifndef USERQUEUEFORM_H
#define USERQUEUEFORM_H

#include <QWidget>

namespace Ui {
class UserQueueForm;
}
class QSqlQueryModel;

class UserQueueForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserQueueForm(QWidget *parent = 0);
    ~UserQueueForm();

protected:
    QSqlQueryModel *queueModel = nullptr;

private:
    Ui::UserQueueForm *ui;
};

#endif // USERQUEUEFORM_H
