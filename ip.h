#ifndef IP_H
#define IP_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QImage>
#include <QLabel>
#include "gtransform.h"
#include "zoomeditor.h"
#include <QMouseEvent>
#include <QRect>
#include <QRubberBand>
#include <QInputDialog>


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

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void showOpenFile();
    void bigsize();
    void ssize();
    void showGeometryTransform();
    void enableZoomSelection();

private:
    gtransform *gWin;
    QWidget *central;
    QMenu *fileMenu;
    QToolBar *fileTool;
    QImage img;
    QString filename;
    QLabel *imgWin;

    QLabel *statusLabel;
    QLabel *mousePosLabel;

    QAction *openFileAction;
    QAction *exitAction;
    QAction *bigFileAction;
    QAction *sAction;
    QAction *geometryAction;
    QAction *zoomSelectionAction;

    // For zoom selection
    bool zoomSelectionMode;
    QPoint selectionStart;
    QPoint selectionEnd;
    QRubberBand *rubberBand;

};
#endif // IP_H
