#include "userdialog.h"
#include "ui_userdialog.h"

#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QSqlRelationalDelegate>
#include <QRegularExpression>
#include <QSqlRecord>

UserDialog::UserDialog(QWidget *parent, uint userID) :
    QDialog(parent),
    ui(new Ui::UserDialog)
{
    ui->setupUi(this);
    this->userID = userID;

    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen())
    {
        QMessageBox::critical(this, tr("Connecntion error"), tr("Database connection lost"));
        return;
    }

    // user model basic user information
    mainModel = new QSqlRelationalTableModel(Q_NULLPTR, db);
    // Manual sumbit used for correcting some fileds, if empty set NULL
    // for example passport seria and number must be unqiue
    mainModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mainModel->setTable("user_main");
    if (userID)
    {
        mainModel->setFilter(QString("id=%1").arg(userID));
    }

    // Set relation with user status
    indexUserID = mainModel->fieldIndex("id");
    indexUserStatus = mainModel->fieldIndex("UserStatusID");
    indexSelfAbility = mainModel->fieldIndex("SeflServiceAbility");
    indexDisability = mainModel->fieldIndex("Disability");
    indexWorkCompetence = mainModel->fieldIndex("WorkCompetence");

    mainModel->setRelation(indexUserStatus, QSqlRelation("user_status", "status_id", "title"));
    mainModel->setRelation(indexSelfAbility, QSqlRelation("user_selfability",
                                                          "ability_id", "ability"));
    mainModel->setRelation(indexDisability, QSqlRelation("user_disability",
                                                         "disability_id", "title"));
    mainModel->setRelation(indexWorkCompetence, QSqlRelation("user_work_competence",
                                                             "competence_id", "title"));

    // Try to get data
    if (userID && !mainModel->select())
    {
        showError(mainModel->lastError());
        return;
    }

    // Init userStatus combobox
    QSqlTableModel *userStatusModel = mainModel->relationModel(indexUserStatus);
    ui->userStatus->setModel(userStatusModel);
    ui->userStatus->setModelColumn(userStatusModel->fieldIndex("title"));
    ui->userStatus->setItemDelegate(new QSqlRelationalDelegate(ui->userStatus));

    QSqlTableModel *userSelfAbilityModel = mainModel->relationModel(indexSelfAbility);
    ui->selfAbility->setModel(userSelfAbilityModel);
    ui->selfAbility->setModelColumn(userSelfAbilityModel->fieldIndex("ability"));
    ui->selfAbility->setItemDelegate(new QSqlRelationalDelegate(ui->selfAbility));

    QSqlTableModel *userDisabilityModel = mainModel->relationModel(indexDisability);
    ui->disability->setModel(userDisabilityModel);
    ui->disability->setModelColumn(userDisabilityModel->fieldIndex("title"));
    ui->disability->setItemDelegate(new QSqlRelationalDelegate(this));

    QSqlTableModel *userWorkCompetenceModel = mainModel->relationModel(indexWorkCompetence);
    ui->workCompetence->setModel(userWorkCompetenceModel);
    ui->workCompetence->setModelColumn(userWorkCompetenceModel->fieldIndex("title"));
    ui->workCompetence->setItemDelegate(new QSqlRelationalDelegate(this));


    mainMapper = new QDataWidgetMapper();
    mainMapper->setModel(mainModel);
    mainMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mainMapper->setOrientation(Qt::Horizontal);
    mainMapper->setItemDelegate(new QSqlRelationalDelegate(this));

    indexBurthday = mainModel->fieldIndex("Birthday");
    indexRegister = mainModel->fieldIndex("RegisterDate");
    indexOccupation = mainModel->fieldIndex("OccupancyDate");
    indexLeave = mainModel->fieldIndex("LeaveDate");
    indexPassportSeria = mainModel->fieldIndex("PassportSeria");
    indexPassportNumber = mainModel->fieldIndex("PassportNumber");
    indexProtocolDate = mainModel->fieldIndex("ProtocolDate");
    indexExclusionDate = mainModel->fieldIndex("ExclusionDate");

    // MAIN FOLDER
    // LEFT COLUMN
    mainMapper->addMapping(ui->firstName, mainModel->fieldIndex("FirstName"));
    mainMapper->addMapping(ui->lastName, mainModel->fieldIndex("LastName"));
    mainMapper->addMapping(ui->middleName, mainModel->fieldIndex("MiddleName"));
    mainMapper->addMapping(ui->birthday, indexBurthday);
    mainMapper->addMapping(ui->senderDeparment, mainModel->fieldIndex("SenderDepartment"));
    mainMapper->addMapping(ui->currenPlace, mainModel->fieldIndex("CurrentPlace"));
    mainMapper->addMapping(ui->contactUser, mainModel->fieldIndex("ContactUser"));
    mainMapper->addMapping(ui->additionalInfo, mainModel->fieldIndex("AdditionalInfo"));

    // RIGHT COLUMN
    mainMapper->addMapping(ui->passportSeria, indexPassportSeria);
    mainMapper->addMapping(ui->passportNumber, indexPassportNumber);
    mainMapper->addMapping(ui->passportIssue, mainModel->fieldIndex("PassportIssueDate"));
    mainMapper->addMapping(ui->passportDepartment, mainModel->fieldIndex("PassportDepartment"));

    mainMapper->addMapping(ui->registerDate, indexRegister);
    mainMapper->addMapping(ui->occupationDate, indexOccupation);
    mainMapper->addMapping(ui->leaveDate, indexLeave);

    mainMapper->addMapping(ui->commissionProtocol, mainModel->fieldIndex("CommissionProtocol"));
    mainMapper->addMapping(ui->commissionDecision, mainModel->fieldIndex("CommissionDecision"));
    mainMapper->addMapping(ui->exclusionDecision, mainModel->fieldIndex("ExclusionDecision"));
    mainMapper->addMapping(ui->protocolDate, indexProtocolDate);
    mainMapper->addMapping(ui->exclusionDate, indexExclusionDate);

    // MED FOLDER
    mainMapper->addMapping(ui->medInfoEdit, mainModel->fieldIndex("MedInfo"));
    mainMapper->addMapping(ui->selfAbility, indexSelfAbility);
    mainMapper->addMapping(ui->disability, indexDisability);
    mainMapper->addMapping(ui->workCompetence, indexWorkCompetence);

    // SOC FOLDER
    mainMapper->addMapping(ui->socInfoEdit, mainModel->fieldIndex("SocInfo"));

    mainMapper->addMapping(ui->userStatus, indexUserStatus);


    if (userID == 0)
    {
        // For new user set Combobo
        mainModel->insertRow(0);
        ui->userStatus->setCurrentIndex(0);
        ui->selfAbility->setCurrentIndex(0);
        ui->disability->setCurrentIndex(0);
        ui->workCompetence->setCurrentIndex(0);

        ui->medDocumentsList->setDisabled(true);
        ui->medAdd->setDisabled(true);
        ui->socDocumentsList->setDisabled(true);
        ui->socAdd->setDisabled(true);

    }
    mainMapper->toFirst();

    ui->userIdL->setText(QVariant(userID).toString());

    connect(ui->saveButton, SIGNAL(clicked(bool)),
            this, SLOT(saveUser()));

    connect(ui->removeButton, SIGNAL(clicked(bool)),
            this, SLOT(removeUser()));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(initMedFolder(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(initSocFolder(int)));
}



