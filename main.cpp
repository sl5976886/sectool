#include "logindialog.h"
#include "connect.h"
#include "commonfunction.h"
#include "ccrashstack.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QDir>

#ifdef Q_OS_WIN
long __stdcall   callback(_EXCEPTION_POINTERS*   excp)
{
    CCrashStack crashStack(excp);
    QString sCrashInfo = crashStack.GetExceptionInfo();

    QString file_path = qApp->applicationDirPath();

    QDir *folder_path = new QDir;
    bool exist = folder_path->exists(file_path.append("\\MyApp"));
    if(!exist)
    {
        folder_path->mkdir(file_path);
    }
    delete folder_path;
    folder_path = nullptr;

    QString sFileName = file_path + "\\crash.log";

    QFile file(sFileName);
    if (file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        file.write(sCrashInfo.toUtf8());
        file.close();
    }

    return   EXCEPTION_EXECUTE_HANDLER;
}
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_WIN
    SetUnhandledExceptionFilter(callback);
#endif

    QString strLibPath(QDir::toNativeSeparators(QApplication::applicationDirPath())+QDir::separator()+ "plugins");
    qApp->addLibraryPath(strLibPath);
    qInstallMessageHandler(outputMessage);
    if(!createConnect())
    {
    }
    //a.setQuitOnLastWindowClosed(false);
    //MainWindow w;
    LoginDialog lw;
    //lw.setAttribute(Qt::WA_DeleteOnClose);
    lw.show();
    return a.exec();
}
