#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include <QWidget>

namespace Ui {
class Recommendations;
}

class Recommendations : public QWidget
{
    Q_OBJECT

public:
    explicit Recommendations(QWidget *parent = nullptr);
    ~Recommendations();

private:
    Ui::Recommendations *ui;
};

#endif // RECOMMENDATIONS_H
