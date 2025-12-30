 #include "ip.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QStatusBar>
#include <QInputDialog>
#include <QRubberBand>

ip::ip(QWidget *parent)
    : QMainWindow(parent),
      zoomSelectionMode(false),  // 初始化：區域選取模式預設為關閉
      rubberBand(nullptr)        // 初始化：選取框指標設為 nullptr
{
    statusLabel = new QLabel;
    statusLabel->setText (QStringLiteral("指標位置"));
    statusLabel->setFixedWidth (100);
    mousePosLabel = new QLabel;
    mousePosLabel->setText(tr(" "));
    mousePosLabel->setFixedWidth (100);
    statusBar()->addPermanentWidget (statusLabel);
    statusBar()->addPermanentWidget (mousePosLabel);
    setMouseTracking (true);

    setWindowTitle (QStringLiteral("影像處理"));
    central =new QWidget();
    central->setMouseTracking (true);
    QHBoxLayout *mainLayout = new QHBoxLayout (central);
    imgWin = new QLabel();
    imgWin->setMouseTracking (true);
    QPixmap *initPixmap = new QPixmap(300,200);
    gWin =new gtransform();
    initPixmap->fill (QColor(255,255,255));
    imgWin->resize (300,200);
    imgWin->setScaledContents (true);
    imgWin->setPixmap (*initPixmap);
    mainLayout->addWidget(imgWin);
    setCentralWidget (central);
    createActions();
    createMenus();
    createToolBars();
}


ip::~ip()
{
}

