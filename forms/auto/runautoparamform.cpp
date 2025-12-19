#include "runautoparamform.h"
#include "ui_runautoparamform.h"
#include <QLabel>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include "../modbus/NCMachine.h"
#include "utils/dataform.h"
#include "utils/pluginutils.h"
#include "BaseMainWindow.h"
#include "utils/DataTable.h"
#include "utils/formutils.h"
#include "modbus/NCMachineParametersC.h"
#include "utils/QTextFileModel.h"

RunAutoParamForm::RunAutoParamForm(QWidget* parent) :
	BaseChildWindow(parent),
	ui(new Ui::RunAutoParamForm)
{
	ui->setupUi(this);

	ui->inClzh->setModel(new QTextFileModel(this, GetDataFilePath(ui->inClzh)));
	ui->inDjxz->setModel(new QTextFileModel(this, GetDataFilePath(ui->inDjxz)));
	ui->inJgz->setModel(new QTextFileModel(this, GetDataFilePath(ui->inJgz)));
	ui->inJgmj->setModel(new QTextFileModel(this, GetDataFilePath(ui->inJgmj)));
	//ui->inJgmj->setModel(new QSqliteModel(this, "./data/cp.db", QStringLiteral("SELECT 加工面积 FROM 加工面积")));
	ui->inCcd->setModel(new QTextFileModel(this, GetDataFilePath(ui->inCcd)));
	ui->inYdxz->setModel(new QTextFileModel(this, GetDataFilePath(ui->inYdxz)));
	ui->inYdms->setModel(new QTextFileModel(this, GetDataFilePath(ui->inYdms)));
	ui->inJgcz->setModel(new QTextFileModel(this, GetDataFilePath(ui->inJgcz)));

	/*ui->inClzh->setModel(new QStringListModel(QStringLiteral("铜-钢,石墨-钢,铜-钨钢,铜-铜,铜-铝,石墨-铝,钢-铍铜,石墨-铍铜").split(',', Qt::SkipEmptyParts)));
	ui->inDjxz->setModel(new QStringListModel(QStringLiteral("通用,深腔,大面积,骨位,插件,多脚,螺牙,蜗杆,清角,进胶口").split(',', Qt::SkipEmptyParts)));
	ui->inJgz->setModel(new QStringListModel(QStringLiteral("Z,X,Y").split(',', Qt::SkipEmptyParts)));
	ui->inJgmj->setModel(new QStringListModel(QStringLiteral(R"(□1○1
□2○2
□3○3
□4○4
□4○5
□5○6
□6○7
□7○8
□8○9
□9○10
□10○11
□11○12
□12○13
□12○14
□13○15
□14○16
□15○17
□16○18
□17○19
□18○20
□19○21
□19○22
□20○23
□21○24
□22○25
□23○26
□24○27
□25○28
□26○29
□27○30
□27○31
□28○32
□29○33
□30○34
□31○35
□32○36
□33○37
□34○38
□35○39
□35○40
□36○41
□37○42
□38○43
□39○44
□40○45
□41○46
□42○47
□43○48
□43○49
□44○50
□45○51
□46○52
□47○53
□48○54
□49○55
□50○56
□51○57
□51○58
□52○59
□53○60
□54○61
□55○62
□56○63
□57○64
□58○65
□58○66
□59○67
□60○68
□61○69
□62○70
□63○71
□64○72
□65○73
□66○74
□66○75
□67○76
□68○77
□69○78
□70○79
□71○80
□72○81
□73○82
□74○83
□74○84
□75○85
□76○86
□77○87
□78○88
□79○89
□80○90
□81○91
□82○92
□82○93
□83○94
□84○95
□85○96
□86○97
□87○98
□88○99
□89○100
□89○101
□90○102
□91○103
□92○104
□93○105
□94○106
□95○107
□96○108
□97○109
□97○110
□98○111
□99○112
□100○113
□101○114
□102○115
□103○116
□104○117
□105○118
□105○119
□106○120
□107○121
□108○122
□109○123
□110○124
□111○125
□112○126
□113○127
□113○128
□114○129
□115○130
□116○131
□117○132
□118○133
□119○134
□120○135
□120○136
□121○137
□122○138
□123○139
□124○140
□125○141
□126○142
□127○143
□128○144
□128○145
□129○146
□130○147
□131○148
□132○149
□133○150
□134○151
□135○152
□136○153
□136○154
□137○155
□138○156
□139○157
□140○158
□141○159
□142○160
□143○161
□144○162
□144○163
□145○164
□146○165
□147○166
□148○167
□149○168
□150○169
□151○170
□152○171
□152○172
□153○173
□154○174
□155○175
□156○176
□157○177
□158○178
□159○179
□159○180
□160○181
□161○182
□162○183
□163○184
□164○185
□165○186
□166○187
□167○188
□167○189
□168○190
□169○191
□170○192
□171○193
□172○194
□173○195
□174○196
□175○197
□175○198
□176○199
□177○200
□178○201
□179○202
□180○203
□181○204
□182○205
□183○206
□183○207
□184○208
□185○209
□186○210
□187○211
□188○212
□189○213
□190○214
□190○215
□191○216
□192○217
□193○218
□194○219
□195○220
□196○221
□197○222
□198○223
□198○224
□199○225
□200○226
□201○227
□202○228
□203○229
□204○230
□205○231
□206○232
□206○233
□207○234
□208○235
□209○236
□210○237
□211○238
□212○239
□213○240
□214○241
□214○242
□215○243
□216○244
□217○245
□218○246
□219○247
□220○248
□221○249
□222○250
□222○251
□223○252
□224○253
□225○254
□226○255
□227○256
□228○257
□229○258
□229○259
□230○260
□231○261
□232○262
□233○263
□234○264
□235○265
□236○266
□237○267
□237○268
□238○269
□239○270
□240○271
□241○272
□242○273
□243○274
□244○275
□245○276
□245○277
□246○278
□247○279
□248○280
□249○281
□250○282
□251○283
□252○284
□253○285
□253○286
□254○287
□255○288
□256○289
□257○290
□258○291
□259○292
□260○293
□260○294
□261○295
□262○296
□263○297
□264○298
□265○299
□266○300)").split('\n', QString::SkipEmptyParts)));
	ui->inCcd->setModel(new QStringListModel(QStringLiteral("VDI0-Ra0.1,VDI3-Ra0.14,VDI10-Ra0.32,VDI14-Ra0.5,VDI16-Ra0.63,VDI17-Ra0.7,VDI18-Ra0.8,VDI20-Ra1,VDI22-Ra1.26,VDI24-Ra1.6,VDI26-Ra2,VDI28-Ra2.5,VDI30-Ra3.2,VDI32-Ra4,VDI34-Ra5,VDI36-Ra6.3,VDI40-Ra10").split(',', Qt::SkipEmptyParts)));
	ui->inYdxz->setModel(new QStringListModel(QStringLiteral("无,圆,方,菱形,交叉,十字,象限").split(',', Qt::SkipEmptyParts)));
	ui->inYdms->setModel(new QStringListModel(QStringLiteral("无,自由,锁定,标准").split(',', Qt::SkipEmptyParts)));
	ui->inJgcz->setModel(new QStringListModel(QStringLiteral("标准,低损耗,速度快").split(',', Qt::SkipEmptyParts)));*/

	ui->inJgmj->setText("□18○20");

	ui->inYdxzSpecial->setValidator(new QRegExpValidator(QRegExp("^[1-5]{4}$"), ui->inYdxzSpecial));
	ui->inYdxzSpecial->setVisible(false);
	connect(ui->inYdxz, &QComboBox::currentTextChanged, this, [this](const QString& text) {
		if (text == QStringLiteral("象限")) {
			ui->inYdxzSpecial->setVisible(true);
		}
		else {
			ui->inYdxzSpecial->setVisible(false);
		}
		});
}

