#ifndef NCMACHINEPROPERTIES_H
#define NCMACHINEPROPERTIES_H

#define SPEED_LOC_V 1800  // loc, Tolmt
#define SPEED_LOC_T 200
#define SPEED_JOG_V 2400
#define SPEED_JOG_T 100
#define SPEED_CMV_V 12 //CmvTouch, fromlmt, Tozsig
#define SPEED_CMV_T 200
#define SPEED_DIANDONG_V 100

#include "NCMachineDefines.h"

#include <cbang/util/Singleton.h>
#include <QObject>
#include <QHash>

using namespace cb;

class NCMachine;
struct ModbusTask;

class SystemSettings : public Singleton<SystemSettings>
{
public:
    SystemSettings(Inaccessible) {};

    QString GetProjectDir() { return m_projectDir; }
    void SetProjectDir(const QString& projectDir);

    static QString GetDataFilePath(const QString& objectName, QString projectDir = NULL);
    static QString AppendDataFilePath(const QString& filePath, const QString& append);

	void SetValue(const QString& key, QString value) { m_values[key] = value; }
    QString GetValue(const QString& key) { return m_values.contains(key) ? m_values[key] : ""; }
	void DeleteValue(const QString& key) { m_values.remove(key); }

	void LoadFromFile(const QString& filePath);
public:
    QString LastRunNCFileName;
private:
    QString m_projectDir;
	QHash<QString, QString> m_values;
};

class BasePropertyObject : public QObject
{
    Q_OBJECT;
public:
    enum  DdfxEnum { X_PLUS, X_MINUS, Y_PLUS, Y_MINUS, Z_PLUS, Z_MINUS, U_PLUS, U_MINUS};
    Q_ENUM(DdfxEnum)

    BasePropertyObject(QObject* parent = nullptr, const QString& name = "");

    std::map<QByteArray, QString> headers;
    std::map<QByteArray, QString> enumHeaders;

    virtual std::vector<uint16_t> GetValues() const = 0;
    virtual int GetCmdAddress() const = 0;
    virtual QList<ModbusTask*> ExecuteCmds(NCMachine* ncMachine, int idx = -1) const;
};

//using DdfxEnum = ::BasePropertyObject::DdfxEnum2; // also tried typedef, no change

class JogPropertyObject : public BasePropertyObject
{
    Q_OBJECT;

    Q_PROPERTY(bool JOGENABLE READ JOGENABLE WRITE setJOGENABLE)
    //Q_PROPERTY(bool tddzsnw READ tddzsnw WRITE settddzsnw)
    Q_PROPERTY(bool fdzlw READ fdzlw WRITE setfdzlw)

    Q_PROPERTY(MsEnum ms READ ms WRITE setms)
    //Q_PROPERTY(bool jcdl READ jcdl WRITE setjcdl)

    Q_PROPERTY(bool isgz READ isgz WRITE setisgz)

    Q_PROPERTY(int xV READ xV WRITE setxV)
    Q_PROPERTY(int xaccl READ xaccl WRITE setxaccl)
    Q_PROPERTY(int yV READ yV WRITE setyV)
    Q_PROPERTY(int yaccl READ yaccl WRITE setyaccl)
    Q_PROPERTY(int zV READ zV WRITE setzV)
    Q_PROPERTY(int zaccl READ zaccl WRITE setzaccl)
    //Q_PROPERTY(int uV READ uV WRITE setuV)
    //Q_PROPERTY(int uaccl READ uaccl WRITE setuaccl)

public:
    // Custom enum will be editable via a QComboBox so long as we tell Qt about it with Q_ENUMS().
    enum  MsEnum { SDMS, DDMS, SLMS };
    Q_ENUM(MsEnum)

    // Init.
    JogPropertyObject(QObject* parent = nullptr, const QString & name = "");

    //// Property getters.
    bool JOGENABLE() const { return m_JOGENABLE; }
    bool tddzsnw() const { return m_tddzsnw; }
    bool fdzlw() const { return m_fdzlw; }

    MsEnum ms() const { return m_ms; }
    bool jcdl() const { return m_jcdl; }

    bool isgz() const { return m_isgz; }

    int xV() const { return m_xV; }
    int xaccl() const { return m_xaccl; }
    int yV() const { return m_yV; }
    int yaccl() const { return m_yaccl; }
    int zV() const { return m_zV; }
    int zaccl() const { return m_zaccl; }
    int uV() const { return m_uV; }
    int uaccl() const { return m_uaccl; }

    // Property setters.
    void setJOGENABLE(bool v) { m_JOGENABLE = v; }
    void settddzsnw(bool v) { m_tddzsnw = v; }
    void setfdzlw(bool v) { m_fdzlw = v; }

    void setms(MsEnum v) { m_ms = v; }
    void setjcdl(bool v) { m_jcdl = v; }

    void setisgz(bool v) { m_isgz = v; }

    void setxV(int v) { m_xV = v; }
    void setxaccl(int v) { m_xaccl = v; }
    void setyV(int v) { m_yV = v; }
    void setyaccl(int v) { m_yaccl = v; }
    void setzV(int v) { m_zV = v; }
    void setzaccl(int v) { m_zaccl = v; }
    void setuV(int v) { m_uV = v; }
    void setuaccl(int v) { m_uaccl = v; }

    void setrun(bool v) { m_run = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;

    void ExecuteCmdsFdzlw(NCMachine* ncMachine, bool fdzlw, int idx = -1);
	void ExecuteCmdSetMs(NCMachine* ncMachine, int idx = -1);

protected:
    bool m_JOGENABLE = true;
    bool m_tddzsnw = false;
    bool m_fdzlw = false;

    MsEnum m_ms;
    bool m_jcdl = true;

    bool m_isgz;

    int m_xV = SPEED_JOG_V;
    int m_xaccl = SPEED_JOG_T;
    int m_yV = SPEED_JOG_V;
    int m_yaccl = SPEED_JOG_T;
    int m_zV = SPEED_JOG_V;
    int m_zaccl = SPEED_JOG_T;
    int m_uV = SPEED_JOG_V;
    int m_uaccl = SPEED_JOG_T;

    bool m_run = true;
};

class LocPropertyObject : public BasePropertyObject
{
Q_OBJECT;

    Q_PROPERTY(int x READ x WRITE setx)
    Q_PROPERTY(int y READ y WRITE sety)
    Q_PROPERTY(int z READ z WRITE setz)
    Q_PROPERTY(int u READ u WRITE setu)

    Q_PROPERTY(int V READ V WRITE setV)
    Q_PROPERTY(int accl READ accl WRITE setaccl)

    Q_PROPERTY(bool jcdl READ jcdl WRITE setjcdl)

public:
    LocPropertyObject(QObject* parent = nullptr, const QString& name = "");

    int x() const { return m_x; }
    int y() const { return m_y; }
    int z() const { return m_z; }
    int u() const { return m_u; }

    int V() const { return m_V; }
    int accl() const { return m_accl; }

