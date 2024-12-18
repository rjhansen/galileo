#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "ui_about.h"
#include "sightlines.h"

#include <string>
#include <format>
#include <algorithm>
#include <QApplication>
#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QDesktopServices>
#include <QUrl>

using std::make_unique;
using std::format;
using std::string;
using std::clamp;

namespace
{
    constexpr auto PI = 3.141592653589793;
    constexpr auto RAD_PER_DEG = (2 * PI) / 360;
    constexpr auto DEG_PER_RAD = 360 / (2 * PI);
    constexpr auto LOW_DEG = 0.1;
    constexpr auto HIGH_DEG = 89.9;
    constexpr auto LOW_RAD = LOW_DEG * RAD_PER_DEG;
    constexpr auto HIGH_RAD = HIGH_DEG * RAD_PER_DEG;
    constexpr auto KM_PER_MI = 1.609344;
    constexpr auto MI_PER_KM = 1 / 1.609344;
    constexpr auto LOW_KM = 100.0;
    constexpr auto LOW_MI = LOW_KM * MI_PER_KM;
    constexpr auto HIGH_KM = 1000.0;
    constexpr auto HIGH_MI = HIGH_KM * MI_PER_KM;

    const QUrl PROJECT_URL = QUrl { "https://rjhansen.github.io/galileo" };
    const QUrl BUGS_URL = QUrl { "https://github.com/rjhansen/galileo/issues" };
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow { parent },
                                          ui { make_unique<Ui::MainWindow>() }
{
    ui->setupUi(this);
    ui->elevation->setMinimum(LOW_RAD);
    ui->elevation->setMaximum(HIGH_RAD);
    ui->elevation->setValue(1.0);
    ui->altitude->setMinimum(LOW_KM);
    ui->altitude->setMaximum(HIGH_KM);
    ui->altitude->setValue(200.00);
    updateSightLines(0.0);

    void (QDoubleSpinBox::*vc_ptr)(double) = &QDoubleSpinBox::valueChanged;
    void (QComboBox::*cic_ptr)(int) = &QComboBox::currentIndexChanged;

    connect(ui->actionAbout, &QAction::triggered, [=]() {
        About* ptr = new About {};
        ptr->setAttribute(Qt::WA_DeleteOnClose);
        ptr->show();
    });
    connect(ui->actionReport, &QAction::triggered, [=]() { QDesktopServices::openUrl(BUGS_URL);});
    connect(ui->actionQuit, &QAction::triggered, [=]() { QApplication::quit(); });
    connect(ui->elevation, vc_ptr, this, &MainWindow::updateSightLines);
    connect(ui->altitude, vc_ptr, this, &MainWindow::updateSightLines);
    connect(ui->rad_deg, cic_ptr, this, &MainWindow::degRadChanged);
    connect(ui->km_mi, cic_ptr, this, &MainWindow::kmMiChanged);
}

MainWindow::~MainWindow()
{
}

void MainWindow::degRadChanged(int new_index)
{
    const auto CURRENT_VALUE = ui->elevation->value();
    if (new_index == 0)
    { // converting from degrees to radians
        const auto NEW_VALUE = clamp(CURRENT_VALUE * RAD_PER_DEG, LOW_RAD, HIGH_RAD);
        ui->elevation->setMinimum(LOW_RAD);
        ui->elevation->setMaximum(HIGH_RAD);
        ui->elevation->setValue(NEW_VALUE);
    }
    else
    { // converting from radians to degrees
        const auto NEW_VALUE = clamp(CURRENT_VALUE * DEG_PER_RAD, LOW_DEG, HIGH_DEG);
        ui->elevation->setMinimum(LOW_DEG);
        ui->elevation->setMaximum(HIGH_DEG);
        ui->elevation->setValue(NEW_VALUE);
    }
    updateSightLines(0.0);
}

void MainWindow::kmMiChanged(int new_index)
{
    const auto CURRENT_VALUE = ui->altitude->value();
    if (new_index == 0)
    { // converting from miles to kilometers
        const auto NEW_VALUE = clamp(CURRENT_VALUE * KM_PER_MI, LOW_KM, HIGH_KM);
        ui->altitude->setMinimum(LOW_KM);
        ui->altitude->setMaximum(HIGH_KM);
        ui->altitude->setValue(NEW_VALUE);
    }
    else
    { // converting from kilometers to miles
        const auto NEW_VALUE = clamp(CURRENT_VALUE * MI_PER_KM, LOW_MI, HIGH_MI);
        ui->altitude->setMinimum(LOW_MI);
        ui->altitude->setMaximum(HIGH_MI);
        ui->altitude->setValue(NEW_VALUE);
    }
    updateSightLines(0.0);
}

void MainWindow::updateSightLines(double)
{
    const bool IS_ELEV_IN_RADS = (ui->rad_deg->currentIndex() == 0);
    const bool IS_ALTITUDE_IN_KM = (ui->km_mi->currentIndex() == 0);
    const char *DISTANCE_UNITS = IS_ALTITUDE_IN_KM ? "kilometers" : "miles";
    const auto ELEVATION = ui->elevation->value() * (IS_ELEV_IN_RADS ? 1.0 : RAD_PER_DEG);
    const auto ALTITUDE = ui->altitude->value() * (IS_ALTITUDE_IN_KM ? 1.0 : MI_PER_KM);
    const auto SIGHTLINE_KM = get_sightline(ELEVATION, ALTITUDE);
    const auto SIGHTLINE_MI = SIGHTLINE_KM * KM_PER_MI;
    const auto SIGHTLINE = IS_ALTITUDE_IN_KM ? SIGHTLINE_KM : SIGHTLINE_MI;
    const string new_message { 
        format("Sightline is {:.2f} {}", SIGHTLINE, DISTANCE_UNITS) 
    };

    ui->statusBar->clearMessage();
    ui->statusBar->showMessage(QString(new_message.c_str()));
}
