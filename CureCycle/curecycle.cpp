#include "curecycle.h"

CureCycle::CureCycle()
{

}

void CureCycle::AddStage( const CureCycleStage &stage )
{
    cure_cycle_.append( stage );
}
