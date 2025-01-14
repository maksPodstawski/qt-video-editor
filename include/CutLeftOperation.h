//
// Created by czest on 14.01.2025.
//

#ifndef CUTLEFTOPERATION_H
#define CUTLEFTOPERATION_H
#include "Operation.h"


class CutLeftOperation: public Operation {
public:
    CutLeftOperation(QString &filePath, double cutTime): Operation(filePath), cutTime(cutTime) {};
    QString execute() override;

private:
    double cutTime;
};



#endif //CUTLEFTOPERATION_H
