#ifndef QREVOKEESEALDIALOG_H
#define QREVOKEESEALDIALOG_H

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

class QRevokeEsealDialog :
    public QDialog
{
public:
    QRevokeEsealDialog(char **ppCertNames, int iCertCount);
    ~QRevokeEsealDialog();

    void SetSelectCancelButton();
    void SetCertListSelect(char** ppCertNames, int iCertCount);

    int OnSelectOk();
    int GetCertIndex();

    void SlotClickSelect();
    void SlotClickCancel();

private:
    QLabel* m_pCertNameTipLabel;
    QComboBox* m_pCertListSelect;
    QPushButton* m_pSelectButton;
    QPushButton* m_pCancelButton;

    int m_iRet;         //用户是否确定要选择证书flag
    int m_iCertIndex;   //用户选择的证书下标
};

#endif // QREVOKEESEALDIALOG_H