UserDialog::~UserDialog()
{
    if (mainMapper != NULL)
    {
        delete mainMapper;
    }
    if (mainModel != NULL)
    {
        delete mainModel;
    }
    if (medModel != NULL)
    {
        delete medModel;
    }
    if (socModel != NULL)
    {
        delete socModel;
    }
    delete ui;
}



void UserDialog::showError(const QSqlError &err)
{
    QMessageBox::critical(this, tr("Error in connection"),
                tr("Error initializing database: ") + err.text());
}



void UserDialog::trimInput(QLineEdit *edit)
{
    edit->setText(edit->text().simplified());
}


void UserDialog::trimInput(QTextEdit *edit)
{
    edit->setPlainText(edit->toPlainText().trimmed());
}



void UserDialog::trimInputs()
{
    trimInput(ui->firstName);
    trimInput(ui->lastName);
    trimInput(ui->middleName);
    trimInput(ui->senderDeparment);
    trimInput(ui->currenPlace);
    trimInput(ui->contactUser);

    trimInput(ui->passportDepartment);
    trimInput(ui->additionalInfo);

    trimInput(ui->commissionProtocol);
    trimInput(ui->commissionDecision);
    trimInput(ui->exclusionDecision);

    trimInput(ui->medInfoEdit);
    trimInput(ui->socInfoEdit);
}



void UserDialog::setNullEmptyData()
{
    int row = mainMapper->currentIndex();

    if (ui->birthday->date() == QDate(1900,1,1))
    {
           mainModel->setData(mainModel->index(row, indexBurthday), QVariant());
    }
    if (ui->registerDate->date() == QDate(1900,1,1))
    {
           mainModel->setData(mainModel->index(row, indexRegister), QVariant());
    }
    if (ui->occupationDate->date() == QDate(1900,1,1))
    {
           mainModel->setData(mainModel->index(row, indexOccupation), QVariant());
    }
    if (ui->leaveDate->date() == QDate(1900,1,1))
    {
           mainModel->setData(mainModel->index(row, indexLeave), QVariant());
    }
    if (ui->protocolDate->date() == QDate(1900,1,1))
    {
        mainModel->setData(mainModel->index(row, indexProtocolDate), QVariant());
    }
    if (ui->exclusionDate->date() == QDate(1900,1,1))
    {
        mainModel->setData(mainModel->index(row, indexExclusionDate), QVariant());
    }
    if (ui->passportSeria->text().isEmpty())
    {
        mainModel->setData(mainModel->index(row, indexPassportSeria), QVariant(QVariant::Int));
    }
    if (ui->passportNumber->text().isEmpty())
    {
        mainModel->setData(mainModel->index(row, indexPassportNumber), QVariant(QVariant::Int));
    }
}



