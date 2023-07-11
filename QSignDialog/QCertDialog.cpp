#include "QCertDialog.h"
#include "../qtsignlib.h"

QCertDialog::QCertDialog(char** ppCertNames, int iCertCount, QWidget* parent)
    : QDialog(parent)
{
    //设置Dialog
    this->setFixedSize(300, 375);
    this->setWindowTitle(QString("请选择证书"));
    this->setWindowFlags(Qt::Drawer);

    //设置选择List
    SetCertListWidget(ppCertNames, iCertCount);
    //设置button
    SetSelectCancelButton();
    //绑定用户选择Item信号 槽函数
    connect(m_pCertListWidget, &QTreeWidget::itemClicked, this, &QCertDialog::SlotClickItem);

    m_iCertIndex = -1;
    m_iRet = QError::UserNotSelect;
}

QCertDialog::~QCertDialog()
{}

void QCertDialog::SetCertListWidget(char** ppCertNames, int iCertCount)
{
    m_pCertListWidget = new QTreeWidget(this);
    m_pCertListWidget->resize(280, 300);
    m_pCertListWidget->move(10, 10);

    m_pCertListWidget->setHeaderLabel("请选择证书");

    for (int iCertIndex = 0; iCertIndex < iCertCount; iCertIndex++)
    {
        QTreeWidgetItem* pCertItem = new QTreeWidgetItem(m_pCertListWidget);
        QString qstrTempCertName = "证书:";
        qstrTempCertName.append(ppCertNames[iCertIndex]);
        pCertItem->setText(0, qstrTempCertName);
        pCertItem->setToolTip(iCertIndex, "");
    }
}

void QCertDialog::SetSelectCancelButton()
{
    m_pSelectButton = new QPushButton(this);
    m_pSelectButton->resize(70, 25);
    m_pSelectButton->move(60, 330);
    m_pSelectButton->setText("确定");
    connect(m_pSelectButton, &QPushButton::clicked, this, &QCertDialog::SlotClickSelect);

    m_pCancelButton = new QPushButton(this);
    m_pCancelButton->resize(70, 25);
    m_pCancelButton->move(170, 330);
    m_pCancelButton->setText("取消");
    connect(m_pCancelButton, &QPushButton::clicked, this, &QCertDialog::SlotClickCancel);
}

int QCertDialog::GetCertIndex()
{
    return m_iCertIndex;
}

int QCertDialog::OnSelectOk()
{
    return m_iRet;
}

void QCertDialog::SlotClickSelect()
{
    m_iRet = QError::Normally;
    if(m_iCertIndex == -1) {
        m_iCertIndex = 0;
    }

    this->close();
}

void QCertDialog::SlotClickCancel()
{
    this->close();
}

void QCertDialog::SlotClickItem(QTreeWidgetItem* pItem)
{
    m_iCertIndex = pItem->columnCount() - 1;    //从1开始
}
