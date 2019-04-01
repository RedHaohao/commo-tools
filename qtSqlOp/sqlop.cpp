#pragma execution_character_set("utf-8")
#include "sqlOp.h"
#include"getUUID.cpp"
#include "qdebug.h"
#include<QVectorIterator>
//#include <sqlite3.h>

QVector<QString> SqlOp::m_vSqlCreatable{ 1 };
SqlOp::SqlOp()
{
	//��ʼ��sql���
	//MFL_DevInfo 
	m_vSqlCreatable.pop_back();
	m_vSqlCreatable.append("�������");



	
	
}

SqlOp::~SqlOp()
{

}




/*******************************************************************************************
* �������ƣ�CreateDatabase
* �������ܣ������µ�mysql���ݿ�
* �����������1��HostName�����ݿ������ַ�� ��2��DatabaseName�����ݿ����ƣ� ��3��UserName���û��� ����4��Password������
* �����������
* ����ֵ  �������ɹ�����true  ʧ��ʱ����false
* ע���½������ݿ���������Ϊ������
********************************************************************************************/

bool SqlOp::CreateDatabase(const QString& HostName, const QString& DatabaseName, const QString& UserName, const QString& Password)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(HostName);
	db.setPort(3306);
	db.setUserName(UserName);
	db.setPassword(Password);
	if (!db.isValid())
	{
		qDebug() << db.lastError().text();
		return false;
	}
	db.open();
	QString  str = QString("Create Database If  Not Exists %1 Character Set UTF8;").arg(DatabaseName);//�½����ݿ��SQL���
	QSqlQuery query = db.exec(str);  //ִ�н����SQL���
	db.setDatabaseName(DatabaseName);

	if (db.open())
	{

		QVectorIterator<QString> i(m_vSqlCreatable);   //java��ʽ�ĵ�����
		while (i.hasNext())      //����ִ�н������
		{
			
			query.exec(i.next());
			if (query.lastError().isValid())
			{
				qDebug() <<query.lastError();
				return false;
			}
		
		}
		qDebug() << "CreatDatabase " + DatabaseName + "success";
		return true;
	}
	else
	{
		return false;
	}

}


/*******************************************************************************************
* �������ƣ�CreateConnection
* �������ܣ��½����ݿ����ӣ�����ָ������������Ĭ������
* �����������1��HostName�����ݿ������ַ�� ��2��DatabaseName�����ݿ����ƣ� ��3��UserName���û��� ����4��Password������
��5��ConnectionName: ������ ����Ĭ��ֵ������ָ���򴴽�����Ĭ������
��6��DbType: ���ݿ��������� Ĭ��ΪMYSQL   ��ѡ�QSQLITE��MYSQL
* �����������
* ����ֵ  �����ӳɹ�����true  ʧ��ʱ����false
********************************************************************************************/

bool SqlOp::CreateConnection(const QString& HostName, const QString& DatabaseName, const QString& UserName, const QString& Password, QString ConnectionName, QString DbType)
{ 	QSqlDatabase db;
	if (ConnectionName == QString() && DbType == "QMYSQL")
		db = QSqlDatabase::addDatabase("QMYSQL");
	else if (ConnectionName == QString() && DbType != "QMYSQL")
		db = QSqlDatabase::addDatabase(DbType);
	else if (ConnectionName != QString() && DbType == "QMYSQL")
		db = QSqlDatabase::addDatabase("QMYSQL", ConnectionName);
	else if (ConnectionName != QString() && DbType != "QMYSQL")
		db = QSqlDatabase::addDatabase(DbType, ConnectionName);

	db.setDatabaseName(DatabaseName);
	db.setHostName(HostName);
	db.setUserName(UserName);
	db.setPassword(Password); // û�������ʱ�����ע�͵�

	if (db.open())
	{
		qDebug() << "connect " + DatabaseName + " success";
		return true;
	}
	else
	{
		qDebug() << db.lastError().text();
		return false;
	}
}

/*******************************************************************************************
* �������ƣ�GetDatabaseName
* �������ܣ���ȡ���ݿ��� ����Ŀ����������ݿ������
* ���������HostName ������IP��ַ UserName ���û��� Password������
* �����������
* ����ֵ  ��QVector<QString> result �������ݿ������
********************************************************************************************/

QVector<QString>  SqlOp::GetDatabaseName(const QString& HostName, const QString& UserName, const QString& Password)
{
	CreateConnection(HostName, "mysql", UserName, Password, "getDBName");
	QVector<QString> result;
	QSqlDatabase db = QSqlDatabase::database("getDBName");
	QSqlQuery query(db);
	query.exec("show databases");
	while (query.next())
	{
		QString name = query.value(0).toString();
		if (name.contains("cgpc", Qt::CaseInsensitive))//ֻ��ȡ  �����а�����cgpc���ַ����� ����pc���ݿ������
		{
			qDebug() << name;
			result.append(name);
		}

	}
	return result;
}

/*******************************************************************************************
* �������ƣ�InitConnection
* �������ܣ������������Ŀ���������ݿ⽨�����ӣ��������������浽��������
* ���������DbName�����ݿ����б� HostName ������IP��ַ UserName ���û��� Password������
* �����������
* ����ֵ  ��QVector<QString> result �������ݿ������
********************************************************************************************/
bool SqlOp::InitConnection(QVector<QString> DbName, const QString& HostName, const QString& UserName, const QString& Password)
{
	//����������map ������� ����ʱ��û�н������ӣ�
	//���������ӵȵ�ʹ��ʱ�ŻὨ��
	ThreadParameter::getInstance()->mapDbConnectionName.insert(importYear, "local2remoteDB");
	ThreadParameter::getInstance()->mapDbConnectionName.insert(exportYear, "remote2localDB");
	
	
	for (int i = 0; i < DbName.size(); i++)
	{

		QString temp = DbName[i].mid(4, 4);		//���ݿ�����Ϊcgpc2018��  tempֻȡ��������
		QString ConnectionName = "remoteDB" + temp; //������ �����淶 remoteDB+���
		if (CreateConnection(HostName, DbName[i], UserName, Password, ConnectionName))
		{
			ThreadParameter::getInstance()->mapDbConnectionName.insert(temp.toInt(), ConnectionName); //��<��ݣ�������>�浽������
			qDebug() << "create " << ConnectionName << " sucess";
		}
		else
		{
			qDebug() << "Failed to initialize connection";
			return false;
		}
	}
	return true;
}

