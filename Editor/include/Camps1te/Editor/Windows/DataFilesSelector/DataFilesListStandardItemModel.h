#include <QStandardItem>
#include <QTimer>

class DataFilesListStandardItemModel : public QStandardItemModel {
    Q_OBJECT

public:
    DataFilesListStandardItemModel(QObject* parent = nullptr) : QStandardItemModel(parent) {
        setColumnCount(3);
        setHorizontalHeaderLabels({"Select", "Description", "Order"});
    }

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
        if (parent.isValid()) {
            // Reject drops onto other items
            return false;
        }

        bool success = QStandardItemModel::dropMimeData(data, action, row, column, parent);
        if (success) {
            // Delay the order update to ensure the row numbers are updated
            QTimer::singleShot(0, this, &DataFilesListStandardItemModel::updateOrder);
        }
        return success;
    }

public slots:
    void updateOrder() {
        for (int i = 0; i < rowCount(); ++i) {
            item(i, 2)->setData(i, Qt::DisplayRole);  // Use column 2 for "Order"
        }
    }
};