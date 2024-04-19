#include "histogram.hpp"
#include <queue>
#include <cmath>

namespace histogram {

HistogramModel::HistogramModel(domain::ReaderUseCase & reader, QObject *parent) : QAbstractListModel(parent), reader_(reader), proc_(0), max_(0) {
    connect(this, &HistogramModel::PushData,this, &HistogramModel::PushDataImpl, Qt::QueuedConnection);
}

int HistogramModel::Max() {
    return max_;
}

void HistogramModel::PushDataImpl(const top_list_t &str_list, float proc) {

    int before_count = top_.size();

    for(int i = 0; i < str_list.size();i++ ) {
        if(i < before_count) {
            if(top_[i].first != str_list[i].first) {
                top_[i].first = str_list[i].first;
                emit dataChanged(createIndex(i,0),createIndex(i,0),{static_cast<int>(role_t::NAME)});
            }
            if(top_[i].second != str_list[i].second) {
                top_[i].second = str_list[i].second;
                emit dataChanged(createIndex(i,0),createIndex(i,0),{static_cast<int>(role_t::VALUE)});
            }
        } else {
            emit beginInsertRows(QModelIndex(),top_.size(),top_.size());
            top_.push_back(str_list[i]);
            emit endInsertRows();
        }
    }

    if(top_.size() != before_count) {
        emit sizeChanged();
    }

    if(!top_.empty()) {
        if(max_ != top_.front().second) {
            max_ = top_.front().second;
            emit maxChanged();
        }
    }

    if(proc_ != proc) {
        proc_ = proc;
        emit procChanged();
    }
}

int HistogramModel::rowCount(const QModelIndex &parent) const {
    return top_.size();
}

QVariant HistogramModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (-1 < index.row() && index.row() < top_.size()) {
        switch (static_cast<role_t>(role)) {
            case role_t::NAME:
                return top_[index.row()].first;
            case role_t::VALUE:
                return top_[index.row()].second;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> HistogramModel::roleNames() const {
    static QHash<int,QByteArray> roles = {
                {static_cast<int>(role_t::NAME), "name"},
                {static_cast<int>(role_t::VALUE), "value"}
    };
    return roles;
}

QString HistogramModel::GetStringError(model::MeasureStatus status) {
    using ms = model::MeasureStatus;
    switch(status) {
        case ms::AlreadyStarted:
            return tr("Процесс уже запущен");
        case ms::AlreadyStoped:
            return tr("Процесс уже завершен");
        case ms::NotStoped:
            return tr("Процесс еще не завершен");
        case ms::NotStarted:
            return tr("Процесс еще не запущен");
        case ms::Ok:
            return tr("");
        default:
            break;
    }
    return "Незивестная ошибка";
}

}