    bool jcdl() const { return m_jcdl; }

    // Property setters.
    void setx(int v) { m_x = v; }
    void sety(int v) { m_y = v; }
    void setz(int v) { m_z = v; }
    void setu(int v) { m_u = v; }

    void setV(int v) { m_V = v; }
    void setaccl(int v) { m_accl = v; }

    void setjcdl(bool v) { m_jcdl = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    int m_x;
    int m_y;
    int m_z;
    int m_u;
    int m_V = SPEED_LOC_V;
    int m_accl = SPEED_LOC_T;
    bool m_jcdl = true;
};


class CmvTouchPropertyObject : public BasePropertyObject
{
    Q_OBJECT;

        Q_PROPERTY(DdfxEnum ddfx READ ddfx WRITE setddfx)

        Q_PROPERTY(int xV READ xV WRITE setxV)
        Q_PROPERTY(int xaccl READ xaccl WRITE setxaccl)
        Q_PROPERTY(int yV READ yV WRITE setyV)
        Q_PROPERTY(int yaccl READ yaccl WRITE setyaccl)

public:
    // Custom enum will be editable via a QComboBox so long as we tell Qt about it with Q_ENUMS().
   
    

    CmvTouchPropertyObject(QObject* parent = nullptr, const QString& name = "");

    int xV() const { return m_xV; }
    int xaccl() const { return m_xaccl; }
    int yV() const { return m_yV; }
    int yaccl() const { return m_yaccl; }
    DdfxEnum ddfx() const { return m_ddfx; }

    void setddfx(DdfxEnum v) { m_ddfx = v; }

    void setxV(int v) { m_xV = v; }
    void setxaccl(int v) { m_xaccl = v; }
    void setyV(int v) { m_yV = v; }
    void setyaccl(int v) { m_yaccl = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    DdfxEnum m_ddfx = DdfxEnum::X_PLUS;

    int m_xV = SPEED_CMV_V;
    int m_xaccl = SPEED_CMV_T;
    int m_yV = SPEED_CMV_V;
    int m_yaccl = SPEED_CMV_T;
};

class CmvTolmtPropertyObject : public BasePropertyObject
{
    Q_OBJECT;

    Q_PROPERTY(DdfxEnum ddfx READ ddfx WRITE setddfx)

        Q_PROPERTY(int xV READ xV WRITE setxV)
        Q_PROPERTY(int xaccl READ xaccl WRITE setxaccl)
        Q_PROPERTY(bool jcdl READ jcdl WRITE setjcdl)

public:

    // Init.
    CmvTolmtPropertyObject(QObject* parent = nullptr, const QString& name = "");

    int xV() const { return m_xV; }
    int xaccl() const { return m_xaccl; }
    int yV() const { return m_yV; }
    int yaccl() const { return m_yaccl; }
    DdfxEnum ddfx() const { return m_ddfx; }
    bool jcdl() const { return m_jcdl; }

    void setddfx(DdfxEnum v) { m_ddfx = v; }

    void setxV(int v) { m_xV = v; }
    void setxaccl(int v) { m_xaccl = v; }
    void setyV(int v) { m_yV = v; }
    void setyaccl(int v) { m_yaccl = v; }
    void setjcdl(bool v) { m_jcdl = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    DdfxEnum m_ddfx;

    int m_xV = SPEED_LOC_V;
    int m_xaccl = SPEED_LOC_T;
    int m_yV = SPEED_LOC_V;
    int m_yaccl = SPEED_LOC_T;

    bool m_jcdl = true;
};

class CmvFrmlmtPropertyObject : public BasePropertyObject
{
    Q_OBJECT;

    Q_PROPERTY(DdfxEnum ddfx READ ddfx WRITE setddfx)

        Q_PROPERTY(int xV READ xV WRITE setxV)
        Q_PROPERTY(int xaccl READ xaccl WRITE setxaccl)
        Q_PROPERTY(bool jcdl READ jcdl WRITE setjcdl)

public:
        // Init.
        CmvFrmlmtPropertyObject(QObject* parent = nullptr, const QString& name = "");

    int xV() const { return m_xV; }
    int xaccl() const { return m_xaccl; }
    int yV() const { return m_yV; }
    int yaccl() const { return m_yaccl; }
    DdfxEnum ddfx() const { return m_ddfx; }
    bool jcdl() const { return m_jcdl; }

    void setddfx(DdfxEnum v) { m_ddfx = v; }

    void setxV(int v) { m_xV = v; }
    void setxaccl(int v) { m_xaccl = v; }
    void setyV(int v) { m_yV = v; }
    void setyaccl(int v) { m_yaccl = v; }
    void setjcdl(bool v) { m_jcdl = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    DdfxEnum m_ddfx;

    int m_xV = SPEED_CMV_V;
    int m_xaccl = SPEED_CMV_T;
    int m_yV = SPEED_CMV_V;
    int m_yaccl = SPEED_CMV_T;

    bool m_jcdl = true;
};

class CmvTozsigPropertyObject : public BasePropertyObject
{
    Q_OBJECT;

    Q_PROPERTY(DdfxEnum ddfx READ ddfx WRITE setddfx)

        Q_PROPERTY(int xV READ xV WRITE setxV)
        Q_PROPERTY(int xaccl READ xaccl WRITE setxaccl)
        Q_PROPERTY(bool jcdl READ jcdl WRITE setjcdl)

public:
        // Init.
        CmvTozsigPropertyObject(QObject* parent = nullptr, const QString& name = "");

    int xV() const { return m_xV; }
    int xaccl() const { return m_xaccl; }
    int yV() const { return m_yV; }
    int yaccl() const { return m_yaccl; }
    DdfxEnum ddfx() const { return m_ddfx; }
    bool jcdl() const { return m_jcdl; }

    void setddfx(DdfxEnum v) { m_ddfx = v; }

    void setxV(int v) { m_xV = v; }
    void setxaccl(int v) { m_xaccl = v; }
    void setyV(int v) { m_yV = v; }
    void setyaccl(int v) { m_yaccl = v; }
    void setjcdl(bool v) { m_jcdl = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    DdfxEnum m_ddfx;

    int m_xV = SPEED_CMV_V;
    int m_xaccl = SPEED_CMV_T;
    int m_yV = SPEED_CMV_V;
    int m_yaccl = SPEED_CMV_T;

    bool m_jcdl = true;
};

class SPKPropertyObject : public BasePropertyObject
{
Q_OBJECT;
    Q_PROPERTY(int x READ x WRITE setx)
    Q_PROPERTY(int y READ y WRITE sety)
    Q_PROPERTY(int z READ z WRITE setz)
    Q_PROPERTY(int u READ u WRITE setu)
    Q_PROPERTY(int zgj READ zgj WRITE setzgj)
public:
    SPKPropertyObject(QObject* parent = nullptr, const QString& name = "");

    // X
    int x() const { return m_x; }
    // Y
    int y() const { return m_y; }
    // Z
    int z() const { return m_z; }
    int u() const { return m_u; }