/*******************************************************************************************
* �������ƣ�CreateCurrentYearDB
* �������ܣ���⵱ǰ��� �Ƿ��е�ǰ��ݵ����ݿ⣬���򽨿� �����ӣ�������Ϣ��ӵ�������� mapDbConnectionName
* ���������HostName ������IP��ַ UserName ���û��� Password������  map��ϵͳ�������е�map
* �����������
* ����ֵ  ��QVector<QString> result �������ݿ������
********************************************************************************************/

bool SqlOp::CreateCurrentYearDB(QMap<int, QString>& map, const QString& HostName, const QString& UserName, const QString& Password)
{
	int year = ThreadParameter::getInstance()->currentYear; //��ȡ��ǰ���
	QString dbName = "cgpc" + QString::number(year);  //���ݿ����ָ�ʽ cgpc+���
	QString connectionName = "remoteDB" + QString::number(year); //�������ָ�ʽ remoteDB +���
	if (map.contains(year)) //���������ǰ��ݵ����ݿ�  �򷵻� 
	{
		qDebug() << "cgpc" << year << "exits";
		return true;
	}
	else
	{

		if (SqlOp::CreateDatabase(HostName, dbName, UserName, Password)) //������ǰ������ݿ�
		{
			if (SqlOp::CreateConnection(HostName, dbName, UserName, Password, connectionName)) //��������
			{
				map.insert(year, connectionName);   //��ӵ���������
				QString procedureImportCombine =
					"CREATE DEFINER = `tomato`@`%` PROCEDURE `ImportCombine`(IN `pID` CHAR(38))"
					" BEGIN"
					" START TRANSACTION;"
					" DELETE"
					" FROM"
					" MFL_TrackSSRecord"
					" WHERE"
					" RecordID IN("
					" SELECT"
					" MFL_TrackSSRecord.RecordID"
					" FROM"
					" (SELECT RecordID FROM MFL_TrackSSRecord WHERE"
					" TrackID IN(SELECT TrackID FROM MFL_TrackInfo WHERE PlateID IN(SELECT PlateID FROM MFL_PlateInfo WHERE ProjectID = pID))"
					" ) MFL_TrackSSRecord);"
					" DELETE"
					" FROM"
					" MFL_TrackCheckRecord"
					" WHERE"
					" RecordID IN(SELECT MFL_TrackCheckRecord.RecordID FROM(SELECT RecordID FROM MFL_TrackCheckRecord WHERE TrackID IN(SELECT TrackID FROM MFL_TrackInfo WHERE PlateID = pID)) MFL_TrackCheckRecord);"
					" DELETE"
					" FROM"
					" MFL_TrackInfo"
					" WHERE"
					" PlateID = pID;"
					" DELETE"
					" FROM"
					" MFL_PlateInfo"
					" WHERE"
					" PlateID = pID;"
					" COMMIT;"
					" END;";
				QString procedureImportCover =
					"CREATE DEFINER = `tomato`@`%` PROCEDURE `ImportCover`( IN `pID` CHAR(38)) "
					" BEGIN"
					" START TRANSACTION;"
					" DELETE"
					" FROM"
					" MFL_TrackSSRecord"
					" WHERE"
					" RecordID IN("
					" SELECT"
					" MFL_TrackSSRecord.RecordID"
					" FROM"
					" (SELECT RecordID FROM MFL_TrackSSRecord WHERE"
					" TrackID IN(SELECT TrackID FROM MFL_TrackInfo WHERE PlateID IN(SELECT PlateID FROM MFL_PlateInfo WHERE ProjectID = pID))"
					" ) MFL_TrackSSRecord);"
					" DELETE"
					" FROM"
					" MFL_TrackCheckRecord"
					" WHERE"
					" RecordID IN("
					" SELECT"
					" MFL_TrackCheckRecord.RecordID"
					" FROM"
					" (SELECT RecordID FROM MFL_TrackCheckRecord WHERE"
					" TrackID IN(SELECT TrackID FROM MFL_TrackInfo WHERE PlateID IN(SELECT PlateID FROM MFL_PlateInfo WHERE ProjectID = pID))"
					" ) MFL_TrackCheckRecord);"
					" DELETE FROM MFL_TrackInfo WHERE TrackID IN(SELECT MFL_TrackInfo.TrackID FROM(SELECT * FROM MFL_TrackInfo WHERE PlateID IN(SELECT PlateID FROM MFL_PlateInfo WHERE ProjectID = pID)) MFL_TrackInfo);"
					" DELETE FROM MFL_PlateInfo WHERE ProjectID = pID;"
					" DELETE FROM MFL_ProjectInfo WHERE ProjectID = pID;"
					" COMMIT;"
					" END;";
				QSqlQuery q(QSqlDatabase::database(connectionName));
				q.exec(procedureImportCombine);
				q.exec(procedureImportCover);
				return true;
			}
			else
			{
				qDebug() << "failed to create" << connectionName;
				return false;
			}
		}
		else
		{
			qDebug() << "failed to create" << dbName;
			return false;
		}


	}
}
/*******************************************************************************************
* �������ƣ�CloseConnection
* �������ܣ��ر����ݿ�����
* ���������ConnectionName������������Ĭ��ֵ����ָ����ر�Ĭ������
* �����������
* ����ֵ  ����
********************************************************************************************/
bool SqlOp::CloseConnection(QString ConnectionName)
{
	QSqlDatabase db = QSqlDatabase::database(ConnectionName, false);//false ָ������δ�򿪵����ӣ���Ϊtrue��δ�򿪵����ӽ��ᱻ��
	if (db.isOpen())
	{
		db.close();
		QSqlDatabase::removeDatabase(ConnectionName);
		qDebug() << "close " + ConnectionName + " success";
		return true;
	}


	return false;
}



bool SqlOp::CloseAllConnection(QMap<int, QString>& map)
{
	QMap<int, QString>::iterator itr;
	for (itr = map.begin(); itr != map.end(); itr++)
	{
		int temp = itr.key();
		if (!CloseConnection(map[temp]))
		{
			return false;
		}
	}
	return true;
}
/*******************************************************************************************
* �������ƣ�SelectQuery
* �������ܣ���ѯ��䣬���ص�����Ϊ�ַ�����ʽ
* �����������1��select��sql��� ��2��num����ѯ������ (3)year�� Ҫ��ѯ���ݿ�����
* ���������result
* ����ֵ  ����ѯ���
********************************************************************************************/
QVector< QVector <QString> > SqlOp::SelectQuery(QString select, int num, const int year)
{
	QVector< QVector <QString> >  result;   //��Ų�ѯ���
	if (!ThreadParameter::getInstance()->mapDbConnectionName.contains(year))
	{
		qDebug() << "No connections are available";
		return result;
	}
	QSqlDatabase db = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[year], false);
	QSqlQuery query(db);
	query.exec(select);

	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
	}
	else if (!db.isOpen())
	{
		qDebug() << "database not open";
	}
	else
	{
		qDebug() << "select success";
		while (query.next())
		{
			QVector <QString> resultTemp;
			QString temp;
			for (int i = 0; i < num; i++)
			{
				temp = query.value(i).toString();
				resultTemp.append(temp);
			}
			result.append(resultTemp);
		}
	}

	return result;
}

