#include "widgetpopsauceclient.h"
#include "ui_widgetpopsauceclient.h"

WidgetPopSauceClient::WidgetPopSauceClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetPopSauceClient)
{
    ui->setupUi(this);
}

WidgetPopSauceClient::~WidgetPopSauceClient()
{
    delete ui;
}

//ca marche
// test
