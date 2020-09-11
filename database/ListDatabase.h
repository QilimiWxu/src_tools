#ifndef __IRISUSER_DATABASE_INCLUDE_DEFINE_
#define __IRISUSER_DATABASE_INCLUDE_DEFINE__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <iostream>
#include <mutex>

namespace qilimi{

    typedef struct listNode {
        struct listNode *prev;
        struct listNode *next;
        void *val;
    } listNode;

    typedef struct listIter {
        listNode *next;
        int direction;
    } listIter;

    typedef int (*compare)(void* param1, void*param2);

    /*
     * 自用内存分配器
     */
    class z_malloc_alloc{
    private:
        int max_len;
        int use_len;
        int signle_len;
        typedef struct list_alloc{
            list_alloc* next;
            void* data;
        }list_alloc;
        list_alloc* head;
        list_alloc* tail;
    public:
        z_malloc_alloc(int len);
        ~z_malloc_alloc();
    };

    /**
     * 自排序双链表
     */
    class ListSort {
    private:
        listNode* head;   //头
        listNode* tail;   //尾
        compare func_compare;//比较两个节点大小
        compare func_match;//比较节点和可以，第一个参数是节点数据 第二个参数是key
        unsigned long len;  //总长
    public:
        ListSort();
        ~ListSort();

        void listEmpty();

        void addNodeHead(void* val);

        void addNodeTail(void* val);

        void insertNode(listNode *old_node, void* val, int after);

        void delNode(listNode *node);

        listNode* SearchKey(void* key);

        listNode* Index(int index);
    };

    /**
     * 不要调用拷贝构造函数
     */
    class ListDatabase{
    private:
        static const int g_max_user_count;
    private:
        //文件操作互斥锁
        std::mutex m_mutex_database;
        //数据链表
        DataList* m_data;
        //人数
        int m_user_count;

        //数据库文件的人数
        int m_file_user_count;
        //数据库文件句柄
        FILE* m_database_fp;

        IrisUserDatabase(const IrisUserDatabase &);
    public:
        IrisUserDatabase();
        ~IrisUserDatabase();
        
        /**
         * @brief 数据库初始化
         * @param const char* 数据库文件路径
         * @return 0成功
         */
        int database_init(const char*);

        /**
         * @brief 退出数据库
         * @param 无
         * @return 无
         */
        void database_exit(void);
        
        /**
         * @brief 拷贝数据库文件
         * @param const char* 拷贝的路径
         * @return 无
         */
        void database_bak(const char*);

        /**
         * @brief 遍历数据库数据
         * @param const char* 拷贝的路径
         * @return 无
         */
        void traversal_data(UserInfoCallback func);

        /**
         * @brief 插入用户数据,插入数据的计数会加一
         * @param IrisUserInfo *用户数据
         * @return 无
         */
        int insert_data(IrisUserInfo *);

        /**
         * @brief 删除用户数据
         * @param int _id用户id
         * @return 无
         */
        int delete_data(int _id);

        /**
         * @brief 查询用户数据
         * @param int _id用户id
         * @return 无
         */
        const IrisUserInfo * find_data(int _id);

        /**
         * @brief 查询用户数据
         * @param const char* _name 用户名称
         * @return 无
         */
        const IrisUserInfo * find_data(const char* _name);

        /**
         * @brief 将IrisUserInfo *指针获取为可用，计数增加，需要调用put释放指针
         * @param const IrisUserInfo * 需要增加计数的指针
         * @return 结果
         */
        IrisUserInfo* getIrisUserInfo(const IrisUserInfo *);

        /**
         * @brief 将IrisUserInfo *计数减1，如果计数减少到0则释放空间
         * @param const IrisUserInfo * 需要增加计数的指针
         * @return 无
         */
        void putIrisUserInfo(const IrisUserInfo *);

        /**
         * @brief 将修改的数据写入到文件中
         * @param 无
         * @return 无
         */
        int flush();
    };

}

#endif
