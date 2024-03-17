#include "recommendations.h"
#include "ui_recommendations.h"
#include "sidebar.h"

Recommendations::Recommendations(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Recommendations)
{
    ui->setupUi(this);
}

Recommendations::~Recommendations()
{
    delete ui;
}
