#include <QApplication>
#include <QPainter>
#include "iconutils.h"

namespace IconUtils
{
    void PaintPixmap(QPixmap &pixmap, const QColor& color)
    {
        QPainter painter(&pixmap);
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.fillRect(pixmap.rect(), color);
    }

    QPixmap GetPixmap(QWidget* widget, const QString& fileName)
    {
        QPixmap pixmap(fileName);
        QColor color = widget->palette().color(QPalette::Active, QPalette::Foreground);
        PaintPixmap(pixmap, color);
        return pixmap;
    }

    void SetLabelIcon(QLabel *label, const QString& fileName)
    {
        label->setPixmap(GetPixmap(label, fileName));
    }

    void SetupComboFigures(QComboBox *combo)
    {
        QList<QPair<QString,QString>> elements = {
            {QApplication::tr("Whole"),         ":/icons/1.png" },
            {QApplication::tr("Half"),          ":/icons/2.png" },
            {QApplication::tr("Quarter"),       ":/icons/4.png" },
            {QApplication::tr("Eight"),         ":/icons/8.png" },
            {QApplication::tr("Sixteenth"),     ":/icons/16.png" },
            {QApplication::tr("Thirty-Second"), ":/icons/32.png" },
            {QApplication::tr("Sixty-Fourth"),  ":/icons/64.png" }
        };
        for(auto p : elements) {
            combo->addItem(QIcon(GetPixmap(combo, p.second)), p.first);
        }
    }

    void SetWindowIcon(QWidget *widget)
    {
        widget->setWindowIcon(QIcon(GetPixmap(widget, ":/icons/midi/icon32.png")));
    }
}
