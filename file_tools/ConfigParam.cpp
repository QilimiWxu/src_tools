#include <string.h>

#include "ConfigParam.h"

#include "../tools_common.h"

#include "json-c/json_util.h"


namespace qilimi{

    const char* XnConfigParam::tag = "XnConfigParam";
    const int XnConfigParam::tag_debug = 1;

    XnConfigParam::XnConfigParam(){
        pobj = NULL;
    }

    XnConfigParam::~XnConfigParam(){
        close();
    }

    /**
     * @brief 打开文件
     * @param const char* file 文件路径
     * @return 0成功
     */
    int XnConfigParam::open(const char* file){
        if(pobj != NULL){
            json_object_put(pobj);
            pobj = NULL;
        }

        pobj = json_object_from_file(file);
        if(pobj == LH_EMPTY){
            my_printf_error("open file[%s] failed! json_object_from_file failed.", file);
            return -1;//打开文件失败
        }
        return 0;
    }

    /**
     * @brief 关闭文件
     * @param 无
     * @return 无
     */
    void XnConfigParam::close(){
        if(pobj != NULL){
            json_object_put(pobj);
            pobj = NULL;
        }
    }

    /**
     * @brief 获取对应的以值
     * @param const char* _key key值
     * @param int _default 默认值
     * @return 获取的值,如果类型或者数据不存在则返回默认值
     */
    int XnConfigParam::getParamInt(const char* _key, int _default){
        if(pobj == NULL){
            my_printf_error("pobj is not init.");
            return _default;
        }
        //获取数据
        json_object* p = json_object_object_get(pobj, _key);
        if(p == NULL){
            my_printf_error("pobj is not init.");
            return _default;
        }
        json_type type = json_object_get_type(p);
        if(type != json_type_int){
            my_printf_error("json type is not int.");
            return _default;
        }
        int val = json_object_get_int(p);
        p = NULL;
        return val;
    }

    /**
     * @brief 获取对应的以值
     * @param const char* _key key值
     * @param const char* default 默认值
     * @return 获取的值,如果类型或者数据不存在则返回默认值
     */
    const char* XnConfigParam::getParamStr(const char* _key, const char* _default){
        if(pobj == NULL){
            my_printf_error("pobj is not init.");
            return _default;
        }
        //获取数据
        json_object* p = json_object_object_get(pobj, _key);
        if(p == NULL){
            my_printf_error("pobj is not init.");
            return _default;
        }
        //判断类型
        json_type type = json_object_get_type(p);
        if(type != json_type_string){
            my_printf_error("json type is not str.");
            return _default;
        }
        //获取返回的数据
        const char* val = json_object_get_string(p);
        p = NULL;
        //数据返回，注意如果pobj释放，内存空间将不可用
        return val;
    }

    /**
     * @brief 设置对应的以值
     * @param const char* _key, key值
     * @param int val 默认值
     * @return 0成功 -4 没有找到数据 -3类型错误 -2未初始化 -6 参数错误 -5 未知错误
     */
    int XnConfigParam::setParamInt(const char* _key, int _val){
        if(pobj == NULL){
            my_printf_error("pobj is not init.");
            return -2;
        }
        if(_key == NULL || strlen(_key) <= 0)return -6;//参数错误
        //获取数据
        json_object* p = json_object_object_get(pobj, _key);
        if(p != NULL){
            //判断类型
            json_type type = json_object_get_type(p);
            if(type != json_type_int){
                my_printf_error("json type is not int.");
                return -3;
            }
            //设置数据
            int ret = json_object_set_int(p, _val);
            if(ret != 1){
                return -5;
            }
        }
        else{
            //增加key值对
            int ret = json_object_object_add(pobj, _key, json_object_new_int(_val));
            if(ret != 0){
                return -5;
            }
        }
        p = NULL;
        return 0;
    }

    /**
     * @brief 设置对应的以值
     * @param const char* key key值
     * @param const char* val 默认值
     * @return 0成功 -4 没有找到数据 -3类型错误 -2未初始化 -6 参数错误 -5 未知错误
     */
    int XnConfigParam::setParamStr(const char* _key, const char* _val){
        if(pobj == NULL){
            my_printf_error("pobj is not init.");
            return -2;
        }
        if(_key == NULL || strlen(_key) <= 0)return -6;//参数错误
        //获取数据
        json_object* p = json_object_object_get(pobj, _key);
        if(p != NULL){
            //判断类型
            json_type type = json_object_get_type(p);
            if(type != json_type_string){
                my_printf_error("json type is not int.");
                return -3;
            }
            //设置数据
            int ret = json_object_set_string(p, _val);
            if(ret != 1){
                return -5;
            }
        }
        else{
            //增加key值对
            int ret = json_object_object_add(pobj, _key, json_object_new_string(_val));
            if(ret != 0){
                return -5;
            }
        }
        p = NULL;
        return 0;
    }

}