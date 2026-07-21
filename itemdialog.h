#ifndef ITEMDIALOG_H
#define ITEMDIALOG_H

#include <QString>
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include "util.h"

class REDPANDASTOOLBOX_EXPORT ItemDialog : public QDialog
{
    Q_OBJECT

public:
    ItemDialog(const QStringList& items, QWidget* parent = nullptr)
        : QDialog(parent)
    {
        auto* layout = new QVBoxLayout(this);

        ;

        list = new QListWidget;
        list->addItems(items);

        layout->addWidget(list);

        connect(list, &QListWidget::itemClicked,
                this, [this](QListWidgetItem* item)
                {
                    selected = item->text();
                    accept();
                });
    }

    virtual  ~ItemDialog () {}

    void setTopBarColor(int r, int g, int b) {
        COLORED_TOP_BAR(r, g, b);
    }

    QString value() const { return selected; }

    inline static QString getItem(QWidget* parent, const QString &title, const QStringList &items, QColor topBar, bool* accepted = nullptr) {
        bool ok = false;
        if (accepted == nullptr) accepted = &ok;

        ItemDialog* work = new ItemDialog(items, parent);
        work->setTopBarColor(topBar.red(), topBar.green(), topBar.blue());
        int code = work->exec();
        if (code == QDialog::Accepted) {
            *accepted = true;
        } else *accepted = false;

        QString res = work->value();
        work->deleteLater();
        return res;
    }
private:
    QListWidget* list;
    QString selected;
};

#endif // ITEMDIALOG_H
