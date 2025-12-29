#ifndef IP_H
#define IP_H

#include <QMainWindow>
#include <QACtion>
#include <QMenu>
#include <QToolBar>
#include <QImage>
#include <QLabel>
#include "gtransform.h"


class ip : public QMainWindow
{
    Q_OBJECT

public:
    ip(QWidget *parent = nullptr);
    ~ip();
    void createActions();
    void createMenus();
    void createToolBars();
    void loadFile (QString filename);

private slots:
    void showOpenFile();
    void bigsize();
    void ssize();
    void showGeometryTransform();

private:
    gtransform *gWin;
    QWidget *central;
    QMenu *fileMenu;
    QToolBar *fileTool;
    QImage img;
    QString filename;
    QLabel *imgWin;
    QAction *openFileAction;
    QAction *exitAction;
    QAction *bigFileAction;
    QAction *sAction;
    QAction *geometryAction;
};
#endif // IP_H
