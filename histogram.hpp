#pragma once
#include <QAbstractListModel>
#include "model.h"
#include <map>
#include <string>
#include "use_case.h"

namespace histogram {

using word_count_pair_t = std::pair<QString, int>;
using top_list_t = QVector<word_count_pair_t>;

class HistogramModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int max READ Max NOTIFY maxChanged)
    Q_PROPERTY(int size READ Size NOTIFY sizeChanged)
    Q_PROPERTY(float proc READ Proc NOTIFY procChanged)

signals:
    void maxChanged();
    void sizeChanged();
    void procChanged();

    void PushData(const top_list_t & str, float proc);
public:

    explicit HistogramModel(domain::ReaderUseCase & reader, QObject *parent = nullptr);

    Q_INVOKABLE void setFile(const QString & path) { return reader_.SetFile(path.toLocal8Bit().data()); }
    Q_INVOKABLE QString start() { return GetStringError(reader_.Start()); }
    Q_INVOKABLE QString pause() { return GetStringError(reader_.Pause()); }
    Q_INVOKABLE QString cancel() { return GetStringError(reader_.Stop()); }

    int Max();
    int Size() { return top_.size(); }
    float Proc() { return proc_; }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    void PushDataImpl(const top_list_t & str, float proc);
    void PushDataToChart();
    QHash<int,QByteArray> roleNames() const override;
private:
    enum class role_t {NAME  = Qt::UserRole, VALUE};

    QString GetStringError(model::MeasureStatus);

    top_list_t top_;
    float proc_;
    int max_;

    domain::ReaderUseCase & reader_;
};

}
