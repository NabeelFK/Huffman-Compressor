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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Huffman Compressor");

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

    ui->browseOutputButton->hide();
    ui->outputFolderLabel->hide();
    ui->outputFolderPathLabel->hide();

    outputFolderPath.clear();

    connect(
        ui->checkBox,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            ui->browseOutputButton->setVisible(checked);
            ui->outputFolderLabel->setVisible(checked);
            ui->outputFolderPathLabel->setVisible(checked);

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

        updateButtonStyles();

        return;
    }

    bool isHuffman =
        QFileInfo(inputFilePath)
            .suffix()
            .compare("huff", Qt::CaseInsensitive) == 0;

    ui->compressButton->setEnabled(!isHuffman);
    ui->decompressButton->setEnabled(isHuffman);

    updateButtonStyles();
}

void MainWindow::updateButtonStyles()
{
    const QString enabledStyle =
        "QPushButton {"
        "background-color:#2563EB;"
        "color:white;"
        "border-radius:6px;"
        "padding:6px 12px;"
        "}"
        "QPushButton:hover {"
        "background-color:#3B82F6;"
        "}"
        "QPushButton:pressed {"
        "background-color:#1D4ED8;"
        "}";

    const QString disabledStyle =
        "QPushButton {"
        "background-color: #2F2F2F;"
        "color: #8A8A8A;"
        "border-radius: 6px;"
        "padding: 6px 12px;"
        "}";

    ui->compressButton->setStyleSheet(
        ui->compressButton->isEnabled() ? enabledStyle : disabledStyle
        );

    ui->decompressButton->setStyleSheet(
        ui->decompressButton->isEnabled() ? enabledStyle : disabledStyle
        );
}


MainWindow::~MainWindow()
{
    delete ui;
}