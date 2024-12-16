//
// Created by czest on 03.12.2024.
//

#ifndef VIDEO_EDITOR_EDITOR_H
#define VIDEO_EDITOR_EDITOR_H


#include <QString>

class Editor {
public:
    bool combineVideos(const QStringList &inputFiles, const QString &outputFile);
};



#endif //VIDEO_EDITOR_EDITOR_H
