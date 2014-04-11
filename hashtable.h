/*
 * 关于链表的详细信息参考list.h的开始部分
 *
 * 链表中节点的分布：
 *  +----------------------+
 *  |   ListNode           |
 *  |                      |
 *  |  +----------------+  |
 *  |  |                |  |
 *  |  |                |  |
 *  |  |    NodeInfo    |  |
 *  |  |                |  |
 *  |  |                |  |
 *  |  |----------------|  |
 *  |  |                |  |
 *  |  |    ListHead    |  |
 *  |  |                |  |
 *  |  +----------------+  |
 *  +----------------------+
 *
 * 注：
 * 	这个代码的重用只是源码级的重用，使用方式：将该实现源码拷入
 * 	目标工程中，然后更改44行的typedef，将NodeInfo重定义为自己的
 * 	类型，然后注释掉TestNode的定义。（自定义类型至少要包含名为
 * 	str和code两个成员）
 */

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "list.h"

/*
 * ================ WARNING ==================
 *
 * To use this hashtable, you need to redefine 
 * these typedefs */

//用户自定义的数据类型至少包含str和code这两个成员
typedef struct _TestNode {
	char str[16];
	unsigned code;  //除了字符串本身外，有可能还有其他值需要校验，
	                //不用则将其置0，在这里我们用于存放包的uid值
} TestNode;

typedef TestNode NodeInfo;	// redefine this typedef!!
/* ==========================================*/


/*
 * hashtable的初始化；
 * 参数： table = 分配好的hashtable空间
          size = hashtable数组的大小
 * 返回值：0 */
int hashtable_init(ListHead *table, int size);

/*
 * 释放hashtable所占空间；
 * 参数： void
 * 返回值： void */
void hashtable_exit(void);

/*
 * 获取hashtable中的某个元素的指针
 * 参数： str = 字符串
 *        code = 附件的code校验值
 * 返回值： NULL = 该字符串不在hashtable中
 *          !NULL = 该字符串在hashtable中的节点的指针*/
NodeInfo *hashtable_getitem(const char *str, unsigned code);

/* 
 * 检测某个字符串是否存在于hashtable中；
 * 参数： str = 字符串
 *        code = 附加code校验值
 * 返回值： -1 = 字符串不在hashtable中
 *          0或者其他正数 = 字符串在hashtable中的index值*/
int hashtable_getindex(const char *str, unsigned code);

/*
 * 添加一个链表节点的信息到hashtable中；
 * 参数: node_info = 链表节点NodeInfo部分存放的内容
 * 返回值： 0或者其他正数 = 新加节点在hashtable中index值 */
int hashtable_add(const NodeInfo *node_info);

/*
 * 从hashtable中删除一个节点的信息；
 * 参数： node_info = 要删除的节点的内容
 * 返回值： -1 = 节点不存在
 *          0或者其他正数 = 被删除节点的index值*/
int hashtable_del(const NodeInfo *node_info);

/*
 * 清空整个hashtable中的链表
 * 参数： void
 * 返回值： void*/
void hashtable_clean(void);

#endif
