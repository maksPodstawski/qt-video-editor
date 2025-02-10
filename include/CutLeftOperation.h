#ifndef CUTLEFTOPERATION_H
#define CUTLEFTOPERATION_H

#include "Operation.h"

class CutLeftOperation : public Operation
{
public:
    CutLeftOperation(QString& filePath, double cutTime);
    QString execute() override;

    QJsonObject serialize() const override;
};

#endif //CUTLEFTOPERATION_H
