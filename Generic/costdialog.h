#ifndef COSTDIALOG_H
#define COSTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class CostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CostDialog(QWidget *parent = nullptr);

    int getCost() const;

private slots:
    void onAcceptButtonClicked();

private:
    QLineEdit *m_costLineEdit;
    QPushButton *m_acceptButton;
};

#endif // COSTDIALOG_H
