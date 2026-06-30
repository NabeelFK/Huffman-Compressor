#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "compressor.hpp"
#include "utils.hpp"
#include "dropframe.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QLayout>
#include <QIcon>
#include <QSize>
#include <QPainter>
#include <QPixmap>

namespace
{
// SVG icon assets are loaded with whatever color is baked into the
// file (looks like a dark/black stroke), which is nearly invisible
// on a dark theme. QSS can't recolor an already-rendered pixmap, so
// we tint it ourselves at load time using SourceIn compositing
// (keeps the icon's alpha shape, replaces all opaque pixels with
// the requested color).
QPixmap coloredPixmap(const QString &svgPath, const QSize &size, const QColor &color)
{
    QPixmap source = QIcon(svgPath).pixmap(size);

    QPixmap result(source.size());

    // QPixmap::size() returns physical pixel dimensions, which on a
    // HiDPI screen are larger than the logical "size" we asked for
    // (e.g. 27x27 physical for an 18x18 request at 150% scaling).
    // A freshly constructed QPixmap defaults to devicePixelRatio 1.0,
    // so without this line the result is mislabeled as a 27x27
    // logical pixmap -- which is what was making icons look both
    // blurry (Qt rescaling) and clipped (overflowing their 18x18 box).
    result.setDevicePixelRatio(source.devicePixelRatio());

    result.fill(Qt::transparent);

    QPainter painter(&result);
    painter.drawPixmap(0, 0, source);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(result.rect(), color);
    painter.end();

    return result;
}

QIcon coloredIcon(const QString &svgPath, const QSize &size, const QColor &color)
{
    return QIcon(coloredPixmap(svgPath, size, color));
}

// Light grey used for small badge icons sitting on dark pill
// backgrounds (fileIcon / folderIcon / statsIcon / uploadIcon).
const QColor kBadgeIconColor = QColor("#D4D4D8");

// White used for icons that sit on solid blue buttons.
const QColor kButtonIconColor = QColor("#FFFFFF");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Huffman Compressor");

    ui->browseButton->setText("  Choose File");
    ui->browseOutputButton->setText("  Browse");
    ui->compressButton->setText("  Compress");
    ui->decompressButton->setText("  Decompress");

    ui->browseButton->setIcon(
        coloredIcon(":/icons/file.svg", QSize(18, 18), kButtonIconColor)
        );

    ui->browseOutputButton->setIcon(
        coloredIcon(":/icons/folder.svg", QSize(18, 18), kButtonIconColor)
        );

    ui->compressButton->setIcon(
        coloredIcon(":/icons/archive.svg", QSize(18, 18), kButtonIconColor)
        );

    ui->decompressButton->setIcon(
        coloredIcon(":/icons/folder-open.svg", QSize(18, 18), kButtonIconColor)
        );

    ui->uploadIcon->setPixmap(
        coloredPixmap(":/icons/upload.svg", QSize(42, 42), kBadgeIconColor)
        );

    ui->fileIcon->setPixmap(
        coloredPixmap(":/icons/file.svg", QSize(18, 18), kBadgeIconColor)
        );

    ui->statsIcon->setPixmap(
        coloredPixmap(":/icons/chart-column.svg", QSize(18, 18), kBadgeIconColor)
        );

    ui->folderIcon->setPixmap(
        coloredPixmap(":/icons/folder.svg", QSize(18, 18), kBadgeIconColor)
        );

    ui->uploadIcon->setAlignment(Qt::AlignCenter);

    ui->browseButton->setIconSize(QSize(18, 18));
    ui->browseOutputButton->setIconSize(QSize(18, 18));
    ui->compressButton->setIconSize(QSize(18, 18));
    ui->decompressButton->setIconSize(QSize(18, 18));

    DropFrame *dropFrame = qobject_cast<DropFrame*>(ui->dropFrame);

    connect(
        dropFrame,
        &DropFrame::fileDropped,
        this,
        [this](const QString &path)
        {
            setSelectedFile(path);

            if (!ui->checkBox->isChecked())
                ui->outputFolderPathLabel->setText("Same as input folder");
        }
        );

    connect(
        dropFrame,
        &DropFrame::dragStarted,
        this,
        [this]()
        {
            ui->dropTitle->setText("Release to open");
            ui->dropSubtitle->setText("Drop anywhere inside this box");
        }
        );

