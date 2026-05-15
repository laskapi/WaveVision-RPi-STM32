#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include "serialworker.h"
#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QPainterPath>

class WaveformWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WaveformWidget(QWidget *parent = nullptr);

    // Metoda do przekazywania danych do wykresu
    void setSamples(AudioDataPtr data);

protected:
    // To jest kluczowe - deklaracja nadpisania funkcji rysującej
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<int16_t> m_samples; // Tu przechowujemy dane
};

#endif // WAVEFORMWIDGET_H