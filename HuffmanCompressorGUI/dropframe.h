#ifndef DROPFRAME_H
#define DROPFRAME_H

#include <QFrame>
#include <QString>

class DropFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DropFrame(QWidget *parent = nullptr);

signals:
    void fileDropped(const QString &path);
    void dragStarted();
    void dragEnded();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif // DROPFRAME_H