/*******************************************************************************************
* �������ƣ�SelectQuery
* �������ܣ�����̬��������ѯ��䣬���ص�����Ϊ�ַ�����ʽ
* �����������1��select��sql��� ��2��num����ѯ������ ��3��bindValue����̬�������� ��4��year�� Ҫ��ѯ���ݿ�����
* ���������result
* ����ֵ  ����ѯ���
********************************************************************************************/
QVector< QVector <QString> > SqlOp::SelectQuery(QString select, int num, QVector<QString> bindValue, const int year)
{
	QVector< QVector <QString> >  result;   //��Ų�ѯ���
	if (!ThreadParameter::getInstance()->mapDbConnectionName.contains(year))
	{
		qDebug() << "No connections are available";
		return result;
	}
	QSqlDatabase db = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[year], false);
	QSqlQuery query(db);
	query.prepare(select);
	for (int i = 0; i < bindValue.size(); i++)
	{
		query.bindValue(i, bindValue[i]);
	}
	query.exec();

	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
	}
	else if (!db.isOpen())
	{
		qDebug() << "database not open";
	}
	else
	{
		qDebug() << "select success";
		while (query.next())
		{
			QVector <QString> resultTemp;
			QString temp;
			for (int i = 0; i < num; i++)
			{
				temp = query.value(i).toString();
				resultTemp.append(temp);
			}
			result.append(resultTemp);
		}
	}

	return result;
}

/*******************************************************************************************
* �������ƣ�SelectQuery2
* �������ܣ���ѯ��䣬���ص�����Ϊ�ɱ�������ʽ
* �����������1��select��sql��� ��2��num����ѯ������ (3) year�� Ҫ��ѯ���ݿ�����
* ���������result
* ����ֵ  ����ѯ���
********************************************************************************************/
QVector< QVector <QVariant> > SqlOp::SelectQuery2(QString select, int num, const int year)
{
	QVector< QVector <QVariant> >  result;   //��Ų�ѯ���
	if (!ThreadParameter::getInstance()->mapDbConnectionName.contains(year))
	{
		qDebug() << "No connections are available";
		return result;
	}
	QSqlDatabase db = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[year], false);
	QSqlQuery query(db);
	query.exec(select);
	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
	}
	else if (!db.isOpen())
	{
		qDebug() << "database not open";
	}
	else
	{
		qDebug() << "select success";
		while (query.next())
		{
			QVector <QVariant> resultTemp;
			QVariant temp;
			for (int i = 0; i < num; i++)
			{
				temp = query.value(i);
				resultTemp.append(temp);
			}
			result.append(resultTemp);
		}
	}

	return result;
}

/*******************************************************************************************
* �������ƣ�SelectQuery2
* �������ܣ�����̬��������ѯ��䣬���ص�����Ϊ�ɱ�������ʽ
* �����������1��select��sql��� ��2��num����ѯ������ ��3��bindValue����̬�������� (4)  year�� Ҫ��ѯ���ݿ�����
* ���������result
* ����ֵ  ����ѯ���
********************************************************************************************/
QVector< QVector <QVariant> > SqlOp::SelectQuery2(QString select, int num, QVector<QString> bindValue, const int year)
{
	QVector< QVector <QVariant> >  result;   //��Ų�ѯ���
	if (!ThreadParameter::getInstance()->mapDbConnectionName.contains(year))
	{
		qDebug() << "No connections are available";
		return result;
	}
	QSqlDatabase db = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[year], false);
	QSqlQuery query(db);
	query.prepare(select);
	for (int i = 0; i < bindValue.size(); i++)
	{
		query.bindValue(i, bindValue[i]);
	}
	query.exec();

	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
	}
	else if (!db.isOpen())
	{
		qDebug() << "database not open";
	}
	else
	{
		qDebug() << "select success";
		while (query.next())
		{
			QVector <QVariant> resultTemp;
			QVariant temp;
			for (int i = 0; i < num; i++)
			{
				temp = query.value(i);
				resultTemp.append(temp);
			}
			result.append(resultTemp);
		}
	}

	return result;
}


/*******************************************************************************************
* �������ƣ�SelectQuery3
* �������ܣ���ѯ��䣬���ص�����Ϊ�ɱ�������ʽ
* �����������1��select��sql��� ��2��num����ѯ������ ��3��ָ�����ݿ��ѯ
* ���������result
* ����ֵ  ����ѯ���
********************************************************************************************/
QVector< QVector <QVariant> > SqlOp::SelectQuery3(QString select, int num, QSqlDatabase qSqlDB)
{
	QVector< QVector <QVariant> >  result;   //��Ų�ѯ���
	QSqlQuery query(qSqlDB);
	query.exec(select);
	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
	}
	else
	{
		qDebug() << "select success";
		while (query.next())
		{
			QVector <QVariant> resultTemp;
			QVariant temp;
			for (int i = 0; i < num; i++)
			{
				temp = query.value(i);
				resultTemp.append(temp);
			}
			result.append(resultTemp);
		}
	}

	return result;
}
/*******************************************************************************************
* �������ƣ�Delete_UpdateQuery
* �������ܣ�ɾ�����߸������
* �����������1��del��sql���  ��2��year�� Ҫ��ѯ���ݿ����� (3)
* ���������bool
* ����ֵ  ���������
********************************************************************************************/
bool SqlOp::Delete_UpdateQuery(QString del, const int year)
{
	if (!ThreadParameter::getInstance()->mapDbConnectionName.contains(year))
	{
		qDebug() << "No connections are available";
		return false;
	}
	QSqlDatabase db = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[year], false);
	QSqlQuery query(db);
	query.exec(del);

	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
		return false;
	}
	else if (!db.isOpen())
	{
		qDebug() << "database not open";
		return false;
	}
	qDebug() << "Delete_Update success";
	return true;
}

