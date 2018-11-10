#ifndef CURECYCLE_H
#define CURECYCLE_H

#include <QList>
#include "curecyclestage.h"

class CureCycle
{
public:
    CureCycle();

    void AddStage( const CureCycleStage &stage );

private:
    QList<CureCycleStage> cure_cycle_;
};

#endif // CURECYCLE_H
