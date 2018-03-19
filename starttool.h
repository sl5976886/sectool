#ifndef STARTTOOL_H
#define STARTTOOL_H

#include <QObject>
#include <QThread>

class startTool : public QThread
{
    Q_OBJECT
public:
    explicit startTool(QObject *parent = 0);
    void run();
    void stop();
    void test();

protected:
    volatile bool stopped;
signals:

public slots:
};

#endif // STARTTOOL_H
