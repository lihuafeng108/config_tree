/***********************************************************
 * 版权信息：李华锋同学，保留一切权利
 * 文件名称: list.h
 * 文件作者: lihf
 * 完成日期: 2017-9-13 18:06:15
 * 当前版本: 1.0.0
 * 主要功能: 链表处理
 * 版本历史:
 ***********************************************************/
#ifndef __LIST_H__
#define __LIST_H__

//链表节点
typedef struct list_node
{
    struct list_node *pprev;
    struct list_node *pnext;
}list_node_t;

typedef struct list_node list_t;

//返回嵌入该节点的父结构
#define LIST_ENTRY(type, member, node)    \
    (type *)((char *)(node) - (int)(&(((type *)0)->member)))

//历遍链表所有成员, 从最老到最新
#define LIST_FOR_EACH_FIFO(phead, pnode) \
    for(pnode = (phead)->pprev; pnode != (phead); pnode = pnode->pprev)

//历遍链表所有成员, 从最新到最老
#define LIST_FOR_EACH_FILO(phead, pnode) \
    for(pnode = (phead)->pnext; pnode != (phead); pnode = pnode->pnext)

//初始化
#define LIST_NODE_INIT(name) {.pprev = &name, .pnext = &name} /* parasoft-suppress FORMAT-18 "结构体成员初始化操作" */
//最新
#define LIST_NEWEST(phead)  ((phead)->pnext)
//最旧
#define LIST_OLDEST(phead)  ((phead)->pprev)

/*****************************************************************
 函数名称：list_init
 函数描述：链表初始化
 输入参数：phead链表头
 输出参数：phead链表头
 返回说明：无
 其它说明：
 *****************************************************************/
void list_init(struct list_node *phead);

/*****************************************************************
 函数名称：list_insert_after
 函数描述：向后插入项
 输入参数：phead，链表头
           pnode，节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_insert_after(struct list_node *phead, struct list_node *pnode);

/*****************************************************************
 函数名称：list_insert_before
 函数描述：向前插入项
 输入参数：phead，链表头
           pnode，节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_insert_before(struct list_node *phead, struct list_node *pnode);

/*****************************************************************
 函数名称：list_insert_index
 函数描述：按索引插入
 输入参数：phead，链表头
           idx，第几个后
           pnode，节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_insert_index(struct list_node *phead, int idx, struct list_node *pnode);

/*****************************************************************
 函数名称：list_remove
 函数描述：移除节点
 输入参数：pnode，节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_remove(struct list_node *pnode);

/*****************************************************************
 函数名称：list_remove_index
 函数描述：移除第几个节点
 输入参数：pnode，从当前节点开始的第N个节点
           idx，第几个节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_remove_index(struct list_node *pnode, int idx);

/*****************************************************************
 函数名称：list_is_empty
 函数描述：检查链表是否为空
 输入参数：phead，链表头
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
int list_is_empty(struct list_node *phead);

/*****************************************************************
 函数名称：list_replace
 函数描述：替换一个节点
 输入参数：pnew，替换节点
           pold，被替换节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_replace(struct list_node *pnew, struct list_node *pold);

/*****************************************************************
 函数名称：list_copy
 函数描述：复制链表到另外一个头
 输入参数：psrchead，源链表头
 输出参数：pdsthead，目标链表头
 返回说明：无
 其它说明：
                head    ins2        ins1        head
                prev        next        prev        next
                    ins1                ins2
                    prev                next
                            head
 *****************************************************************/
void list_copy(struct list_node *pdsthead, struct list_node *psrchead);

/*****************************************************************
 函数名称：list_node_count
 函数描述：检查有多少个元素节点
 输入参数：phead链表头
 输出参数：无
 返回说明：个数
 其它说明：
 *****************************************************************/
int list_node_count(struct list_node *phead);

//////////////////////////////////////////////////////////////////
#include <pthread.h>

//////////////////////////////////////////////////////////////////
struct protect_list_head
{
    struct list_node head;
    pthread_mutex_t lock;
};

#define PROTECT_LIST_HEAD_INIT(n)    \
    {{.pprev = &n.head, .pnext = &n.head}, PTHREAD_MUTEX_INITIALIZER}  /* parasoft-suppress FORMAT-18 "结构体成员初始化操作" */
//////////////////////////////////////////////////////////////////

#endif

