// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @file ui/qhexeditor.cpp
 * @brief Implementation of the QHexEditor Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QFile>
#include <QFileInfo>
#include <QtMath>
#include <QRandomGenerator>
#include <model/buffer/qmemorybuffer.h>
#include <dialogs/hexfinddialog.h>
#include "qhexeditor.hpp"

// ---------------------------------------------------------------------------

QHexEditor::QHexEditor(QWidget *parent)
    : QHexView(parent),
      changed_(false),
      filename_(""),
      size_(0),
      type_(QEpromFile::EpromFileBin),
      mode_(Mode8Bits) {
    QFont font = this->font();
    font.setPointSize(10);
    this->setFont(font);
    this->setSizePolicy(QSizePolicy::Policy::Expanding,
                        QSizePolicy::Policy::Expanding);
    QHexOptions options;
    options.addresswidth = 5;
    options.asciilabel = "ASCII";
    options.headercolor = Qt::black;
    options.flags = QHexFlags::Styled | QHexFlags::Separators |
                    QHexFlags::HighlightAddress | QHexFlags::HighlightColumn;
    this->setOptions(options);
    setSize(1024);
    connect(this, &QHexView::dataChanged, this, &QHexEditor::onDataChanged);
}

bool QHexEditor::open(const QString &filename) {
    QEpromFile file;
    QByteArray data = file.read(filename, size_);
    if (data.isEmpty()) {
        return false;
    }
    QHexDocument *document = QHexDocument::fromMemory<QMemoryBuffer>(data);
    this->setDocument(document);
    filename_ = filename;
    changed_ = false;
    type_ = file.getType();
    emit changed(false);
    return true;
}

bool QHexEditor::save(void) {
    if (filename_.isEmpty()) {
        return false;
    }
    return saveAs(type_, filename_);
}

bool QHexEditor::saveAs(QEpromFile::QEpromFileType type,
                        const QString &filename) {
    QHexDocument *document = this->hexDocument();
    QByteArray data = document->read(0, static_cast<int>(document->length()));
    QEpromFile file;
    bool ret = file.write(type, filename, data);
    if (!ret) {
        return false;
    }
    filename_ = file.getFilename();
    changed_ = false;
    type_ = type;
    emit changed(false);
    return true;
}

void QHexEditor::putData(const QByteArray &data) {
    QHexDocument *document =
        QHexDocument::fromMemory<QMemoryBuffer>(data.left(size_));
    this->setDocument(document);
    changed_ = true;
    emit changed();
}

QByteArray QHexEditor::getData(void) const {
    QHexDocument *document = this->hexDocument();
    return document->read(0, static_cast<int>(document->length()));
}

void QHexEditor::fill(quint8 value) {
    QByteArray data = QByteArray(size_, value);
    QHexDocument *document = QHexDocument::fromMemory<QMemoryBuffer>(data);
    this->setDocument(document);
    changed_ = (value != 0xFF);
    emit changed(changed_);
}

void QHexEditor::random(void) {
    QByteArray data;
    for (qint32 i = 0; i < size_; i++) {
        data += (QRandomGenerator::global()->bounded(256) & 0xFF);
    }
    QHexDocument *document = QHexDocument::fromMemory<QMemoryBuffer>(data);
    this->setDocument(document);
    changed_ = true;
    emit changed();
}

void QHexEditor::setSize(qint32 value) {
    if (value == size_) {
        return;
    }
    QHexDocument *document = this->hexDocument();
    QByteArray data;
    if (document != nullptr) {
        int len = static_cast<int>(
            document->length() < value ? document->length() : value);
        data = document->read(0, len);
    }
    if (data.size() < value) {
        data.append(QByteArray(value - data.size(), 0xFF));
    }
    document = QHexDocument::fromMemory<QMemoryBuffer>(data);
    this->setDocument(document);
    size_ = value;
    emit changed(false);
}

qint32 QHexEditor::size(void) const {
    return size_;
}

void QHexEditor::setMode(QHexEditorMode mode) {
    mode_ = mode;
    switch (mode_) {
        case Mode32Bits:
            setGroupLength(4);
            break;
        case Mode16Bits:
            setGroupLength(2);
            break;
        case Mode8Bits:
        default:
            setGroupLength(1);
            break;
    }
}

QHexEditor::QHexEditorMode QHexEditor::mode(void) const {
    return mode_;
}

const QString &QHexEditor::filename(void) const {
    return filename_;
}

bool QHexEditor::isChanged(void) const {
    return changed_;
}

void QHexEditor::showFindDialog(void) {
    HexFindDialog dialog(HexFindDialog::Type::Find, this);
    dialog.setWindowFlags(dialog.windowFlags() &
                          ~Qt::WindowContextHelpButtonHint);
    dialog.exec();
}

void QHexEditor::showReplaceDialog(void) {
    HexFindDialog dialog(HexFindDialog::Type::Replace, this);
    dialog.setWindowFlags(dialog.windowFlags() &
                          ~Qt::WindowContextHelpButtonHint);
    dialog.exec();
    QByteArray data = getData();
    if (data.size() != size_) {
        QHexCursor *cursor = hexCursor();
        QHexPosition pos = cursor->position();
        qint64 start = cursor->selectionStartOffset();
        qint64 len = cursor->selectionLength();
        if (data.size() < size_) {
            data.append(QByteArray(size_ - data.size(), 0xFF));
        } else if (data.size() > size_) {
            data.resize(size_);
        }
        putData(data);
        cursor->select(start);
        cursor->selectSize(len);
        cursor->move(pos);
    }
}

void QHexEditor::onDataChanged(const QByteArray &data, quint32 offset,
                               QHexDocument::ChangeReason reason) {
    changed_ = true;
    emit changed();
}

void QHexEditor::keyPressEvent(QKeyEvent *e) {
    QHexCursor *cursor = hexCursor();
    auto start = cursor->selectionStartOffset();
    auto end = cursor->selectionEndOffset();
    switch (e->key()) {
        case Qt::Key_Backspace:
        case Qt::Key_Delete: {
            if (end - start < 0) {
                e->accept();
                return;
            }
            QByteArray data = getData();
            if (e->key() == Qt::Key_Backspace && start == end)
                data = data.remove(start - 1, end - start + 1);
            else
                data = data.remove(start, end - start + 1);
            data.append(QByteArray(end - start + 1, 0xFF));
            putData(data);
            if (e->key() == Qt::Key_Backspace)
                cursor->move(start - 1);
            else
                cursor->move(end);
            e->accept();
            break;
        }
        case Qt::Key_Insert:
            e->accept();
            break;
        default:
            if (cursor->hasSelection()) cursor->move(start);
            QHexView::keyPressEvent(e);
            break;
    }
}

void QHexEditor::setGroupLength(unsigned int l) {
    QHexView::setGroupLength(l);
}