void ip::createActions()
{
    openFileAction = new QAction (QStringLiteral("開啟檔案&O"),this);
    openFileAction->setShortcut (tr("Ctrl+o"));
    openFileAction->setStatusTip (QStringLiteral("開啟影像檔案"));
    connect (openFileAction, SIGNAL (triggered()), this, SLOT (showOpenFile()));

    exitAction = new QAction (QStringLiteral("結束&Q"),this);
    exitAction->setShortcut (tr("Ctrl+o"));
    exitAction->setStatusTip (QStringLiteral("退出程式"));
    connect (exitAction, SIGNAL (triggered()), this, SLOT (close()));

    bigFileAction = new QAction (QStringLiteral("放大&+"),this);
    bigFileAction->setShortcut (tr("Ctrl+"));
    bigFileAction->setStatusTip (QStringLiteral("放大"));
    connect (bigFileAction, SIGNAL (triggered()), this, SLOT (bigsize()));

    sAction = new QAction (QStringLiteral("縮小&-"),this);
    sAction->setShortcut (tr("Ctrl-"));
    sAction->setStatusTip (QStringLiteral("縮小"));
    connect (sAction, SIGNAL (triggered()), this, SLOT (ssize()));

    geometryAction = new QAction (QStringLiteral("幾何轉換"),this);
    geometryAction->setShortcut (tr("Ctrl+G"));
    geometryAction->setStatusTip (QStringLiteral("影像幾何轉換"));
    connect (geometryAction, SIGNAL (triggered()), this, SLOT (showGeometryTransform()));
    connect (exitAction, SIGNAL (triggered()),gWin, SLOT (close()));

    // 建立區域放大功能的動作
    // 此功能允許使用者拖曳選取圖片的特定區域，然後以自訂倍率放大
    zoomSelectionAction = new QAction (QStringLiteral("區域放大"),this);
    zoomSelectionAction->setShortcut (tr("Ctrl+Z"));
    zoomSelectionAction->setStatusTip (QStringLiteral("拖曳選取區域進行放大"));
    connect (zoomSelectionAction, SIGNAL (triggered()), this, SLOT (enableZoomSelection()));
}
void ip::createMenus()
{
    fileMenu = menuBar ()->addMenu (QStringLiteral ("檔案&F"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction (exitAction);

    fileMenu = menuBar ()->addMenu (QStringLiteral ("工具&T"));
    fileMenu->addAction(bigFileAction);
    fileMenu->addAction (sAction);
    fileMenu->addAction (zoomSelectionAction);  // 將區域放大加入工具選單
    fileMenu->addAction (geometryAction);
}
void ip::createToolBars ()
{
    fileTool = addToolBar("file");
    fileTool->addAction (openFileAction);
    fileTool = addToolBar("file");
    fileTool->addAction (bigFileAction);
    fileTool->addAction (sAction);
    fileTool->addAction (zoomSelectionAction);  // 將區域放大加入工具列
    fileTool->addAction (geometryAction);
}
void ip::loadFile (QString filename)
{
    qDebug() <<QString("file name: %1").arg(filename);
    QByteArray ba=filename.toLatin1();
    printf("FN:%s\n", (char *) ba.data());
    img.load(filename);
    imgWin->setPixmap (QPixmap::fromImage(img));
}
void ip::showOpenFile()
{
    filename = QFileDialog::getOpenFileName(this,
                                            QStringLiteral("開啟影像"),
                                            tr("."),
                                            "bmp(*.bmp);;png(*.png)"
                                            ";;Jpeg(*.jpg)");
    if (!filename.isEmpty())
    {
        if (img.isNull())
        {
            loadFile(filename);
        }
        else
        {
            ip *newIPWin
                = new ip();
            newIPWin->show();
            newIPWin->loadFile(filename);
        }
    }
}

void ip::bigsize()
{
    QImage bigsize;
    bigsize =img.scaled(img.width()*2,img.width()*2);
    QLabel *ret=new QLabel();
    ret->setPixmap(QPixmap::fromImage(bigsize));
    ret->setWindowTitle(tr("放大結果"));
    ret->show();
}

void ip::ssize()
{
    QImage ssize;
    ssize =img.scaled(img.width()/2,img.width()/2);
    QLabel *ret=new QLabel();
    ret->setPixmap(QPixmap::fromImage(ssize));
    ret->setWindowTitle(tr("縮小結果"));
    ret->show();
}

void ip:: showGeometryTransform()
{
    if (!img.isNull())
    gWin->srcImg = img;
    gWin->inWin->setPixmap (QPixmap:: fromImage (gWin->srcImg));
    gWin->show();
}


void ip::enableZoomSelection()
{
    if (img.isNull()) {
        statusBar()->showMessage(QStringLiteral("請先開啟圖片！"), 3000);
        return;
    }
    zoomSelectionMode = !zoomSelectionMode;
    if (zoomSelectionMode) {
        statusBar()->showMessage(QStringLiteral("區域放大模式已啟用，請拖曳滑鼠選取區域"), 3000);
        setCursor(Qt::CrossCursor);
    } else {
        statusBar()->showMessage(QStringLiteral("區域放大模式已關閉"), 3000);
        setCursor(Qt::ArrowCursor);
        if (rubberBand) {
            rubberBand->hide();
        }
    }
}

void ip::mouseMoveEvent (QMouseEvent *event)
{
    int x=event->x();
    int y=event->y();
    QString str = "("+ QString::number(x) + "," +
                  QString::number (y) +")";
    if (!img.isNull() && x >= 0 && x < img.width() && y >= 0 && y < img.height())
    {
        QColor color = img.pixelColor(x, y);
        int grayValue = (color.red() + color.green() + color.blue()) / 3;
        str += " = " + QString::number(grayValue);
    }
    mousePosLabel->setText(str);

    // 區域選取功能：更新選取框的大小
    // 當啟用區域選取模式且使用者正在拖曳時，即時更新 QRubberBand 的範圍
    if (zoomSelectionMode && rubberBand && !selectionStart.isNull()) {
        rubberBand->setGeometry(QRect(selectionStart, event->pos()).normalized());
    }
}
void ip::mousePressEvent (QMouseEvent *event)
{
    QString str = "("+ QString::number (event->x()) + "," +
                  QString::number (event->y()) +")";
    
    // 區域選取功能：開始選取
    // 當啟用區域選取模式且按下左鍵時，記錄起始點並建立/顯示選取框
    if (zoomSelectionMode && event->button() == Qt::LeftButton) {
        selectionStart = event->pos();  // 記錄選取起始點
        if (!rubberBand) {
            // 若選取框尚未建立，則建立一個新的矩形選取框
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        }
        rubberBand->setGeometry(QRect(selectionStart, QSize()));
        rubberBand->show();  // 顯示選取框
        statusBar()->showMessage(QStringLiteral("選取中..."));
        return;  // 直接返回，不執行下方的一般滑鼠事件處理
    }
    
    if (event->button() == Qt::LeftButton)
    {
        statusBar()->showMessage (QStringLiteral("左鍵:")+str);
    }
    else if (event->button()== Qt::RightButton)
    {
        statusBar()->showMessage (QStringLiteral("右鍵:")+str);
    }
    else if (event->button() == Qt::MiddleButton)
    {
        statusBar()->showMessage (QStringLiteral("中鍵:")+str);
    }
}

void ip::mouseReleaseEvent (QMouseEvent *event)
{
    QString str = "(" + QString::number (event->x()) + "," +
                  QString::number (event->y()) +")";
    
    if (zoomSelectionMode && event->button() == Qt::LeftButton && rubberBand) {
        selectionEnd = event->pos();  // 記錄選取結束點
        rubberBand->hide();           // 隱藏選取框
        
        // 建立選取矩形並標準化
        QRect selectionRect = QRect(selectionStart, selectionEnd).normalized();
        
        // 驗證選取區域大小是否足夠
        if (selectionRect.width() < 10 || selectionRect.height() < 10) {
            statusBar()->showMessage(QStringLiteral("選取區域太小！"), 3000);
            return;
        }
        
        // 取得圖片顯示標籤的位置，用於座標轉換
        QPoint labelPos = imgWin->pos();
        QRect imageRect = QRect(labelPos.x(), labelPos.y(), imgWin->width(), imgWin->height());
        
        // 檢查選取範圍是否與圖片區域有交集
        if (!imageRect.intersects(selectionRect)) {
            statusBar()->showMessage(QStringLiteral("請在圖片區域內選取！"), 3000);
            return;
        }
        
        // 將選取座標轉換為相對於圖片的座標
        int imgX = selectionRect.x() - labelPos.x();
        int imgY = selectionRect.y() - labelPos.y();
        int imgW = selectionRect.width();
        int imgH = selectionRect.height();
        
        // 計算縮放比例
        // 因為 imgWin 可能會縮放顯示圖片，所以需要將座標對應回實際圖片
        double scaleX = (double)img.width() / imgWin->width();
        double scaleY = (double)img.height() / imgWin->height();
        
        // 將座標和大小轉換為實際圖片的座標系統
        imgX = (int)(imgX * scaleX);
        imgY = (int)(imgY * scaleY);
        imgW = (int)(imgW * scaleX);
        imgH = (int)(imgH * scaleY);
        
        // 限制座標在圖片邊界內，避免超出範圍
        imgX = qMax(0, qMin(imgX, img.width() - 1));
        imgY = qMax(0, qMin(imgY, img.height() - 1));
        imgW = qMin(imgW, img.width() - imgX);
        imgH = qMin(imgH, img.height() - imgY);
        
        // 從原圖中擷取選取的區域
        QImage selectedRegion = img.copy(imgX, imgY, imgW, imgH);
        
        // 彈出對話框讓使用者輸入放大倍率（1.0 到 10.0 倍）
        bool ok;
        double zoomFactor = QInputDialog::getDouble(this,
                                                    QStringLiteral("放大倍率"),
                                                    QStringLiteral("請輸入放大倍率 (1.0 - 10.0):"),
                                                    2.0, 1.0, 10.0, 1, &ok);
        
        // 如果使用者確認且選取區域有效，則進行放大並開啟編輯視窗
        if (ok && !selectedRegion.isNull()) {
            // 計算放大後的新尺寸
            int newWidth = (int)(selectedRegion.width() * zoomFactor);
            int newHeight = (int)(selectedRegion.height() * zoomFactor);
            
            // 使用高品質的平滑轉換演算法進行放大
            // Qt::KeepAspectRatio 保持長寬比例
            // Qt::SmoothTransformation 使用平滑演算法以獲得較好的放大品質
            QImage zoomedImage = selectedRegion.scaled(newWidth, newHeight, 
                                                       Qt::KeepAspectRatio, 
                                                       Qt::SmoothTransformation);
            
            // 建立並開啟放大編輯視窗
            // Qt::WA_DeleteOnClose 屬性確保視窗關閉時自動釋放記憶體
            ZoomEditor *zoomEditor = new ZoomEditor(zoomedImage);
            zoomEditor->setAttribute(Qt::WA_DeleteOnClose);
            zoomEditor->show();
            
            statusBar()->showMessage(QStringLiteral("已開啟放大編輯視窗"), 3000);
        }
        
        // 完成後重置選取模式和游標
        zoomSelectionMode = false;
        setCursor(Qt::ArrowCursor);
        return;
    }
    
    statusBar ()->showMessage (QStringLiteral("釋放:")+str);
}
