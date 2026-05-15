#include "waveformwidget.h"

WaveformWidget::WaveformWidget(QWidget *parent) : QWidget{parent} {
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void WaveformWidget::setSamples(AudioDataPtr data) {
    if (!data) return;
    m_samples = *data;
    update();
}

void WaveformWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);

    if (m_samples.isEmpty()) return;

    painter.setPen(Qt::darkGreen);
    painter.drawLine(0, height()/2, width(), height()/2);

    QPainterPath pathL, pathR;
    int halfH = height() / 2;
    int quartH = height() / 4;
    double stepX = static_cast<double>(width()) / (m_samples.size() / 2);

    pathL.moveTo(0, quartH);
    pathR.moveTo(0, 3 * quartH);

    for (int i = 0; i < m_samples.size(); i += 2) {
        double x = (i / 2.0) * stepX;

        // Normalize int16 to -1.0...1.0 and scale to quartH
        double yL = quartH - (m_samples[i] / 32768.0 * quartH);
        pathL.lineTo(x, yL);

        if (i + 1 < m_samples.size()) {
            double yR = (3 * quartH) - (m_samples[i+1] / 32768.0 * quartH);
            pathR.lineTo(x, yR);
        }
    }

    painter.setPen(QPen(Qt::green, 1));
    painter.drawPath(pathL);
    painter.setPen(QPen(Qt::red, 1));
    painter.drawPath(pathR);
}