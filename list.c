/***********************************************************
 * 版权信息: 李华锋同学，保留一切权利
 * 文件名称: list.c
 * 文件作者: lihf
 * 完成日期: 2015-09-22
 * 当前版本: 1.0.0
 * 主要功能: 链表处理
 * 版本历史:
 ***********************************************************/
#include "list.h"
/*****************************************************************
 函数名称：list_init
 函数描述：链表初始化
 输入参数：phead链表头
 输出参数：phead链表头
 返回说明：无
 其它说明：
 *****************************************************************/
void list_init(struct list_node *phead)
{
    phead->pnext = phead->pprev = phead;
}

/*****************************************************************
 函数名称：list_insert_after
 函数描述：向后插入项
 输入参数：phead，链表头
           pnode，节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_insert_after(struct list_node *phead, struct list_node *pnode)
{
    phead->pnext->pprev = pnode;
    pnode->pnext = phead->pnext;

    phead->pnext = pnode;
    pnode->pprev = phead;
}

/*****************************************************************
 函数名称：list_insert_before
 函数描述：向前插入项
 输入参数：phead，链表头
           pnode，节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_insert_before(struct list_node *phead, struct list_node *pnode)
{
    phead->pprev->pnext = pnode;
    pnode->pprev = phead->pprev;

    phead->pprev = pnode;
    pnode->pnext = phead;
}

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
void list_insert_index(struct list_node *phead, int idx, struct list_node *pnode)
{
    struct list_node *pdump;
    if(idx == 0)//
    {
        list_insert_before(phead, pnode);
    }
    else if(idx > 0)//
    {
        LIST_FOR_EACH_FIFO(phead, pdump)
        {
            if((idx--) == 0)//
            {
                break;
            }
        }
        list_insert_before(pdump, pnode);
    }
    else
    {
        LIST_FOR_EACH_FILO(phead, pdump)
        {
            if((idx++) == 0)//
            {
                break;
            }
        }
        list_insert_after(pdump, pnode);
    }
}

/*****************************************************************
 函数名称：list_remove
 函数描述：移除节点
 输入参数：pnode，节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_remove(struct list_node *pnode)
{
    pnode->pnext->pprev = pnode->pprev;
    pnode->pprev->pnext = pnode->pnext;

    pnode->pnext = pnode->pprev = pnode;
}

/*****************************************************************
 函数名称：list_remove_index
 函数描述：移除第几个节点
 输入参数：pnode，从当前节点开始的第N个节点
           idx，第几个节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_remove_index(struct list_node *pnode, int idx)
{
    struct list_node *pdump = pnode;
    if(idx > 0)//
    {
        LIST_FOR_EACH_FIFO(pnode, pdump)//
        {
            if((idx--) == 0)//
            {
                break;
            }
        }
    }
    else if(idx < 0)//
    {
        LIST_FOR_EACH_FILO(pnode, pdump)//
        {
            if((idx++) == 0)//
            {
                break;
            }
        }
    }
    list_remove(pdump);
}

/*****************************************************************
 函数名称：list_is_empty
 函数描述：检查链表是否为空
 输入参数：phead，链表头
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
int list_is_empty(struct list_node *phead)
{
    return phead->pnext == phead;
}

/*****************************************************************
 函数名称：list_replace
 函数描述：替换一个节点
 输入参数：pnew，替换节点
           pold，被替换节点
 输出参数：无
 返回说明：无
 其它说明：
 *****************************************************************/
void list_replace(struct list_node *pnew, struct list_node *pold)
{
    pnew->pprev = pold->pprev;
    pnew->pnext = pold->pnext;
    pold->pprev->pnext = pnew;
    pold->pnext->pprev = pnew;
}

/*****************************************************************
 函数名称：list_copy
 函数描述：复制链表到另外一个头
 输入参数：psrchead，源链表头
 输出参数：pdsthead，目标链表头
 返回说明：无
 其它说明：
              head        head        head        head
                    ins1                    ins1
                    prev                    next
                                head
//////////////////////////////////////////////////////////////////////
              ins2        head        head        ins1
              prev        next        prev        next
                    ins1                    ins2
                    prev                    next
                                head
//////////////////////////////////////////////////////////////////////
ins2        head          ins3         ins1        head        ins2
prev        next          prev         next        prev        next
     ins1                       ins2                     ins3
     prev                                                next
                                head

 *****************************************************************/
void list_copy(struct list_node *pdsthead, struct list_node *psrchead)
{
    if(!list_is_empty(psrchead))//
    {
        pdsthead->pprev = psrchead->pprev;
        pdsthead->pnext = psrchead->pnext;
        pdsthead->pprev->pnext = pdsthead;
        pdsthead->pnext->pprev = pdsthead;
    }
    else
    {
        list_init(pdsthead);
    }
}

/*****************************************************************
 函数名称：list_node_count
 函数描述：检查有多少个元素节点
 输入参数：phead链表头
 输出参数：无
 返回说明：个数
 其它说明：
 *****************************************************************/
int list_node_count(struct list_node *phead)
{
    int count = 0;
    struct list_node *pnode;
    LIST_FOR_EACH_FIFO(phead, pnode)
    {
        count++;
    }
    return count;
}


