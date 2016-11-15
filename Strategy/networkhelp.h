#ifndef NETWORKHELP_H
#define NETWORKHELP_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class NetworkHelp;
}

class NetworkHelp : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkHelp(QWidget *parent = 0);
    ~NetworkHelp();

private slots:
    void replyFinished(QNetworkReply *);

private:
    QNetworkAccessManager *manager;
    Ui::NetworkHelp *ui;
};

#endif // NETWORKHELP_H
