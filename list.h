/***********************************************************
 * ��Ȩ��Ϣ�����ͬѧ������һ��Ȩ��
 * �ļ�����: list.h
 * �ļ�����: lihf
 * �������: 2017-9-13 18:06:15
 * ��ǰ�汾: 1.0.0
 * ��Ҫ����: ������
 * �汾��ʷ:
 ***********************************************************/
#ifndef __LIST_H__
#define __LIST_H__

//����ڵ�
typedef struct list_node
{
    struct list_node *pprev;
    struct list_node *pnext;
}list_node_t;

typedef struct list_node list_t;

//����Ƕ��ýڵ�ĸ��ṹ
#define LIST_ENTRY(type, member, node)    \
    (type *)((char *)(node) - (int)(&(((type *)0)->member)))

//�����������г�Ա, �����ϵ�����
#define LIST_FOR_EACH_FIFO(phead, pnode) \
    for(pnode = (phead)->pprev; pnode != (phead); pnode = pnode->pprev)

//�����������г�Ա, �����µ�����
#define LIST_FOR_EACH_FILO(phead, pnode) \
    for(pnode = (phead)->pnext; pnode != (phead); pnode = pnode->pnext)

//��ʼ��
#define LIST_NODE_INIT(name) {.pprev = &name, .pnext = &name} /* parasoft-suppress FORMAT-18 "�ṹ���Ա��ʼ������" */
//����
#define LIST_NEWEST(phead)  ((phead)->pnext)
//���
#define LIST_OLDEST(phead)  ((phead)->pprev)

/*****************************************************************
 �������ƣ�list_init
 ���������������ʼ��
 ���������phead����ͷ
 ���������phead����ͷ
 ����˵������
 ����˵����
 *****************************************************************/
void list_init(struct list_node *phead);

/*****************************************************************
 �������ƣ�list_insert_after
 ������������������
 ���������phead������ͷ
           pnode���ڵ�
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
void list_insert_after(struct list_node *phead, struct list_node *pnode);

/*****************************************************************
 �������ƣ�list_insert_before
 ������������ǰ������
 ���������phead������ͷ
           pnode���ڵ�
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
void list_insert_before(struct list_node *phead, struct list_node *pnode);

/*****************************************************************
 �������ƣ�list_insert_index
 ��������������������
 ���������phead������ͷ
           idx���ڼ�����
           pnode���ڵ�
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
void list_insert_index(struct list_node *phead, int idx, struct list_node *pnode);

/*****************************************************************
 �������ƣ�list_remove
 �����������Ƴ��ڵ�
 ���������pnode���ڵ�
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
void list_remove(struct list_node *pnode);

/*****************************************************************
 �������ƣ�list_remove_index
 �����������Ƴ��ڼ����ڵ�
 ���������pnode���ӵ�ǰ�ڵ㿪ʼ�ĵ�N���ڵ�
           idx���ڼ����ڵ�
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
void list_remove_index(struct list_node *pnode, int idx);

/*****************************************************************
 �������ƣ�list_is_empty
 ������������������Ƿ�Ϊ��
 ���������phead������ͷ
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
int list_is_empty(struct list_node *phead);

/*****************************************************************
 �������ƣ�list_replace
 �����������滻һ���ڵ�
 ���������pnew���滻�ڵ�
           pold�����滻�ڵ�
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
void list_replace(struct list_node *pnew, struct list_node *pold);

/*****************************************************************
 �������ƣ�list_copy
 ����������������������һ��ͷ
 ���������psrchead��Դ����ͷ
 ���������pdsthead��Ŀ������ͷ
 ����˵������
 ����˵����
                head    ins2        ins1        head
                prev        next        prev        next
                    ins1                ins2
                    prev                next
                            head
 *****************************************************************/
void list_copy(struct list_node *pdsthead, struct list_node *psrchead);

/*****************************************************************
 �������ƣ�list_node_count
 ��������������ж��ٸ�Ԫ�ؽڵ�
 ���������phead����ͷ
 �����������
 ����˵��������
 ����˵����
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
    {{.pprev = &n.head, .pnext = &n.head}, PTHREAD_MUTEX_INITIALIZER}  /* parasoft-suppress FORMAT-18 "�ṹ���Ա��ʼ������" */
//////////////////////////////////////////////////////////////////

#endif

