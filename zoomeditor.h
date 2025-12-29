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

// Custom label that handles drawing
class DrawableLabel : public QLabel
{
    Q_OBJECT
public:
    DrawableLabel(QWidget *parent = nullptr);
    void setDrawingImage(const QImage &image);
    QImage getDrawingImage() const { return drawingImage; }
    void setOriginalImage(const QImage &image) { originalImage = image; }
    void setBrushColor(const QColor &color) { brushColor = color; }
    void setBrushSize(int size) { brushSize = size; }
    void clearDrawing();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    
    QImage originalImage;
    QImage drawingImage;
    bool isDrawing;
    QPoint lastPoint;
    QColor brushColor;
    int brushSize;
};

class ZoomEditor : public QMainWindow
{
    Q_OBJECT

public:
    ZoomEditor(const QImage &image, QWidget *parent = nullptr);
    ~ZoomEditor();

private slots:
    void saveImage();
    void chooseBrushColor();
    void setBrushSize(int size);
    void clearDrawing();

private:
    void createActions();
    void createToolBar();

    DrawableLabel *imageLabel;
    
    // Actions
    QAction *saveAction;
    QAction *colorAction;
    QAction *clearAction;
    QSpinBox *brushSizeSpinBox;
};

#endif // ZOOMEDITOR_H
