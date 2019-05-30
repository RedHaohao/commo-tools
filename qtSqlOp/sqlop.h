#ifndef SQLOP_H
#define SQLOP_H

#include <QtSql>
#include <QSqlDatabase>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QTime>
#include "ThreadParameter.h"
#include "qvector.h"


/********************************************************************************************
* ������  ��SqlOp
* �������ܣ�ʵ��������ݿ��������ɾ�Ĳ�
* //����ʱ �뱾���ļ������ݿ�����  map��Ӧ��key Ϊ1 
* //����ʱ �뱾���ļ������ݿ�����  map��Ӧ��key Ϊ2
* //����������������ݿ������ map ��Ӧ��key ��Ϊ��� ��int��

* *********************************************************************************************/
enum
{
	importYear = 1, //����ʱ �뱾���ļ������ݿ�����  map��Ӧ��key Ϊ1 
	exportYear = 2, //����ʱ �뱾���ļ������ݿ�����  map��Ӧ��key Ϊ2
};
class SqlOp 
{

public:
	SqlOp();
	~SqlOp();
	//����MySQL�����ݿ�  CreateDatabase("118.178.136.10", "cgpc2015", "tomato", "tomato");
	static bool CreateDatabase(const QString& HostName, const QString& DatabaseName, const QString& UserName, const QString& Password);

	
	//����MySql���ݿ��Ĭ�����ӣ�SqlOp::CreateConnection("118.178.136.10", "cgpc2018", "tomato", "tomato");
	//����Sqlite���ݿ��Ĭ�����ӣ�SqlOp::CreateConnection("118.178.136.10", "cgpc2017", "tomato", "tomato","","QSQLITE");
	//����MySql���ݿ�ķ�Ĭ�����ӣ�SqlOp::CreateConnection("118.178.136.10", "cgpc2018", "tomato", "tomato","connectionName");
	//����Sqlite���ݿ��Ĭ�����ӣ�SqlOp::CreateConnection("118.178.136.10", "cgpc2017", "tomato", "tomato","connectionName","QSQLITE");
	static bool CreateConnection(const QString& HostName, const QString& DatabaseName, const QString& UserName, const QString& Password, QString ConnectionName = QString(), const QString DbType = "QMYSQL");

	//��ȡ������MySql������ ������Ŀ���ݿ������
	static QVector<QString> GetDatabaseName(const QString& HostName,  const QString& UserName, const QString& Password);
	
	//���ݷ����������ݿ������  �������ӣ�������Ϣ��ӵ�������� mapDbConnectionName
	static bool InitConnection(QVector<QString > DbName, const QString& HostName, const QString& UserName, const QString& Password);
	
	//��⵱ǰ��� �Ƿ������ݿ⣬���򽨿� �����ӣ�������Ϣ��ӵ�������� mapDbConnectionName
	static bool CreateCurrentYearDB(QMap<int, QString>& map, const QString& HostName, const QString& UserName, const QString& Password); 
	
	//�ر�Ĭ�����ӣ�CloseConnection();
	//�رշ�Ĭ�����ӣ�CloseConnection("ConnectionName");
	static bool CloseConnection(const QString ConnectionName = QSqlDatabase::defaultConnection);

	//�رղ������������ݿ�����
	static bool CloseAllConnection(QMap<int, QString>& map);


	//����Ҫ��̬������SQL��䣬�����ַ����ͣ���Ҫ�õ�������ס
	//��Ҫ�õ���̬������SQL��䣬����Ҫ���
	//���� name��varchar���ͣ�age������
	//����Ҫ��̬������insert into person (name,age) values("zhangsan",13)  
	//��Ҫ��̬������ insert into person (name ,age) values(:zhangsan,:age)

	//select column from table where column=?
	//select column from table where column=:column

	//��ѯ��䣬��һ������ΪSQL��䣬�ڶ�������Ϊ��Ҫ��������������Ϊ����������ݿ�
	static QVector< QVector <QString> > SelectQuery(QString select, int num, const int year);
	//��ѯ��䣬��һ������ΪSQL��䣬�ڶ�������Ϊ����,������Ϊ��̬���������ĸ�Ϊ����������ݿ�
	static QVector< QVector <QString> > SelectQuery(QString select, int num, QVector<QString> bindValue, const int year);
	
	//�������Ƽ�ʹ����������
	//��һ������ΪSQL��䣬�ڶ�������Ϊ��Ҫ��������������Ϊ����������ݿ�
	static QVector< QVector <QVariant> > SelectQuery2(QString select, int num, const int year);
	//��ѯ��䣬��һ������ΪSQL��䣬�ڶ�������Ϊ����,������Ϊ��̬���������ĸ�Ϊ����������ݿ�
	static QVector< QVector <QVariant> > SelectQuery2(QString select, int num, QVector<QString> bindValue, const int year);
	
	//��ѯ�ⲿ���ݿ����������ʹ�õ�����
	//��ѯ��䣬��һ������ΪSQL��䣬�ڶ�������Ϊ��Ҫ��������������Ϊָ�����ⲿ����
	static QVector< QVector <QVariant> > SelectQuery3(QString select, int num, QSqlDatabase qSqlDB);
	
	//delete from table where column=?
	//delete from table where column=:column
	//update table set column=? where column=?
	//update table set column=:column where column=:column
	static bool Delete_UpdateQuery(QString del, const int year);
	static bool Delete_UpdateQuery(QString del, QVector<QVariant> bindValue, const int year);
	//insert into table (column1,cloumn2,column3,����) values(:column1,:column2,:column3,����)
	//insert into table (column1,cloumn2,column3,����) values(?,?,?,����)
	static bool InsertQuery(QString insert, const int year);
	static bool InsertQuery(QString insert, QVector< QVariant> bindValue, const int year);
	//��������

	//�����������bindvalue ��һ��һ���ֶ�
	//�������һ��һ�е��ֶ�
	static bool InsertQuery(QString insert, QVector<QVector<QVariant> > bindValue, const int year);
	static bool	InsertQuery1(QString insert, QVector<QVector<QVariant> > bindValue, const int year);
	//���� trackCheck�ȱ�
	static bool InsertTable(QString insert, QString append,QVector<QVector<QVariant> > result, int num, QSqlDatabase qSqlDB);

	//������Ŀ
	static int ExportProjectDb(const QString &dbName, const QString &ID,const int year);
	//������Ŀ
	static QVector< QVector <QVariant>> ImportProjectDbInfo(const QString &dbPath);
	static int ImportProjectDb(int year, int type);//type==1������ȫ�����ݣ�type==2��ֻ������Ŀ��Ϣ�͸ְ�ߴ磻
	
private:

	static QVector<QString> m_vSqlCreatable;//�洢SQL���
	
};



#endif // SQL_H
