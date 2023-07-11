#ifndef QTSIGNDIALOGLIB_H
#define QTSIGNDIALOGLIB_H

#include <vector>
#include <string>

#include <cstdint>

//印章信息结构体定义
struct eseal_info {
    int			type;			//类型
    std::string file_name;
    std::string	eseal_id;       //印章id
    std::string eseal_name;		//印章名称
    std::string	eseal;			//印章文件
    uint32_t	eseal_len;		//印章长度
    std::string	pic;			//图片
    uint32_t	pic_len;		//图片长度
    int			width;			//图片宽度
    int			height;			//图片长度
};

//证书信息结构体定义
typedef struct {
    std::string  cert_id;
    std::string	 cert_name;
    int		 cert_type;         //证书类型：0-ukey证书；1-信步云证书；2-易签盾证书
    std::vector<eseal_info>	vec_eseals;
} cert_eseal_info;

//错误码
namespace QError {
    enum QErrorInfo {
        Normally,
        QAppInitError,
        UserNotSelect,
        ParamIlegal,
        EnvSetFailed,
    };
}

//调用本动态库的签章程序类型
enum ESS_TYPE {
    ESS_WEB_SIGN,
    ESS_OFD_SIGN,
    ESS_PDF_SIGN,
};

#ifdef __cplusplus
extern "C" {
#endif

//init Qt Sign Dialog Lib
int init_lib(int iSignProgramType);

//exit Qt Sign Dialog Lib
void exit_lib();

//call cert and eseal select dialog
int select_cert_eseal(void *vecCertEseal, int *pCertIndex, int *pEsealIndex);

//call cert select dialog
int select_cert(char **ppCertNames, int iCertCount, int *pCertIndex);

//call revoke eseal select dialog
int select_revoke_eseal(char **ppCertNames, int iCertCount, int *pCertIndex);

#ifdef __cplusplus
}
#endif

#endif // QTSIGNDIALOGLIB_H
