#ifndef ZOOMEDITOR_H
#define ZOOMEDITOR_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QMouseEvent>
#include <QSpinBox>
#include <QColorDialog>
#include <QPainter>
#include <QVBoxLayout>

class ZoomEditor : public QMainWindow
{
    Q_OBJECT

public:
    ZoomEditor(const QImage &image, QWidget *parent = nullptr);
    ~ZoomEditor();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void saveImage();
    void chooseBrushColor();
    void setBrushSize(int size);
    void clearDrawing();

private:
    void createActions();
    void createToolBar();
    void drawLineTo(const QPoint &endPoint);

    QImage originalImage;
    QImage drawingImage;
    QLabel *imageLabel;
    
    // Drawing tools
    bool isDrawing;
    QPoint lastPoint;
    QColor brushColor;
    int brushSize;
    
    // Actions
    QAction *saveAction;
    QAction *colorAction;
    QAction *clearAction;
    QSpinBox *brushSizeSpinBox;
};

#endif // ZOOMEDITOR_H
