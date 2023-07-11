#include "QCertEsealDialog.h"
#include "../qtsignlib.h"

//QCertEsealDialog Method
QCertEsealDialog::QCertEsealDialog(std::vector<cert_eseal_info>* pCertEsealVector, QWidget* parent)
    : QDialog(parent)
{
    //设置Dialog
    this->setFixedSize(600, 400);
    this->setWindowTitle(QString("请选择印章"));
    this->setWindowFlags(Qt::Drawer);

    //设置选择Tree
    SetCertEsealTree(pCertEsealVector);
    //设置“预览”Text提示框
    SetPreviewTextLabel();
    //设置图片预览框
    SetImageShowCase();
    //设置Button
    SetSelectCancelButton();

    m_iCertIndex = -1;
    m_iEsealIndex = -1;
    m_iRet = QError::UserNotSelect;
}

QCertEsealDialog::~QCertEsealDialog()
{

}

void QCertEsealDialog::SetCertEsealTree(std::vector<cert_eseal_info>* pCertEsealVector)
{
    m_pCertEsealTree = new QTreeWidget(this);
    m_pCertEsealTree->resize(280, 320);
    m_pCertEsealTree->move(10, 10);

    m_pCertEsealVector = pCertEsealVector;
    InitCertEsealTree(*m_pCertEsealVector);
}

void QCertEsealDialog::SetPreviewTextLabel()
{
    m_pPreviewTextLabel = new QLabel(this);
    m_pPreviewTextLabel->resize(240, 20);
    m_pPreviewTextLabel->move(330, 10);
    QFont font("Microsoft YaHei", 13, 50);
    m_pPreviewTextLabel->setFont(font);
    m_pPreviewTextLabel->setText("预览");
}

void QCertEsealDialog::SetImageShowCase()
{
    m_pImageShowCase = new QWidget(this);
    m_pImageShowCase->resize(290, 300);
    m_pImageShowCase->move(300, 35);
    m_pImageShowLabel = new QLabel(m_pImageShowCase);
    m_pImageShowLabel->resize(290, 300);

    connect(m_pCertEsealTree, &QTreeWidget::itemClicked, this, &QCertEsealDialog::SlotClickItem);
}

void QCertEsealDialog::SetSelectCancelButton()
{
    m_pSelectButton = new QPushButton(this);
    m_pSelectButton->resize(70, 25);
    m_pSelectButton->move(360, 340);
    m_pSelectButton->setText("确定");
    connect(m_pSelectButton, &QPushButton::clicked, this, &QCertEsealDialog::SlotClickSelect);

    m_pCancelButton = new QPushButton(this);
    m_pCancelButton->resize(70, 25);
    m_pCancelButton->move(m_pSelectButton->width() + 390, 340);
    m_pCancelButton->setText("取消");
    connect(m_pCancelButton, &QPushButton::clicked, this, &QCertEsealDialog::SlotClickCancel);
}

int QCertEsealDialog::GetCertIndex()
{
    return m_iCertIndex;
}

int QCertEsealDialog::GetEsealIndex()
{
    return m_iEsealIndex;
}

int QCertEsealDialog::OnSelectOk()
{
    return m_iRet;
}

void QCertEsealDialog::SlotClickSelect()
{
    if(!(m_iCertIndex == -1 && m_iEsealIndex == -1))    //防止用户不选择直接点确定
    {
        m_iRet = QError::Normally;
    }

    this->close();
}

void QCertEsealDialog::SlotClickCancel()
{
    this->close();
}

void QCertEsealDialog::SlotClickItem(QTreeWidgetItem* pParentItem)
{
    QCertEsealTreeItem* pItem = static_cast<QCertEsealTreeItem*>(pParentItem);

    if (pItem->m_CertEsealInfo.iscert == 1)     //Cert
    {
        if (pItem->m_CertEsealInfo.certType == 1)       //信步云证书
        {
            m_iCertIndex = pItem->m_CertEsealInfo.certId;
            m_iEsealIndex = -1;
            m_pImageShowLabel->setText("选定此证书将进行扫码签章！");
        }
        else if (pItem->m_CertEsealInfo.certType == 0)   //UKey证书
        {
            m_iCertIndex = -1;
            m_iEsealIndex = -1;
            m_pImageShowLabel->setText(QString("此证书包含印章%1个！").arg(pItem->m_CertEsealInfo.sealId));
        }
    }
    else                                        //Eseal
    {
        m_iCertIndex = pItem->m_CertEsealInfo.certId;
        m_iEsealIndex = pItem->m_CertEsealInfo.sealId;

        ShowImageForLabel();
    }

    m_pImageShowLabel->setAlignment(Qt::AlignCenter);
}

