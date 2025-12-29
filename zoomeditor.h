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

/**
 * DrawableLabel - 可繪圖的標籤元件
 * 
 * 這是一個自訂的 QLabel 類別，專門用於處理圖片上的繪圖功能。
 * 繼承自 QLabel 並覆寫滑鼠事件來實現自由繪圖。
 * 
 * 主要功能：
 * 1. 顯示放大後的圖片
 * 2. 處理滑鼠事件以實現繪圖功能
 * 3. 支援畫筆顏色和大小的自訂
 * 4. 提供清除繪圖的功能
 */
class DrawableLabel : public QLabel
{
    Q_OBJECT
public:
    DrawableLabel(QWidget *parent = nullptr);
    
    // 設定要顯示和編輯的圖片
    void setDrawingImage(const QImage &image);
    
    // 取得目前編輯後的圖片（包含繪圖內容）
    QImage getDrawingImage() const { return drawingImage; }
    
    // 設定原始圖片（用於清除繪圖時還原）
    void setOriginalImage(const QImage &image) { originalImage = image; }
    
    // 設定畫筆顏色
    void setBrushColor(const QColor &color) { brushColor = color; }
    
    // 取得目前的畫筆顏色
    QColor getBrushColor() const { return brushColor; }
    
    // 設定畫筆大小（像素）
    void setBrushSize(int size) { brushSize = size; }
    
    // 清除所有繪圖，恢復到原始圖片
    void clearDrawing();

protected:
    // 覆寫滑鼠事件處理函式，實現繪圖功能
    void mousePressEvent(QMouseEvent *event) override;   // 開始繪圖
    void mouseMoveEvent(QMouseEvent *event) override;    // 持續繪圖
    void mouseReleaseEvent(QMouseEvent *event) override; // 結束繪圖

private:
    // 在圖片上畫線到指定點
    void drawLineTo(const QPoint &endPoint);
    
    QImage originalImage;  // 原始圖片，用於清除繪圖時還原
    QImage drawingImage;   // 目前正在編輯的圖片（包含繪圖）
    bool isDrawing;        // 是否正在繪圖中
    QPoint lastPoint;      // 上一個繪圖點的座標
    QColor brushColor;     // 畫筆顏色
    int brushSize;         // 畫筆大小（像素）
};

/**
 * ZoomEditor - 放大編輯視窗
 * 
 * 這是一個獨立的視窗，用於顯示放大後的圖片區域並提供編輯功能。
 * 
 * 主要功能：
 * 1. 在可捲動的區域中顯示放大後的圖片
 * 2. 提供畫筆工具讓使用者在圖片上繪圖
 * 3. 支援自訂畫筆顏色和大小
 * 4. 提供另存新檔功能（PNG、JPEG、BMP 格式）
 * 5. 提供清除繪圖功能，可還原到原始放大圖片
 */
class ZoomEditor : public QMainWindow
{
    Q_OBJECT

public:
    ZoomEditor(const QImage &image, QWidget *parent = nullptr);
    ~ZoomEditor();

private slots:
    // 將編輯後的圖片另存新檔
    void saveImage();
    
    // 開啟顏色選擇對話框，讓使用者選擇畫筆顏色
    void chooseBrushColor();
    
    // 設定畫筆大小
    void setBrushSize(int size);
    
    // 清除所有繪圖，恢復到原始放大圖片
    void clearDrawing();

private:
    // 建立選單動作（另存新檔、選擇顏色、清除等）
    void createActions();
    
    // 建立工具列並加入控制項
    void createToolBar();

    DrawableLabel *imageLabel;  // 用於顯示和編輯圖片的自訂標籤
    
    // 工具列動作
    QAction *saveAction;    // 另存新檔動作
    QAction *colorAction;   // 選擇顏色動作
    QAction *clearAction;   // 清除繪圖動作
    QSpinBox *brushSizeSpinBox;  // 畫筆大小選擇器
};

#endif // ZOOMEDITOR_H
