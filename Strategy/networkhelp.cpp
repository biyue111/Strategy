#include "networkhelp.h"
#include "ui_networkhelp.h"

NetworkHelp::NetworkHelp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkHelp){
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),
                this,SLOT(replyFinished(QNetworkReply*)));
    ui->helpTextBrowser->setText("Please wait...");
    manager->get(QNetworkRequest(QUrl("https://github.com/biyue111/Strategy/blob/master/UserGuide/1.0.md#strategy")));
}

NetworkHelp::~NetworkHelp(){
    delete ui;
}

void NetworkHelp::replyFinished(QNetworkReply *reply){
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString all = codec->toUnicode(reply->readAll());
    ui->helpTextBrowser->setText(all);
    QTextCursor cursor = ui->helpTextBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->helpTextBrowser->setTextCursor(cursor);
    ui->helpTextBrowser->find("Strategy", QTextDocument::FindBackward);
    reply->deleteLater();
}
