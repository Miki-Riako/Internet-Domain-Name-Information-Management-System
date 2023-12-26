/*
互联网域名信息管理系统
【问题描述】
互联网域名系统是一个典型的树形层次结构。从根节点往下的第一层是顶层域，如cn、com等，最底层（第四层）是叶子结点，如www等。
【设计要求】
设计互联网域名信息管理的模拟程序。
（1）采用树的孩子兄弟链表等存储结构。
（2）可以随机、文件及人工输入数据。
（3）实现域名的创建、分类、插入、删除、修改等管理功能。
（4）实现域名的查询和统计功能。
（5）考虑采用优化的查询和排序算法。
（6）其它完善性或扩展性功能。
*/
// Internet Domain Name Information Management System
#include "IDNIMS.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IDNIMS w;
    return a.exec();
}
