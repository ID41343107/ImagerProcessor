#ifndef GTRANSFORM_H
#define GTRANSFORM_H

#include <QWidget>
#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDial>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QImage>

class gtransform : public QWidget
{
    Q_OBJECT

public:
    gtransform(QWidget *parent = nullptr);
    ~gtransform();
    QLabel*inWin;
    QGroupBox *mirrorGroup;
    QCheckBox *hCheckBox;
    QCheckBox *vCheckBox;
    QPushButton *mirrorButton;
    QPushButton *saveButton;
    QDial *rotateDial;
    QSpacerItem *vSpacer;
    QHBoxLayout *mainLayout;
    QVBoxLayout *groupLayout;
    QVBoxLayout *leftLayout;
    QImage srcImg;
    QImage dstImg;

private slots:
    void mirroredImage();
    void rotatedImage();
    void saveimage();
};
#endif // GTRANSFORM_H