    connect(
        dropFrame,
        &DropFrame::dragEnded,
        this,
        [this]()
        {
            if (inputFilePath.isEmpty())
            {
                ui->dropTitle->setText("Drop a file here");
                ui->dropSubtitle->setText("or choose a file below");
            }
            else
            {
                setSelectedFile(inputFilePath);
            }
        }
        );

    ui->selectedFileLabel->setText("No file selected");

    ui->dropTitle->setText("Drop a file here");
    ui->dropSubtitle->setText("or choose a file below");

    ui->statusLabel->setText("Status: Ready");

    setAcceptDrops(true);
    updateButtons();

    // The output-folder row used to be hidden/shown with the checkbox,
    // which collapsed/expanded the row's height and made everything
    // below it jump. Instead, keep it always visible and just disable
    // the Browse button when it isn't relevant -- nothing in the
    // layout moves.
    ui->browseOutputButton->setEnabled(false);
    outputFolderPath.clear();

    connect(
        ui->checkBox,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            ui->browseOutputButton->setEnabled(checked);

            if (checked)
            {
                if (outputFolderPath.isEmpty())
                {
                    ui->outputFolderPathLabel->setText("No folder selected");
                }
            }
            else
            {
                outputFolderPath.clear();
                ui->outputFolderPathLabel->setText("Same as input folder");
            }
        }
        );

    connect(
        ui->browseButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString filter = "All Files (*)";

            if (!inputFilePath.isEmpty())
            {
                QFileInfo info(inputFilePath);

                if (info.suffix().compare("huff", Qt::CaseInsensitive) == 0)
                {
                    filter = "Huffman Files (*.huff)";
                }
            }

            QString fileName = QFileDialog::getOpenFileName(
                this,
                "Select Input File",
                "",
                filter
                );

            if (!fileName.isEmpty())
            {
                setSelectedFile(fileName);

                if (!ui->checkBox->isChecked())
                {
                    ui->outputFolderPathLabel->setText("Same as input folder");
                }
            }
        }
        );

    connect(
        ui->browseOutputButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString folder = QFileDialog::getExistingDirectory(
                this,
                "Select Output Folder"
                );

            if (!folder.isEmpty())
            {
                outputFolderPath = folder;

                // outputFolderPathLabel is a read-only QLineEdit, so a
                // long path isn't clipped -- the user can click in and
                // scroll/select to see the rest of it. setText() also
                // leaves the cursor at the end, which conveniently
                // scrolls the view to show the final (most relevant)
                // folder name first.
                ui->outputFolderPathLabel->setText(folder);
            }
        }
        );

    connect(
        ui->compressButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (inputFilePath.isEmpty())
            {
                QMessageBox::warning(
                    this,
                    "Missing File",
                    "Please choose a file to compress."
                    );
                return;
            }

            QString outputFolder;

            if (ui->checkBox->isChecked())
            {
                if (outputFolderPath.isEmpty())
                {
                    QMessageBox::warning(
                        this,
                        "Missing Output Folder",
                        "Please choose an output folder."
                        );
                    return;
                }

                outputFolder = outputFolderPath;
            }
            else
            {
                outputFolder = QFileInfo(inputFilePath).absolutePath();
            }

            QString outputPath = QString::fromStdString(
                getAvailableFileName(
                    QDir(outputFolder).filePath("compressed").toStdString(),
                    "huff"
                    )
                );

            ui->compressButton->setEnabled(false);
            ui->decompressButton->setEnabled(false);

            ui->statusLabel->setText("Status: Compressing...");
            QApplication::processEvents();

            CompressionStats stats = runCompression(
                inputFilePath.toStdString(),
                outputPath.toStdString()
                );

            if (!stats.success)
            {
                ui->statusLabel->setText("Status: Compression Failed");

                QMessageBox::critical(
                    this,
                    "Compression Failed",
                    "Unable to compress the selected file."
                    );

                updateButtons();

                return;
            }

            ui->originalSizeLabel->setText(
                "Original Size: " +
                QString::fromStdString(formatFileSize(stats.originalSize))
                );

            ui->compressedSizeLabel->setText(
                "Compressed Size: " +
                QString::fromStdString(formatFileSize(stats.compressedSize))
                );

            ui->headerSizeLabel->setText(
                "Header Size: " +
                QString::fromStdString(formatFileSize(stats.headerSize))
                );

            ui->savingsLabel->setText(
                "Savings: " +
                QString::number(stats.savings, 'f', 2) + "%"
                );

            setSelectedFile(outputPath);

            ui->statusLabel->setText("Status: Compression Complete");

            QMessageBox::information(
                this,
                "Compression Complete",
                "Compression completed successfully!\n\nSaved as:\n" +
                    QFileInfo(outputPath).fileName()
                );
        }
        );

    connect(
        ui->decompressButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (inputFilePath.isEmpty())
            {
                QMessageBox::warning(
                    this,
                    "Missing File",
                    "Please choose a file to decompress."
                    );
                return;
            }

            QString outputFolder;

            if (ui->checkBox->isChecked())
            {
                if (outputFolderPath.isEmpty())
                {
                    QMessageBox::warning(
                        this,
                        "Missing Output Folder",
                        "Please choose an output folder."
                        );
                    return;
                }

                outputFolder = outputFolderPath;
            }
            else
            {
                outputFolder = QFileInfo(inputFilePath).absolutePath();
            }

            std::string extension = getStoredExtension(
                inputFilePath.toStdString()
                );

            QString outputPath = QString::fromStdString(
                getAvailableFileName(
                    QDir(outputFolder).filePath("decompressed").toStdString(),
                    extension
                    )
                );

            ui->compressButton->setEnabled(false);
            ui->decompressButton->setEnabled(false);

            ui->statusLabel->setText("Status: Decompressing...");
            QApplication::processEvents();

            bool success = runDecompression(
                inputFilePath.toStdString(),
                outputPath.toStdString()
                );

            if (!success)
            {
                ui->statusLabel->setText("Status: Decompression Failed");

                QMessageBox::critical(
                    this,
                    "Decompression Failed",
                    "Unable to decompress the selected file."
                    );

                updateButtons();

                return;
            }

            ui->originalSizeLabel->setText("Original Size: -");
            ui->compressedSizeLabel->setText("Compressed Size: -");
            ui->headerSizeLabel->setText("Header Size: -");
            ui->savingsLabel->setText("Savings: -");

            setSelectedFile(outputPath);

            ui->statusLabel->setText("Status: Decompression Complete");

            QMessageBox::information(
                this,
                "Decompression Complete",
                "Decompression completed successfully!\n\nSaved as:\n" +
                    QFileInfo(outputPath).fileName()
                );
        }
        );
}