/*******************************************************************************************
* �������ƣ�Delete_UpdateQuery
* �������ܣ�����̬������ɾ�����߸������
* �����������1��del��sql��䣻��2��bindValue����̬�������� ��3��year�� Ҫ��ѯ���ݿ�����
* ���������bool
* ����ֵ  ���������
********************************************************************************************/
bool SqlOp::Delete_UpdateQuery(QString del, QVector<QVariant> bindValue, const int year)
{
	if (!ThreadParameter::getInstance()->mapDbConnectionName.contains(year))
	{
		qDebug() << "No connections are available";
		return false;
	}
	QSqlDatabase db = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[year], false);
	QSqlQuery query(db);
	query.prepare(del);
	for (int i = 0; i < bindValue.size(); i++)
	{
		query.bindValue(i, bindValue[i]);
	}
	QList<QVariant> list = query.boundValues().values();
	for (int i = 0; i < list.size(); ++i)
		qDebug() << i << ": " << list.at(i).toString().toUtf8().data() << endl;
	query.exec();

	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
		return false;
	}
	else if (!db.isOpen())
	{
		qDebug() << "database not open";
		return false;
	}
	qDebug() << "Delete_Update success";
	return true;
}

/*******************************************************************************************
* �������ƣ�InsertQuery
* �������ܣ��������
* �����������1��insert��sql��䣻��2��year�� Ҫ��ѯ���ݿ�����
* ���������bool
* ����ֵ  ���������
********************************************************************************************/
bool SqlOp::InsertQuery(QString insert, const int year)
{
	if (!ThreadParameter::getInstance()->mapDbConnectionName.contains(year))
	{
		qDebug() << "No connections are available";
		return false;
	}
	QSqlDatabase db = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[year], false);
	QSqlQuery query(db);
	query.exec(insert);
	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
		return false;
	}
	else if (!db.isOpen())
	{
		qDebug() << "database not open";
	}
	qDebug() << "insert success";
	return true;
}

/*******************************************************************************************
* �������ƣ�InsertQuery
* �������ܣ�������䣬���в���
* �����������1��insert��sql��䣻 ��2��bindValue����̬����  ��3��year�� Ҫ��ѯ���ݿ�����
* ���������bool
* ����ֵ  ���������
********************************************************************************************/
bool SqlOp::InsertQuery(QString insert, QVector<QVariant> bindvalue, const int year)
{
	if (!ThreadParameter::getInstance()->mapDbConnectionName.contains(year))
	{
		qDebug() << "No connections are available";
		return false;
	}
	QSqlDatabase db = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[year], false);
	int a = 0;
	QSqlQuery query(db);
	query.prepare(insert);
	for (int i = 0; i < bindvalue.size(); i++)
	{
		query.bindValue(i, bindvalue[i]);
	}
	qDebug() << "insert success" << query.exec();

	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
		return false;
	}
	else if (!db.isOpen())
	{
		qDebug() << "database not open";
	}

	return true;
}
/*******************************************************************************************
* �������ƣ�InsertTable
* �������ܣ������¼�ܶ�ı� �磨trackcheckRecord SSRecord�ȣ�
* 1.����sql����� insert into ��table�� values (?,?)������(?,?) һ�β���ʮ����¼���ӿ��ٶ� 
* 2.�Խ�������д��� ʹ��ֳ�10�� �������е�ƥ��
* 3.ִ��sql���������
* 4.�����ĵ���ʣ���� �ļ�¼���д���  (�� 104����¼ ���ʣ��4�� �Ͳ���ƴ�ճ�һ�β���ʮ��)
* 5.ע ��Ϊʲôѡ��һ��10�� ����Ϊ ����һЩ���� ����10�����
* �����������1��insert��������䣻 ��2��append��ƴ�����  (3) result: Ҫ��������ݼ� ��4�� num ��������� ��5��qSqlDB��Ҫ��������ݿ�
* ����ֵ  ��false ʧ�� true �ɹ�
********************************************************************************************/
bool SqlOp::InsertTable(QString insert,QString append, QVector<QVector<QVariant> > result, int num, QSqlDatabase qSqlDB)
{
	QSqlQuery query(qSqlDB);
	QString tmpStr = insert;
	if (result.size() > 100)
	{
		//����sql���
		for (int i = 0; i < 9; i++)
		{
			insert.append(',');
			insert += append;
		}
		query.prepare(insert);

		//���������һ��һ�б��һ��һ��
		QVariantList *colResult = new QVariantList[num];
		for (int i = 0; i < result.size(); i++)
		{
			for (int j = 0; j < num; j++)
				colResult[j] << result[i][j];
		}

		int rowCount = result.size() / 10;  //һ���ֵܷ�����
		//���з��� colRes10 ����˼�ǽ�colRes�ֳ�10��
		QVariantList *colRes10 = new QVariantList[num];
		for (int i = 0; i < 10 * rowCount; i++)
		{
			for (int j = 0; j < num; j++)
			{
				colRes10[j] << colResult[j][i];
			}
			if ((i + 1) % rowCount == 0)
			{
				for (int k = 0; k < num; k++)
				{
					query.addBindValue(colRes10[k]);
					colRes10[k].clear();
				}

			}
		}
		
		if (!query.execBatch()) //�������������������������
		{
			qDebug() << query.lastError();
			return false;
		}


		//�������ʣ�µķ���
		query.prepare(tmpStr);
		for (int i = 10 * rowCount; i < result.size(); i++)
		{
			for (int j = 0; j < num; j++)
			{
				colRes10[j] << colResult[j][i];
			}
		}

		for (int i = 0; i < num; i++)
		{
			query.addBindValue(colRes10[i]);
		}
		
		if (!query.execBatch()) //�������������������������
		{
			qDebug() << query.lastError();
			return false;
		}
		
	}
	//�����������С ֱ��ʹ�����������
	else
	{
		
		query.prepare(tmpStr);
		QVariantList *colResult = new QVariantList[num];
		for (int i = 0; i < result.size(); i++)
		{
			for (int j = 0; j < num; j++)
				colResult[j] << result[i][j];
		}
		for (int i = 0; i < num; i++)
		{
			query.addBindValue(colResult[i]);
		}
		if (!query.execBatch()) //�������������������������
		{
			qDebug() << query.lastError();
			return false;
		}
	}
	return true;
}
/*******************************************************************************************
* �������ƣ�ExportProjectDb
* �������ܣ�����sqlite���ݿ⵽����
* �����������1��dbName�����ݿ�����·���� ��2��ID��projectID
* ���������int
* ����ֵ  ��0���ɹ�  -1�� ����������ʧ�� -2�� sql���ִ��ʧ�� -3�� �����ύʧ��
********************************************************************************************/

