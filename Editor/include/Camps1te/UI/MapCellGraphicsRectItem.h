#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include <QPainter>
#include <functional>
#include <vector>

namespace Camps1te::UI {

    class MapCellGraphicsRectItem : public QGraphicsRectItem {
        std::vector<std::function<void()>> _clickHandlers;

        // Just one image now, but it'll be a vector of layer objects later
        QImage _image;

    public:
        MapCellGraphicsRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr)
            : QGraphicsRectItem(x, y, w, h, parent) {}

        void SetImage(const QImage& image) { _image = image; }

        void OnClick(std::function<void()> handler) { _clickHandlers.push_back(handler); }
        void Click() {
            for (auto& handler : _clickHandlers) handler();
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            Q_UNUSED(option);
            Q_UNUSED(widget);

            QGraphicsRectItem::paint(painter, option, widget);

            // Scale image to rect size
            QImage scaledImage = _image.scaled(rect().size().toSize());

            // Draw scaled image
            painter->drawImage(rect(), scaledImage);
        }

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            if (event->button() == Qt::LeftButton)
                for (auto& handler : _clickHandlers) handler();
            QGraphicsRectItem::mousePressEvent(event);
        }
    };
}
