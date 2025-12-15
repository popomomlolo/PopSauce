#ifndef WIDGETPOPSAUCESERVEUR_H
#define WIDGETPOPSAUCESERVEUR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetPopSauceServeur; }
QT_END_NAMESPACE

class WidgetPopSauceServeur : public QWidget
{
    Q_OBJECT

public:
    WidgetPopSauceServeur(QWidget *parent = nullptr);
    ~WidgetPopSauceServeur();

private:
    Ui::WidgetPopSauceServeur *ui;
};
#endif // WIDGETPOPSAUCESERVEUR_H
