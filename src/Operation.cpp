#include "../include/Operation.h"

Operation::Operation(const QString& filePath, double operationTime)
{
    this->filePath = filePath;
    this->operationTime = operationTime;
}
