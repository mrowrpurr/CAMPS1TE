#pragma once

#include <QElapsedTimer>
#include <QGraphicsView>
#include <QScrollBar>
#include <QTimer>
#include <QTouchEvent>

#include "MapCellGraphicsRectItem.h"

namespace Camps1te::UI::Impl {

    class MapGraphicsView : public QGraphicsView {
        qreal   _totalScaleFactor = 1;
        qreal   _lastScaleFactor  = 1;
        bool    _mousePressed     = false;
        QPointF _lastMousePos;

        constexpr static auto _recentTouchInterval = 1000;
        QTimer*               _recentTouchTimer    = nullptr;
        QPointF               _recentTouchPos;
        QPointF               _currentTouchPos;

    public:
        MapGraphicsView() {
            setRenderHint(QPainter::Antialiasing);
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setAttribute(Qt::WA_AcceptTouchEvents);
            _recentTouchTimer = new QTimer(this);
            connect(_recentTouchTimer, &QTimer::timeout, [this]() {
                this->_recentTouchPos = this->_currentTouchPos;
            });
            _recentTouchTimer->start(_recentTouchInterval);
        }

    protected:
        void wheelEvent(QWheelEvent* event) override {
            setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            double scaleFactor = 1.15;
            if (event->angleDelta().y() > 0) {
                // Zoom in
                scale(scaleFactor, scaleFactor);
            } else {
                // Zooming out
                scale(1 / scaleFactor, 1 / scaleFactor);
            }
        }

        void mousePressEvent(QMouseEvent* event) override {
            if (event->button() == Qt::RightButton) {
                _mousePressed = true;
                _lastMousePos = event->globalPosition();
            } else {
                QGraphicsView::mousePressEvent(event);
            }
        }

        void mouseMoveEvent(QMouseEvent* event) override {
            if (_mousePressed) {
                QPointF delta    = event->globalPosition() - _lastMousePos;
                _lastMousePos    = event->globalPosition();
                QScrollBar* hBar = horizontalScrollBar();
                QScrollBar* vBar = verticalScrollBar();
                hBar->setValue(hBar->value() - delta.x());
                vBar->setValue(vBar->value() - delta.y());
            }
        }

        void mouseReleaseEvent(QMouseEvent* event) override {
            if (event->button() == Qt::RightButton) {
                _mousePressed = false;
            }
        }

        QElapsedTimer _touchTimer;

        bool viewportEvent(QEvent* event) override {
            switch (event->type()) {
                case QEvent::TouchBegin: {
                    QTouchEvent*                   touchEvent  = static_cast<QTouchEvent*>(event);
                    QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->points();
                    _touchTimer.restart();
                    _recentTouchPos = _currentTouchPos = touchPoints.first().pressPosition();
                }
                case QEvent::TouchEnd: {
                    QTouchEvent*                   touchEvent  = static_cast<QTouchEvent*>(event);
                    QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->points();
                    if (touchPoints.count() == 1) {
                        if (_touchTimer.elapsed() > 1 && _touchTimer.elapsed() < 250) {
                            QPointF        pos  = touchEvent->points().first().position();
                            QGraphicsItem* item = this->itemAt(pos.toPoint());
                            if (MapCellGraphicsRectItem* rectItem =
                                    qgraphicsitem_cast<MapCellGraphicsRectItem*>(item)) {
                                rectItem->Click();
                            }
                        }
                    }
                }
                case QEvent::TouchUpdate: {
                    QTouchEvent*                   touchEvent  = static_cast<QTouchEvent*>(event);
                    QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->points();
                    _currentTouchPos                           = touchPoints.first().position();

                    if (touchPoints.count() == 2) {
                        // Determine scale factor
                        const QTouchEvent::TouchPoint& touchPoint0 = touchPoints.first();
                        const QTouchEvent::TouchPoint& touchPoint1 = touchPoints.last();

                        qreal currentScaleFactor =
                            QLineF(touchPoint0.position(), touchPoint1.position()).length() /
                            QLineF(touchPoint0.pressPosition(), touchPoint1.pressPosition())
                                .length();
                        if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                            // If one of the fingers is released, remember the last scale factor so
                            // that adding another finger later will continue zooming by adding new
                            // scale factor to the existing remembered value.
                            _lastScaleFactor = _totalScaleFactor;
                            _totalScaleFactor *= currentScaleFactor;
                        } else {
                            setTransformationAnchor(QGraphicsView::NoAnchor);
                            setTransform(QTransform::fromScale(
                                _totalScaleFactor * currentScaleFactor,
                                _totalScaleFactor * currentScaleFactor
                            ));
                        }
                    } else if (touchPoints.count() == 1) {
                        // Pan
                        if (touchEvent->touchPointStates() & Qt::TouchPointMoved) {
                            QScrollBar* hBar  = horizontalScrollBar();
                            QScrollBar* vBar  = verticalScrollBar();
                            QPointF     delta = touchPoints.first().position() - _recentTouchPos;
                            // QPointF     delta =
                            //     touchPoints.first().position() -
                            //     touchPoints.first().pressPosition();

                            qreal scaleFactor = transform().m11();

                            // Change this value to adjust the panning speed
                            qreal speedAdjustment = 0.1;

                            hBar->setValue(
                                hBar->value() + speedAdjustment * (delta.x() > 0 ? -1 : 1) *
                                                    qAbs(delta.x()) / scaleFactor
                            );
                            vBar->setValue(
                                vBar->value() + speedAdjustment * (delta.y() > 0 ? -1 : 1) *
                                                    qAbs(delta.y()) / scaleFactor
                            );
                        }
                    }
                    return true;
                }
                default:
                    return QGraphicsView::viewportEvent(event);
            }
        }
    };
}
