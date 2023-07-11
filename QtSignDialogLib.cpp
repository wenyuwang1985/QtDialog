#include "QtSignDialogLib.h"
#include "qtsignlib.h"

extern "C" {
    #include "clog.h"
}
#include "cpplogex.h"

int init_lib(int iSignProgramType)
{
    if(iSignProgramType != ESS_WEB_SIGN && iSignProgramType != ESS_OFD_SIGN) {
        return QError::ParamIlegal;
    }

    return InitQtSignLib(iSignProgramType);
}

void exit_lib()
{
    exitlog();
}

int select_cert_eseal(void *vecCertEseal, int *pCertIndex, int *pEsealIndex)
{
    if(vecCertEseal == nullptr || pCertIndex == nullptr || pEsealIndex == nullptr) {
        return QError::ParamIlegal;
    }

    std::vector<cert_eseal_info> *pCertEseal = reinterpret_cast<std::vector<cert_eseal_info> *>(vecCertEseal);
    return SelectCertEseal(pCertEseal, pCertIndex, pEsealIndex);
}

int select_cert(char **ppCertNames, int iCertCount, int *pCertIndex)
{
    if(ppCertNames == nullptr || pCertIndex == nullptr) {
        return QError::ParamIlegal;
    }

    return SelectCert(ppCertNames, iCertCount, pCertIndex);
}

int select_revoke_eseal(char **ppCertNames, int iCertCount, int *pCertIndex)
{
    if(ppCertNames == nullptr || pCertIndex == nullptr) {
        return QError::ParamIlegal;
    }

    return RevokeEsealSelect(ppCertNames, iCertCount, pCertIndex);
}