int SqlOp::ExportProjectDb(const QString &dbName, const QString &ID, const int year)
{
	QTime time;
	time.start();
	// *************��������sqlite���ݿ� ������
	if (!CreateConnection("localhost", dbName, "", "", "remote2localDB", "QSQLITE"))
	{
		qDebug() << "Create Connection failed";
		return -1;
	}

	QSqlDatabase exportdb = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[exportYear]);

	exportdb.transaction();
	QSqlQuery query(exportdb);




	//����
	for (int i = 0; i < m_vSqlCreatable.size(); i++)
	{
		if (!query.exec(m_vSqlCreatable[i]))
			qDebug()<<i << "create table failed";
	}



	//��ѯ  MFL_ProjectInfo ��
	const int projectTableCounts = 35; //��Ŀ��Ϣ�������
	QVector<QString> bindValue;
	bindValue.append(ID);
	QString query_projectinfo = "select * from MFL_ProjectInfo where ProjectID = ? ";
	QVector< QVector<QVariant> > projectinfo;
	projectinfo = SqlOp::SelectQuery2(query_projectinfo, projectTableCounts, bindValue, year);
	qDebug() << "select projectinfo time:" << time.elapsed() / 1000.0 << "s";

	//��ѯ MFL_DevInfo  ��
	const int devinfoTableCounts = 20;  //�豸��Ϣ�������
	QString query_devinfo = "select * from MFL_DevInfo where DevID in (select DevID from MFL_ProjectInfo where ProjectID = ?)";
	QVector< QVector<QVariant> > devinfo;
	devinfo = SqlOp::SelectQuery2(query_devinfo, devinfoTableCounts, bindValue, year);
	qDebug() << devinfo[0].size();

	//��ѯ MFL_DevFactors ��
	const int devFactorsTableCounts = 41;
	QString query_devFactors = QObject::tr("SELECT * FROM MFL_DevFactors WHERE DevID = '%1' ").arg(devinfo[0][0].toString());
	QVector< QVector<QVariant> > devFactorsInfo;
	devFactorsInfo = SqlOp::SelectQuery2(query_devFactors, devFactorsTableCounts,year);
	
	//��ѯ MFL_DeAdjust ��
	const int deAdjustTableCounts = 26;   //�ְ�������
	QString query_Adjust = QObject::tr("SELECT * FROM MFL_DeAdjust WHERE DevID = '%1' ").arg(devinfo[0][0].toString());
	QVector< QVector<QVariant> > deAdjustInfo;
	deAdjustInfo = SqlOp::SelectQuery2(query_Adjust, deAdjustTableCounts, year);
	qDebug() << "deAdjustInfo time:" << time.elapsed() / 1000.0 << "s";

	//��ѯ MFL_PlateInfo  ��
	const int plateTableCounts = 33;   //�ְ�������
	QString query_plateinfo = "select * from MFL_PlateInfo where ProjectID = ? ";
	QVector< QVector<QVariant> > plateinfo;
	plateinfo = SqlOp::SelectQuery2(query_plateinfo, plateTableCounts, bindValue, year);
	qDebug() << "plateinfo time:" << time.elapsed() / 1000.0 << "s";

	//��ѯMFL_TrackInfo ��
	const int trackInfoTableCounts = 22;   //�켣��Ϣ�������
	QString query_trackinfo = "select * from MFL_TrackInfo where PlateID in(select PlateID from MFL_PlateInfo where ProjectID = ?)";
	QVector< QVector<QVariant> > trackinfo;
	trackinfo = SqlOp::SelectQuery2(query_trackinfo, trackInfoTableCounts, bindValue, year);
	qDebug() << "trackinfo time:" << time.elapsed() / 1000.0 << "s";

	//��ѯMFL_TrackCheckRecord��
	const int trackRecordTableCounts = 10;
	QString query_trackrecord = "select * from MFL_TrackCheckRecord where TrackID in (select TrackID from MFL_TrackInfo where PlateID in(select PlateID from MFL_PlateInfo where ProjectID = ?))";
	QVector< QVector<QVariant> > trackrecord;
	trackrecord = SqlOp::SelectQuery2(query_trackrecord, trackRecordTableCounts, bindValue, year);
	qDebug() << trackrecord.size();
	qDebug() << "trackrecord time:" << time.elapsed() / 1000.0 << "s";

	//��ѯMFL_TrackSSRecord��
	const int trackSSRecordTableCounts = 9;
	QString  query_trackSSRecordInfo = "select * from MFL_TrackSSRecord where TrackID in (select TrackID from MFL_TrackInfo where PlateID in(select PlateID from MFL_PlateInfo where ProjectID = ?))";
	QVector< QVector<QVariant> > trackSSRecordInfo;
	trackSSRecordInfo = SqlOp::SelectQuery2(query_trackSSRecordInfo, trackSSRecordTableCounts, bindValue, year);
	
	//�����ݿ����   MFL_DevInfo ��
	query.prepare("INSERT INTO MFL_DevInfo values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
	for (int i = 0; i < devinfo[0].size(); i++)
	{
		query.bindValue(i, devinfo[0][i]);
	}
	if (!query.exec()) //�������������������������
	{
		qDebug() << query.lastError();
		exportdb.rollback();
		return -2;
	}

	//�����ݿ��в��� MFL_devAdjust��
	query.prepare("INSERT INTO MFL_DeAdjust values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
	QVariantList *deAdjust = new QVariantList[deAdjustTableCounts];
	for (int i = 0; i < deAdjustInfo.size(); i++)
	{
		for (int j = 0; j < deAdjustTableCounts; j++)
			deAdjust[j] << deAdjustInfo[i][j];
	}
	for (int i = 0; i < deAdjustTableCounts; i++)
	{
		query.addBindValue(deAdjust[i]);
	}
	if (!query.exec()) //�������������������������
	{
		qDebug() <<"aaaa"<< query.lastError();
		exportdb.rollback();
		return -2;
	}


	//�����ݿ��в��� MFL_DevFactors��
	query.prepare("INSERT INTO MFL_DevFactors values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,? )");
	QVariantList *devFactors = new QVariantList[devFactorsTableCounts];
	for (int i = 0; i < devFactorsInfo.size(); i++)
	{
		for (int j = 0; j < devFactorsTableCounts; j++)
			devFactors[j] << devFactorsInfo[i][j];
	}
	for (int i = 0; i < devFactorsTableCounts; i++)
	{
		query.addBindValue(devFactors[i]);
	}
	if (!query.execBatch()) //�������������������������
	{
		qDebug() <<"bbbbbbb"<< query.lastError();
		exportdb.rollback();
		return -2;
	}

	//�������ݿ����  MFL_ProjectInfo ��
	query.prepare("INSERT INTO MFL_ProjectInfo values(?,?,? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ?, ?, ?, ?,?  )");
	QVariantList *project = new QVariantList[projectTableCounts];
	for (int i = 0; i < projectinfo.size(); i++)
	{
		for (int j = 0; j < projectTableCounts; j++)
			project[j] << projectinfo[i][j];
	}
	for (int i = 0; i < projectTableCounts; i++)
	{
		query.addBindValue(project[i]);
	}
	if (!query.execBatch()) //�������������������������
	{
		qDebug() << query.lastError();
		exportdb.rollback();
		return -2;
	}

	//�������ݿ����  MFL_PlateInfo ��
	query.prepare("INSERT INTO MFL_PlateInfo values(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ?, ?, ?, ?,?  )");
	QVariantList *plate = new QVariantList[plateTableCounts];
	for (int i = 0; i < plateinfo.size(); i++)
	{
		for (int j = 0; j < plateTableCounts; j++)
			plate[j] << plateinfo[i][j];
	}
	for (int i = 0; i < plateTableCounts; i++)
	{
		query.addBindValue(plate[i]);
	}
	if (!query.execBatch()) //�������������������������
	{
		qDebug() << query.lastError();
		exportdb.rollback();
		return -2;
	}

	//�������ݿ����  MFL_TrackInfo ��
	query.prepare("INSERT INTO MFL_TrackInfo values(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ?, ?, ?)");
	QVariantList *track = new QVariantList[trackInfoTableCounts];
	for (int i = 0; i < trackinfo.size(); i++)
	{
		for (int j = 0; j < trackInfoTableCounts; j++)
			track[j] << trackinfo[i][j];
	}
	for (int i = 0; i < trackInfoTableCounts; i++)
	{
		query.addBindValue(track[i]);
	}
	if (!query.execBatch()) //�������������������������
	{
		qDebug() << query.lastError();
		exportdb.rollback();
		return -2;
	}

	//�������ݿ����  MFL_TrackCheckRecord ��
	query.prepare("INSERT INTO MFL_TrackCheckRecord values(? ,?, ? , ? , ? , ? , ? , ? , ? , ? )");
	QVariantList *trackcheck = new QVariantList[trackRecordTableCounts];
	for (int i = 0; i < trackrecord.size(); i++)
	{
		for (int j = 0; j < trackRecordTableCounts; j++)
			trackcheck[j] << trackrecord[i][j];
	}
	for (int i = 0; i < trackRecordTableCounts; i++)
	{
		query.addBindValue(trackcheck[i]);
	}
	if (!query.execBatch()) //�������������������������
	{
		qDebug() << query.lastError();
		exportdb.rollback();
		return -2;
	}
	qDebug() << "export time:" << time.elapsed() / 1000.0 << "s";

	//�����ݿ��в���MFL_TrackSSRecord��
	query.prepare("INSERT INTO MFL_TrackSSRecord values(? ,?, ? , ? , ? , ? , ? , ? , ?  )");
	QVariantList *trackSSRecord = new QVariantList[trackSSRecordTableCounts];
	for (int i = 0; i < trackSSRecordInfo.size(); i++)
	{
		for (int j = 0; j < trackSSRecordTableCounts; j++)
			trackSSRecord[j] << trackSSRecordInfo[i][j];
	}
	for (int i = 0; i < trackSSRecordTableCounts; i++)
	{
		query.addBindValue(trackSSRecord[i]);
	}
	if (!query.execBatch()) //�������������������������
	{
		qDebug() <<"bbbbbbvccc"<< query.lastError();
		exportdb.rollback();
		return -2;
	}
	bool result = exportdb.commit();
	if (result)
	{
		return 0;
	}
	else
	{
		return -3;
	}
}

/*******************************************************************************************
* �������ƣ�ImportProjectDbInfo
* �������ܣ����ⲿ�����ļ�.db�е�����Ŀ��Ϣ
* �����������1���ⲿ�����ļ���·����
* ���������QVector< QVector <QVariant>> ����projectInfo,
* ����ֵ  ���������
********************************************************************************************/
QVector< QVector <QVariant>> SqlOp::ImportProjectDbInfo(const QString &dbPath)
{
	QSqlDatabase importDB = QSqlDatabase::addDatabase("QSQLITE", "importDB");
	importDB.setDatabaseName(dbPath);
	if (!importDB.open()) {
		qDebug() << "Database Can't open!";
	}
	QVector< QVector<QVariant> > projectinfo;
	QString queryProject = "SELECT ProjectID FROM MFL_ProjectInfo";
	QVector< QVector <QVariant> > queryResult = SelectQuery3(queryProject, 1, importDB);
	if (queryResult.size() != 0)
	{
		QString projectID = queryResult[0][0].toString();

		//��ѯ  MFL_ProjectInfo ��
		QString query_projectinfo = QObject::tr("select ProjectName,CompanyName,CheckMan,MFLAlert,EnvLength,EdgePlateNumber,EdgePlateWidth,UsedYear,CoatStatus,PlateMaterial,PlateThickness,StoreMaterial,CoatThickness,ProjectID from MFL_ProjectInfo where ProjectID ='%1' ").arg(projectID);

		projectinfo = SelectQuery3(query_projectinfo, 14, importDB);
	}

	return projectinfo;
}
/*******************************************************************************************
* �������ƣ�ImportProjectDb
* �������ܣ����ⲿ�����ļ�.db�е��뱾�����ݿ�
* �����������1���ⲿ�����ļ���·����
* ���������void
* ����ֵ  ��int 0���ɹ�   -1 ������������ʧ��   -2��sql���ִ��ʧ��  -3�������ύʧ��
********************************************************************************************/
int SqlOp::ImportProjectDb(int year, int type)
{


	QTime time;
	time.start();

	
	const int devTableCounts = 20;
	const int deAdjustTableCounts = 26;
	const int deFactorsTableCounts = 41;
	const int projectInfoTableCounts = 35;
	const int plateInfoTableCounts = 33;
	const int trackInfoTableCounts = 22;
	const int checkRecordTableCounts = 10;
	const int checkSSRecordTableCounts = 9;

	QSqlDatabase importDB = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[importYear]);

	//��ѯ  MFL_DevInfo��
	QString query_devinfo = "select * from MFL_DevInfo";
	QVector< QVector <QVariant> > devinfo = SelectQuery3(query_devinfo, devTableCounts, importDB);
	QString devID = devinfo[0][0].toString();

	QString queryProject = "SELECT ProjectID FROM MFL_ProjectInfo";
	QVector< QVector <QVariant> > queryResult = SelectQuery3(queryProject, 1, importDB);
	QString projectID = queryResult[0][0].toString();



	//��ѯ  MFL_deAdjust ��
	QString query_devAdjustInfo = QObject::tr("SELECT * FROM MFL_DeAdjust WHERE DevID = '%1' ").arg(devID);
	QVector< QVector<QVariant> > deAdjustInfo;
	deAdjustInfo = SelectQuery3(query_devAdjustInfo, deAdjustTableCounts, importDB);
	
	//��ѯ  MFL_DevFactors��
	QString query_devFactorsInfo = QObject::tr("SELECT * FROM MFL_DevFactors WHERE DevID = '%1' ").arg(devID);
	QVector< QVector<QVariant> > deFactorsInfo;
	deFactorsInfo = SelectQuery3(query_devFactorsInfo, deFactorsTableCounts, importDB);

	//��ѯ  MFL_ProjectInfo ��
	QString query_projectinfo = QObject::tr("select * from MFL_ProjectInfo where ProjectID = '%1' ").arg(projectID);
	QVector< QVector<QVariant> > projectinfo;
	projectinfo = SelectQuery3(query_projectinfo, projectInfoTableCounts, importDB);


	//��ѯ MFL_PlateInfo  ��
	QString query_plateinfo = QObject::tr("select * from MFL_PlateInfo where ProjectID = '%1' ").arg(projectID);
	QVector< QVector<QVariant> > plateinfo;
	plateinfo = SelectQuery3(query_plateinfo, plateInfoTableCounts, importDB);

	//��ѯMFL_TrackInfo ��
	QString query_trackinfo = QObject::tr("select * from MFL_TrackInfo where PlateID in(select PlateID from MFL_PlateInfo where ProjectID = '%1')").arg(projectID);
	QVector< QVector<QVariant> > trackinfo;
	trackinfo = SelectQuery3(query_trackinfo, trackInfoTableCounts, importDB);



	qDebug() << "selected time:" << time.elapsed() / 1000.0 << "s";

	/*-----------------����������ݿ��������-----------------------------*/
	bool flag = true; //�ж�sql����Ƿ�ִ�гɹ�

	QSqlDatabase dbSqlOp = QSqlDatabase::database(ThreadParameter::getInstance()->mapDbConnectionName[year]);
	QSqlQuery query(dbSqlOp);

	if (!query.exec("START TRANSACTION"))//�������� 
	{
		flag = false;
	};

	//��ѯ���������Ƿ��и��豸 
	int devCount = -1;
	QString devQuery = "select count(*) from MFL_DevInfo where DevID =%1 ";
	query.exec(devQuery.arg(devID));
	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
	}
	else
	{
		qDebug() << "select success";
		query.next();
		devCount = query.value(0).toInt();
	}

	//����и��豸�͸�����Ϣ ��û�оͲ���
	if (devCount == 0)
	{
		//�����ݿ����   MFL_DevInfo ��
		query.prepare("INSERT INTO MFL_DevInfo values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		for (int i = 0; i < devinfo[0].size(); i++)
		{
			query.bindValue(i, devinfo[0][i]);
		}
		if (!query.exec()) //�������������������������
		{
			qDebug() << query.lastError();
			flag = false;
		}

		//���� MFL_devAdjust��
		query.prepare("INSERT INTO MFL_DeAdjust values(?,?,date(?),?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		QVariantList *deAdjust = new QVariantList[deAdjustTableCounts];
		for (int i = 0; i < deAdjustInfo.size(); i++)
		{
			for (int j = 0; j < deAdjustTableCounts; j++)
				deAdjust[j].push_back(deAdjustInfo[i][j]);
		}
		for (int i = 0; i < deAdjustTableCounts; i++)
		{
			query.addBindValue(deAdjust[i]);

		}
		if (!query.execBatch()) //�������������������������
		{
			qDebug() << "aaaa" << query.lastError();
			flag = false;
		}

		query.prepare("INSERT INTO MFL_DevFactors values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,? )");
		QVariantList *devFactors = new QVariantList[deFactorsTableCounts];
		for (int i = 0; i < deFactorsInfo.size(); i++)
		{
			for (int j = 0; j < deFactorsTableCounts; j++)
				devFactors[j] << deFactorsInfo[i][j];
		}
		for (int i = 0; i < deFactorsTableCounts; i++)
		{
			query.addBindValue(devFactors[i]);
		}
		if (!query.execBatch()) //�������������������������
		{
			qDebug() << "bbbbbbb" << query.lastError();
			flag = false;
			
		}
	}
	else
	{
		//QString strUpdate = "UPDATE MFL_DevInfo SET DevID = :DevID ,DevName = :DevName ,SysName = :SysName,CheckCompany= :CheckCompany,CheckMan= :CheckMan ,ProductionDate= :ProductionDate ,SellDate= :SellDate ,HardwareVersion= :HardwareVersion,SoftwareVersion= :SoftwareVersion,ChannelNumber= :ChannelNumber,SpaceLength=:SpaceLength,StepLength=:StepLength,DevWidth=:DevWidth,DevLength=:DevLength,DevBackEndLength=:DevBackEndLength,DevFrontEndLength=:DevFrontEndLength,DevWeight=:DevWeight,ServicePhone=:ServicePhone,Remark=:Remark,AdjustGroupID=:AdjustGroupID WHERE DevID = '%1' ";
		QString strUpdate = "UPDATE MFL_DevInfo SET DevID = ?,DevName = ? ,SysName = ?,CheckCompany= ?,CheckMan= ? ,ProductionDate= ? ,SellDate= ? ,HardwareVersion= ?,SoftwareVersion= ?,ChannelNumber= ?,SpaceLength=?,StepLength=?,DevWidth=?,DevLength=?,DevBackEndLength=?,DevFrontEndLength=?,DevWeight=?,ServicePhone=?,Remark=?,AdjustGroupID=? WHERE DevID = '%1' ";
		//Delete_UpdateQuery(strUpdate.arg(devID), devinfo[0], year);

		Delete_UpdateQuery(strUpdate.arg(devID), devinfo[0], year);

		//ɾ�����²�����𵽸��µ�����
		QString str2 = "DELETE FROM MFL_DeAdjust WHERE DevID =   '%1' ;";
		QString str3 = "DELETE FROM MFL_DevFactors WHERE DevID = '%1' ;";
		qDebug() << query.exec(str2.arg(devID));
		qDebug() << query.exec(str3.arg(devID));
		
		//���� MFL_devAdjust��
		query.prepare("INSERT INTO MFL_DeAdjust values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		QVariantList *deAdjust = new QVariantList[deAdjustTableCounts];
		for (int i = 0; i < deAdjustInfo.size(); i++)
		{
			for (int j = 0; j < deAdjustTableCounts; j++)
				deAdjust[j].push_back(deAdjustInfo[i][j]);
		}
		for (int i = 0; i < deAdjustTableCounts; i++)
		{
			query.addBindValue(deAdjust[i]);
		}
		if (!query.execBatch()) //�������������������������
		{
			qDebug() << "aaaa" << query.lastError();
			flag = false;
		}

		query.prepare("INSERT INTO MFL_DevFactors values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,? )");
		QVariantList *devFactors = new QVariantList[deFactorsTableCounts];
		for (int i = 0; i < deFactorsInfo.size(); i++)
		{
			for (int j = 0; j < deFactorsTableCounts; j++)
				devFactors[j] << deFactorsInfo[i][j];
		}
		for (int i = 0; i < deFactorsTableCounts; i++)
		{
			query.addBindValue(devFactors[i]);
		}
		if (!query.execBatch()) //�������������������������
		{
			qDebug() << "bbbbbbb" << query.lastError();
			flag = false;

		}
		
	}



	//����  MFL_ProjectInfo ��
	query.prepare("INSERT INTO MFL_ProjectInfo values(?,?,?,?,?,?,?,?,?, ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ?, ?, ? , ? ,? )");
	QVariantList *project = new QVariantList[35];
	for (int i = 0; i < projectinfo.size(); i++)
	{
		for (int j = 0; j < 35; j++)
			project[j] << projectinfo[i][j];
	}
	for (int i = 0; i < 35; i++)
	{
		if (i == 32)
			project[i][0] = 0;		//�������Ŀ����Ϊδ���
		query.addBindValue(project[i]);
	}
	if (!query.execBatch()) //�������������������������
	{
		qDebug() << query.lastError();
		flag = false;
	}


	if (type == 2)
	{
		//����  MFL_PlateInfo ��
		query.prepare("INSERT INTO MFL_PlateInfo values(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , 0 , ? , 0 , 0 , null , null, null, null , null , null, null, null , ? ,? )");
		QVariantList *plate = new QVariantList[33];
		for (int i = 0; i < plateinfo.size(); i++)
		{
			for (int j = 0; j < 33; j++)
				plate[j] << plateinfo[i][j];
		}
		for (int i = 0; i < 33; i++)
		{
			//���ȱ�ݣ����ٷֱȣ��Ƿ���Ӧ�����ְ�ͼƬ  ȫ����Ϊ��ʼ
			if (i == 19)
				continue;
			if (i >= 21 && i <= 30)
			{
				continue;
			}
			else
				query.addBindValue(plate[i]);
		}
		if (!query.execBatch()) //�������������������������
		{
			qDebug() << query.lastError();
			flag = false;
		}

		//���� MFL_trackInfo ��
		query.prepare("INSERT INTO MFL_TrackInfo values(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ?, ?, ?)");
		QVariantList *track = new QVariantList[22];
		for (int i = 0; i < trackinfo.size(); i++)
		{
			if (trackinfo[i][3] == 0)//ֻ����0��
			{
				for (int j = 0; j < 22; j++)
				{
					track[j] << trackinfo[i][j];
				}
			}
		}
		for (int i = 0; i < 22; i++)
		{
			query.addBindValue(track[i]);
		}
		if (!query.execBatch()) //�������������������������
			qDebug() << query.lastError();

	}
	else if (type == 1)
	{


		//��ѯMFL_TrackCheckRecord��
		QString query_trackrecord = QObject::tr("select * from MFL_TrackCheckRecord where TrackID in (select TrackID from MFL_TrackInfo where PlateID in(select PlateID from MFL_PlateInfo where ProjectID = '%1'))").arg(projectID);
		QVector< QVector<QVariant> > trackrecord;
		trackrecord = SelectQuery3(query_trackrecord, 10, importDB);

		//��ѯ MFL_TrackSSRecord ��
		QString query_trackSSRecord = QObject::tr("select * from MFL_TrackSSRecord where TrackID in (select TrackID from MFL_TrackInfo where PlateID in(select PlateID from MFL_PlateInfo where ProjectID = '%1'))").arg(projectID);
		QVector< QVector<QVariant> > trackSSRecordInfo;
		trackSSRecordInfo = SelectQuery3(query_trackSSRecord, checkSSRecordTableCounts, importDB);

		qDebug() << "selected time:" << time.elapsed() / 1000.0 << "s";

		//����  MFL_PlateInfo ��
		query.prepare("INSERT INTO MFL_PlateInfo values(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ?, ?, ? , ? ,? )");
		QVariantList *plate = new QVariantList[33];
		for (int i = 0; i < plateinfo.size(); i++)
		{
			for (int j = 0; j < 33; j++)
				plate[j] << plateinfo[i][j];
		}
		for (int i = 0; i < 33; i++)
		{
			query.addBindValue(plate[i]);
		}
		if (!query.execBatch()) //�������������������������
		{
			qDebug() << query.lastError();
			flag = false;
		}
		qDebug() << "plateinfo time:" << time.elapsed() / 1000.0 << "s";

		QString sqlString = "INSERT INTO MFL_TrackInfo values(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ?, ?, ?)";
		QString appendStr = "(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? )";
		if (!InsertTable(sqlString, appendStr, trackinfo, trackInfoTableCounts, dbSqlOp))
			flag = false;
		qDebug() << "trackinfo time:" << time.elapsed() / 1000.0 << "s";
		
		sqlString = "INSERT INTO MFL_TrackCheckRecord values(?,?,?,?,?,?,?,?,?,?)";
		appendStr = "(?,?,?,?,?,?,?,?,?,?)";
		if (!InsertTable(sqlString, appendStr, trackrecord,checkRecordTableCounts,dbSqlOp))
		{
			flag = false;
		}
		qDebug() << "check time:" << time.elapsed() / 1000.0 << "s";
	
		sqlString = "INSERT INTO MFL_TrackSSRecord values(?,?,?,?,?,?,?,?,?)";
		appendStr = "(?,?,?,?,?,?,?,?,?)";
		if (!InsertTable(sqlString, appendStr, trackSSRecordInfo, checkSSRecordTableCounts, dbSqlOp))
			flag = false;
		qDebug() << "SS time:" << time.elapsed() / 1000.0 << "s";
	}


	if (!flag)//���flag==false���ع���
	{
		query.exec("ROLLBACK");
		return -2; //-2 sql���ִ�д���
	}
	else
	{
		bool result = query.exec("COMMIT");
		if (result)
		{
			return 0;
		}
		else
		{
			return -3; //-3 Ϊ�ύ����ʧ��
		}
	}


}

