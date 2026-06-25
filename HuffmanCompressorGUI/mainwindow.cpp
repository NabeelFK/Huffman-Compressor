#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "compressor.hpp"
#include "utils.hpp"

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(
        ui->browseButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString fileName = QFileDialog::getOpenFileName(
                this,
                "Select Input File"
                );

            if (!fileName.isEmpty())
            {
                ui->inputFileEdit->setText(fileName);
            }
        }
    );

    connect(
        ui->browseOutputButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString fileName = QFileDialog::getSaveFileName(
                this,
                "Select Output File"
                );

            if (!fileName.isEmpty())
            {
                ui->outputFileEdit->setText(fileName);
            }
        }
    );

    connect(
        ui->compressButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString inputPath = ui->inputFileEdit->text();
            QString outputPath = ui->outputFileEdit->text();

            if (inputPath.isEmpty() || outputPath.isEmpty())
            {
                QMessageBox::warning(
                    this,
                    "Missing Files",
                    "Please select both an input and output file."
                );
                return;
            }

            ui->compressButton->setEnabled(false);

            ui->statusLabel->setText("Status: Compressing...");
            QApplication::processEvents();

            CompressionStats stats = runCompression(
                inputPath.toStdString(),
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

                ui->compressButton->setEnabled(true);

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

            ui->statusLabel->setText("Status: Compression Complete");

            ui->inputFileEdit->setText(outputPath);

            ui->compressButton->setEnabled(true);

            QMessageBox::information(
                this,
                "Success",
                "Compression completed successfully!"
            );
        }
    );

    connect(
        ui->decompressButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString inputPath = ui->inputFileEdit->text();
            QString outputPath = ui->outputFileEdit->text();

            if (inputPath.isEmpty() || outputPath.isEmpty())
            {
                QMessageBox::warning(
                    this,
                    "Missing Files",
                    "Please select both an input and output file."
                );
                return;
            }

            ui->decompressButton->setEnabled(false);

            ui->statusLabel->setText("Status: Decompressing...");
            QApplication::processEvents();

            bool success = runDecompression(
                inputPath.toStdString(),
                outputPath.toStdString()
            );

            if (!success)
            {
                ui->statusLabel->setText("Status: Decompression Failed");

                ui->decompressButton->setEnabled(true);

                QMessageBox::critical(
                    this,
                    "Decompression Failed",
                    "Unable to decompress the selected file."
                );

                return;
            }

            ui->originalSizeLabel->setText("Original Size: -");
            ui->compressedSizeLabel->setText("Compressed Size: -");
            ui->headerSizeLabel->setText("Header Size: -");
            ui->savingsLabel->setText("Savings: -");

            ui->statusLabel->setText("Status: Decompression Complete");

            // Make it easy to recompress the output if desired.
            ui->inputFileEdit->setText(outputPath);

            ui->decompressButton->setEnabled(true);

            QMessageBox::information(
                this,
                "Success",
                "Decompression completed successfully!"
            );
        }
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
