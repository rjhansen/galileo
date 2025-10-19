#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <memory>

namespace Ui {
class About;
}

class About : public QDialog {
    Q_OBJECT

public:
    explicit About(QWidget* parent = nullptr);
    virtual ~About();

private:
    std::unique_ptr<Ui::About> ui;
};

#endif // ABOUT_H
