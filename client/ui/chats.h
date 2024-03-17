#ifndef CHATS_H
#define CHATS_H

#include <QWidget>

namespace Ui {
class Chats;
}

class Chats : public QWidget
{
    Q_OBJECT

public:
    explicit Chats(QWidget *parent = nullptr);
    ~Chats();

private:
    Ui::Chats *ui;
};

#endif // CHATS_H
