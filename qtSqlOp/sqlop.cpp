#pragma execution_character_set("utf-8")
#include "sqlOp.h"
#include "qdebug.h"
//#include <sqlite3.h>

SqlOp::SqlOp()
{

}

SqlOp::~SqlOp()
{

}


/*******************************************************************************************
* �������ƣ�CreateConnection
* �������ܣ��������ݿ�
* �����������1��HostName�����ݿ������ַ�� ��2��DatabaseName�����ݿ����ƣ� ��3��UserName���û��� ����4��Password������
* �����������
* ����ֵ  �����ӳɹ�����true  ʧ��ʱ����false
* ע     ��ʹ��MYSQLʱ һ��Ҫ���úû��� ��autocommit�ص� ʹ��ӵ����Ӧ���ݿ�Ȩ�޵����û����������÷������ĵ�������mysql���÷�����
********************************************************************************************/
bool SqlOp::CreateConnection(const QString& HostName, const QString& DatabaseName, const QString& UserName, const QString& Password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); //���ݿ����� ʹ��SQLITEʱ���Ը�ΪQSQLITE
	db.setDatabaseName(DatabaseName);
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
* �������ƣ�CreateConnection
* �������ܣ��ر����ݿ�
* �����������
* �����������
* ����ֵ  ����
********************************************************************************************/
bool SqlOp::CloseConnection()
{
	QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, false);
	if (db.isOpen())
	{
		db.close();
		qDebug() << "close " + db.databaseName() + " success";
		return true;
	}
	return false;
}

