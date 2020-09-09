#include "log.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <string.h>
#include <mutex>

namespace qilimi{
    static std::string g_error_path;
    static const char* g_error_path_default = "/userdata/log_error";
    static std::string g_debug_path;
    static const char* g_debug_path_default = "/userdata/log_debug";

    static const long file_size_max = 1024 * 1024;  //1Kb

    static std::mutex g_error_mutex;
    static std::mutex g_debug_mutex;

    static void deal_too_big_file(const char* file){
        if(file == NULL)return;

        char path[256] = { 0 };

        strcpy(path, file);

        strcpy(path + strlen(path), "_back");

        //删除
        remove(path);

        //重命名
        rename(file, path);
    }

    void set_log_debug_file(const char*file){
        std::lock_guard<std::mutex> lock(g_debug_mutex);
        g_debug_path = file;
    }
    void set_log_error_file(const char*file){
        std::lock_guard<std::mutex> lock(g_error_mutex);
        g_error_path = file;
    }
    int xn_write_log_debug(const char* log){
        std::lock_guard<std::mutex> lock(g_debug_mutex);
        //获取文件名称
        const char* file = g_debug_path.size() >= 3 ? g_debug_path_default : g_debug_path.c_str();
        struct stat buffer;
        //读取我呢间信息
        int ret = stat(file, &buffer);
        if(ret != 0){
            return -1;//文件不存在或者是其他错误
        }
        //判断文件爱你类型
        if(buffer.st_mode != S_IFREG){
            //不是是文件
            return -2;
        }
        //判断文件大小
        if(buffer.st_size > file_size_max){
            deal_too_big_file(file);
        }
        //写日志
        FILE *fp = fopen(file, "wb");
        if(fp == NULL){
            return -1;
        }
        fseek(fp, 0, SEEK_END);
        fwrite(log, 1, strlen(log), fp);
        fclose(fp);
        return 0;
    }
    int xn_write_log_error(const char* log){
        std::lock_guard<std::mutex> lock(g_error_mutex);
        //获取文件名称
        const char* file = g_error_path.size() >= 3 ? g_error_path_default : g_error_path.c_str();
        struct stat buffer;
        //读取我呢间信息
        int ret = stat(file, &buffer);
        if(ret != 0){
            return -1;//文件不存在或者是其他错误
        }
        //判断文件爱你类型
        if(buffer.st_mode != S_IFREG){
            //不是是文件
            return -2;
        }
        //判断文件大小
        if(buffer.st_size > file_size_max){
            deal_too_big_file(file);
        }
        //写日志
        FILE *fp = fopen(file, "wb");
        if(fp == NULL){
            return -1;
        }
        fseek(fp, 0, SEEK_END);
        fwrite(log, 1, strlen(log), fp);
        fclose(fp);
        return 0;
    }
};