RunAutoParamForm::~RunAutoParamForm()
{
    delete ui;
}


void RunAutoParamForm::showEvent(QShowEvent* event)
{
	BaseChildWindow::showEvent(event);

  //  if (m_tool4ZeroAndHalfAxis == NULL) {
  //      m_tool4ZeroAndHalfAxis = new Tool4ZeroAndHalfAxis(ncMachine, this);
		//Tool4ZeroAndHalfAxis::ShowIt(m_tool4ZeroAndHalfAxis);
  //  }
}

//void RunAutoParamForm::on_btnGenerate_clicked()
//{
//	RunGCode();
//}

QString RunAutoParamForm::GetGCode(){
	//return GenerateManualData_V1("runautoone");
	return GenerateManualData("runautoone");
}
// "runautoone"
QString RunAutoParamForm::GenerateManualData(QString parentName)
{
	QString cCode;
#ifdef PYTHON_SUPPORT
	QMap<QString, QString> pyFiles = PluginUtils::loadPythonScripts(this->objectName());
	for (const auto& pyFileName : pyFiles.keys()) {
		if (pyFileName == "rungcode") {
			this->SetData("result", NULL);

			//this->SetData("inClzh", ui->inClzh->text());
			this->SetData("inClzh", ui->inClzh->currentIndex() + 1);
			this->SetData("inDjxz", ui->inDjxz->currentIndex() + 1);
			this->SetData("inJgz", ui->inJgz->text());
			this->SetData("inJgsd", ui->inJgsd->text());
			this->SetData("inJgmj", ui->inJgmj->text());
			this->SetData("inCcd", ui->inCcd->text());
			QString inYdxz;
			if (ui->inYdxz->currentIndex() == 6) {
				inYdxz = ui->inYdxzSpecial->text();
			}
			else {
				inYdxz = QString::number(ui->inYdxz->currentIndex()).repeated(4);
			}
			this->SetData("inYdxz", inYdxz);
			this->SetData("inYdms", ui->inYdms->currentIndex());
			this->SetData("inJgcz", ui->inJgcz->currentIndex() + 1);
			this->SetData("inDbhhw", ui->inDbhhw->text());
			//this->SetData("inYdxzSpecial", ui->inYdxzSpecial->text());

			QString value = pyFiles.value(pyFileName);
			PluginUtils::RunFile(value, this);

			QVariant result = this->GetData("result");
			if (result != NULL && result.isValid()) {
				cCode = this->GetData("result").value<QString>();
			}
			break;
		}
	}
#endif
	cCode = cCode.trimmed();
	if (!cCode.isEmpty()) {
		QStringList ss = cCode.split('\n', Qt::SkipEmptyParts);

		QList<QString> cCodes;

		QList<QString> gSteps;
		QList<QString> gZs;
		QList<QString> gTzs;
		QString nowTz = "";

		QString ln, step, toz;
		for (int row = 0; row < ss.size(); ++row) {
			if (ss[row].startsWith("C")) {
				if (ss[row].startsWith("C") && ss[row].contains("=")) {
					std::string s = NCMachineParametersC::FormatCCode(EUtils::QString2StdString(ss[row]));

					cCodes.append(QString::fromStdString(s));
				}
				else {
					QStringList ss2 = ss[row].split(' ', Qt::SkipEmptyParts);
					gSteps.append(ss2[1].mid(8));
					QString zs = ss2[4].mid(5);
					if (zs.startsWith("+")) {
						zs = zs.mid(1);
					}
					gZs.append(zs);
					gTzs.append(nowTz);
					nowTz = "";
				}
			}
			else if (ss[row].startsWith("G85")) {
				nowTz = ss[row];
			}
			else if (ss[row].startsWith("H100")) {
				toz = ss[row].mid(7).trimmed();
			}
			else if (ss[row].startsWith("H101")) {
				step = ss[row].mid(7).trimmed();
				if (step.startsWith("+")) {
					step = step.mid(1);
				}
			}
			else if (ss[row].startsWith("H102")) {
				ln = ss[row].mid(7).trimmed();
			}
		}

		DataTable table2(cCodes.length() + 1, 10 + 1);
		for (int row = 0; row < cCodes.length(); ++row) {
			table2.setValue(row, 0, "Y");
			table2.setValue(row, 9, cCodes[row]);

			auto d = NCMachine::ParseCCodeLine(ss[row]);
			table2.setValue(row, 1, d["No"]);
			table2.setValue(row, 2, ln);
			table2.setValue(row, 3, d["LP"]);
			table2.setValue(row, 4, QString::number(step.toDouble() + gSteps[row].toDouble()));	//STEP
			table2.setValue(row, 5, gZs[row]);
			if (gTzs[row].isEmpty()) {
				table2.setValue(row, 6, "OFF");
				table2.setValue(row, 7, "00000");
			}
			else {
				QString tz = gTzs[row].mid(3);
				if (tz[0] == 'Z' || tz[0] == 'T') {
					table2.setValue(row, 6, QString(tz[0]));
					table2.setValue(row, 7, tz.mid(1).replace(";", "").leftJustified(5, '0'));
				}
			}
		}

		QString filePath2 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(parentName));
		table2.serialize(filePath2);

		DataTable table1(3 + 1, 1 + 1);
		table1.setValue(0, -1, "False");
		table1.setValue(1, -1, "False");
		table1.setValue(2, -1, "False");
		if (ui->inJgz->currentText() == "Z") {
			table1.setValue(2, 0, toz);
			table1.setValue(2, -1, "True");
		}
		else if (ui->inJgz->currentText() == "X") {
			table1.setValue(0, 0, toz);
			table1.setValue(0, -1, "True");
		}
		else if (ui->inJgz->currentText() == "Y") {
			table1.setValue(1, 0, toz);
			table1.setValue(1, -1, "True");
		}
		QString filePath1 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(parentName));
		table1.serialize(filePath1);

		//BaseMainWindow* mainWindow = GetMainWindow();
		//mainWindow->showChildWindow(QStringLiteral("手动单个"));

		return cCode;
	}
	else {
		QString filePath1 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(parentName));
		QFile::remove(filePath1);

		QString filePath2 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(parentName));
		QFile::remove(filePath2);

		return QString();
		FormUtils::MessageBoxInfo(QStringLiteral("没有相应数据！"));
	}
}

