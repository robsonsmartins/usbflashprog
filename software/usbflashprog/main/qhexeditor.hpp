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
 * @file main/qhexeditor.hpp
 * @brief Header of the QHexEditor Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef MAIN_QHEXEDITOR_HPP_
#define MAIN_QHEXEDITOR_HPP_

#include <qhexview.h>

// ---------------------------------------------------------------------------

class QHexEditor : public QHexView {
    Q_OBJECT

 public:
    explicit QHexEditor(QWidget *parent = nullptr);
    bool open(const QString& filename);
    bool save(void);
    bool saveAs(const QString& filename);
    void fill(quint8 value = 0xFF);
    void random(void);
    void setSize(qint64 value);
    qint64 size(void) const;
    const QString& filename(void) const;
    bool isChanged(void) const;
    void showFindDialog(void);
    void showReplaceDialog(void);

  Q_SIGNALS:
    void changed(bool status = true);

 private slots:
    void onDataChanged(const QByteArray& data, quint64 offset,
                       QHexDocument::ChangeReason reason);

 private:
    bool changed_;
    QString filename_;
    qint64 size_;
};

#endif  // MAIN_QHEXEDITOR_HPP_
