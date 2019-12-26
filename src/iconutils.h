#ifndef ICONUTILS_H
#define ICONUTILS_H

#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QComboBox>

namespace  IconUtils
{
    void PaintPixmap(QPixmap &pixmap, const QColor& color);
    QPixmap GetPixmap(QWidget* widget, const QString& fileName);
    void SetLabelIcon(QLabel *label, const QString& fileName);
    void SetupComboFigures(QComboBox *combo);
    void SetWindowIcon(QWidget *widget);
}

#endif // ICONUTILS_H