    // 子轨迹
    int zgj() const { return m_zgj; }
    // X
    void setx(int v) { m_x = v; }
    // Y
    void sety(int v) { m_y = v; }
    // Z
    void setz(int v) { m_z = v; }
    void setu(int v) { m_u = v; }

    // 子轨迹
    void setzgj(int v) { m_zgj = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    int m_x = 0;
    int m_y = 0;
    int m_z = 0;
    int m_u = 0;
    int m_zgj = 1;
};

// 放电驱动IO
class FDQDPropertyObject : public BasePropertyObject
{
  Q_OBJECT;

    Q_PROPERTY(bool a1 READ a1 WRITE seta1)
    Q_PROPERTY(bool a2 READ a2 WRITE seta2)
    Q_PROPERTY(bool a3 READ a3 WRITE seta3)
    Q_PROPERTY(bool a4 READ a4 WRITE seta4)
    Q_PROPERTY(bool a5 READ a5 WRITE seta5)
    Q_PROPERTY(bool a6 READ a6 WRITE seta6)
    Q_PROPERTY(bool a7 READ a7 WRITE seta7)
    Q_PROPERTY(bool Slv1 READ Slv1 WRITE setSlv1)
    Q_PROPERTY(bool Slv2 READ Slv2 WRITE setSlv2)
    Q_PROPERTY(bool LLV READ LLV WRITE setLLV)
    Q_PROPERTY(bool HLV READ HLV WRITE setHLV)
    Q_PROPERTY(bool Slv3 READ Slv3 WRITE setSlv3)

      Q_PROPERTY(Enum1 kgygzs READ kgygzs WRITE setkgygzs)
      Q_PROPERTY(Enum2 fddrxz READ fddrxz WRITE setfddrxz)
      Q_PROPERTY(Enum3 dgsnw READ dgsnw WRITE setdgsnw)
      Q_PROPERTY(Enum4 dzsnw READ dzsnw WRITE setdzsnw)
      Q_PROPERTY(Enum5 fjxjgw READ fjxjgw WRITE setfjxjgw)
      Q_PROPERTY(Enum6 fzdyxz READ fzdyxz WRITE setfzdyxz)

public:
  // Custom enum will be editable via a QComboBox so long as we tell Qt about it with Q_ENUMS().
  enum  Enum1 { Enum1NONE, HV1, HV2, HV1HV2 };
  Q_ENUM(Enum1)
  enum  Enum2 { Enum2NONE, C1, C2, C3, C4, C5, C6, C7, C8 };
  Q_ENUM(Enum2)
    enum  Enum3 { DGJRHL, DLDG };
  Q_ENUM(Enum3)
    enum  Enum4 { DZJRHL, DLDZ };
  Q_ENUM(Enum4)
    enum  Enum5 { DJWZJ, DJWFJ };
  Q_ENUM(Enum5)
    enum  Enum6 { BWQGB, ZYFZ, GZFZ, WQGB };
  Q_ENUM(Enum6)

    // Init.
    FDQDPropertyObject(QObject* parent = nullptr, const QString& name = "");

  bool a1() const { return m_a1; }
  bool a2() const { return m_a2; }
  bool a3() const { return m_a3; }
  bool a4() const { return m_a4; }
  bool a5() const { return m_a5; }
  bool a6() const { return m_a6; }
  bool a7() const { return m_a7; }
  bool Slv1() const { return m_Slv1; }
  bool Slv2() const { return m_Slv2; }
  bool LLV() const { return m_LLV; }
  bool HLV() const { return m_HLV; }
  bool Slv3() const { return m_Slv3; }

  Enum1 kgygzs() const { return m_kgygzs; }
  Enum2 fddrxz() const { return m_fddrxz; }
  Enum3 dgsnw() const { return m_dgsnw; }
  Enum4 dzsnw() const { return m_dzsnw; }
  Enum5 fjxjgw() const { return m_fjxjgw; }
  Enum6 fzdyxz() const { return m_fzdyxz; }

  void seta1(bool v) { m_a1 = v; }
  void seta2(bool v) { m_a2 = v; }
  void seta3(bool v) { m_a3 = v; }
  void seta4(bool v) { m_a4 = v; }
  void seta5(bool v) { m_a5 = v; }
  void seta6(bool v) { m_a6 = v; }
  void seta7(bool v) { m_a7 = v; }
  void setSlv1(bool v) { m_Slv1 = v; }
  void setSlv2(bool v) { m_Slv2 = v; }
  void setLLV(bool v) { m_LLV = v; }
  void setHLV(bool v) { m_HLV = v; }
  void setSlv3(bool v) { m_Slv3 = v; }

  void setkgygzs(Enum1 v) { m_kgygzs = v; }
  void setfddrxz(Enum2 v) { m_fddrxz = v; }
  void setdgsnw(Enum3 v) { m_dgsnw = v; }
  void setdzsnw(Enum4 v) { m_dzsnw = v; }
  void setfjxjgw(Enum5 v) { m_fjxjgw = v; }
  void setfzdyxz(Enum6 v) { m_fzdyxz = v; }

  std::vector<uint16_t> GetValues() const override;
  int GetCmdAddress() const override;
protected:
  bool m_a1 = false;
  bool m_a2 = false;
  bool m_a3 = false;
  bool m_a4 = false;
  bool m_a5 = false;
  bool m_a6 = false;
  bool m_a7 = false;
  bool m_Slv1 = false;
  bool m_Slv2 = false;
  bool m_LLV = true;
  bool m_HLV = false;
  bool m_Slv3 = false;

  Enum1 m_kgygzs;
  Enum2 m_fddrxz;
  Enum3 m_dgsnw;
  Enum4 m_dzsnw;
  Enum5 m_fjxjgw;
  Enum6 m_fzdyxz;
};

// 辅助IO驱动
class FZQDPropertyObject : public BasePropertyObject
{
Q_OBJECT;

    Q_PROPERTY(bool yb1 READ yb1 WRITE setyb1)
    Q_PROPERTY(bool yb2 READ yb2 WRITE setyb2)
    Q_PROPERTY(bool out1 READ out1 WRITE setout1)
    Q_PROPERTY(bool out2 READ out2 WRITE setout2)
    Q_PROPERTY(bool out3 READ out3 WRITE setout3)
    Q_PROPERTY(bool out4 READ out4 WRITE setout4)
    //Q_PROPERTY(int bjd READ bjd WRITE setbjd)
public:

  // Init.
  FZQDPropertyObject(QObject* parent = nullptr, const QString& name = "");

  bool yb1() const { return m_yb1; }
  bool yb2() const { return m_yb2; }
  bool out1() const { return m_out1; }
  bool out2() const { return m_out2; }
  bool out3() const { return m_out3; }
  bool out4() const { return m_out4; }
  //int bjd() const { return m_bjd; }

