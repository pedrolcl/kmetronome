#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "helpwindow.h"
#include "iconutils.h"

HelpWindow::HelpWindow(const QString &path, const QString &page, QWidget *parent):
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_GroupLeader);
    IconUtils::SetWindowIcon(this);

    textBrowser = new QTextBrowser(this);
    homeButton = new QPushButton(this); //tr("&Home"));
    homeButton->setIcon(QIcon::fromTheme("go-home"));
    backButton = new QPushButton(this); //tr("&Back"));
    backButton->setIcon(QIcon::fromTheme("go-previous"));
    closeButton = new QPushButton(this); //tr("Close"));
    closeButton->setShortcut(tr("Esc"));
    closeButton->setIcon(QIcon::fromTheme("window-close"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(homeButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(textBrowser);
    setLayout(mainLayout);

    connect(homeButton, SIGNAL(clicked()), textBrowser, SLOT(home()));
    connect(backButton, SIGNAL(clicked()), textBrowser, SLOT(backward()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(textBrowser, SIGNAL(sourceChanged(const QUrl &)), this, SLOT(updateWindowTitle()));

    QPalette p = textBrowser->palette();
    p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::Text, Qt::black);
    textBrowser->setPalette(p);

    textBrowser->setSearchPaths(QStringList() << path << ":/help");
    textBrowser->setSource(page);
    textBrowser->setOpenExternalLinks(true);
}

void HelpWindow::updateWindowTitle()
{
    setWindowTitle(tr("Help: %1").arg(textBrowser->documentTitle()));
}

void HelpWindow::showPage(const QString &page)
{
    HelpWindow *browser = new HelpWindow(QLatin1String(":/"), page);
    browser->resize(500, 400);
    browser->show();
}
