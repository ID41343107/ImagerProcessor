#include "zoomeditor.h"
#include <QFileDialog>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPainter>
#include <QMessageBox>
#include <QColorDialog>

// DrawableLabel implementation
DrawableLabel::DrawableLabel(QWidget *parent)
    : QLabel(parent),
      isDrawing(false),
      brushColor(Qt::red),
      brushSize(3)
{
    setMouseTracking(true);
}

void DrawableLabel::setDrawingImage(const QImage &image)
{
    drawingImage = image;
    originalImage = image;
    setPixmap(QPixmap::fromImage(drawingImage));
}

void DrawableLabel::clearDrawing()
{
    drawingImage = originalImage.copy();
    setPixmap(QPixmap::fromImage(drawingImage));
    update();
}

void DrawableLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();
        if (pos.x() >= 0 && pos.x() < drawingImage.width() &&
            pos.y() >= 0 && pos.y() < drawingImage.height()) {
            lastPoint = pos;
            isDrawing = true;
        }
    }
}

void DrawableLabel::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && isDrawing) {
        QPoint pos = event->pos();
        if (pos.x() >= 0 && pos.x() < drawingImage.width() &&
            pos.y() >= 0 && pos.y() < drawingImage.height()) {
            drawLineTo(pos);
        }
    }
}

void DrawableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDrawing) {
        QPoint pos = event->pos();
        if (pos.x() >= 0 && pos.x() < drawingImage.width() &&
            pos.y() >= 0 && pos.y() < drawingImage.height()) {
            drawLineTo(pos);
        }
        isDrawing = false;
    }
}

void DrawableLabel::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&drawingImage);
    painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    
    lastPoint = endPoint;
    
    setPixmap(QPixmap::fromImage(drawingImage));
    update();
}

// ZoomEditor implementation
ZoomEditor::ZoomEditor(const QImage &image, QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("放大編輯器"));
    
    // Create central widget with scroll area
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    QScrollArea *scrollArea = new QScrollArea(this);
    imageLabel = new DrawableLabel();
    imageLabel->setDrawingImage(image);
    imageLabel->setScaledContents(false);
    
    scrollArea->setWidget(imageLabel);
    scrollArea->setWidgetResizable(false);
    mainLayout->addWidget(scrollArea);
    
    setCentralWidget(centralWidget);
    
    createActions();
    createToolBar();
    
    resize(800, 600);
}

ZoomEditor::~ZoomEditor()
{
}

void ZoomEditor::createActions()
{
    saveAction = new QAction(QStringLiteral("另存新檔"), this);
    saveAction->setStatusTip(QStringLiteral("將編輯後的圖片另存新檔"));
    connect(saveAction, &QAction::triggered, this, &ZoomEditor::saveImage);
    
    colorAction = new QAction(QStringLiteral("選擇顏色"), this);
    colorAction->setStatusTip(QStringLiteral("選擇畫筆顏色"));
    connect(colorAction, &QAction::triggered, this, &ZoomEditor::chooseBrushColor);
    
    clearAction = new QAction(QStringLiteral("清除塗鴉"), this);
    clearAction->setStatusTip(QStringLiteral("清除所有塗鴉，恢復原圖"));
    connect(clearAction, &QAction::triggered, this, &ZoomEditor::clearDrawing);
}

void ZoomEditor::createToolBar()
{
    QToolBar *toolBar = addToolBar(QStringLiteral("工具"));
    
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
    
    toolBar->addAction(colorAction);
    toolBar->addSeparator();
    
    // Brush size control
    toolBar->addWidget(new QLabel(QStringLiteral("筆刷大小: "), toolBar));
    brushSizeSpinBox = new QSpinBox(toolBar);
    brushSizeSpinBox->setRange(1, 50);
    brushSizeSpinBox->setValue(3);  // Default brush size
    connect(brushSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ZoomEditor::setBrushSize);
    toolBar->addWidget(brushSizeSpinBox);
    
    toolBar->addSeparator();
    toolBar->addAction(clearAction);
}

void ZoomEditor::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    QStringLiteral("另存新檔"),
                                                    "",
                                                    "PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)");
    if (fileName.isEmpty())
        return;
    
    if (imageLabel->getDrawingImage().save(fileName)) {
        QMessageBox::information(this, QStringLiteral("成功"),
                                QStringLiteral("圖片已成功儲存！"));
    } else {
        QMessageBox::warning(this, QStringLiteral("錯誤"),
                            QStringLiteral("無法儲存圖片！"));
    }
}

void ZoomEditor::chooseBrushColor()
{
    QColor color = QColorDialog::getColor(imageLabel->getBrushColor(), 
                                         this, QStringLiteral("選擇畫筆顏色"));
    if (color.isValid()) {
        imageLabel->setBrushColor(color);
    }
}

void ZoomEditor::setBrushSize(int size)
{
    imageLabel->setBrushSize(size);
}

void ZoomEditor::clearDrawing()
{
    imageLabel->clearDrawing();
}

