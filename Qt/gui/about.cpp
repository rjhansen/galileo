#include "about.h"
#include "ui_about.h"

using std::make_unique;

About::About(QWidget* parent)
    : QDialog { parent }
    , ui { make_unique<Ui::About>() }
{
    ui->setupUi(this);
}

About::~About()
{
}