  void setyb1(bool v) { m_yb1 = v; }
  void setyb2(bool v) { m_yb2 = v; }
  void setout1(bool v) { m_out1 = v; }
  void setout2(bool v) { m_out2 = v; }
  void setout3(bool v) { m_out3 = v; }
  void setout4(bool v) { m_out4 = v; }
  //void setbjd(int v) { m_bjd = v; }

  std::vector<uint16_t> GetValues() const override;
  int GetCmdAddress() const override;
protected:
  bool m_yb1 = false;
  bool m_yb2 = false;
  bool m_out1 = false;
  bool m_out2 = false;
  bool m_out3 = false;
  bool m_out4 = false;
  //int m_bjd = 0;
};


// 手控盒控制数据
class SKHKZPropertyObject : public BasePropertyObject
{
  Q_OBJECT;

  Q_PROPERTY(Enum1 ztkz READ ztkz WRITE setztkz)
    Q_PROPERTY(bool xs READ xs WRITE setxs)
    Q_PROPERTY(bool jpsd READ jpsd WRITE setjpsd)
    Q_PROPERTY(bool sczsn READ sczsn WRITE setsczsn)
    Q_PROPERTY(bool sc32wjm READ sc32wjm WRITE setsc32wjm)
    Q_PROPERTY(bool isgz READ isgz WRITE setisgz)
    Q_PROPERTY(int zgs READ zgs WRITE setzgs)
    Q_PROPERTY(Enum2 sdxskz READ sdxskz WRITE setsdxskz)
    Q_PROPERTY(Enum3 ddsdkz READ ddsdkz WRITE setddsdkz)
    
public:
  enum  Enum1 { Enum1NONE, ZTJGZT, JGZT };
  Q_ENUM(Enum1)
    enum  Enum2 { X1, X10, X50, X100 };
  Q_ENUM(Enum2)
      enum  Enum3 { SD, DD };
  Q_ENUM(Enum3)

  // Init.
    SKHKZPropertyObject(QObject* parent = nullptr, const QString& name = "");

  Enum1 ztkz() const { return m_ztkz; }
  bool xs() const { return m_xs; }
  bool jpsd() const { return m_jpsd; }
  bool sczsn() const { return m_sczsn; }
  bool sc32wjm() const { return m_sc32wjm; }
  bool isgz() const { return m_isgz; }
  int zgs() const { return m_zgs; }
  Enum2 sdxskz() const { return m_sdxskz; }
  Enum3 ddsdkz() const { return m_ddsdkz; }
  

  void setztkz(Enum1 v) { m_ztkz = v; }
  void setxs(bool v) { m_xs = v; }
  void setjpsd(bool v) { m_jpsd = v; }
  void setsczsn(bool v) { m_sczsn = v; }
  void setsc32wjm(bool v) { m_sc32wjm = v; }
  void setisgz(bool v) { m_isgz = v; }
  void setzgs(int v) { m_zgs = v; }
  void setsdxskz(Enum2 v) { m_sdxskz = v; }
  void setddsdkz(Enum3 v) { m_ddsdkz = v; }

  std::vector<uint16_t> GetValues() const override;
  int GetCmdAddress() const override;

protected:
  Enum1 m_ztkz = Enum1::Enum1NONE;
  bool m_xs = true;
  bool m_jpsd = false;
  bool m_sczsn = true;
  bool m_sc32wjm = POS_PAD_AXIS_LEN == 4 ? true : false;
  bool m_isgz = true;
  int m_zgs = POS_PAD_AXIS_LEN;
  Enum2 m_sdxskz = Enum2::X100;
  Enum3 m_ddsdkz = Enum3::SD;
};

// 轴螺距补偿配置
class ZLJBCPropertyObject : public BasePropertyObject
{
  Q_OBJECT;

  Q_PROPERTY(int bcjg READ bcjg WRITE setbcjg)
    Q_PROPERTY(int bcckd READ bcckd WRITE setbcckd)
    Q_PROPERTY(int zfxbczdjl READ zfxbczdjl WRITE setzfxbczdjl)
    Q_PROPERTY(int ffxbczdjl READ ffxbczdjl WRITE setffxbczdjl)
    Q_PROPERTY(int fxjxbc READ fxjxbc WRITE setfxjxbc)
    Q_PROPERTY(int dwsjzdbc READ dwsjzdbc WRITE setdwsjzdbc)
    Q_PROPERTY(bool bcsx READ bcsx WRITE setbcsx)
public:
    // Init.
  ZLJBCPropertyObject(QObject* parent = nullptr, const QString& name = "");

  int bcjg() const { return m_bcjg; }
  int bcckd() const { return m_bcckd; }
  int zfxbczdjl() const { return m_zfxbczdjl; }
  int ffxbczdjl() const { return m_ffxbczdjl; }
  int fxjxbc() const { return m_fxjxbc; }
  int dwsjzdbc() const { return m_dwsjzdbc; }
  bool bcsx() const { return m_bcsx; }

  void setbcjg(int v) { m_bcjg = v; }
  void setbcckd(int v) { m_bcckd = v; }
  void setzfxbczdjl(int v) { m_zfxbczdjl = v; }
  void setffxbczdjl(int v) { m_ffxbczdjl = v; }
  void setfxjxbc(int v) { m_fxjxbc = v; }
  void setdwsjzdbc(int v) { m_dwsjzdbc = v; }
  void setbcsx(bool v) { m_bcsx = v; }

  std::vector<uint16_t> GetValues() const override;
  int GetCmdAddress() const override;

  static void ExecuteCmds1(NCMachine* ncMachine, ZLJBCPropertyObject* propertyObjects[], int idx = -1);
  static void ExecuteCmds2(NCMachine* ncMachine, ZLJBCPropertyObject* propertyObjects[], int idx = -1);

protected:
  int m_bcjg = 20000;
  int m_bcckd = 50;
  int m_zfxbczdjl = 99;
  int m_ffxbczdjl = 0;
  int m_fxjxbc = 2;
  int m_dwsjzdbc = 240;
  bool m_bcsx = true;
};


// 点动移动
class DDYDPropertyObject : public BasePropertyObject
{
    Q_OBJECT;

    Q_PROPERTY(AxisEnum bjjgz READ bjjgz WRITE setbjjgz)
    Q_PROPERTY(int bjmcs READ bjmcs WRITE setbjmcs)

public:
    enum  AxisEnum { X, Y, Z };
    Q_ENUM(AxisEnum)
    // Init.
    DDYDPropertyObject(QObject* parent = nullptr, const QString& name = "");

    AxisEnum bjjgz() const { return m_bjjgz; }
    int bjmcs() const { return m_bjmcs; }

    void setbjjgz(AxisEnum v) { m_bjjgz = v; }
    void setbjmcs(int v) { m_bjmcs = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    AxisEnum m_bjjgz = AxisEnum::X;
    int m_bjmcs = 100;
};

// 电机使能
class DJSNPropertyObject : public BasePropertyObject
{
    Q_OBJECT;
        Q_PROPERTY(bool djsn READ djsn WRITE setdjsn)

public:

