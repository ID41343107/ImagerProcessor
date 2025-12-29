#include "zoomeditor.h"
#include <QFileDialog>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPainter>
#include <QMessageBox>
#include <QColorDialog>

/**
 * DrawableLabel 類別實作
 * 這個類別繼承 QLabel，並加入繪圖功能
 */

// 建構函式：初始化繪圖相關變數
DrawableLabel::DrawableLabel(QWidget *parent)
    : QLabel(parent),
      isDrawing(false),       // 初始狀態：未在繪圖
      brushColor(Qt::red),    // 預設畫筆顏色：紅色
      brushSize(3)            // 預設畫筆大小：3 像素
{
    setMouseTracking(true);   // 啟用滑鼠追蹤，即使沒按下按鈕也能接收 mouseMoveEvent
}

// 設定要顯示和編輯的圖片
void DrawableLabel::setDrawingImage(const QImage &image)
{
    drawingImage = image;     // 設定目前編輯的圖片
    originalImage = image;    // 同時保存原始圖片，用於清除功能
    setPixmap(QPixmap::fromImage(drawingImage));  // 顯示圖片
}

// 清除所有繪圖，恢復到原始圖片
void DrawableLabel::clearDrawing()
{
    drawingImage = originalImage.copy();  // 複製原始圖片
    setPixmap(QPixmap::fromImage(drawingImage));
    update();  // 更新顯示
}

/**
 * 滑鼠按下事件：開始繪圖
 * 當使用者按下左鍵且滑鼠在圖片範圍內時，開始繪圖
 */
void DrawableLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();
        // 檢查點擊位置是否在圖片範圍內
        if (pos.x() >= 0 && pos.x() < drawingImage.width() &&
            pos.y() >= 0 && pos.y() < drawingImage.height()) {
            lastPoint = pos;      // 記錄起始點
            isDrawing = true;     // 開始繪圖狀態
        }
    }
}

/**
 * 滑鼠移動事件：持續繪圖
 * 當使用者按住左鍵並移動滑鼠時，從上一個點畫線到當前點
 */
void DrawableLabel::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && isDrawing) {
        QPoint pos = event->pos();
        // 檢查位置是否在圖片範圍內
        if (pos.x() >= 0 && pos.x() < drawingImage.width() &&
            pos.y() >= 0 && pos.y() < drawingImage.height()) {
            drawLineTo(pos);  // 畫線到當前位置
        }
    }
}

/**
 * 滑鼠釋放事件：結束繪圖
 * 當使用者釋放左鍵時，畫最後一段線並結束繪圖
 */
void DrawableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDrawing) {
        QPoint pos = event->pos();
        if (pos.x() >= 0 && pos.x() < drawingImage.width() &&
            pos.y() >= 0 && pos.y() < drawingImage.height()) {
            drawLineTo(pos);  // 畫最後一段線
        }
        isDrawing = false;    // 結束繪圖狀態
    }
}

/**
 * 在圖片上畫線
 * 從上一個點（lastPoint）畫線到指定的結束點
 * 使用 QPainter 在圖片上繪製，並設定畫筆顏色、大小和樣式
 */
void DrawableLabel::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&drawingImage);  // 建立畫家物件，在 drawingImage 上繪圖
    
    // 設定畫筆：顏色、大小、樣式（實線）、端點樣式（圓角）、連接樣式（圓角）
    painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    
    // 從上一個點畫線到當前點
    painter.drawLine(lastPoint, endPoint);
    
    lastPoint = endPoint;  // 更新上一個點為當前點
    
    // 更新顯示
    setPixmap(QPixmap::fromImage(drawingImage));
    update();
}

/**
 * ZoomEditor 類別實作
 * 放大編輯視窗的主要實作
 */

// 建構函式：建立放大編輯視窗
ZoomEditor::ZoomEditor(const QImage &image, QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("放大編輯器"));
    
    // 建立中央元件和佈局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // 建立可捲動區域，用於顯示大圖片
    QScrollArea *scrollArea = new QScrollArea(this);
    
    // 建立 DrawableLabel 來顯示和編輯圖片
    imageLabel = new DrawableLabel();
    imageLabel->setDrawingImage(image);  // 設定要顯示的圖片
    imageLabel->setScaledContents(false); // 不縮放圖片內容，保持原始大小
    
    // 將 imageLabel 設為捲動區域的內容
    scrollArea->setWidget(imageLabel);
    scrollArea->setWidgetResizable(false);  // 不自動調整大小
    mainLayout->addWidget(scrollArea);
    
    setCentralWidget(centralWidget);
    
    // 建立動作和工具列
    createActions();
    createToolBar();
    
    resize(800, 600);  // 設定視窗初始大小
}