QString RunAutoParamForm::GenerateManualData_V1(QString parentName)
{
	QString cCode;
#ifdef PYTHON_SUPPORT
	QMap<QString, QString> pyFiles = PluginUtils::loadPythonScripts(this->objectName());
	for (const auto& pyFileName : pyFiles.keys()) {
		if (pyFileName == "rungcode") {
			this->SetData("result", NULL);

			this->SetData("inClzh", ui->inClzh->text());
			this->SetData("inDjxz", ui->inDjxz->text());
			this->SetData("inJgz", ui->inJgz->text());
			this->SetData("inJgsd", ui->inJgsd->text());
			this->SetData("inJgmj", ui->inJgmj->text());
			this->SetData("inCcd", ui->inCcd->text());
			this->SetData("inYdxz", ui->inYdxz->text());
			this->SetData("inYdms", ui->inYdms->text());
			this->SetData("inJgcz", ui->inJgcz->text());
			this->SetData("inDbhhw", ui->inDbhhw->text());
			
			QString value = pyFiles.value(pyFileName);
			PluginUtils::RunFile(value, this);

			QVariant result = this->GetData("result");
			if (result != NULL && result.isValid()) {
				cCode = this->GetData("result").value<QString>();
			}
			break;
		}
	}
#endif
	cCode = cCode.trimmed();
	if (!cCode.isEmpty()) {
		QStringList ss = cCode.split('\n', Qt::SkipEmptyParts);

		QList<QString> cCodes;
		QList<QString> gSteps;
		QList<QString> gZs;
		QString ln, step, toz;
		for (int row = 0; row < ss.size(); ++row) {
			if (ss[row].startsWith("C")) {
				if (!ss[row].startsWith("C000")) {
					std::string s = NCMachineParametersC::FormatCCode(EUtils::QString2StdString(ss[row]));

					cCodes.append(QString::fromStdString(s));
				}
				else {
					QStringList ss2 = ss[row].split(' ', Qt::SkipEmptyParts);
					gSteps.append(ss2[1].mid(8));
					QString zs = ss2[4].mid(5);
					if (zs.startsWith("+")) {
						zs = zs.mid(1);
					}
					gZs.append(zs);
				}
			}
			else if (ss[row].startsWith("H100")) {
				toz = ss[row].mid(7).trimmed();
			}
			else if (ss[row].startsWith("H101")) {
				step = ss[row].mid(7).trimmed();
				if (step.startsWith("+")) {
					step = step.mid(1);
				}
			}
			else if (ss[row].startsWith("H102")) {
				ln = ss[row].mid(7).trimmed();
			}
		}

		DataTable table2(cCodes.length() + 1, 10 + 1);
		for (int row = 0; row < cCodes.length(); ++row) {
			table2.setValue(row, 0, "Y");
			table2.setValue(row, 9, cCodes[row]);

			auto d = NCMachine::ParseCCodeLine(ss[row]);
			table2.setValue(row, 1, d["No"]);
			table2.setValue(row, 2, ln);
			table2.setValue(row, 3, d["LP"]);
			table2.setValue(row, 4, QString::number(step.toDouble() + gSteps[row].toDouble()));
			table2.setValue(row, 5, gZs[row]);
			table2.setValue(row, 6, "0");
			table2.setValue(row, 7, "0000");
		}

		QString filePath2 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(parentName));
		table2.serialize(filePath2);

		DataTable table1(3 + 1, 1 + 1);
		table1.setValue(0, -1, "False");
		table1.setValue(1, -1, "False");
		table1.setValue(2, -1, "False");
		if (ui->inJgz->currentText() == "Z") {
			table1.setValue(2, 0, toz);
			table1.setValue(2, -1, "True");
		}
		else if (ui->inJgz->currentText() == "X") {
			table1.setValue(0, 0, toz);
			table1.setValue(0, -1, "True");
		}
		else if (ui->inJgz->currentText() == "Y") {
			table1.setValue(1, 0, toz);
			table1.setValue(1, -1, "True");
		}
		QString filePath1 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(parentName));
		table1.serialize(filePath1);

		//BaseMainWindow* mainWindow = GetMainWindow();
		//mainWindow->showChildWindow(QStringLiteral("手动单个"));
	}
	else {
		QString filePath1 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(parentName));
		QFile::remove(filePath1);

		QString filePath2 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(parentName));
		QFile::remove(filePath2);

		
	}

	return cCode;
}


