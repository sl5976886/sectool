#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>
#include <QThread>

class RenderThread : public QThread
{
    Q_OBJECT
public:
    explicit RenderThread(QObject *parent = 0);
    ~RenderThread();
    void test();
    void stop();
protected:
    void run();
    volatile bool stopped;
signals:
    void refresh(QString);
};

#endif // RENDERTHREAD_H