        // Init.
    DJSNPropertyObject(QObject* parent = nullptr, const QString& name = "");

    bool djsn() const { return m_djsn; }

    void setdjsn(bool v) { m_djsn = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    bool m_djsn = true;
};

// 蜂鸣器
class FmqPropertyObject : public BasePropertyObject
{
    Q_OBJECT;

    Q_PROPERTY(MjfsEnum mjfs READ mjfs WRITE setmjfs)
        Q_PROPERTY(int data2 READ data2 WRITE setdata2)
        Q_PROPERTY(int data3 READ data3 WRITE setdata3)
        Q_PROPERTY(int data4 READ data4 WRITE setdata4)

public:
    enum  MjfsEnum { TYPE_OFF, TYPE1, TYPE2, TYPE3, TYPE_ON };
    Q_ENUM(MjfsEnum)

        // Init.
        FmqPropertyObject(QObject* parent = nullptr, const QString& name = "");

    MjfsEnum mjfs() const { return m_mjfs; }
    int data2() const { return m_data2; }
    int data3() const { return m_data3; }
    int data4() const { return m_data4; }

    void setmjfs(MjfsEnum v) { m_mjfs = v; }
    void setdata2(int v) { m_data2 = v; }
    void setdata3(int v) { m_data3 = v; }
    void setdata4(int v) { m_data4 = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    MjfsEnum m_mjfs = MjfsEnum::TYPE_OFF;
    int m_data2 = 50;
    int m_data3 = 100;
    int m_data4 = 10;
};

// 调试用途
class DebugPropertyObject : public BasePropertyObject
{
    Q_OBJECT;
        Q_PROPERTY(uint dl READ dl WRITE setdl)

public:
    DebugPropertyObject(QObject* parent = nullptr, const QString& name = "");

    // 短路
    uint dl() const { return m_dl; }

    void setdl(uint v) { m_dl = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    uint m_dl = 1;
};

// ADC
class ADCPropertyObject : public BasePropertyObject
{
    Q_OBJECT;
    Q_PROPERTY(double adczyxs READ adczyxs WRITE setadczyxs)
        Q_PROPERTY(int adcjzsj READ adcjzsj WRITE setadcjzsj)
        Q_PROPERTY(int sfdysc READ sfdysc WRITE setsfdysc)
        Q_PROPERTY(int gobl READ gobl WRITE setgobl)
public:
    ADCPropertyObject(QObject* parent = nullptr, const QString& name = "");

    // Adc增益系数
    double adczyxs() const { return m_adczyxs; }
    // Adc校正数据
    int adcjzsj() const { return m_adcjzsj; }

    // 伺服电压输出比例系数K331
    double sfdysc() const { return m_sfdysc; }
    // 光耦比例系数K201
    double gobl() const { return m_gobl; }

    void setadczyxs(double v) { m_adczyxs = v; }
    void setadcjzsj(int v) { m_adcjzsj = v; }
    void setsfdysc(double v) { m_sfdysc = v; }
    void setgobl(double v) { m_gobl = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    double m_adczyxs = 1.0;
    int m_adcjzsj = 2023;
    double m_sfdysc = 1;
    double m_gobl = 0.5;
};

// 放电回路
class FDHLPropertyObject : public BasePropertyObject
{
    Q_OBJECT;

    Q_PROPERTY(bool a1 READ a1 WRITE seta1)
        Q_PROPERTY(bool a2 READ a2 WRITE seta2)
        Q_PROPERTY(bool a3 READ a3 WRITE seta3)
        Q_PROPERTY(bool a4 READ a4 WRITE seta4)
        Q_PROPERTY(bool a5 READ a5 WRITE seta5)
        Q_PROPERTY(bool a6 READ a6 WRITE seta6)
        Q_PROPERTY(bool a7 READ a7 WRITE seta7)
        Q_PROPERTY(bool Slv1 READ Slv1 WRITE setSlv1)
        Q_PROPERTY(bool Slv2 READ Slv2 WRITE setSlv2)
        Q_PROPERTY(int LV READ LV WRITE setLV)
        //Q_PROPERTY(bool HLV READ HLV WRITE setHLV)
        Q_PROPERTY(bool Slv3 READ Slv3 WRITE setSlv3)

        Q_PROPERTY(int kgygzs READ kgygzs WRITE setkgygzs)
        Q_PROPERTY(int fddrxz READ fddrxz WRITE setfddrxz)
        Q_PROPERTY(int dgsnw READ dgsnw WRITE setdgsnw)
        Q_PROPERTY(int dzsnw READ dzsnw WRITE setdzsnw)
        Q_PROPERTY(int fjxjgw READ fjxjgw WRITE setfjxjgw)
        Q_PROPERTY(int fzdyxz READ fzdyxz WRITE setfzdyxz)

public:
    //// Custom enum will be editable via a QComboBox so long as we tell Qt about it with Q_ENUMS().
    //enum  Enum1 { Enum1NONE, HV1, HV2, HV1HV2 };
    //Q_ENUMS(Enum1)
    //    enum  Enum2 { Enum2NONE, C1, C2, C3, C4, C5, C6, C7, C8 };
    //Q_ENUMS(Enum2)
    //    enum  Enum3 { DGJRHL, DLDG };
    //Q_ENUMS(Enum3)
    //    enum  Enum4 { DZJRHL, DLDZ };
    //Q_ENUMS(Enum4)
    //    enum  Enum5 { DJWZJ, DJWFJ };
    //Q_ENUMS(Enum5)
    //    enum  Enum6 { BWQGB, ZYFZ, GZFZ, WQGB };
    //Q_ENUMS(Enum6)

        // Init.
        FDHLPropertyObject(QObject* parent = nullptr, const QString& name = "");

    bool a1() const { return m_a1; }
    bool a2() const { return m_a2; }
    bool a3() const { return m_a3; }
    bool a4() const { return m_a4; }
    bool a5() const { return m_a5; }
    bool a6() const { return m_a6; }
    bool a7() const { return m_a7; }
    bool Slv1() const { return m_Slv1; }
    bool Slv2() const { return m_Slv2; }
    int LV() const { return m_LV; }
    //bool HLV() const { return m_HLV; }
    bool Slv3() const { return m_Slv3; }

    int kgygzs() const { return m_kgygzs; }
    int fddrxz() const { return m_fddrxz; }
    int dgsnw() const { return m_dgsnw; }
    int dzsnw() const { return m_dzsnw; }
    int fjxjgw() const { return m_fjxjgw; }
    int fzdyxz() const { return m_fzdyxz; }

    void seta1(bool v) { m_a1 = v; }
    void seta2(bool v) { m_a2 = v; }
    void seta3(bool v) { m_a3 = v; }
    void seta4(bool v) { m_a4 = v; }
    void seta5(bool v) { m_a5 = v; }
    void seta6(bool v) { m_a6 = v; }
    void seta7(bool v) { m_a7 = v; }
    void setSlv1(bool v) { m_Slv1 = v; }
    void setSlv2(bool v) { m_Slv2 = v; }
    void setLV(int v) { m_LV = v; }
    void setSlv3(bool v) { m_Slv3 = v; }

