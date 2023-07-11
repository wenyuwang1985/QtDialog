#ifndef QCERTESEALDIALOG_H
#define QCERTESEALDIALOG_H

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

#include "../QtSignDialogLib.h"

struct SelectedIndexStruct
{
    int certId;
    int sealId;     //iscert=1时，sealid为印章个数
    int iscert;     //是否证书：0-印章；1-证书
    int certType;   //证书类型：0-ukey证书；1-信步云证书
};

class QCertEsealTreeItem : public QTreeWidgetItem
{
public:
    QCertEsealTreeItem(const SelectedIndexStruct& TempItem, QTreeWidget* parent);       //Cert  Item
    QCertEsealTreeItem(const SelectedIndexStruct& TempItem, QTreeWidgetItem* parent);   //Eseal Item
    ~QCertEsealTreeItem();

    SelectedIndexStruct m_CertEsealInfo;

private:

};

class QCertEsealDialog : public QDialog
{
    Q_OBJECT

public:
    QCertEsealDialog(std::vector<cert_eseal_info> *pCertEsealVector, QWidget* parent = nullptr);
    ~QCertEsealDialog();

    int GetCertIndex();
    int GetEsealIndex();
    int OnSelectOk();

    void SlotClickSelect();
    void SlotClickCancel();

    void SlotClickItem(QTreeWidgetItem*);

private:
    QTreeWidget* m_pCertEsealTree;

    QWidget* m_pImageShowCase;
    QLabel* m_pImageShowLabel;
    QLabel* m_pPreviewTextLabel;

    QPushButton* m_pSelectButton;
    QPushButton* m_pCancelButton;

    int m_iCertIndex;
    int m_iEsealIndex;

    int m_iRet;

    std::vector<cert_eseal_info> *m_pCertEsealVector;

    void SetCertEsealTree(std::vector<cert_eseal_info>* pCertEsealVector);
    void SetPreviewTextLabel();
    void SetImageShowCase();
    void SetSelectCancelButton();

    void ShowImageForLabel();
    QImage GetImageByByteArray(char* pPicBuffer, int iPicBufferSize);

    void InitCertEsealTree(const std::vector<cert_eseal_info> &vecCertEseal);
    void SetCertItemUI(QTreeWidgetItem* pCertItem, const cert_eseal_info& CertInfo, const SelectedIndexStruct& TempCertItem);
    void SetEsealItemUI(QTreeWidgetItem* pEsealItem, const eseal_info& EsealInfo);
};

#endif // QCERTESEALDIALOG_H
