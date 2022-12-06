#include "headers/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QDir>
#include <QString>

#include <Python.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize Qt localization service
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "HurrigatorsProject_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    Py_Initialize();

    PyObject *_sysPath = PySys_GetObject((char*)"path");

    //For Mac
    #ifdef Q_OS_MACX

        PyList_Append(_sysPath, PyUnicode_FromString("../../../python"));

    //For Windows
    #else
        PyList_Append(_sysPath, PyUnicode_FromString("./python"));
    #endif

    //For Mac
    #ifdef Q_OS_MACX
        QDir test(QDir::currentPath());

        //Go up from .app
        test.cdUp();
        test.cdUp();
        test.cdUp();

        QString tempString;

        QFileInfo fi(test, tempString);
        QString _pythonPath = fi.canonicalFilePath() + "/python/main.py";

    //For Windows
    #else
        QString _pythonPath = QDir::currentPath() + "/python/main.py";
    #endif

    PyObject *_pObj = Py_BuildValue("s", _pythonPath.toStdString().c_str());
    FILE *_pyFp = _Py_fopen_obj(_pObj, "r+");

    if (_pyFp != NULL)
    {
        PyRun_AnyFile(_pyFp, "python/main.py");
    }
    else
    {
        QMessageBox _b;
        _b.setText(_pythonPath + " cannot be found. Using whatever data is in place.");
        _b.exec();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
