#include "zoomeditor.h"
#include <QFileDialog>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPainter>
#include <QMessageBox>

ZoomEditor::ZoomEditor(const QImage &image, QWidget *parent)
    : QMainWindow(parent),
      originalImage(image),
      drawingImage(image),
      isDrawing(false),
      brushColor(Qt::red),
      brushSize(3)
{
    setWindowTitle(QStringLiteral("放大編輯器"));
    
    // Create central widget with scroll area
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    QScrollArea *scrollArea = new QScrollArea(this);
    imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap::fromImage(drawingImage));
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
    toolBar->addWidget(new QLabel(QStringLiteral("筆刷大小: ")));
    brushSizeSpinBox = new QSpinBox();
    brushSizeSpinBox->setRange(1, 50);
    brushSizeSpinBox->setValue(brushSize);
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
    
    if (drawingImage.save(fileName)) {
        QMessageBox::information(this, QStringLiteral("成功"),
                                QStringLiteral("圖片已成功儲存！"));
    } else {
        QMessageBox::warning(this, QStringLiteral("錯誤"),
                            QStringLiteral("無法儲存圖片！"));
    }
}

void ZoomEditor::chooseBrushColor()
{
    QColor color = QColorDialog::getColor(brushColor, this, QStringLiteral("選擇畫筆顏色"));
    if (color.isValid()) {
        brushColor = color;
    }
}

void ZoomEditor::setBrushSize(int size)
{
    brushSize = size;
}

void ZoomEditor::clearDrawing()
{
    drawingImage = originalImage.copy();
    imageLabel->setPixmap(QPixmap::fromImage(drawingImage));
    update();
}

void ZoomEditor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Get position relative to imageLabel
        QPoint labelPos = imageLabel->mapFrom(this, event->pos());
        
        // Check if click is within image bounds
        if (labelPos.x() >= 0 && labelPos.x() < drawingImage.width() &&
            labelPos.y() >= 0 && labelPos.y() < drawingImage.height()) {
            lastPoint = labelPos;
            isDrawing = true;
        }
    }
}

void ZoomEditor::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && isDrawing) {
        QPoint labelPos = imageLabel->mapFrom(this, event->pos());
        
        // Check if position is within image bounds
        if (labelPos.x() >= 0 && labelPos.x() < drawingImage.width() &&
            labelPos.y() >= 0 && labelPos.y() < drawingImage.height()) {
            drawLineTo(labelPos);
        }
    }
}

void ZoomEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDrawing) {
        QPoint labelPos = imageLabel->mapFrom(this, event->pos());
        
        if (labelPos.x() >= 0 && labelPos.x() < drawingImage.width() &&
            labelPos.y() >= 0 && labelPos.y() < drawingImage.height()) {
            drawLineTo(labelPos);
        }
        isDrawing = false;
    }
}

void ZoomEditor::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&drawingImage);
    painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    
    lastPoint = endPoint;
    
    imageLabel->setPixmap(QPixmap::fromImage(drawingImage));
    update();
}
