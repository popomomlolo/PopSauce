#ifndef WIDGETPOPSAUCECLIENT_H
#define WIDGETPOPSAUCECLIENT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetPopSauceClient; }
QT_END_NAMESPACE

class WidgetPopSauceClient : public QWidget
{
    Q_OBJECT

public:
    WidgetPopSauceClient(QWidget *parent = nullptr);
    ~WidgetPopSauceClient();

private:
    Ui::WidgetPopSauceClient *ui;
};
#endif // WIDGETPOPSAUCECLIENT_H