    void setkgygzs(int v) { m_kgygzs = v; }
    void setfddrxz(int v) { m_fddrxz = v; }
    void setdgsnw(int v) { m_dgsnw = v; }
    void setdzsnw(int v) { m_dzsnw = v; }
    void setfjxjgw(int v) { m_fjxjgw = v; }
    void setfzdyxz(int v) { m_fzdyxz = (v >= 0 && v <= 5) ? v : 0; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    bool m_a1 = false;
    bool m_a2 = false;
    bool m_a3 = false;
    bool m_a4 = false;
    bool m_a5 = false;
    bool m_a6 = false;
    bool m_a7 = false;
    bool m_Slv1 = false;
    bool m_Slv2 = false;
    //bool m_LLV = true;
    //bool m_HLV = false;
    int m_LV = 0;
    bool m_Slv3 = false;

    int m_kgygzs = 0;
    int m_fddrxz = 0;
    int m_dgsnw = 0;
    int m_dzsnw = 0;
    int m_fjxjgw = 0;
    int m_fzdyxz = 0;
};


// 放电加工
class FDJGPropertyObject : public BasePropertyObject
{
    Q_OBJECT;
    Q_PROPERTY(int fdjgcfdy READ fdjgcfdy WRITE setfdjgcfdy)
        Q_PROPERTY(int jgmcmk READ jgmcmk WRITE setjgmcmk)
        Q_PROPERTY(int jgmcmj READ jgmcmj WRITE setjgmcmj)
        Q_PROPERTY(int jgmcdnlsn READ jgmcdnlsn WRITE setjgmcdnlsn)
        Q_PROPERTY(int jgmcdyycjcsn READ jgmcdyycjcsn WRITE setjgmcdyycjcsn)
        Q_PROPERTY(int jgmcfzmcxz READ jgmcfzmcxz WRITE setjgmcfzmcxz)
        Q_PROPERTY(int jcycjcms READ jcycjcms WRITE setjcycjcms)
        //Q_PROPERTY(int jcycsjyz READ jcycsjyz WRITE setjcycsjyz)
        Q_PROPERTY(int fdcfsd READ fdcfsd WRITE setfdcfsd)
        Q_PROPERTY(int fdcftzsd READ fdcftzsd WRITE setfdcftzsd)
public:
    FDJGPropertyObject(QObject* parent = nullptr, const QString& name = "");

    // 放电加工伺服电压
    int fdjgcfdy() const { return m_fdjgcfdy; }
    // 加工脉冲脉宽
    int jgmcmk() const { return m_jgmcmk; }
    // 加工脉冲脉间
    int jgmcmj() const { return m_jgmcmj; }
    // 参数OC的值 --加工脉冲等能量使能--
    int jgmcdnlsn() const { return m_jgmcdnlsn; }
    // 参数AL的值(bit5-0) --加工脉冲电压异常检测使能--
    int jgmcdyycjcsn() const { return m_jgmcdyycjcsn; }
    // 加工脉冲辅助脉冲选择
    int jgmcfzmcxz() const { return m_jgmcfzmcxz; }
    // --击穿异常时间阈值--
    // 击穿异常检测模式
    int jcycjcms() const { return m_jcycjcms; }
    //int jcycsjyz() const { return m_jcycsjyz; }
    // 放电伺服速度
    int fdcfsd() const { return m_fdcfsd; }
    // 放电伺服调整速度
    int fdcftzsd() const { return m_fdcftzsd; }
    void setfdjgcfdy(int v) { m_fdjgcfdy = v; }
    void setjgmcmk(int v) { m_jgmcmk = v; }
    void setjgmcmj(int v) { m_jgmcmj = v; }
    void setjgmcdnlsn(int v) { m_jgmcdnlsn = v; }
    void setjgmcdyycjcsn(int v) { m_jgmcdyycjcsn = v; }
    void setjgmcfzmcxz(int v) { m_jgmcfzmcxz = (v >= 0 && v <= 2) ? v : 1; }
    //void setjcycsjyz(int v) { m_jcycsjyz = v; }
    void setjcycjcms(int v) { m_jcycjcms = v; }
    void setfdcfsd(int v) { m_fdcfsd = v; }
    void setfdcftzsd(int v) { m_fdcftzsd = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
    void ExecuteCmdsSetSv(NCMachine* ncMachine, int sv, int idx = -1);
protected:
    int m_fdjgcfdy = 3;
    int m_jgmcmk = 0;
    int m_jgmcmj = 0;
    int m_jgmcdnlsn = 0;
    int m_jgmcdyycjcsn = 0;
    int m_jgmcfzmcxz = 0;
    int m_jcycjcms = 0;
    //int m_jcycsjyz = 0;
    int m_fdcfsd = 0;
    int m_fdcftzsd = 0;
};

class FDWCPropertyObject : public BasePropertyObject
{
    Q_OBJECT;
    Q_PROPERTY(int fdwcms READ fdwcms WRITE setfdwcms)
        Q_PROPERTY(int fddssj READ fddssj WRITE setfddssj)
        Q_PROPERTY(int tdfx READ tdfx WRITE settdfx)
        Q_PROPERTY(int tdgd READ tdgd WRITE settdgd)
        Q_PROPERTY(int tdsd READ tdsd WRITE settdsd)
        Q_PROPERTY(int fdsj READ fdsj WRITE setfdsj)
        Q_PROPERTY(int ectdxydtdcs READ ectdxydtdcs WRITE setectdxydtdcs)
        Q_PROPERTY(int ectdgdbs READ ectdgdbs WRITE setectdgdbs)
public:
    FDWCPropertyObject(QObject* parent = nullptr, const QString& name = "");

    // 放电完成模式
    int fdwcms() const { return m_fdwcms; }
    
