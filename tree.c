/***********************************************************
 * ��Ȩ��Ϣ: ���ͬѧ������һ��Ȩ��
 * �ļ�����: tree.c
 * �ļ�����: lihf
 * �������: 2018-11-03
 * ��ǰ�汾:
 * ��Ҫ����: �������ַ���("xxx->xxx->xxx=y")ת��Ϊ���ε������������洢
 * �汾��ʷ:
 ***********************************************************/

/********************************************************************************************************
 *                                        File Include                                                  *
 ********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "typedef.h"
#include "list.h"
#include "tree.h"

/********************************************************************************************************
 *                                        Defines                                                       *
 ********************************************************************************************************/
typedef struct tree_note
{
    list_node_t      node;
    u8               *pname;        //�ڵ�����
    int              item_cout;     //�ڵ������ж��ٸ��ӽڵ�
    struct tree_note *pfather_note; //ָ�򸸽ڵ��ָ��
    list_node_t      son_head;      //�ӽڵ��ͷ��
    void             *data;         //ָ���������ָ��
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
 * �������ƣ�name_list_find_next
 * ����������Ѱ��name_list�ַ����¸��ڵ��λ��
 * ���������
 *           name_list ���ִ��е�ַ
 * �����������
 * �� �� ֵ��������ڣ��򷵻��¸��ڵ�ĵ�ַ�����򷵻�NULL
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

    return ptmp - name_list;
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
    return NULL;
}
/********************************************************************************************************
 *                                        Global Functions                                              *
 ********************************************************************************************************/
/***********************************************************
 * �������ƣ�tree_add
 * ������������name_list��ʾ��������ͽڵ㣬��ӵ���������
 * ���������name_list  �ַ�����ʽ("xxx->xxx->xxx")����ʾ�ĸ��ڵ���ӽڵ�֮��Ĺ�ϵ
             value      ĩβ�ڵ��ֵ���ַ�����ʾ
             value_len  �ַ���ֵ�ĳ���
 * �����������
 * �� �� ֵ��NO_ERROR
             ERROR
 ***********************************************************/
int tree_add(const u8 *name_list, const void *value, const int value_len)
{
    s_tree_note_t *ptree = NULL;
    const u8      *pnext = NULL;
    list_node_t   *phead = tree_root.head.pnext;

    pthread_mutex_lock(&tree_root.tree_lock);

    for(; NULL != name_list; name_list = pnext)
    {
        s_tree_note_t *ptree_parent = ptree;  //<lihf> ָ��ǰ�ڵ�ĸ��ڵ�

        pnext = name_list_find_next(name_list);
        ptree = find_note_from_namelist(phead, name_list);
        if(NULL == ptree)
        {
            if( (NULL != ptree_parent) && (ptree_parent->item_cout >1024) )
            {
                printf("��Ա�ˣ���������");
                pthread_mutex_unlock(&tree_root.tree_lock);
                return ERROR;
            }

            ptree = malloc_for_new_note(name_list, (NULL == pnext) ? value_len : 0);
            if(NULL == ptree)
            {
                printf("�ڵ������ڴ治�ɹ�");
                pthread_mutex_unlock(&tree_root.tree_lock);
                return ERROR;
            }

            ptree->pfather_note = ptree_parent;
            if(NULL != ptree_parent)
            {
                ptree_parent->item_cout++;
            }

            //<lihf> ��������Ľڵ��������֮��
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

