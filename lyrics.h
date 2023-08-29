#ifndef LYRICS_H
#define LYRICS_H

#include <QWidget>
#include <QString>
#include <QMap>
#include <iostream>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

using namespace std;
class Lyrics
{
private:
    QString lyricsPath;

public:
    QList<QString> listLyricsText;
    QList<int> listLyricsTime;
public:
    Lyrics(QString lyricsPath);
    Lyrics();
    bool readLyricsFile(QString lyricsPath);
    bool analysisLyricsFile(QString line);
    QList<QString> getListLyricsText() const;
    QList<int> getListLyricsTime() const;
};

#endif // LYRICS_H
