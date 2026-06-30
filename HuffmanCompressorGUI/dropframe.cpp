#include "dropframe.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include <QResizeEvent>

DropFrame::DropFrame(QWidget *parent)
    : QFrame(parent)
{
    setAcceptDrops(true);

    setStyleSheet(
        "DropFrame {"
        "border: 2px solid #707070;"
        "border-radius: 10px;"
        "background-color: transparent;"
        "}"
        );
}

void DropFrame::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasUrls())
        return;

    const QList<QUrl> urls = event->mimeData()->urls();

    if (urls.size() != 1 || !QFileInfo(urls.first().toLocalFile()).isFile())
        return;

    event->acceptProposedAction();

    emit dragStarted();

    setStyleSheet(
        "DropFrame {"
        "border: 3px dashed #3B82F6;"
        "border-radius: 10px;"
        "background-color: rgba(59,130,246,40);"
        "}"
        );
}

void DropFrame::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    QWidget *content = findChild<QWidget*>("contentWidget");

    if (!content)
        return;

    int x = (width() - content->width()) / 2;
    int y = (height() - content->height()) / 2;

    content->move(x, y);


}

void DropFrame::dragLeaveEvent(QDragLeaveEvent *)
{
    setStyleSheet(
        "DropFrame {"
        "border: 2px solid #707070;"
        "border-radius: 10px;"
        "background-color: transparent;"
        "}"
        );

    emit dragEnded();
}

void DropFrame::dropEvent(QDropEvent *event)
{
    setStyleSheet(
        "DropFrame {"
        "border: 2px solid #707070;"
        "border-radius: 10px;"
        "background-color: transparent;"
        "}"
        );

    const QList<QUrl> urls = event->mimeData()->urls();

    if (urls.isEmpty())
        return;

    emit fileDropped(urls.first().toLocalFile());

    event->acceptProposedAction();

    emit dragEnded();
}