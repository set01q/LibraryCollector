#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H
//---------------------------------------------------------------
#include <QString>
//---------------------------------------------------------------
namespace My
{
  bool isSubPath(const QString& dir,const QString& path);
  QString copyFile(const QString& filePath,
                   const QString& destinationDir ,
                   const QString& baseDir= QString());

  void makeSymLinks(const QString& filePath,const QString &newFilePath);

  bool isMSVClib(const QString &filePath);
  bool isMySQLlib(const QString& filePath);
}
//---------------------------------------------------------------
#endif // FILEFUNCTIONS_H