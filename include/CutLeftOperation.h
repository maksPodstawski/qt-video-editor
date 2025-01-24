//
// Created by czest on 14.01.2025.
//

#ifndef CUTLEFTOPERATION_H
#define CUTLEFTOPERATION_H
#include "Operation.h"


class CutLeftOperation: public Operation {
public:
    CutLeftOperation(QString &filePath, double cutTime): Operation(filePath, cutTime){};
    QString execute() override;

};



#endif //CUTLEFTOPERATION_H
