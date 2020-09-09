#include <string.h>
#include <unistd.h>

#include "ConfigParam.h"

#include "../tools_common.h"

#include "json-c/json_util.h"


namespace qilimi{

    const char* ConfigParam::tag = "ConfigParam";
    const int ConfigParam::tag_debug = 1;
    const char* ConfigParam::version = "1.0";

    ConfigParam::ConfigParam(){
        pobj = (json_object*)LH_EMPTY;
        memset(file_name, 0, 256);
    }

    ConfigParam::~ConfigParam(){
        close();
    }

    /**
     * @brief 打开文件
     * @param const char* file 文件路径
     * @return 0成功
     */
    int ConfigParam::open(const char* file){
        if(pobj != (json_object*)LH_EMPTY){
            json_object_put(pobj);
            pobj = (json_object*)LH_EMPTY;
        }

        if(0 != access(file, F_OK)){
            //文件不存在，创建一个文件，默认有一个ConfigParam_version
            pobj = json_object_new_object();
            if(pobj == NULL || pobj == (json_object*)LH_EMPTY){
                pobj =  (json_object*)LH_EMPTY;
                return -8;//初始化失败
            }
            //增加key值对
            int ret = json_object_object_add(pobj, "ConfigParam_version", json_object_new_string(version));
            if(ret != 0){
                my_printf_error("json_object_object_add failed. file = %d.", file);
                //操作失败,添加元素失败
                return -5;
            }
            //写入文件
            ret = json_object_to_file_ext(file, pobj, JSON_C_TO_STRING_SPACED);
            if(ret != 0){
                my_printf_error("json_object_to_file_ext failed. file = %d.", file);
                return -7;// -7写入文件失败
            }
        }
        else{
            //文件存在
            pobj = json_object_from_file(file);
            if(pobj == (json_object*)LH_EMPTY){
                my_printf_error("open file[%s] failed! json_object_from_file failed.", file);
                return -1;//打开文件失败
            }
        }
        strcpy(file_name, file);
        return 0;
    }

    /**
     * @brief 关闭文件
     * @param 无
     * @return 无
     */
    void ConfigParam::close(){
        if(pobj != NULL && pobj != (json_object*)LH_EMPTY){
            json_object_to_file_ext(file_name, pobj, JSON_C_TO_STRING_PRETTY);
            json_object_put(pobj);
            pobj = (json_object*)LH_EMPTY;
        }
    }

    /**
     * @brief 获取对应的以值
     * @param const char* _key key值
     * @param int _default 默认值
     * @return 获取的值,如果类型或者数据不存在则返回默认值
     */
    int ConfigParam::getParamInt(const char* _key, int _default){
        if(pobj == (json_object*)LH_EMPTY){
            my_printf_error("pobj is not init.");
            return _default;
        }
        //获取数据
        json_object* p = json_object_object_get(pobj, _key);
        if(p == (json_object*)LH_EMPTY && p != NULL){
            my_printf_error("pobj is not init.");
            return _default;
        }
        json_type type = json_object_get_type(p);
        if(type != json_type_int){
            my_printf_error("json type is not int.");
            return _default;
        }
        int val = json_object_get_int(p);
        p = (json_object*)LH_EMPTY;
        return val;
    }

    /**
     * @brief 获取对应的以值
     * @param const char* _key key值
     * @param const char* default 默认值
     * @return 获取的值,如果类型或者数据不存在则返回默认值
     */
    const char* ConfigParam::getParamStr(const char* _key, const char* _default){
        if(pobj == (json_object*)LH_EMPTY){
            my_printf_error("pobj is not init.");
            return _default;
        }
        //获取数据
        json_object* p = json_object_object_get(pobj, _key);
        if(p == (json_object*)LH_EMPTY && p != NULL){
            my_printf_error("pobj is not init.");
            return _default;
        }
        //判断类型
        json_type type = json_object_get_type(p);
        if(type != json_type_string){
            my_printf_error("json type is not str. ");
            return _default;
        }
        //获取返回的数据
        const char* val = json_object_get_string(p);
        p = (json_object*)LH_EMPTY;
        //数据返回，注意如果pobj释放，内存空间将不可用
        return val;
    }

    /**
     * @brief 设置对应的以值
     * @param const char* _key, key值
     * @param int val 默认值
     * @return 0成功 -4 没有找到数据 -3类型错误 -2未初始化 -6 参数错误 -5 未知错误
     */
    int ConfigParam::setParamInt(const char* _key, int _val){
        if(pobj == (json_object*)LH_EMPTY){
            my_printf_error("pobj is not init.");
            return -2;
        }
        if(_key == NULL || strlen(_key) <= 0)return -6;//参数错误
        //获取数据
        json_object* p = json_object_object_get(pobj, _key);
        if(p != (json_object*)LH_EMPTY && p != NULL){
            //判断类型
            json_type type = json_object_get_type(p);
            if(type != json_type_int){
                my_printf_error("json type is not int.");
                return -3;
            }
            //设置数据
            int ret = json_object_set_int(p, _val);
            if(ret != 1){
                my_printf_error("json_object_set_int failed. key = %d, val = %d.", _key, _val);
                return -5;
            }
        }
        else{
            //增加key值对
            int ret = json_object_object_add(pobj, _key, json_object_new_int(_val));
            if(ret != 0){
                my_printf_error("json_object_object_add failed. key = %d, val = %d.", _key, _val);
                return -5;
            }
        }
        p = (json_object*)LH_EMPTY;
        return 0;
    }

    /**
     * @brief 设置对应的以值
     * @param const char* key key值
     * @param const char* val 默认值
     * @return 0成功 -4 没有找到数据 -3类型错误 -2未初始化 -6 参数错误 -5 未知错误 -7写入文件失败
     */
    int ConfigParam::setParamStr(const char* _key, const char* _val){
        if(pobj == (json_object*)LH_EMPTY){
            my_printf_error("pobj is not init.");
            return -2;
        }
        if(_key == NULL || strlen(_key) <= 0)return -6;//参数错误
        //获取数据
        json_object* p = json_object_object_get(pobj, _key);
        if(p != (json_object*)LH_EMPTY && p != NULL){
            //判断类型
            json_type type = json_object_get_type(p);
            if(type != json_type_string){
                my_printf_error("json type is not int.");
                return -3;
            }
            //设置数据
            int ret = json_object_set_string(p, _val);
            if(ret != 1){
                my_printf_error("json_object_set_string failed. key = %d, val = %s.", _key, _val);
                return -5;
            }
        }
        else{
            //增加key值对
            int ret = json_object_object_add(pobj, _key, json_object_new_string(_val));
            if(ret != 0){
                my_printf_error("json_object_object_add failed. key = %d, val = %s.", _key, _val);
                return -5;
            }
        }
        p = (json_object*)LH_EMPTY;
        return 0;
    }

}