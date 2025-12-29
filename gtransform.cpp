#include "gtransform.h"
#include <QPixmap>
#include <QPainter>
#include<QFileDialog>
gtransform::gtransform(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QHBoxLayout (this);
    leftLayout = new QVBoxLayout (this);
    mirrorGroup = new QGroupBox (QStringLiteral("鏡射"),this);
    groupLayout = new QVBoxLayout (mirrorGroup);

    hCheckBox = new QCheckBox (QStringLiteral ("水平"), mirrorGroup);
    vCheckBox = new QCheckBox (QStringLiteral("垂直"), mirrorGroup);
    mirrorButton = new QPushButton (QStringLiteral("執行"), mirrorGroup);
    saveButton = new QPushButton (QStringLiteral("存檔"), mirrorGroup);

    hCheckBox->setGeometry (QRect(13, 28, 87, 19));
    vCheckBox->setGeometry (QRect (13, 54, 87, 19));
    mirrorButton->setGeometry (QRect(13, 80, 93, 28));
    saveButton->setGeometry (QRect(13, 90, 93, 28));

    groupLayout->addWidget (hCheckBox);
    groupLayout->addWidget (vCheckBox);
    groupLayout->addWidget (mirrorButton);
    groupLayout->addWidget (saveButton);
    leftLayout->addWidget (mirrorGroup);

    rotateDial = new QDial (this);
    rotateDial->setNotchesVisible(true);
    rotateDial->setRange(0, 360);
    rotateDial->setWrapping(true);

    vSpacer = new QSpacerItem (20, 58, QSizePolicy:: Minimum,
                              QSizePolicy:: Expanding);
    leftLayout->addWidget (rotateDial);
    leftLayout->addItem(vSpacer);
    mainLayout->addLayout (leftLayout);

    inWin = new QLabel (this);
    inWin->setScaledContents(true);
    QPixmap *initPixmap= new QPixmap (300,200);
    initPixmap->fill (QColor (255, 255, 255));
    //-----------------------------------------------
    QPainter *paint = new QPainter(initPixmap);
    paint->setPen(*(new QColor(0,0,0)));
    paint->begin(initPixmap);
    paint->drawRect(15,15,60,40);
    paint->end();
    if(srcImg.isNull())
    {
        srcImg=initPixmap->toImage();
    }
    //------------------------------------------------
    inWin->setPixmap(*initPixmap);
    inWin->setSizePolicy (QSizePolicy:: Expanding, QSizePolicy:: Expanding);

    if (srcImg.isNull())
    {
        QPixmap *initPixmap= new QPixmap (300,200);
        initPixmap->fill(QColor(255,255,255));
        inWin->setPixmap (*initPixmap);
    }
    mainLayout->addWidget (inWin);

    connect (mirrorButton, SIGNAL (clicked()), this, SLOT (mirroredImage()));
    connect (saveButton, SIGNAL (clicked()), this, SLOT (saveimage()));
    connect (rotateDial, SIGNAL (valueChanged(int)), this, SLOT (rotatedImage()));
}

gtransform::~gtransform() {

}

void gtransform:: saveimage(){
    QString filepath = QFileDialog::getSaveFileName(this,
                                                    QStringLiteral("存檔"),
                                                    "",
                                                    QStringLiteral("PNG Files (*.png)"));
    if (filepath.isEmpty())
        return;
    if (!dstImg.isNull()) {
        dstImg.save(filepath);
    }
}
void gtransform::mirroredImage ()
{
    bool H,V;
    if (srcImg.isNull())
        return;
    H=hCheckBox->isChecked ();
    V=vCheckBox->isChecked();
    dstImg=srcImg.mirrored (H,V);
    inWin->setPixmap (QPixmap:: fromImage (dstImg));
    srcImg = dstImg;
}
void gtransform::rotatedImage ()
{
    QTransform tran;
    int angle;
    if (srcImg.isNull())
        return;
    angle=rotateDial->value();
    tran.rotate (angle);
    dstImg=srcImg.transformed (tran);
    inWin->setPixmap (QPixmap:: fromImage (dstImg));
}