/*******************************************************************************************
* �������ƣ�SelectQuery
* �������ܣ���ѯ��䣬���ص�����Ϊ�ַ�����ʽ
* �����������1��select��sql��� ��2��num����ѯ������
* ���������result
* ����ֵ  ����ѯ���
********************************************************************************************/
QVector< QVector <QString> > SqlOp::SelectQuery(QString select, int num)
{
	QVector< QVector <QString> >  result;   //��Ų�ѯ���

	QSqlQuery query;
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
* �����������1��select��sql��� ��2��num����ѯ������ ��3��bindValue����̬��������
* ���������result
* ����ֵ  ����ѯ���
********************************************************************************************/
QVector< QVector <QString> > SqlOp::SelectQuery(QString select, int num, QVector<QString> bindValue)
{
	QVector< QVector <QString> >  result;   //��Ų�ѯ���
	QSqlQuery query;
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
* �����������1��select��sql��� ��2��num����ѯ������ 
* ���������result
* ����ֵ  ����ѯ���
********************************************************************************************/
QVector< QVector <QVariant> > SqlOp::SelectQuery2(QString select, int num)
{
	QVector< QVector <QVariant> >  result;   //��Ų�ѯ���
	QSqlQuery query;
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
* �������ƣ�SelectQuery2
* �������ܣ�����̬��������ѯ��䣬���ص�����Ϊ�ɱ�������ʽ
* �����������1��select��sql��� ��2��num����ѯ������ ��3��bindValue����̬��������
* ���������result
* ����ֵ  ����ѯ���
********************************************************************************************/
QVector< QVector <QVariant> > SqlOp::SelectQuery2(QString select, int num, QVector<QString> bindValue)
{
	QVector< QVector <QVariant> >  result;   //��Ų�ѯ���
	QSqlQuery query;
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
* �����������1��del��sql��� 
* ���������bool
* ����ֵ  ���������
********************************************************************************************/
bool SqlOp::Delete_UpdateQuery(QString del)
{
	QSqlQuery query;
	query.exec(del);

	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
		return false;
	}
	qDebug() << "Delete_Update success";
	return true;
}

/*******************************************************************************************
* �������ƣ�Delete_UpdateQuery
* �������ܣ�����̬������ɾ�����߸������
* �����������1��del��sql��䣻��2��binfValue����̬��������
* ���������bool
* ����ֵ  ���������
********************************************************************************************/
bool SqlOp::Delete_UpdateQuery(QString del, QVector<QVariant> bindValue)
{
	QSqlQuery query;
	query.prepare(del);
	for (int i = 0; i < bindValue.size(); i++)
	{
		query.bindValue(i, bindValue[i]);
	}
	query.exec();

	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
		return false;
	}
	qDebug() << "Delete_Update success";
	return true;
}

/*******************************************************************************************
* �������ƣ�InsertQuery
* �������ܣ��������
* �����������1��insert��sql��䣻
* ���������bool
* ����ֵ  ���������
********************************************************************************************/
bool SqlOp::InsertQuery(QString insert)
{
	QSqlQuery query;
	query.exec(insert);
	//sqlite3_exec(db, "PRAGMA synchronous = OFF; ", 0, 0, 0);
	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
		return false;
	}
	qDebug() << "insert success";
	return true;
}

/*******************************************************************************************
* �������ƣ�InsertQuery
* �������ܣ�������䣬���в���
* �����������1��insert��sql��䣻 ��2��bindValue����̬����
* ���������bool
* ����ֵ  ���������
********************************************************************************************/
bool SqlOp::InsertQuery(QString insert, QVector<QVariant> bindvalue)
{
	QSqlQuery query;
	query.prepare(insert);
	for (int i = 0; i < bindvalue.size(); i++)
	{
		query.bindValue(i, bindvalue[i]);
	}
	query.exec();

	if (query.lastError().isValid())
	{
		qDebug() << query.lastError();
		return false;
	}
	qDebug() << "insert success";
	return true;
}


int SqlOp::ExportProjectDb(const QString &dbName, const QString &ID)
{
	QSqlDatabase exportdb = QSqlDatabase::addDatabase("QSQLITE", "export");
	exportdb.setDatabaseName(dbName);
	if (!exportdb.open()) {
		qDebug() << "Database Can't open!";
		return -1;
	}

	QSqlQuery query(exportdb);
	if (!query.exec("create table MFL_DevInfo ( DevID CHAR(8) not null, DevName VARCHAR(25), SysName VARCHAR(50), CheckCompany VARCHAR(60), CheckMan VARCHAR(30), ProductionDate  DATE, SellDate DATE, HardwareVersion VARCHAR(8), SoftwareVersion VARCHAR(8), ChannelNumber INTEGER, SpaceLength  FLOAT, StepLength FLOAT, DevWidth FLOAT, DevLength FLOAT, DevBackEndLength FLOAT, DevFrontEndLength FLOAT, DevWeight FLOAT, ServicePhone VARCHAR(20), Remark  VARCHAR(200), ThresholdValue1 FLOAT, ThresholdPer1 FLOAT, ThresholdPer1Value FLOAT, ThresholdPer2 FLOAT, ThresholdPer2Value FLOAT, ThresholdPer3 FLOAT, ThresholdPer3Value FLOAT, ThresholdPer4 FLOAT, ThresholdPer4Value FLOAT, Parameter1 FLOAT, Parameter2 FLOAT, Parameter3 FLOAT, Parameter4 FLOAT, Parameter5 FLOAT, Channel1Factor  float, Channel2Factor  float, Channel3Factor  float, Channel4Factor  float, Channel5Factor  float, Channel6Factor  float, Channel7Factor  float, Channel8Factor  float, Channel9Factor  float, Channel10Factor float, Channel11Factor float, Channel12Factor float, Channel13Factor float, Channel14Factor float, Channel15Factor float, Channel16Factor float, Channel17Factor float, Channel18Factor float, Channel19Factor float, Channel20Factor float, Channel21Factor float, Channel22Factor float, Channel23Factor float, Channel24Factor float, Channel25Factor float, Channel26Factor float, Channel27Factor float, Channel28Factor float, Channel29Factor float, Channel30Factor float, Channel31Factor float, Channel32Factor float, Channel33Factor float, Channel34Factor float, Channel35Factor float, Channel36Factor float, primary key (DevID) );"))
		qDebug() << "create  table MFL_DevInfo failed";
	if (!query.exec("create table MFL_DeAdjust ( AdjustID NUMERIC(6) not null, DevID CHAR(8), AdjustTime DATE, Remark VARCHAR(200), ThresholdValue1 FLOAT, ThresholdPer1  FLOAT, ThresholdPer1X FLOAT, ThresholdPer1Value FLOAT, ThresholdPer2  FLOAT, ThresholdPer2X FLOAT, ThresholdPer2Value FLOAT, ThresholdPer3  FLOAT, ThresholdPer3X FLOAT, ThresholdPer3Value FLOAT, ThresholdPer4  FLOAT, ThresholdPer4X FLOAT, ThresholdPer4Value FLOAT, Parameter1 FLOAT, Parameter2 FLOAT, Parameter3 FLOAT, Parameter4 FLOAT, Parameter5 FLOAT, SteelThickness INT, DisValue INT, primary key (AdjustID), foreign key (DevID) references MFL_DevInfo (DevID) );"))
		qDebug() << "create  table MFL_DeAdjust failed";
	if (!query.exec("create table MFL_ProjectInfo ( ProjectID CHAR(36) not null, DevID CHAR(8) not null, ProjectName VARCHAR(50), CompanyName VARCHAR(60), MFLAlert FLOAT, EnvShape VARCHAR(20), PointPosition INTEGER, CoordinatesAutoGen BOOL, EnvLength FLOAT, EnvWidth FLOAT, EnvArea FLOAT, PlateNumber INTEGER, EdgePlateNumber INTEGER, EdgePlateWidth FLOAT, DefectPlateNumber INTEGER default 0, CheckMan VARCHAR(20), CreateTime DATE, CheckTime DATE, UsedYear INTEGER, PlateThickness FLOAT, CoatThickness FLOAT, CoatStatus VARCHAR(50), PlateMaterial VARCHAR(50), StoreMaterial VARCHAR(50), DefectArea20 FLOAT default 0, DefectArea40 FLOAT default 0, DefectArea60 FLOAT default 0, DefectArea80 FLOAT default 0, MaxDefectPercent FLOAT default 0, ProjectPic LONG VARBINARY, IsCompleted BOOL, EdgeCircleDegree INTEGER, Remark VARCHAR(200), primary key (ProjectID), foreign key (DevID) references MFL_DevInfo (DevID) );"))
		qDebug() << "create  table MFL_ProjectInfo failed";
	if (!query.exec("create table MFL_PlateInfo ( PlateID CHAR(36) not null, ProjectID CHAR(36), RowNo int, ColumnNo int, PlateName VARCHAR(50), PointPosition INTEGER, PlateType INTEGER, PlateLength FLOAT, PlateWidth FLOAT, PlateLength2 FLOAT, PlateWidth2 FLOAT, XCoords FLOAT, YCoords FLOAT, CreateTime DATE, CheckTime DATE, DefectArea20 FLOAT default 0, DefectArea40 FLOAT default 0, DefectArea60 FLOAT default 0, DefectArea80 FLOAT default 0, MaxDefectPercent FLOAT default 0, PlateArea FLOAT, ScanPercent FLOAT default 0, IsDefect BOOL default '0', PlatePic LONG VARBINARY, PlatePic20 LONG VARBINARY, PlatePic30 LONG VARBINARY, PlatePic40 LONG VARBINARY, PlatePic50 LONG VARBINARY, PlatePic60 LONG VARBINARY, PlatePic70 LONG VARBINARY, PlatePic80 LONG VARBINARY, Remark VARCHAR(200), DetectDirection INT, primary key (PlateID), foreign key (ProjectID) references MFL_ProjectInfo (ProjectID) );"))
		qDebug() << "create  table MFL_PlateInfo failed";
	if (!query.exec("create table MFL_TrackInfo ( TrackID CHAR(36) not null, PlateID CHAR(36), TrackRowNo  INT, TrackColumnNo INT, XCoords FLOAT, YCoords FLOAT, CheckAngle  INTEGER default 0, CheckDirection INTEGER default 0, TrackLength FLOAT, TrackWidth  FLOAT, CreateTime  DATE, CheckTime DATE, DefectArea20  FLOAT default 0, DefectArea40  FLOAT default 0, DefectArea60  FLOAT default 0, DefectArea80  FLOAT default 0, MaxDefectPercent FLOAT default 0, TrackPic LONG VARBINARY, Remark  VARCHAR(200), ChannelNumber INTEGER, SpaceLength FLOAT, StepLength  FLOAT, primary key (TrackID), foreign key (PlateID) references MFL_PlateInfo (PlateID) );"))
		qDebug() << "create  table MFL_TrackInfo failed";
	if (!query.exec("create table MFL_TrackCheckRecord ( RecordID CHAR(36) not null, TrackID  CHAR(36), ChannelID CHAR(3), CheckTime DATE, XCoords  FLOAT, YCoords  FLOAT, CheckValue FLOAT, WaveletValue FLOAT, DefectPercent  FLOAT default 0, primary key (RecordID), foreign key (TrackID) references MFL_TrackInfo (TrackID) );"))
		qDebug() << "create  table MFL_TrackCheckRecord failed";



	//��ѯ  MFL_ProjectInfo ��
	const int projectTableCounts = 33;
	QVector<QString> bindValue;
	bindValue.append(ID);
	QString query_projectinfo = "select * from MFL_ProjectInfo where ProjectID = ? ";
	QVector< QVector<QVariant> > projectinfo;
	projectinfo = SqlOp::SelectQuery2(query_projectinfo, projectTableCounts, bindValue);

	//��ѯ MFL_PlateInfo  ��
	const int plateTableCounts = 33;   //�ְ�������
	QString query_plateinfo = "select * from MFL_PlateInfo where ProjectID = ? ";
	QVector< QVector<QVariant> > plateinfo;
	plateinfo = SqlOp::SelectQuery2(query_plateinfo, plateTableCounts, bindValue);

	//��ѯMFL_TrackInfo ��
	const int trackInfoTableCounts = 22;   //�켣��Ϣ�������
	QString query_trackinfo = "select * from MFL_TrackInfo where PlateID in(select PlateID from MFL_PlateInfo where ProjectID = ?)";
	QVector< QVector<QVariant> > trackinfo;
	trackinfo = SqlOp::SelectQuery2(query_trackinfo, trackInfoTableCounts, bindValue);

	//��ѯMFL_TrackCheckRecord��
	const int trackRecordTableCounts = 9;
	QString query_trackrecord = "select * from MFL_TrackCheckRecord where TrackID in (select TrackID from MFL_TrackInfo where PlateID in(select PlateID from MFL_PlateInfo where ProjectID = ?))";
	QVector< QVector<QVariant> > trackrecord;
	trackrecord = SqlOp::SelectQuery2(query_trackrecord, trackRecordTableCounts, bindValue);
	exportdb.transaction();

	//�������ݿ����  MFL_ProjectInfo ��

	query.prepare("INSERT INTO MFL_ProjectInfo values(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ?, ?, ?, ?,?  )");
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
		qDebug() << query.lastError();

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
		qDebug() << query.lastError();

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
		qDebug() << query.lastError();

	//�������ݿ����  MFL_TrackCheckRecord ��
	query.prepare("INSERT INTO MFL_TrackCheckRecord values(? , ? , ? , ? , ? , ? , ? , ? , ? )");
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
		qDebug() << query.lastError();

	bool result = exportdb.commit();
	if (result)
	{
		return 0;
	}
	else
	{
		//return -2;
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
* ����ֵ  ��
********************************************************************************************/
int SqlOp::ImportProjectDb(const QString &dbPath,int type)
{
	/*-----------------���ⲿ�����ļ�db�л�ȡ����-----------------------------*/
	QSqlDatabase importDB = QSqlDatabase::addDatabase("QSQLITE", "importDB");
	importDB.setDatabaseName(dbPath);
	if (!importDB.open()) {
		qDebug() << "Database Can't open!";
	}

	QString queryProject = "SELECT ProjectID FROM MFL_ProjectInfo";
	QVector< QVector <QVariant> > queryResult = SelectQuery3(queryProject, 1, importDB);
	QString projectID = queryResult[0][0].toString();

	//��ѯ  MFL_ProjectInfo ��
	QString query_projectinfo = QObject::tr("select * from MFL_ProjectInfo where ProjectID = '%1' ").arg(projectID);
	QVector< QVector<QVariant> > projectinfo;
	projectinfo = SelectQuery3(query_projectinfo, 33, importDB);


	//��ѯ MFL_PlateInfo  ��
	QString query_plateinfo = QObject::tr("select * from MFL_PlateInfo where ProjectID = '%1' ").arg(projectID);
	QVector< QVector<QVariant> > plateinfo;
	plateinfo = SelectQuery3(query_plateinfo, 33, importDB);

	//��ѯMFL_TrackInfo ��
	QString query_trackinfo = QObject::tr("select * from MFL_TrackInfo where PlateID in(select PlateID from MFL_PlateInfo where ProjectID = '%1')").arg(projectID);
	QVector< QVector<QVariant> > trackinfo;
	trackinfo = SelectQuery3(query_trackinfo, 22, importDB);

	//��ѯMFL_TrackCheckRecord��
	QString query_trackrecord = QObject::tr("select * from MFL_TrackCheckRecord where TrackID in (select TrackID from MFL_TrackInfo where PlateID in(select PlateID from MFL_PlateInfo where ProjectID = '%1'))").arg(projectID);
	QVector< QVector<QVariant> > trackrecord;
	trackrecord = SelectQuery3(query_trackrecord, 9, importDB);


	/*-----------------�򱾵����ݲ�������-----------------------------*/
	QSqlDatabase dbSqlOp = QSqlDatabase::database();
	QSqlQuery query(dbSqlOp);
	dbSqlOp.transaction();

	//�������ݿ����  MFL_ProjectInfo ��
	query.prepare("INSERT INTO MFL_ProjectInfo values(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ?, ?, ? , ? ,? )");
	QVariantList *project = new QVariantList[33];
	for (int i = 0; i < projectinfo.size(); i++)
	{
		for (int j = 0; j < 33; j++)
			project[j] << projectinfo[i][j];
	}
	for (int i = 0; i < 33; i++)
	{
		if (i == 30)
			project[i][0]=0;		//�������Ŀ����Ϊδ���
		query.addBindValue(project[i]);
	}
	if (!query.execBatch()) //�������������������������
		qDebug() << query.lastError();
	if (type == 2)
	{
		//�������ݿ����  MFL_PlateInfo ��
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
			qDebug() << query.lastError();

	}
	if (type == 1)
	{
		//�������ݿ����  MFL_PlateInfo ��
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
			qDebug() << query.lastError();

		//�������ݿ����  MFL_TrackInfo ��
		query.prepare("INSERT INTO MFL_TrackInfo values(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ?, ?, ?)");
		QVariantList *track = new QVariantList[22];
		for (int i = 0; i < trackinfo.size(); i++)
		{
			for (int j = 0; j < 22; j++)
				track[j] << trackinfo[i][j];
		}
		for (int i = 0; i < 22; i++)
		{
			query.addBindValue(track[i]);
		}
		if (!query.execBatch()) //�������������������������
			qDebug() << query.lastError();

		//�������ݿ����  MFL_TrackCheckRecord ��
		query.prepare("INSERT INTO MFL_TrackCheckRecord values(? , ? , ? , ? , ? , ? , ? , ? , ? )");
		QVariantList *trackcheck = new QVariantList[9];
		for (int i = 0; i < trackrecord.size(); i++)
		{
			for (int j = 0; j < 9; j++)
				trackcheck[j] << trackrecord[i][j];
		}
		for (int i = 0; i < 9; i++)
		{
			query.addBindValue(trackcheck[i]);
		}
		if (!query.execBatch()) //�������������������������
			qDebug() << query.lastError();
	}
	
	bool result = dbSqlOp.commit();
	if (result)
	{
		return 0;
	}
	else
	{
		return -2;
	}
	return 0;
}