void QCertEsealDialog::ShowImageForLabel()
{
    QImage image = GetImageByByteArray((char *)(*m_pCertEsealVector)[m_iCertIndex].vec_eseals[m_iEsealIndex].pic.c_str(),
                                       (*m_pCertEsealVector)[m_iCertIndex].vec_eseals[m_iEsealIndex].pic_len);
    QImage ScaledImage;

    int iPicWidth  = (*m_pCertEsealVector)[m_iCertIndex].vec_eseals[m_iEsealIndex].width;
    int iPicHeight = (*m_pCertEsealVector)[m_iCertIndex].vec_eseals[m_iEsealIndex].height;

    if(iPicWidth > 0 && iPicHeight > 0)
    {
        ScaledImage = image.scaled(iPicWidth, iPicHeight);
        m_pImageShowLabel->setPixmap(QPixmap::fromImage(image));
    }
    else if (image.width() <= 280 && image.height() <= 300)
    {
        m_pImageShowLabel->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        ScaledImage = image.scaled(m_pImageShowLabel->width(), m_pImageShowLabel->height(), Qt::KeepAspectRatio);
        m_pImageShowLabel->setPixmap(QPixmap::fromImage(ScaledImage));
    }
}

QImage QCertEsealDialog::GetImageByByteArray(char* pPicBuffer, int iPicBufferSize)
{
    if (pPicBuffer == nullptr || iPicBufferSize <= 0)
    {
        return QImage();
    }

    QImage image;
    image.loadFromData((unsigned char *)pPicBuffer, iPicBufferSize);
    return image;
}

void QCertEsealDialog::InitCertEsealTree(const std::vector<cert_eseal_info>& vecCertEseal)
{
    m_pCertEsealTree->setHeaderLabel("请选择印章");

    int iCertSize = vecCertEseal.size();
    int iEsealSize;
    for (int iCertIndex = 0; iCertIndex < iCertSize; iCertIndex++)
    {
        SelectedIndexStruct TempCertItem;
        TempCertItem.iscert = 1;
        TempCertItem.certType = vecCertEseal[iCertIndex].cert_type;
        TempCertItem.certId = iCertIndex;
        TempCertItem.sealId = vecCertEseal[iCertIndex].vec_eseals.size();

        QTreeWidgetItem* pCertItem = new QCertEsealTreeItem(TempCertItem, m_pCertEsealTree);
        SetCertItemUI(pCertItem, vecCertEseal[iCertIndex], TempCertItem);

        iEsealSize = vecCertEseal[iCertIndex].vec_eseals.size();
        for (int iEsealIndex = 0; iEsealIndex < iEsealSize; iEsealIndex++)
        {
            SelectedIndexStruct TempEsealItem;
            TempEsealItem.iscert = 0;
            TempEsealItem.certId = iCertIndex;
            TempEsealItem.sealId = iEsealIndex;

            QTreeWidgetItem* pEsealItem = new QCertEsealTreeItem(TempEsealItem, pCertItem);
            SetEsealItemUI(pEsealItem, vecCertEseal[iCertIndex].vec_eseals[iEsealIndex]);
        }
    }

    m_pCertEsealTree->expandAll();
}

void QCertEsealDialog::SetCertItemUI(QTreeWidgetItem* pCertItem, const cert_eseal_info& CertInfo,
                                                                 const SelectedIndexStruct& TempCertItem)
{
    QString strTempCertName = "证书:";
    strTempCertName.append(CertInfo.cert_name.c_str());
    pCertItem->setText(0, strTempCertName);
    pCertItem->setToolTip(0, "证书");
    if (TempCertItem.certType == 0)
    {
        pCertItem->setIcon(0, QIcon(":QCertEsealDialog/resource/icon/USB_app_flash_drive_icon.png"));
    }
    else if (TempCertItem.certType == 1)
    {
        pCertItem->setIcon(0, QIcon(":/QCertEsealDialog/resource/icon/upload_cloud_icon.png"));
    }
}

void QCertEsealDialog::SetEsealItemUI(QTreeWidgetItem* pEsealItem, const eseal_info& EsealInfo)
{
    pEsealItem->setText(0, EsealInfo.file_name.c_str());
    pEsealItem->setToolTip(0, "印章");
    pEsealItem->setIcon(0, QIcon(":/QCertEsealDialog/resource/icon/stamp_icon.png"));
}



//QCertEsealTreeItem Method
QCertEsealTreeItem::QCertEsealTreeItem(const SelectedIndexStruct& TempItem, QTreeWidget* parent)
    : QTreeWidgetItem(parent)
{
    m_CertEsealInfo.iscert = TempItem.iscert;
    m_CertEsealInfo.certType = TempItem.certType;
    m_CertEsealInfo.certId = TempItem.certId;
    m_CertEsealInfo.sealId = TempItem.sealId;
}

QCertEsealTreeItem::QCertEsealTreeItem(const SelectedIndexStruct& TempItem, QTreeWidgetItem* parent)
    : QTreeWidgetItem(parent)
{
    m_CertEsealInfo.iscert = TempItem.iscert;
    m_CertEsealInfo.certId = TempItem.certId;
    m_CertEsealInfo.sealId = TempItem.sealId;
}

QCertEsealTreeItem::~QCertEsealTreeItem()
{

}
