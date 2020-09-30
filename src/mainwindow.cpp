#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "ui_about.h"

#include <algorithm>
#include <cmath>
#include <array>
#include <cstdio>
#include <QApplication>
#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QDesktopServices>
#include <QUrl>

using std::array;
using std::clamp;
using std::fill;
using std::make_unique;

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
    constexpr auto EARTH_RADIUS = 6371.0;
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
        (new About{ })->show();
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
    const bool ELEV_IN_RADS = (ui->rad_deg->currentIndex() == 0);
    const bool ALTITUDE_IN_KM = (ui->km_mi->currentIndex() == 0);
    const char *DISTANCE_UNITS = ALTITUDE_IN_KM ? "kilometers" : "miles";
    const auto ELEVATION = ui->elevation->value() * (ELEV_IN_RADS ? 1.0 : RAD_PER_DEG);
    const auto ALTITUDE = ui->altitude->value() * (ALTITUDE_IN_KM ? 1.0 : MI_PER_KM);
    const auto ARCSIN_TERM = asin(cos(ELEVATION) * (EARTH_RADIUS / (EARTH_RADIUS + ALTITUDE)));
    const auto COS_TERM = cos(ELEVATION + ARCSIN_TERM);
    const auto NUMERATOR = (EARTH_RADIUS + ALTITUDE) * COS_TERM;
    const auto DENOMINATOR = cos(ELEVATION);
    const auto SIGHTLINE = (NUMERATOR / DENOMINATOR) * (ALTITUDE_IN_KM ? 1.0 : KM_PER_MI);

    array<char, 100> new_message;
    fill(new_message.begin(), new_message.end(), '\0');

#if _WIN32 || _WIN64
    _snprintf_s(&new_message[0], new_message.size(), _TRUNCATE,
                "Sightline is %.2f %s", SIGHTLINE, DISTANCE_UNITS);
#else
    snprintf(&new_message[0], new_message.size(),
             "Sightline is %.2f %s", SIGHTLINE, DISTANCE_UNITS);
#endif

    ui->statusBar->clearMessage();
    ui->statusBar->showMessage(QString(&new_message[0]));
}
