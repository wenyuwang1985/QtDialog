#include "QRevokeEsealDialog.h"
#include "../qtsignlib.h"

QRevokeEsealDialog::QRevokeEsealDialog(char** ppCertNames, int iCertCount)
{
    //set dialog UI
    this->setFixedSize(300, 120);
    this->setWindowTitle("请选择证书");
    this->setWindowFlags(Qt::Drawer);

    //set tip label
    m_pCertNameTipLabel = new QLabel(this);
    m_pCertNameTipLabel->resize(70, 25);
    m_pCertNameTipLabel->move(20, 20);
    m_pCertNameTipLabel->setText("证书名称");

    SetCertListSelect(ppCertNames, iCertCount);

    SetSelectCancelButton();

    m_iRet = QError::UserNotSelect;

    m_iCertIndex = -1;
}

QRevokeEsealDialog::~QRevokeEsealDialog()
{

}

void QRevokeEsealDialog::SetCertListSelect(char** ppCertNames, int iCertCount)
{
    //inti ComboBox
    m_pCertListSelect = new QComboBox(this);
    m_pCertListSelect->resize(180, 25);
    m_pCertListSelect->move(110, 20);

    for (int iCertIndex = 0; iCertIndex < iCertCount; iCertIndex++)
    {
        m_pCertListSelect->addItem(QString(ppCertNames[iCertIndex]), QVariant(iCertIndex));
    }
}

void QRevokeEsealDialog::SetSelectCancelButton()
{
    m_pSelectButton = new QPushButton(this);
    m_pSelectButton->resize(70, 25);
    m_pSelectButton->move(60, 70);
    m_pSelectButton->setText("确定");
    connect(m_pSelectButton, &QPushButton::clicked, this, &QRevokeEsealDialog::SlotClickSelect);

    m_pCancelButton = new QPushButton(this);
    m_pCancelButton->resize(70, 25);
    m_pCancelButton->move(170, 70);
    m_pCancelButton->setText("取消");
    connect(m_pCancelButton, &QPushButton::clicked, this, &QRevokeEsealDialog::SlotClickCancel);
}

int QRevokeEsealDialog::GetCertIndex()
{
    return m_iCertIndex;
}

int QRevokeEsealDialog::OnSelectOk()
{
    return m_iRet;
}

void QRevokeEsealDialog::SlotClickSelect()
{
    m_iRet = QError::Normally;

    m_iCertIndex = m_pCertListSelect->currentData().toInt();

    this->close();
}

void QRevokeEsealDialog::SlotClickCancel()
{
    this->close();
}