ZoomEditor::~ZoomEditor()
{
}

/**
 * 建立視窗的動作（Action）
 * 定義另存新檔、選擇顏色、清除塗鴉等功能的動作並連接到對應的槽函式
 */
void ZoomEditor::createActions()
{
    // 另存新檔動作
    saveAction = new QAction(QStringLiteral("另存新檔"), this);
    saveAction->setStatusTip(QStringLiteral("將編輯後的圖片另存新檔"));
    connect(saveAction, &QAction::triggered, this, &ZoomEditor::saveImage);
    
    // 選擇顏色動作
    colorAction = new QAction(QStringLiteral("選擇顏色"), this);
    colorAction->setStatusTip(QStringLiteral("選擇畫筆顏色"));
    connect(colorAction, &QAction::triggered, this, &ZoomEditor::chooseBrushColor);
    
    // 清除塗鴉動作
    clearAction = new QAction(QStringLiteral("清除塗鴉"), this);
    clearAction->setStatusTip(QStringLiteral("清除所有塗鴉，恢復原圖"));
    connect(clearAction, &QAction::triggered, this, &ZoomEditor::clearDrawing);
}

/**
 * 建立工具列
 * 在工具列中加入各種控制項：另存新檔、選擇顏色、筆刷大小、清除塗鴉
 */
void ZoomEditor::createToolBar()
{
    QToolBar *toolBar = addToolBar(QStringLiteral("工具"));
    
    // 加入另存新檔按鈕
    toolBar->addAction(saveAction);
    toolBar->addSeparator();  // 分隔線
    
    // 加入選擇顏色按鈕
    toolBar->addAction(colorAction);
    toolBar->addSeparator();
    
    // 加入筆刷大小控制項
    toolBar->addWidget(new QLabel(QStringLiteral("筆刷大小: "), toolBar));
    brushSizeSpinBox = new QSpinBox(toolBar);
    brushSizeSpinBox->setRange(1, 50);   // 設定範圍：1 到 50 像素
    brushSizeSpinBox->setValue(3);       // 預設值：3 像素
    // 當使用者改變數值時，呼叫 setBrushSize 函式
    connect(brushSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ZoomEditor::setBrushSize);
    toolBar->addWidget(brushSizeSpinBox);
    
    toolBar->addSeparator();
    // 加入清除塗鴉按鈕
    toolBar->addAction(clearAction);
}

/**
 * 另存新檔功能
 * 開啟檔案對話框讓使用者選擇儲存位置和檔案格式
 * 支援 PNG、JPEG、BMP 三種格式
 */
void ZoomEditor::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    QStringLiteral("另存新檔"),
                                                    "",
                                                    "PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)");
    if (fileName.isEmpty())
        return;  // 使用者取消儲存
    
    // 嘗試儲存圖片
    if (imageLabel->getDrawingImage().save(fileName)) {
        // 儲存成功，顯示成功訊息
        QMessageBox::information(this, QStringLiteral("成功"),
                                QStringLiteral("圖片已成功儲存！"));
    } else {
        // 儲存失敗，顯示錯誤訊息
        QMessageBox::warning(this, QStringLiteral("錯誤"),
                            QStringLiteral("無法儲存圖片！"));
    }
}

/**
 * 選擇畫筆顏色
 * 開啟顏色選擇對話框，讓使用者選擇想要的畫筆顏色
 */
void ZoomEditor::chooseBrushColor()
{
    // 顯示顏色選擇對話框，預設顏色為目前的畫筆顏色
    QColor color = QColorDialog::getColor(imageLabel->getBrushColor(), 
                                         this, QStringLiteral("選擇畫筆顏色"));
    if (color.isValid()) {
        // 使用者選擇了有效的顏色，設定新的畫筆顏色
        imageLabel->setBrushColor(color);
    }
}

/**
 * 設定畫筆大小
 * 當使用者在 SpinBox 中改變數值時被呼叫
 */
void ZoomEditor::setBrushSize(int size)
{
    imageLabel->setBrushSize(size);
}

/**
 * 清除所有繪圖
 * 移除所有使用者繪製的內容，恢復到原始的放大圖片
 */
void ZoomEditor::clearDrawing()
{
    imageLabel->clearDrawing();
}