    // 放电定时时间
    int fddssj() const { return m_fddssj; }
    // 抬刀方向
    int tdfx() const { return m_tdfx; }
    // 抬刀高度
    int tdgd() const { return m_tdgd; }
    // 抬刀速度
    int tdsd() const { return m_tdsd; }
    // 放电时间
    int fdsj() const { return m_fdsj; }
    // 二次抬刀需要的抬刀次数
    int ectdxydtdcs() const { return m_ectdxydtdcs; }
    // 二次抬刀高度倍数
    int ectdgdbs() const { return m_ectdgdbs; }
    void setfdwcms(int v) { m_fdwcms = v; }
    void setfddssj(int v) { m_fddssj = v; }
    void settdfx(int v) { m_tdfx = v; }
    void settdgd(int v) { m_tdgd = v; }
    void settdsd(int v) { m_tdsd = v; }
    void setfdsj(int v) { m_fdsj = v; }
    void setectdxydtdcs(int v) { m_ectdxydtdcs = v; }
    void setectdgdbs(int v) { m_ectdgdbs = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    int m_fdwcms = 0;
    
    int m_fddssj = 0;
    int m_tdfx = 0;
    int m_tdgd = 0;
    int m_tdsd = 0;
    int m_fdsj = 0;
    int m_ectdxydtdcs = 0;
    int m_ectdgdbs = 0;
};

class YDPropertyObject : public BasePropertyObject
{
    Q_OBJECT;
    Q_PROPERTY(int ydfx READ ydfx WRITE setydfx)
        Q_PROPERTY(int zdydsd READ zdydsd WRITE setzdydsd)
        Q_PROPERTY(int sdjcxfs READ sdjcxfs WRITE setsdjcxfs)
        Q_PROPERTY(int ydfwcfms READ ydfwcfms WRITE setydfwcfms)
        Q_PROPERTY(int ydfwdzms READ ydfwdzms WRITE setydfwdzms)
        Q_PROPERTY(int ydpm READ ydpm WRITE setydpm)
        Q_PROPERTY(int d1xxydm READ d1xxydm WRITE setd1xxydm)
        Q_PROPERTY(int d2xxydm READ d2xxydm WRITE setd2xxydm)
        Q_PROPERTY(int d3xxydm READ d3xxydm WRITE setd3xxydm)
        Q_PROPERTY(int d4xxydm READ d4xxydm WRITE setd4xxydm)
        Q_PROPERTY(int ydbj READ ydbj WRITE setydbj)
        Q_PROPERTY(int hzxtdgd READ hzxtdgd WRITE sethzxtdgd)
        Q_PROPERTY(int hzxtdsd READ hzxtdsd WRITE sethzxtdsd)
        Q_PROPERTY(int hzxtdjssj READ hzxtdjssj WRITE sethzxtdjssj)
        Q_PROPERTY(int ztdjssj READ ztdjssj WRITE setztdjssj)
        Q_PROPERTY(int tcqhtjl READ tcqhtjl WRITE settcqhtjl)
        Q_PROPERTY(int tdqmshtjl READ tdqmshtjl WRITE settdqmshtjl)
        Q_PROPERTY(int tdqmshtsd READ tdqmshtsd WRITE settdqmshtsd)
public:
    YDPropertyObject(QObject* parent = nullptr, const QString& name = "");

    // 摇动方向
    int ydfx() const { return m_ydfx; }
    // 最大摇动速度
    int zdydsd() const { return m_zdydsd; }
    // 深度检测细分数
    int sdjcxfs() const { return m_sdjcxfs; }
    // 摇动复位伺服模式
    int ydfwcfms() const { return m_ydfwcfms; }
    // 摇动复位动作模式
    int ydfwdzms() const { return m_ydfwdzms; }
    // 摇动平面
    int ydpm() const { return m_ydpm; }
    // 第1象限摇动码
    int d1xxydm() const { return m_d1xxydm; }
    // 第2象限摇动码
    int d2xxydm() const { return m_d2xxydm; }
    // 第3象限摇动码
    int d3xxydm() const { return m_d3xxydm; }
    // 第4象限摇动码
    int d4xxydm() const { return m_d4xxydm; }
    // 摇动半径
    int ydbj() const { return m_ydbj; }
    // 回中心抬刀高度
    int hzxtdgd() const { return m_hzxtdgd; }
    // 回中心抬刀速度
    int hzxtdsd() const { return m_hzxtdsd; }
    // 回中心抬刀加速时间
    int hzxtdjssj() const { return m_hzxtdjssj; }
    // 主抬刀加速时间
    int ztdjssj() const { return m_ztdjssj; }
    // 退出前回退距离
    int tcqhtjl() const { return m_tcqhtjl; }
    // 抬刀前慢速回退距离
    int tdqmshtjl() const { return m_tdqmshtjl; }
    // 抬刀前慢速回退速度
    int tdqmshtsd() const { return m_tdqmshtsd; }
    // 大面积抬刀模式开关量
    int dmjtdmskgl() const { return m_dmjtdmskgl; }

    void setydfx(int v) { m_ydfx = v; }
    void setzdydsd(int v) { m_zdydsd = v; }
    void setsdjcxfs(int v) { m_sdjcxfs = v; }
    void setydfwcfms(int v) { m_ydfwcfms = v; }
    void setydfwdzms(int v) { m_ydfwdzms = v; }
    void setydpm(int v) { m_ydpm = v; }
    void setd1xxydm(int v) { m_d1xxydm = v; }
    void setd2xxydm(int v) { m_d2xxydm = v; }
    void setd3xxydm(int v) { m_d3xxydm = v; }
    void setd4xxydm(int v) { m_d4xxydm = v; }
    void setydbj(int v) { m_ydbj = v >= 0 ? v : 0; }
    void sethzxtdgd(int v) { m_hzxtdgd = v; }
    void sethzxtdsd(int v) { m_hzxtdsd = v; }
    void sethzxtdjssj(int v) { m_hzxtdjssj = v; } // n
    void setztdjssj(int v) { m_ztdjssj = v; }// n
    void settcqhtjl(int v) { m_tcqhtjl = v; }// n
    void settdqmshtjl(int v) { m_tdqmshtjl = v; }
    void settdqmshtsd(int v) { m_tdqmshtsd = v; }
    void setdmjtdmskgl(int v) { m_dmjtdmskgl = v;}

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    int m_ydfx = 0;
    int m_zdydsd = 0;
    int m_sdjcxfs = 0;  // 8. set default by fdcs form
    int m_ydfwcfms = 0;
    int m_ydfwdzms = 0;
    int m_ydpm = 0;
    int m_d1xxydm = 0;
    int m_d2xxydm = 0;
    int m_d3xxydm = 0;
    int m_d4xxydm = 0;
    int m_ydbj = 0;
    int m_hzxtdgd = 0;
    int m_hzxtdsd = 0;
    int m_hzxtdjssj = 50;
    int m_ztdjssj = 50;
    int m_tcqhtjl = 1000;
    int m_tdqmshtjl = 50;
    int m_tdqmshtsd = 10;
    int m_dmjtdmskgl = 0;
};

// 版本数据
class VersionPropertyObject : public BasePropertyObject
{
Q_OBJECT;
    Q_PROPERTY(QString versionSkh READ versionSkh)
    Q_PROPERTY(QString versionXskz READ versionXskz)
    Q_PROPERTY(QString versionXwj READ versionXwj)
    Q_PROPERTY(QString versionXwjnh READ versionXwjnh)
public:

        // Init.
        VersionPropertyObject(QObject* parent = nullptr, const QString& name = "");

