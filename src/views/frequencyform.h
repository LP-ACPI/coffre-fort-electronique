#ifndef FREQUENCYFORM_H
#define FREQUENCYFORM_H

#include <QDialog>
#include "ui_frequencydialog.h"


class FrequencyForm : public QDialog, private Ui::FrequancyDialog
{
    void init();
public:
    explicit FrequencyForm(QWidget *parent = 0);


};

#endif // FREQUENCYFORM_H
