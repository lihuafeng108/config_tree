/***********************************************************
 * ��Ȩ��Ϣ: ���ͬѧ������һ��Ȩ��
 * �ļ�����: list.c
 * �ļ�����: lihf
 * �������: 2015-09-22
 * ��ǰ�汾: 1.0.0
 * ��Ҫ����: ������
 * �汾��ʷ:
 ***********************************************************/
#include "list.h"
/*****************************************************************
 �������ƣ�list_init
 ���������������ʼ��
 ���������phead����ͷ
 ���������phead����ͷ
 ����˵������
 ����˵����
 *****************************************************************/
void list_init(struct list_node *phead)
{
    phead->pnext = phead->pprev = phead;
}

/*****************************************************************
 �������ƣ�list_insert_after
 ������������������
 ���������phead������ͷ
           pnode���ڵ�
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
void list_insert_after(struct list_node *phead, struct list_node *pnode)
{
    phead->pnext->pprev = pnode;
    pnode->pnext = phead->pnext;

    phead->pnext = pnode;
    pnode->pprev = phead;
}

/*****************************************************************
 �������ƣ�list_insert_before
 ������������ǰ������
 ���������phead������ͷ
           pnode���ڵ�
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
void list_insert_before(struct list_node *phead, struct list_node *pnode)
{
    phead->pprev->pnext = pnode;
    pnode->pprev = phead->pprev;

    phead->pprev = pnode;
    pnode->pnext = phead;
}

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
 �������ƣ�list_remove
 �����������Ƴ��ڵ�
 ���������pnode���ڵ�
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
void list_remove(struct list_node *pnode)
{
    pnode->pnext->pprev = pnode->pprev;
    pnode->pprev->pnext = pnode->pnext;

    pnode->pnext = pnode->pprev = pnode;
}

/*****************************************************************
 �������ƣ�list_remove_index
 �����������Ƴ��ڼ����ڵ�
 ���������pnode���ӵ�ǰ�ڵ㿪ʼ�ĵ�N���ڵ�
           idx���ڼ����ڵ�
 �����������
 ����˵������
 ����˵����
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
 �������ƣ�list_is_empty
 ������������������Ƿ�Ϊ��
 ���������phead������ͷ
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
int list_is_empty(struct list_node *phead)
{
    return phead->pnext == phead;
}

/*****************************************************************
 �������ƣ�list_replace
 �����������滻һ���ڵ�
 ���������pnew���滻�ڵ�
           pold�����滻�ڵ�
 �����������
 ����˵������
 ����˵����
 *****************************************************************/
void list_replace(struct list_node *pnew, struct list_node *pold)
{
    pnew->pprev = pold->pprev;
    pnew->pnext = pold->pnext;
    pold->pprev->pnext = pnew;
    pold->pnext->pprev = pnew;
}

/*****************************************************************
 �������ƣ�list_copy
 ����������������������һ��ͷ
 ���������psrchead��Դ����ͷ
 ���������pdsthead��Ŀ������ͷ
 ����˵������
 ����˵����
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
 �������ƣ�list_node_count
 ��������������ж��ٸ�Ԫ�ؽڵ�
 ���������phead����ͷ
 �����������
 ����˵��������
 ����˵����
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