    QString versionSkh() const { return QString("%1.%2").arg((m_version1 >> 8) & 0xFF).arg(m_version1 & 0xFF); }
    QString versionXskz() const { return QString("%1.%2").arg((m_version2 >> 8) & 0xFF).arg(m_version2 & 0xFF); }
    //QString versionXwj() const { return QString("%1.%2.%3").arg((m_version3 >> 8) & 0xFF).arg((m_version3 >> 4) & 0x0F).arg(m_version3 & 0x0F);  }
    //QString versionXwjnh() const { return QString("%1.%2.%3").arg((m_version4 >> 8) & 0xFF).arg((m_version4 >> 4) & 0x0F).arg(m_version4 & 0x0F); }
    QString versionXwj() const { return QString("%1.%2.%3").arg(m_version3 / 10000).arg((m_version3 % 10000) / 100, 2, 10, QLatin1Char('0')).arg(m_version3 % 10, 2, 10, QLatin1Char('0')); }
    QString versionXwjnh() const { return QString("%1.%2.%3").arg(m_version4 / 10000).arg((m_version4 % 10000) / 100, 2, 10, QLatin1Char('0')).arg(m_version4 % 10, 2, 10, QLatin1Char('0')); }

    std::vector<uint16_t> GetValues() const override { return std::vector<uint16_t>();  }
    int GetCmdAddress() const override { return -1; }

    void SetValues1(std::vector<uint16_t> v);
    void SetValues2(std::vector<uint16_t> v);
protected:
    int m_version1 = 0;
    int m_version2 = 0;
    int m_version3 = 0;
    int m_version4 = 0;
};


// 油温油位火焰检测
class YouwenJianchePropertyObject : public BasePropertyObject
{
    Q_OBJECT;
    Q_PROPERTY(bool youWen READ youWen)
        Q_PROPERTY(bool youWei READ youWei)
        Q_PROPERTY(bool huoYan READ huoYan)
public:

    // Init.
    YouwenJianchePropertyObject(QObject* parent = nullptr, const QString& name = "");

    bool youWen() const { return m_ywjc; }
    bool youWei() const { return m_ywjc2; }
    bool huoYan() const { return m_hyjc; }

    void setYouWen(bool v) { m_ywjc = v; }
    void setYouWei(bool v) { m_ywjc2 = v; }
    void setHuoYan(bool v) { m_hyjc = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    bool m_ywjc = true;
    bool m_ywjc2 = true;
    bool m_hyjc = true;
};

// AJC
class AJCPropertyObject : public BasePropertyObject
{
    Q_OBJECT;
    Q_PROPERTY(int ajc READ ajc WRITE setajc)
public:

    // Init.
    AJCPropertyObject(QObject* parent = nullptr, const QString& name = "");

    int ajc() const { return m_ajc; }

    void setajc(int v) { m_ajc = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    int m_ajc = 9;
};

class Reg78PropertyObject : public BasePropertyObject
{
    Q_OBJECT;
    Q_PROPERTY(int fsycgbsj READ fsycgbsj WRITE setfsycgbsj)
public:

    // Init.
    Reg78PropertyObject(QObject* parent = nullptr, const QString& name = "");

    int fsycgbsj() const { return m_fsycgbsj; }
    void setfsycgbsj(int v) { m_fsycgbsj = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:
    int m_fsycgbsj = 0;
};

class Reg81PropertyObject : public BasePropertyObject
{
    Q_OBJECT;
public:

    // Init.
    Reg81PropertyObject(QObject* parent = nullptr, const QString& name = "");

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;
protected:

};

class RegOnePropertyObject : public BasePropertyObject
{
    Q_OBJECT;
public:

    // Init.
	RegOnePropertyObject(QObject* parent = nullptr, const QString& name = "", int cmdAddress = -1) : BasePropertyObject(parent, name), m_cmdAddress(cmdAddress) {}

    int v() const { return m_v; }
    void setv(int v) { m_v = v; }

    std::vector<uint16_t> GetValues() const override;
	int GetCmdAddress() const override { return m_cmdAddress; }

protected:
    int m_v = 0;

private:
    int m_cmdAddress = -1;
};

class Reg83PropertyObject : public BasePropertyObject
{
    Q_OBJECT;
public:

    // Init.
    Reg83PropertyObject(QObject* parent = nullptr, const QString& name = "");

    int ppgw() const { return m_ppgw; }
    void setppgw(int v) { m_ppgw = v; }

    std::vector<uint16_t> GetValues() const override;
    int GetCmdAddress() const override;

protected:
    int m_ppgw = 0;
};

class Reg84PropertyObject : public RegOnePropertyObject
{
    Q_OBJECT;
public:
    Reg84PropertyObject(QObject* parent = nullptr, const QString& name = "") : RegOnePropertyObject(parent, name, TMBS_MAP0_ID_REG84) {};
};

class Reg85PropertyObject : public RegOnePropertyObject
{
    Q_OBJECT;
public:
    Reg85PropertyObject(QObject* parent = nullptr, const QString& name = "") : RegOnePropertyObject(parent, name, TMBS_MAP0_ID_REG85) {};
};

class Reg86PropertyObject : public RegOnePropertyObject
{
    Q_OBJECT;
public:
    Reg86PropertyObject(QObject* parent = nullptr, const QString& name = "") : RegOnePropertyObject(parent, name, TMBS_MAP0_ID_REG86) {};
};
//////////////////////////////

class PropertyObjects
{
private:
    static PropertyObjects instance;
    PropertyObjects();

public:
    static PropertyObjects* getInstance();
    ~PropertyObjects();

	void CreateData();
    void LoadData();

    JogPropertyObject* propertyObjectJog;
    LocPropertyObject* propertyObjectLoc;
    CmvTouchPropertyObject* propertyObjectCmvTouch;
    CmvTolmtPropertyObject* propertyObjectCmvToLMT;
    CmvFrmlmtPropertyObject* propertyObjectCmvFRMLMT;
    CmvTozsigPropertyObject* propertyObjectCmvToZSig;
    SPKPropertyObject* propertyObjectSPK;
    FDQDPropertyObject* propertyObjectFanddianIO;
    FZQDPropertyObject* propertyObjectFuzhuIO;
    SKHKZPropertyObject* propertyObjectShouKongHe;
    ZLJBCPropertyObject* propertyObjectZLJBC[4];
    DDYDPropertyObject* propertyObjectDianDong;
    DJSNPropertyObject* propertyObjectDianJiShiNeng;
    FmqPropertyObject* propertyObjectFengMingQi;
    DebugPropertyObject* propertyObjectDebug;

    ADCPropertyObject* propertyObjectADC;
    FDHLPropertyObject* propertyObjectFdhl;
    FDJGPropertyObject* propertyObjectFdjg;
    FDWCPropertyObject* propertyObjectFdwc;
    YDPropertyObject* propertyObjectYd;

	VersionPropertyObject* propertyObjectVersion;
    YouwenJianchePropertyObject* propertyObjectYouwenJianche;
	AJCPropertyObject* propertyObjectAJC;
	Reg78PropertyObject* propertyObjectReg78;
    Reg81PropertyObject* propertyObjectReg81;
    Reg83PropertyObject* propertyObjectReg83;
    Reg84PropertyObject* propertyObjectReg84;
    Reg85PropertyObject* propertyObjectReg85;
    Reg86PropertyObject* propertyObjectReg86;
};

#endif