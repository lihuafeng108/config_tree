/***********************************************************
 * 版权信息: 李华锋同学，保留一切权利
 * 文件名称: tree.c
 * 文件作者: lihf
 * 完成日期: 2018-11-03
 * 当前版本:
 * 主要功能: 将配置字符串("xxx->xxx->xxx=y")转换为树形的数据链表，来存储
 * 版本历史:
 ***********************************************************/

/********************************************************************************************************
 *                                        File Include                                                  *
 ********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#include "typedef.h"
#include "list.h"
#include "tree.h"

/********************************************************************************************************
 *                                        Defines                                                       *
 ********************************************************************************************************/
typedef struct tree_note
{
    list_node_t      node;
    u8               *pname;        //节点名称
    int              item_cout;     //节点下面有多少个子节点
    struct tree_note *pfather_note; //指向父节点的指针
    list_node_t      son_head;      //子节点的头部
    void             *data;         //指向数据域的指针
    int              datasize;      //数据长度，字符串形式
}s_tree_note_t;

/********************************************************************************************************
 *                                        Global Variables                                              *
 ********************************************************************************************************/


/********************************************************************************************************
 *                                        Local Variables                                               *
 ********************************************************************************************************/


/********************************************************************************************************
 *                                        Local Functions Declaring                                     *
 ********************************************************************************************************/
static s_tree_root_t tree_root = NOTE_ROOT_INIT(tree_root);

/********************************************************************************************************
 *                                        Local Functions                                               *
 ********************************************************************************************************/

/***********************************************************
 * 函数名称：name_list_find_next
 * 功能描述：寻找name_list字符串下个节点的位置
 * 输入参数：
 *           name_list 名字串中地址
 * 输出参数：无
 * 返 回 值：如果存在，则返回下个节点的地址，否则返回NULL
 ***********************************************************/
static const u8 *name_list_find_next(const u8 *name_list)
{
    const u8 *ptmp = name_list;

    while('\0' != *ptmp)
    {
        ptmp++;

        if( ('-' == *ptmp) && ('>' == *(ptmp+1)) && ('\0' != *(ptmp+2)) )
        {
            return ptmp+2;
        }
    }

    return NULL;
}

static int first_note_name_len(const u8 *name_list)
{
    const u8 *ptmp = name_list;
    while(1)
    {
        if( ('\0' == *ptmp) || \
            ( ('-' == *ptmp) && ('>' == *(ptmp+1)) ) )
        {
            break;
        }

        ptmp++;
    }

    return ptmp - name_list - 1;
}

static s_tree_note_t *find_note_from_namelist(list_node_t *head, const u8 *name_list)
{
    const u8 *pnote_name = name_list;
    int note_name_len = first_note_name_len(name_list);
    list_node_t   *pnode = NULL;
    s_tree_note_t *ptree = NULL;


    LIST_FOR_EACH_FIFO( head, pnode)
    {
        ptree = LIST_ENTRY(s_tree_note_t, node, pnode);
        if( 0 == strncmp(ptree->pname, pnote_name, note_name_len) )
        {
            return ptree;
        }
    }

    return NULL;
}

static s_tree_note_t *malloc_for_new_note(const u8 *name_list, int datasize)
{
    int note_name_len = first_note_name_len(name_list);

    note_name_len += 1;  //预留一个结束符的位置
    s_tree_note_t *ptree = malloc(sizeof(s_tree_note_t) + note_name_len + datasize);
    if(NULL == ptree)
    {
        return NULL;
    }

    //初始化一下分配的内存
    ptree->data = NULL;
    ptree->pname = (u8 *)(ptree + 1);

    memcpy((u8 *)ptree->pname, name_list, note_name_len);
    ptree->pname[note_name_len] = '\0';
    printf("ptree->pname[len = %d]: %s\n", note_name_len, (u8 *)ptree->pname);

    ptree->datasize     = 0;
    ptree->item_cout    = 0;
    ptree->pfather_note = NULL;

    if(datasize > 0)
    {
        ptree->data = ptree->pname + note_name_len;
        ptree->datasize = datasize;
    }

    list_init(&ptree->son_head);

    return ptree;
}
/********************************************************************************************************
 *                                        Global Functions                                              *
 ********************************************************************************************************/
/***********************************************************
 * 函数名称：tree_add
 * 功能描述：将name_list表示的配置项和节点，添加到树链表中
 * 输入参数：name_list  字符串形式("xxx->xxx->xxx")，表示的父节点和子节点之间的关系
             value      末尾节点的值，字符串表示
             value_len  字符串值的长度
 * 输出参数：无
 * 返 回 值：NO_ERROR
             ERROR
 ***********************************************************/
int tree_add(const u8 *name_list, const void *value, const int value_len)
{
    s_tree_note_t *ptree = NULL;
    const u8      *pnext = NULL;
    //list_node_t   *phead = tree_root.head.pprev;  //<lihf> 这样是不行的哦
    list_node_t   *phead = &tree_root.head;

    pthread_mutex_lock(&tree_root.tree_lock);

    for(; NULL != name_list; name_list = pnext)
    {
        s_tree_note_t *ptree_parent = ptree;  //<lihf> 指向当前节点的父节点

        pnext = name_list_find_next(name_list);
        ptree = find_note_from_namelist(phead, name_list);
        if(NULL == ptree)
        {
            printf("<lihf> didn't find the note from head\n");
            if( (NULL != ptree_parent) && (ptree_parent->item_cout >1024) )
            {
                printf("满员了，不再申请\n");
                pthread_mutex_unlock(&tree_root.tree_lock);
                return ERROR;
            }

            ptree = malloc_for_new_note(name_list, (NULL == pnext) ? value_len : 0);
            if(NULL == ptree)
            {
                printf("节点申请内存不成功\n");
                pthread_mutex_unlock(&tree_root.tree_lock);
                return ERROR;
            }

            ptree->pfather_note = ptree_parent;
            if(NULL != ptree_parent)
            {
                ptree_parent->item_cout++;
            }

            //<lihf> 把新申请的节点插入链表之中
            list_insert_after(phead, &ptree->node);
        }
        phead = &ptree->son_head;
    }

    if( (NULL != value) && (value_len > 0) && (NULL != ptree->data) )
    {
        memcpy((u8 *)ptree->data, (u8 *)value, value_len);
    }

    pthread_mutex_unlock(&tree_root.tree_lock);
    return NO_ERROR;
}

