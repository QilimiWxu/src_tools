#ifndef XN_CONFIG_PARAM_INCLUDE_DEFINE
#define XN_CONFIG_PARAM_INCLUDE_DEFINE

#include <stdio.h>
#include <stdint.h>
#include <json-c/json.h>
//buildroot/output/rockchip_rv1126_rv1109_facial_gate/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/json-c
namespace qilimi{

class XnConfigParam{
private:
    json_object *pobj;

    static const char* tag;
    static const int tag_debug;

    /**
     * @brief 告诉线程有数据修改了,需要进行数据同步
     * @param const char* file 文件路径
     * @return 0成功
     */
    void put_change();
    
public:
    XnConfigParam();
    ~XnConfigParam();

    /**
     * @brief 打开文件
     * @param const char* file 文件路径
     * @return 0成功
     */
    int open(const char* file);

    /**
     * @brief 关闭文件
     * @param 无
     * @return 无
     */
    void close();

    /**
     * @brief 获取对应的以值
     * @param const char* key key值
     * @param int _default 默认值
     * @return 获取的值,如果类型或者数据不存在则返回默认值
     */
    int getParamInt(const char* key, int _default);

    /**
     * @brief 获取对应的以值
     * @param const char* key key值
     * @param const char* _default 默认值
     * @return 获取的值,如果类型或者数据不存在则返回默认值
     */
    const char* getParamStr(const char* key, const char* _default);

    /**
     * @brief 设置对应的以值
     * @param const char* key key值
     * @param int val 默认值
     * @return 0成功 -3类型错误 -2未初始化
     */
    int setParamInt(const char* key, int val);

    /**
     * @brief 设置对应的以值
     * @param const char* key key值
     * @param const char* val 默认值
     * @return 0成功 -3类型错误 -2未初始化
     */
    int setParamStr(const char* key, const char* val);
};
}
#endif