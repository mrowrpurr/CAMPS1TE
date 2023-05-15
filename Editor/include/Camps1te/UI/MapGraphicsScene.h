#pragma once

#include <QGraphicsScene>
#include <functional>
#include <vector>

namespace Camps1te::UI {

    class MapGraphicsScene : public QGraphicsScene {
        std::vector<std::function<void(int, int)>> _clickHandlers;

    public:
        MapGraphicsScene(QObject* parent = nullptr) : QGraphicsScene(parent) {}

        void OnClick(std::function<void(int, int)> handler) { _clickHandlers.push_back(handler); }
    };
}
