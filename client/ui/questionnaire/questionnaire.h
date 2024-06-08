#ifndef QUESTIONNAIRE_H
#define QUESTIONNAIRE_H

#include <QCompleter>
#include <QDebug>
#include <QFormLayout>
#include <QGuiApplication>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMap>
#include <QPushButton>
#include <QScreen>
#include <QStringListModel>
#include <QWidget>
#include "manager.h"

namespace Ui {
class Questionnaire;
}

class Questionnaire : public QWidget {
    Q_OBJECT

public:
    explicit Questionnaire(Manager *managerPtr, QWidget *parent = nullptr);
    QStringList searchActors(const QString &input);
    QStringList searchMovies(const QString &input);
    QStringList searchGenres(const QString &input);
    ~Questionnaire();

signals:
    void questionnaireFilled();

private slots:
    void on_pushButton_save_questionnaire_clicked();
    void onQuestionnaireFilled();
    void updateActorsList(const QString &input);
    void updateMoviesList(const QString &input);
    void updateGenresList(const QString &input);
    void onAddActorClicked();
    void onAddMovieClicked();
    void onAddGenreClicked();

private:
    Ui::Questionnaire *ui;
    Manager *manager;
    QLineEdit *lineEditActors;
    QLineEdit *lineEditMovies;
    QLineEdit *lineEditGenres;
    QPushButton *addActorButton;
    QPushButton *addMovieButton;
    QPushButton *addGenreButton;
    QLabel *selectedActorsLabel;
    QLabel *selectedMoviesLabel;
    QLabel *selectedGenresLabel;
    QStringListModel *actors_model;
    QStringListModel *movies_model;
    QStringListModel *genres_model;
    QList<int> selectedActorsIds;
    QList<int> selectedMoviesIds;
    QList<int> selectedMoviesRatings;
    QMap<QString, int> actors_map;
    QMap<QString, int> movies_map;
    QMap<QString, int> genres_map;
};

#endif  // QUESTIONNAIRE_H