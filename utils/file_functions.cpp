#include "file_functions.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QRegExp>

namespace My{
//--------------------my:: --------------------------------------
/*
bool isSeparator(QChar ch)
{
  return ch == QChar::fromLatin1('\\') ||
         ch == QChar::fromLatin1('/');
}
//---------------------------------------------------------------
bool cmp(QChar ch1,QChar ch2)
{
  return ( ch1.toUpper() == ch2.toUpper() ) ||
         ( isSeparator(ch1) && isSeparator(ch2) );
}
//---------------------------------------------------------------
bool isSubPath(const QString& dir, const QString& path)
{
  const int length = dir.length();
  if(!length && length >= path.length()) // FIX: http://www.cyberforum.ru/blogs/131347/blog2457-page2.html#comment10722
    return false;

  if( !isSeparator(dir[length-1]) &&
      !isSeparator(path[length]) )
    return false;

  for(int i=0; i<length; ++i)
    if( !cmp(dir[i],path[i]) )
      return false;

  return true;
}
//---------------------------------------------------------------
*/
//---------------------------------------------------------------
// http://www.cyberforum.ru/blogs/131347/blog2457-page2.html#comment10723
bool isSubPath(const QString &dir, const QString &path)
{
   if ( dir.isEmpty() ) return false;

   QString nativeDir = QDir::toNativeSeparators(dir);
   QString nativePath = QDir::toNativeSeparators(path);

   if( nativeDir.lastIndexOf(QDir::separator()) != nativeDir.size()-1 )
   {
     nativeDir += QDir::separator();
   }

   return !nativePath.indexOf( nativeDir, Qt::CaseInsensitive );
}
//---------------------------------------------------------------
QString copyFile(const QString &filePath, const QString &destinationDir, const QString &baseDir)
{
  QFile file(filePath);
  QDir  destDir(destinationDir);    // Куда копируем
  QFileInfo info(file);
  QDir fileDir= info.absoluteDir(); // директория DLL-ки
  QString newFilePath;
  bool success= false;

  if(baseDir.isNull() || baseDir.isEmpty())
  {
    newFilePath= destinationDir+QDir::separator()+info.fileName();
    success= file.copy(newFilePath);
    return success?newFilePath:QString();
  }

  QDir  relDir(baseDir); // Полный путь к подпапке (относительно папки)
  QString path= relDir.relativeFilePath(fileDir.absolutePath()); //   назначение/plugins/

  newFilePath= destDir.absoluteFilePath(path)+QDir::separator()+info.fileName();
  success= destDir.mkpath(path) && file.copy(newFilePath);

  return success?newFilePath:QString();
}
//---------------------------------------------------------------
void makeSymLinks(const QString& filePath,
                  const QString& newFilePath)
{
  // Ищет симлинки рядом с файлом и создает симлинки
  // с таким же именем на новый файл (после копирования)
  QFileInfo libFileInfo(filePath);
  QFileInfo newLibFileInfo(newFilePath);
  QFileInfoList list= libFileInfo.dir().entryInfoList(QDir::Files);
  foreach (QFileInfo fileInfo, list)
  {
     if(fileInfo.isSymLink() &&
        QFileInfo(fileInfo.symLinkTarget())== libFileInfo )
     {
       QString newSymLinkFileName=
         newLibFileInfo.absolutePath()+QDir::separator()+fileInfo.fileName();
       QFile(newLibFileInfo.fileName()).link(newSymLinkFileName);
     }
  }
}
//void copySymLinks(const QString &filePath, const QString &destinationDir, const QString &baseDir)
//{
//  QFileInfo libFileInfo(filePath);
//  QFileInfoList list= libFileInfo.dir().entryInfoList(QDir::Files);
//  foreach (QFileInfo fileInfo, list)
//  {
//     if(fileInfo.isSymLink() &&
//        QFileInfo(fileInfo.symLinkTarget())== libFileInfo )
//     copyFile(fileInfo.absoluteFilePath(),destinationDir,baseDir);
//  }
//}
//---------------------------------------------------------------
bool isMSVClib(const QString& filePath)
{
  QRegExp re("MSVC[RP]\\d+.dll"); //  MSVCR*.dll и MSVCP*.dll
  re.setMinimal(true);

  QFile file(filePath);
  QFileInfo info(file);

  return re.exactMatch(info.fileName());
}
//---------------------------------------------------------------
bool isMySQLlib(const QString& filePath)
{
#ifdef Q_OS_WIN
  QRegExp re("libmysql\\d*\\.(dll)"); // libmysql.dll
  re.setMinimal(true);
#elif defined Q_OS_LINUX
  QRegExp re("libmysql(client)?\\.(so(\\.\\d+){0,3})|(dll)"); // libmysqlclient.so.18.0.0
#endif

  QFile file(filePath);
  QFileInfo info(file);

  return re.exactMatch(info.fileName());
}
//------------------End my:: -------------------------------------------
}
