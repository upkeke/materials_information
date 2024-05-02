#include "funcclass.h"

FuncClass::FuncClass() {}


MaterialData::MaterialData():has(false)
{

}

MaterialData::MaterialData(const std::vector<int> &idList, const QStringList &other)
    :idList(idList),sgsData(other),has(true)
{

}

int MaterialData::id()
{
    return idList[0];
}
int MaterialData::mt_id()
{
    return idList[1];
}
int MaterialData::sp_id()
{
    return idList[2];
}

QString MaterialData::Pb()
{
    return sgsData[0];
}
QString MaterialData::Cd()
{
    return sgsData[1];
}
QString MaterialData::Hg()
{
    return sgsData[2];
}
QString MaterialData::Cr6Plus()
{
    return sgsData[3];
}
QString MaterialData::PBB()
{
    return sgsData[4];
}
QString MaterialData::PBDE()
{
    return sgsData[5];
}
QString MaterialData::DEHP()
{
    return sgsData[6];
}
QString MaterialData::BBP()
{
    return sgsData[7];
}
QString MaterialData::DBP()
{
    return sgsData[8];
}
QString MaterialData::DIBP()
{
    return sgsData[9];
}
QString MaterialData::Br()
{
    return sgsData[10];
}
QString MaterialData::Cl()
{
    return sgsData[11];
}
QString MaterialData::PVC()
{
    return sgsData[12];
}
QString MaterialData::SCCP()
{
    return sgsData[13];
}
QString MaterialData::HBCDD()
{
    return sgsData[14];
}
QString MaterialData::Sb()
{
    return sgsData[15];
}
QString MaterialData::Be()
{
    return sgsData[16];
}
QString MaterialData::PAHs()
{
    return sgsData[17];
}
QString MaterialData::PFOS()
{
    return sgsData[18];
}
QString MaterialData::PFOA()
{
    return sgsData[19];
}
QString MaterialData::Red_Phosphorus()
{
    return sgsData[20];
}

QString MaterialData::SGS_NO()
{
return sgsData[21];
}

QString MaterialData::SGS_Date()
{
    return sgsData[22];
}
QString MaterialData::SGS_Path()
{
    return sgsData[23];
}
QString MaterialData::MSDS_Path()
{
    return sgsData[24];
}
QString MaterialData::MSDS_Date()
{
    return sgsData[25];
}
QString MaterialData::UL()
{
    return sgsData[26];
}
QString MaterialData::confide()
{
    return sgsData[27];
}

int distance_date(const QString &last)
{

    // 将日期字符串转换为 QDate 对象
    QDate givenDate = QDate::fromString(last, "yyyy/MM/dd");

    // 获取当前日期
    QDate currentDate = QDate::currentDate();

    // 计算天数间隔
    int daysDiff = givenDate.daysTo(currentDate);
    return daysDiff;
}
