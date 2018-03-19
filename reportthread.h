#ifndef REPORTTHREAD_H
#define REPORTTHREAD_H

#include <QObject>
#include <QThread>

class ReportThread : public QThread
{
    Q_OBJECT
public:
    explicit ReportThread(QObject *parent = 0);
    void test();
    void stop();
    QString saveFileName,strTime;
    void getInfo(QString file,QString time);
protected:
    void run();
    volatile bool stopped;
signals:
    void ifSuccess(int code);
};

#endif // RENDERTHREAD_H