void MainWindow::setSelectedFile(const QString &filePath)
{
    inputFilePath = filePath;

    QFileInfo info(filePath);

    bool isHuffman =
        info.suffix().compare("huff", Qt::CaseInsensitive) == 0;

    ui->selectedFileLabel->setText(
        "Selected File: <b>" + info.fileName() + "</b><br>"
                                                 "Detected: " +
        (
            isHuffman
                ? "<span style='color:#3B82F6;'><b>Ready to Decompress</b></span>"
                : "<span style='color:#22C55E;'><b>Ready to Compress</b></span>"
            )
        );

    ui->dropTitle->setText(
        isHuffman ? "Ready to Decompress" : "Ready to Compress"
        );

    ui->dropSubtitle->setText(info.fileName());

    ui->statusLabel->setText(
        isHuffman ? "Status: Ready to Decompress" : "Status: Ready to Compress"
        );

    ui->originalSizeLabel->setText("Original Size:");
    ui->compressedSizeLabel->setText("Compressed Size:");
    ui->headerSizeLabel->setText("Header Size:");
    ui->savingsLabel->setText("Savings:");

    updateButtons();
}

void MainWindow::updateButtons()
{
    if (inputFilePath.isEmpty())
    {
        ui->compressButton->setEnabled(false);
        ui->decompressButton->setEnabled(false);

        return;
    }

    bool isHuffman =
        QFileInfo(inputFilePath)
            .suffix()
            .compare("huff", Qt::CaseInsensitive) == 0;

    ui->compressButton->setEnabled(!isHuffman);
    ui->decompressButton->setEnabled(isHuffman);

    // Enabled/disabled appearance now comes entirely from styles.qss
    // (QPushButton:disabled), so there's no separate updateButtonStyles()
    // call here anymore -- the old version set an inline stylesheet with
    // smaller padding/radius than the rest of the app, which is why
    // Compress/Decompress looked cramped compared to every other button.
}

MainWindow::~MainWindow()
{
    delete ui;
}
