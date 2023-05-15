#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <functional>
#include <vector>

namespace Camps1te::UI {

    class MapCellGraphicsRectItem : public QGraphicsRectItem {
        std::vector<std::function<void()>> _clickHandlers;

    public:
        MapCellGraphicsRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr)
            : QGraphicsRectItem(x, y, w, h, parent) {}

        void OnClick(std::function<void()> handler) { _clickHandlers.push_back(handler); }
        void Click() {
            for (auto& handler : _clickHandlers) handler();
        }

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            if (event->button() == Qt::LeftButton)
                for (auto& handler : _clickHandlers) handler();
            QGraphicsRectItem::mousePressEvent(event);
        }
    };
}
