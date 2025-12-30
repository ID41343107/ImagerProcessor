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

class DrawableLabel : public QLabel
{
    Q_OBJECT
public:
    DrawableLabel(QWidget *parent = nullptr);
    
    // 設定要顯示和編輯的圖片
    void setDrawingImage(const QImage &image);
    
    // 取得目前編輯後的圖片
    QImage getDrawingImage() const { return drawingImage; }
    
    // 設定原始圖片（用於清除時還原）
    void setOriginalImage(const QImage &image) { originalImage = image; }
    
    // 設定畫筆顏色
    void setBrushColor(const QColor &color) { brushColor = color; }
    
    // 取得目前的畫筆顏色
    QColor getBrushColor() const { return brushColor; }
    
    // 設定畫筆大小
    void setBrushSize(int size) { brushSize = size; }
    
    // 清除所有繪圖，恢復到原始圖片
    void clearDrawing();

protected:
    // 寫滑鼠處理函式，實現繪圖功能
    void mousePressEvent(QMouseEvent *event) override;   // 開始繪圖
    void mouseMoveEvent(QMouseEvent *event) override;    // 持續繪圖
    void mouseReleaseEvent(QMouseEvent *event) override; // 結束繪圖

private:
    // 在圖片上畫線到指定點
    void drawLineTo(const QPoint &endPoint);
    
    QImage originalImage;  // 原始圖片，用於清除繪圖時還原
    QImage drawingImage;   // 目前正在編輯的圖片
    bool isDrawing;        // 是否正在繪圖中
    QPoint lastPoint;      // 上一個繪圖點的座標
    QColor brushColor;     // 畫筆顏色
    int brushSize;         // 畫筆大小
};

class ZoomEditor : public QMainWindow
{
    Q_OBJECT

public:
    ZoomEditor(const QImage &image, QWidget *parent = nullptr);
    ~ZoomEditor();

private slots:
    // 將編輯後另存新檔
    void saveImage();
    
    // 開啟顏色選擇對話框，選擇顏色
    void chooseBrushColor();
    
    // 設定畫筆大小
    void setBrushSize(int size);
    
    // 清除所有圖，恢復到原始放大圖片
    void clearDrawing();

private:
    // 建立選單動作
    void createActions();
    
    // 建立工具列並加入控制項
    void createToolBar();

    DrawableLabel *imageLabel;  // 用於顯示和編輯圖片的自訂標籤
    
    // 工具列
    QAction *saveAction;    // 另存新檔
    QAction *colorAction;   // 選擇顏色
    QAction *clearAction;   // 清除繪圖
    QSpinBox *brushSizeSpinBox;  // 畫筆大小選擇
};

#endif // ZOOMEDITOR_H
