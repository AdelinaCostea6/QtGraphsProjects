#include "CostDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

CostDialog::CostDialog(QWidget *parent)
    : QDialog(parent),
    m_costLineEdit(new QLineEdit(this)),
    m_acceptButton(new QPushButton("Accept", this))
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Introduceți costul:", this));
    layout->addWidget(m_costLineEdit);
    layout->addWidget(m_acceptButton);

    setLayout(layout);

    connect(m_acceptButton, &QPushButton::clicked, this, &CostDialog::onAcceptButtonClicked);
}

int CostDialog::getCost() const
{
    bool ok;
    int cost = m_costLineEdit->text().toInt(&ok);
    if (!ok) {
        return -1;
    }
    return cost;
}

void CostDialog::onAcceptButtonClicked()
{
    int cost = getCost();
    if (cost != -1) {
        accept();
    }
}
