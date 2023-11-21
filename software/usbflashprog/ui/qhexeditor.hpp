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
 * @file ui/qhexeditor.hpp
 * @brief Header of the QHexEditor Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef UI_QHEXEDITOR_HPP_
#define UI_QHEXEDITOR_HPP_

#include <qhexview.h>
#include "backend/epromfile/qepromfile.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief QHexEditor Class
 * @details An hexadecimal editor widget, based in QHexView.
 * @nosubgrouping
 */
class QHexEditor : public QHexView {
    Q_OBJECT

 public:
    /** @brief Hex Editor Mode (bits grouping). */
    enum QHexEditorMode {
      /** @brief 8 Bits Mode. */
      Mode8Bits,
      /** @brief 16 Bits Mode. */
      Mode16Bits,
      /** @brief 32 Bits Mode. */
      Mode32Bits
    };

 public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object (default = nullptr).
     */
    explicit QHexEditor(QWidget *parent = nullptr);
    /**
     * @brief Opens a file.
     * @param filename The filename.
     * @return True if success, false otherwise.
     */
    bool open(const QString& filename);
    /**
     * @brief Saves data onto a file.
     * @return True if success, false otherwise.
     */
    bool save(void);
    /**
     * @brief Saves loaded data as another file.
     * @param type Type of the file.
     * @param filename The filename.
     * @return True if success, false otherwise.
     */
    bool saveAs(QEpromFile::QEpromFileType type, const QString& filename);
    /**
     * @brief Fills all data with a value.
     * @param value Value to fill (default = 0xFF).
     */
    void fill(quint8 value = 0xFF);
    /**
     * @brief Fills all data with random values.
     */
    void random(void);
    /**
     * @brief Sets the data size of the viewer.
     * @param value Data size, in bytes.
     */
    void setSize(qint32 value);
    /**
     * @brief Gets the data size of the viewer.
     * @return Data size, in bytes.
     */
    qint32 size(void) const;
    /**
     * @brief Sets the bit grouping mode.
     * @param mode Bit grouping mode.
     */
    void setMode(QHexEditorMode mode);
    /**
     * @brief Gets the current bit grouping mode.
     * @return Bit grouping mode.
     */
    QHexEditorMode mode(void) const;
    /**
     * @brief Gets the current loaded filename.
     * @return Filename, or empty if nothing loaded.
     */
    const QString& filename(void) const;
    /**
     * @brief Returns if current data is changed and not saved.
     * @return True if data is changed and not saved, false otherwise.
     */
    bool isChanged(void) const;
    /**
     * @brief Shows the Find Dialog.
     */
    void showFindDialog(void);
    /**
     * @brief Shows the Replace Dialog.
     */
    void showReplaceDialog(void);

  Q_SIGNALS:
    /**
     * @brief Triggered when data is changed, loaded or saved.
     * @param status True if data is changed and not saved, false otherwise.
     */
    void changed(bool status = true);

 private slots:
    /*
     * @brief QHexView::dataChanged Signal event.
     * @param data Changed data.
     * @param offset Offset of changed data.
     * @param reason Reason of the change.
     */
    void onDataChanged(const QByteArray& data, quint32 offset,
                       QHexDocument::ChangeReason reason);

 private:
    /* @brief Data changed status. */
    bool changed_;
    /* @brief Current filename. */
    QString filename_;
    /* @brief Data viewer size. */
    qint32 size_;
    /* @brief Type of the current loaded file. */
    QEpromFile::QEpromFileType type_;
    /* @brief Editor mode. */
    QHexEditorMode mode_;
    /* @brief Set the group length
     * @param l length, in bytes */
    void setGroupLength(unsigned int l);
};

#endif  // UI_QHEXEDITOR_HPP_
