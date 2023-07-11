#include "qtsignlib.h"
#include "QSignDialog/QCertDialog.h"
#include "QSignDialog/QCertEsealDialog.h"
#include "QSignDialog/QRevokeEsealDialog.h"
#include <exception>
#include <cstdlib>
#include <cstring>

#include <iostream>

extern "C" {
    #include "clog.h"
}
#include "cpplogex.h"

static int g_iQAppCount = 0;
static int g_iProgramType = ESS_WEB_SIGN;

#ifdef  __linux__

#include <dirent.h>

void SearchAimFile(const std::string& directory, const std::string& filename, std::string &file_path)
{
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(directory.c_str())) == nullptr) {
        return;
    }

    while ((entry = readdir(dir)) != nullptr && file_path.size() == 0) {
        if (entry->d_type == DT_REG) {// 判断是否为普通文件
            std::string filepath = directory + "/" + entry->d_name;

            if (std::strcmp(entry->d_name, filename.c_str()) == 0) {
                // 匹配到目标文件
                file_path = filepath;
                closedir(dir);
                return;
            }
        } else if (entry->d_type == DT_DIR && std::strcmp(entry->d_name, ".") != 0 && std::strcmp(entry->d_name, "..") != 0) {
            // 遍历子目录
            std::string subdir = directory + "/" + entry->d_name;
            searchFile(subdir, filename, file_path);
        }
    }

    closedir(dir);
}

bool SetEnvXAUTHORITY()
{
    std::string strXAUTHORITY = "XAUTHORITY=";

    std::string strAimFilePath;
    std::string strAimDir = "/home";
    std::string strAimFile = ".Xauthority";
    SearchAimFile(strAimDir, strAimFile, strAimFilePath);
    log_info("[SetEnvXAUTHORITY] AimFilePath : %s", strAimFilePath.c_str());
    if(strAimFilePath.size() == 0) {
        log_error("[SetEnvXAUTHORITY] AimFilePath : %s", "not found");
        return false;
    }

    strXAUTHORITY += strAimFilePath;
    if(putenv((char *)strXAUTHORITY.c_str()) != 0) {
        return false;
    }

    return true;
}

bool SetEnvLD_PATH(int iSignProgramType)
{
    switch (iSignProgramType) {
        case ESS_WEB_SIGN: {
            if(putenv("LD_LIBRARY_PATH=/opt/apps/cn.bjca.websign/files/lib/lib") != 0) {
                return false;
            }

            break;
        }

        case ESS_PDF_SIGN: {
            if(putenv("LD_LIBRARY_PATH=/opt/apps/cn.bjca.pdfsign/files/lib/lib") != 0) {
                return false;
            }

            break;
        }

        default: {
            break;
        }
    }

    return true;
}

bool SetEnvForConnectGUI(int iSignProgramType)
{
    log_info("[SetEnvForConnectGUI] %s", "start");
    //DISPLAY
    if(putenv((char *)"DISPLAY=:0") != 0) {
        log_error("[SetEnvForConnectGUI] %s", "putenv DISPLAY error");
        return false;
    }
    std::cout << std::endl;

    //XAUTHORITY
    if(SetEnvXAUTHORITY() == false) {
        log_error("[SetEnvForConnectGUI] %s", "putenv XAUTHORITY error");
        return false;
    }

    //LD_LIBRARY_PATH
    if(SetEnvLD_PATH(iSignProgramType) == false) {
        log_error("[SetEnvForConnectGUI] %s", "putenv LD_LIBRARY_PATH error");
        return false;
    }

    //QT_QPA_PLATFORM
    if(putenv((char *)"QT_QPA_PLATFORM=xcb") != 0) {
        log_error("[SetEnvForConnectGUI] %s", "putenv QT_QPA_PLATFORM error");
        return false;
    }

    log_info("[SetEnvForConnectGUI] %s", "finish");
    return true;
}

#endif  //#ifdef  __linux__

//C/C++ Interface Method Define
int InitQtSignLib(int iSignProgramType)
{
    g_iProgramType = iSignProgramType;

    //init log
    initlog();
    logheader();

    stack_trace;

    log_info("[InitQtSignLib] SignProgramType : %d", g_iProgramType);

#ifdef __linux__
    if(g_iProgramType == ESS_WEB_SIGN) {
        //网页签章是root权限服务进程启动 GUI，需要设置环境变量，使其连接到桌面
        if(SetEnvForConnectGUI(0) == false) {
            return QError::EnvSetFailed;
        }
    }
#endif

    log_info("[InitQtSignLib] %s", "finish init");

    return QError::Normally;
}

int SelectCertEseal(std::vector<cert_eseal_info> *pCertEsealVector, int *pCertIndex, int *pEsealIndex)
{
    stack_trace;

    int iRet;

    QApplication app(g_iQAppCount, nullptr);
    g_iQAppCount++;

    QCertEsealDialog SelectDlg(pCertEsealVector);
    SelectDlg.exec();

    iRet = SelectDlg.OnSelectOk();
    if(iRet == QError::Normally) {
        *pCertIndex = SelectDlg.GetCertIndex();
        *pEsealIndex = SelectDlg.GetEsealIndex();
    }

    return iRet;
}

int SelectCert(char **ppCertNames, int iCertCount, int *pCertIndex)
{
    stack_trace;

    int iRet;

    QApplication app(g_iQAppCount, nullptr);
    g_iQAppCount++;

    QCertDialog SelectDlg(ppCertNames, iCertCount);
    SelectDlg.exec();

    iRet = SelectDlg.OnSelectOk();
    if(iRet == QError::Normally) {
        *pCertIndex = SelectDlg.GetCertIndex();
    }

    return iRet;
}

int RevokeEsealSelect(char **ppCertNames, int iCertCount, int *pCertIndex)
{
    stack_trace;

    int iRet;

    QApplication app(g_iQAppCount, nullptr);
    g_iQAppCount++;

    QRevokeEsealDialog RevokeEsealDlg(ppCertNames, iCertCount);
    RevokeEsealDlg.exec();

    iRet = RevokeEsealDlg.OnSelectOk();
    if(iRet == QError::Normally)
    {
        *pCertIndex = RevokeEsealDlg.GetCertIndex();
    }

    return iRet;
}
