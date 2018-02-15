#ifndef USERQUEUEFORM_H
#define USERQUEUEFORM_H

#include <QWidget>

namespace Ui {
class UserQueueForm;
}
class QSqlQueryModel;
class QModelIndex;
class QString;

class UserQueueForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserQueueForm(QWidget *parent = 0);
    ~UserQueueForm();

public slots:
    void updateQueue();

protected:
    QSqlQueryModel *queueModel = nullptr;

    QString getQuery() const;

protected slots:
    void queueSelected(const QModelIndex &index);

    void editUser(const QModelIndex &index);

    void moveQueueUp();
    void moveQueueDown();
    void moveQueueFirst();
    void printQueue();

private:
    Ui::UserQueueForm *ui;
};

#endif // USERQUEUEFORM_H
