#include "curecyclestage.h"

CureCycleStage::CureCycleStage()
{

}

void CureCycleStage::SetData( StageType stage_type, const QString &temp, const QString &rate )
{
    stage_type_ = stage_type;
    temp_ = temp;
    rate_ = rate;
}
void CureCycleStage::SetStageType( StageType stage_type )
{
  stage_type_ = stage_type;
}

void CureCycleStage::SetTemp( const QString &temp )
{
  temp_ = temp;
}

void CureCycleStage::SetRate( const QString &rate )
{
  rate_ = rate;
}
