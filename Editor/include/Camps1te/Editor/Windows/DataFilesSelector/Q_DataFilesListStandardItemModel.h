#pragma once

#include <QStandardItem>
#include <QTimer>

class DataFilesListStandardItemModel : public QStandardItemModel {
    Q_OBJECT

    bool _isPerformingReorder = false;

public:
    DataFilesListStandardItemModel(QObject* parent = nullptr) : QStandardItemModel(parent) {
        setColumnCount(2);
        setHorizontalHeaderLabels({"Data File", "Load Order", "Is Active"});
    }

    bool IsPerformingReorder() const { return _isPerformingReorder; }

    Qt::ItemFlags flags(const QModelIndex& index) const override {
        Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);
        if (index.isValid()) {
            return defaultFlags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
        } else {
            return defaultFlags | Qt::ItemIsDropEnabled;
        }
    }

    Qt::DropActions supportedDropActions() const override {
        return Qt::CopyAction | Qt::MoveAction;
    }

    bool dropMimeData(
        const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent
    ) override {
        _isPerformingReorder = true;
        if (parent.isValid()) {
            // Reject drops onto other items
            return false;
        }

        bool success = QStandardItemModel::dropMimeData(data, action, row, column, parent);
        if (success) {
            // Delay the order update to ensure the row numbers are updated
            QTimer::singleShot(0, this, &DataFilesListStandardItemModel::updateOrder);
        } else {
            _isPerformingReorder = false;
        }
        return success;
    }

public slots:
    void updateOrder() {
        for (int i = 0; i < rowCount(); ++i) {
            qDebug() << "Setting row" << i << "to" << i;
            item(i, 1)->setData(i, Qt::DisplayRole);
        }
        _isPerformingReorder = false;
    }

    void setActive(int row, bool isActive = true) {
        for (int i = 0; i < rowCount(); ++i) {
            if (i == row) {
                auto isAlreadyActive = item(i, 2)->data(Qt::DisplayRole).toBool();
                if (isAlreadyActive && isActive)
                    isActive = false;  // Disable it, if it was already active
                if (isActive && item(i, 0)->data(Qt::CheckStateRole) != Qt::Checked)
                    item(i, 0)->setData(Qt::Checked, Qt::CheckStateRole);
                item(i, 2)->setData(isActive, Qt::DisplayRole);
            } else item(i, 2)->setData(false, Qt::DisplayRole);
        }
    }
};