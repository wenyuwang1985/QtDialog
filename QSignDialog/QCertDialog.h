#ifndef QCERTDIALOG_H
#define QCERTDIALOG_H

#include <QtCore/qglobal.h>
#include <QApplication>
#include <QTreeWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QComboBox>
#include <QImage>
#include <QString>

#include <vector>
#include <string>

#include <cstring>

class QCertDialog : public QDialog
{
    Q_OBJECT

public:
    QCertDialog(char **ppCertNames, int iCertCount, QWidget* parent = nullptr);
    ~QCertDialog();

    void SetCertListWidget(char** ppCertNames, int iCertCount);
    void SetSelectCancelButton();

    int OnSelectOk();
    int GetCertIndex();

    void SlotClickSelect();
    void SlotClickCancel();

    void SlotClickItem(QTreeWidgetItem*);

private:
    QTreeWidget* m_pCertListWidget;
    QPushButton* m_pSelectButton;
    QPushButton* m_pCancelButton;

    int m_iCertIndex;
    int m_iRet;
};

#endif // QCERTDIALOG_H
