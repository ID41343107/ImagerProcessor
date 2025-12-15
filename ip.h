#ifndef IP_H
#define IP_H

#include <QMainWindow>
#include <QACtion>
#include <QMenu>
#include <QToolBar>
#include <QImage>
#include <QLabel>


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

private:
    QWidget *central;
    QMenu *fileMenu;
    QToolBar *fileTool;
    QImage img;
    QString filename;
    QLabel *imgWin;
    QAction *openFileAction;
    QAction *exitAction;
};
#endif // IP_H