void UserDialog::saveUser()
{
    ui->resultL->setText(QString(""));

    trimInputs();
    bool mapper = mainMapper->submit();
    setNullEmptyData();

    QSqlError err = mainModel->lastError();
    if (!mapper || err.isValid())
    {
        QString error = err.text();
        ui->resultL->setText(error);
        return;
    }
    if (!mainModel->submitAll())
    {
        QString error = mainModel->lastError().text();
        ui->resultL->setText(error);
        return;
    }

    if (!saveMedDocuments())
    {
        return;
    }
    this->done(QDialog::Accepted);
}



bool UserDialog::saveMedDocuments()
{
    // Do nothing on new user
    if (!userID || medModel == nullptr)
    {
        return true;
    }
    // Remove empty rows, where category not set
    for (int i = medModel->rowCount() - 1; i >=0; --i)
    {
        QModelIndex index = medModel->index(i, indexDocCategory);
        if (medModel->data(index).isNull())
        {
            medModel->removeRow(i);
        }
    }
    if (!medModel->submitAll())
    {
        QString error = medModel->lastError().text();
        ui->resultL->setText(error);
        return false;
    }
    return true;
}



void UserDialog::removeUser()
{
    bool remove = mainModel->removeRow(0);
    remove = remove && mainModel->submitAll();
    if (!remove)
    {
        QSqlError err = mainModel->lastError();
        ui->resultL->setText(err.text());
    }
    this->done(QDialog::Accepted);
}



void UserDialog::initMedFolder(int folderIndex)
{
    // Do nothing if model initialised
    if (!userID || folderIndex != 1 || medModel != NULL)
    {
        return;
    }
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        QMessageBox::critical(this, tr("Connecntion error"), tr("Database connection lost"));
        return;
    }

    medModel = new QSqlRelationalTableModel(Q_NULLPTR, db);
    medModel->setTable("user_documents");
    medModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    medModel->setFilter(QString("type=\"med\" AND user_id=%1").arg(userID));

    int indexId = medModel->fieldIndex("id");
    indexDocUserID = medModel->fieldIndex("user_id");
    indexDocType = medModel->fieldIndex("type");
    indexDocCategory = medModel->fieldIndex("category_id");

    medModel->setRelation(indexDocCategory, QSqlRelation("med_doc_category",
                                                         "category_id", "category"));

    // Try to get data
    if (!medModel->select())
    {
        showError(medModel->lastError());
        ui->medAdd->setDisabled(true);
        ui->medRemove->setDisabled(true);
        return;
    }
    medModel->setHeaderData(indexDocCategory, Qt::Horizontal, tr("category"));
    medModel->setHeaderData(medModel->fieldIndex("name"), Qt::Horizontal, tr("name"));
    medModel->setHeaderData(medModel->fieldIndex("number"), Qt::Horizontal, tr("number"));
    medModel->setHeaderData(medModel->fieldIndex("register_date"), Qt::Horizontal, tr("register date"));
    medModel->setHeaderData(medModel->fieldIndex("expire_date"), Qt::Horizontal, tr("expire date"));
    medModel->setHeaderData(medModel->fieldIndex("description"), Qt::Horizontal, tr("description"));
    medModel->setHeaderData(medModel->fieldIndex("comment"), Qt::Horizontal, tr("comment"));

    ui->medDocumentsList->setModel(medModel);
    ui->medDocumentsList->setItemDelegate(new QSqlRelationalDelegate(ui->medDocumentsList));
    ui->medDocumentsList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->medDocumentsList->setSelectionBehavior(QAbstractItemView::SelectItems);

    ui->medDocumentsList->setColumnHidden(indexId, true);
    ui->medDocumentsList->setColumnHidden(indexDocType, true);
    ui->medDocumentsList->setColumnHidden(indexDocUserID, true);

    connect(ui->medAdd, SIGNAL(clicked(bool)), this, SLOT(addMedRow()));
    connect(ui->medRemove, SIGNAL(clicked(bool)), this, SLOT(removeMedDocument()));
    connect(ui->medDocumentsList, SIGNAL(pressed(QModelIndex)), this, SLOT(enableMedRemoveButton()));
}



void UserDialog::initSocFolder(int folderIndex)
{
    // Do nothing if model initialised
    if (folderIndex != 2 || socModel != NULL)
    {
        return;
    }
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        QMessageBox::critical(this, tr("Connecntion error"), tr("Database connection lost"));
        return;
    }
//    connect(ui->socAdd, SIGNAL(clicked(bool)), this, SLOT(addSocRow()));
}



void UserDialog::addMedRow()
{
    medModel->insertRow(0);
    medModel->setData(medModel->index(0, indexDocUserID), QVariant(userID));
    medModel->setData(medModel->index(0, indexDocType), QVariant("med"));
    ui->medDocumentsList->selectRow(0);
}



void UserDialog::addSocRow()
{
    socModel->insertRow(0);
}



void UserDialog::enableMedRemoveButton()
{
    ui->medRemove->setEnabled(true);
}



void UserDialog::removeMedDocument()
{
    QModelIndex index =  ui->medDocumentsList->currentIndex();
    medModel->removeRow(index.row());
}