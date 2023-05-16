#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include <QPainter>
#include <functional>
#include <vector>

namespace Camps1te::UI::Impl {

    class MapCellGraphicsRectItem : public QGraphicsRectItem {
        std::vector<std::function<void()>> _clickHandlers;
        std::vector<QImage>                _images;

    public:
        MapCellGraphicsRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr)
            : QGraphicsRectItem(x, y, w, h, parent) {}

        void AddImage(const QImage& image) { _images.push_back(image); }
        void RemoveImageAt(int index) {
            //
            _images.erase(_images.begin() + index);
            //
        }

        void OnClick(std::function<void()> handler) { _clickHandlers.push_back(handler); }
        void Click() {
            for (auto& handler : _clickHandlers) handler();
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            Q_UNUSED(option);
            Q_UNUSED(widget);

            this->setPen(QPen(Qt::white));
            QGraphicsRectItem::paint(painter, option, widget);

            for (auto& image : _images) {
                QImage scaledImage = image.scaled(rect().size().toSize());
                painter->drawImage(rect(), scaledImage);
            }
        }

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            if (event->button() == Qt::LeftButton)
                for (auto& handler : _clickHandlers) handler();
            QGraphicsRectItem::mousePressEvent(event);
        }
    };
}
