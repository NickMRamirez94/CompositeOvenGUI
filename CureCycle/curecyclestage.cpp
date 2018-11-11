#include "curecyclestage.h"

CureCycleStage::CureCycleStage()
{

}

///
/// \brief CureCycleStage::SetData
/// \param stage_type - Stage type.
/// \param temp - Stage temperature.
/// \param rate - Stage rate.
/// Setter for stage type, temperature, and rate.
///
void CureCycleStage::SetData( StageType stage_type, const QString &temp, const QString &rate )
{
    SetStageType( stage_type );
    SetTemp( temp );
    SetRate( rate );
}

///
/// \brief CureCycleStage::SetStageType
/// \param stage_type - Stage type.
/// Setter for stage type.
///
void CureCycleStage::SetStageType( StageType stage_type )
{
  stage_type_ = stage_type;
}

///
/// \brief CureCycleStage::SetTemp
/// \param temp - Stage temperature.
/// Setter for temperature.
///
void CureCycleStage::SetTemp( const QString &temp )
{
  temp_ = temp;
}

///
/// \brief CureCycleStage::SetRate
/// \param rate - Stage rate
/// Setter for rate
///
void CureCycleStage::SetRate( const QString &rate )
{
  rate_ = rate;
}
