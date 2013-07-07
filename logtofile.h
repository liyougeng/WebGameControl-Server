#ifndef LOGTOFILE_H
#define LOGTOFILE_H
#include <QFile>
#include <QTextStream>
#include <QString>

class LogToFile
{
public:
    //LogToFile();
    LogToFile(QString filename){
        errlevel = 0;
        file.setFileName(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            errlevel = 1;//open failed;
            return;
        }
    }
    void write(QString data){
        if(!data.endsWith("\n")){
            data.append("\n");
        }
        file.write(QByteArray().append(data));
        file.flush();
    }
    void write(QByteArray data){
        file.write(data);
        file.flush();
    }
    ~LogToFile(){
        if(file.isOpen()){
            file.flush();
            file.close();
        }
    }
    uint Err(){return errlevel;}
private:
    QString filepath;
    QFile   file;
    uint    errlevel;

};

#endif // LOGTOFILE_H
