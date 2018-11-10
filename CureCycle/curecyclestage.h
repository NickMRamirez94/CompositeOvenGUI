#ifndef CURECYCLESTAGE_H
#define CURECYCLESTAGE_H

#include <QString>

class CureCycleStage
{
public:
    enum StageType
    {
        kInvalid,
        kRamp,
        kHold,
        kDeramp
    };

    CureCycleStage();

    void SetData( StageType stage_type, const QString &temp, const QString &rate );

    void SetStageType( StageType stage_type );

    void SetTemp( const QString &temp );

    void SetRate( const QString &rate );
private:
    StageType stage_type_ = kInvalid;
    QString temp_ = "";
    QString rate_ = "";
};

#endif // CURECYCLESTAGE_H
