#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>
#include <QTextBrowser>
#include <QPushButton>

class HelpWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HelpWindow(const QString &path,
                        const QString &page,
                        QWidget *parent = nullptr);
    static void showPage(const QString &page);

private slots:
    void updateWindowTitle();

private:
    QTextBrowser *textBrowser;
    QPushButton *homeButton;
    QPushButton *backButton;
    QPushButton *closeButton;
};

#endif // HELPWINDOW_H
