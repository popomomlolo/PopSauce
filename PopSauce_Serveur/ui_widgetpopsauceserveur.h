/********************************************************************************
** Form generated from reading UI file 'widgetpopsauceserveur.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGETPOPSAUCESERVEUR_H
#define UI_WIDGETPOPSAUCESERVEUR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WidgetPopSauceServeur
{
public:
    QLabel *labelImage;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonLancer;
    QSpinBox *spinBoxPort;
    QPushButton *pushButton;

    void setupUi(QWidget *WidgetPopSauceServeur)
    {
        if (WidgetPopSauceServeur->objectName().isEmpty())
            WidgetPopSauceServeur->setObjectName("WidgetPopSauceServeur");
        WidgetPopSauceServeur->resize(800, 600);
        labelImage = new QLabel(WidgetPopSauceServeur);
        labelImage->setObjectName("labelImage");
        labelImage->setGeometry(QRect(130, 130, 256, 15));
        layoutWidget = new QWidget(WidgetPopSauceServeur);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(250, 190, 258, 225));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        textEdit = new QTextEdit(layoutWidget);
        textEdit->setObjectName("textEdit");

        verticalLayout->addWidget(textEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButtonLancer = new QPushButton(layoutWidget);
        pushButtonLancer->setObjectName("pushButtonLancer");

        horizontalLayout->addWidget(pushButtonLancer);

        spinBoxPort = new QSpinBox(layoutWidget);
        spinBoxPort->setObjectName("spinBoxPort");
        spinBoxPort->setMaximum(70000);
        spinBoxPort->setValue(8888);

        horizontalLayout->addWidget(spinBoxPort);


        verticalLayout->addLayout(horizontalLayout);

        pushButton = new QPushButton(WidgetPopSauceServeur);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(370, 470, 84, 23));

        retranslateUi(WidgetPopSauceServeur);

        QMetaObject::connectSlotsByName(WidgetPopSauceServeur);
    } // setupUi

    void retranslateUi(QWidget *WidgetPopSauceServeur)
    {
        WidgetPopSauceServeur->setWindowTitle(QCoreApplication::translate("WidgetPopSauceServeur", "WidgetPopSauceServeur", nullptr));
        labelImage->setText(QCoreApplication::translate("WidgetPopSauceServeur", "TextLabel", nullptr));
        pushButtonLancer->setText(QCoreApplication::translate("WidgetPopSauceServeur", "Lancer", nullptr));
        pushButton->setText(QCoreApplication::translate("WidgetPopSauceServeur", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WidgetPopSauceServeur: public Ui_WidgetPopSauceServeur {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGETPOPSAUCESERVEUR_H
