#define MIN_A_Z_UNICODE 65
#define MAX_A_Z_UNICODE 90
#define MIN_a_z_UNICODE 97
#define MAX_a_z_UNICODE 122

#include "MainWindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
#include <QString>
#include <QFile>
#include <QDate>
#include <QTime>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    //текст
    QLabel *label_text;
    label_text = new QLabel("Текст для шифрования:");
    label_text->setMinimumSize(400, 30);
    label_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    line_edit_text = new QLineEdit();
    line_edit_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    line_edit_text->setValidator(new QRegExpValidator(QRegExp(tr("[a-zA-Z .,?!:;]+")), this));
    line_edit_text->setMinimumSize(400, 30);

    QLabel *label_cryption;
    label_cryption = new QLabel("Зашифрованный текст:");
    label_cryption->setMinimumSize(400, 30);
    label_cryption->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    line_edit_cryption = new QLineEdit();
    line_edit_cryption->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    line_edit_cryption->setMinimumSize(400, 30);

    //main_text
    QVBoxLayout *V_layout_text_main = new QVBoxLayout;
    V_layout_text_main->addWidget(label_text);
    V_layout_text_main->addWidget(line_edit_text);
    V_layout_text_main->addWidget(label_cryption);
    V_layout_text_main->addWidget(line_edit_cryption);


    //настройки
    combo_box_setting = new QComboBox();
    combo_box_setting->setMinimumSize(30, 30);
    list_cription << "Шифр Цезаря" << "Шифр Атбаш";
    combo_box_setting->addItems(list_cription);

    QLabel *label_offset;
    label_offset = new QLabel("Смещение: ");
    label_offset->setMinimumSize(100, 30);

    line_edit_offset = new QLineEdit();
    line_edit_offset->setMinimumSize(200, 30);
    line_edit_offset->setValidator(new QRegExpValidator(QRegExp(tr("[0-9]+")), this));
    line_edit_offset->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QHBoxLayout *H_layout_offset = new QHBoxLayout;
    H_layout_offset->addWidget(label_offset);
    H_layout_offset->addWidget(line_edit_offset);


    push_button_cryption = new QPushButton(tr("Зашифровать"));
    push_button_cryption->setMinimumSize(30, 30);
    line_edit_cryption->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);


    push_button_encryption = new QPushButton(tr("Расшифровать"));
    push_button_encryption->setMinimumSize(30, 30);
    line_edit_cryption->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QVBoxLayout *V_layout_setting = new QVBoxLayout;
    V_layout_setting->addWidget(combo_box_setting);
    V_layout_setting->addLayout(H_layout_offset);
    V_layout_setting->addWidget(push_button_cryption);
    V_layout_setting->addWidget(push_button_encryption);

    QHBoxLayout *H_layout_main = new QHBoxLayout;
    H_layout_main->addLayout(V_layout_text_main);
    H_layout_main->addSpacing(50);
    H_layout_main->addLayout(V_layout_setting);


    //лог
    QLabel *label_log;
    label_log = new QLabel("Лог: ");
    label_log->setMinimumSize(100, 30);

    text_edit_log = new QTextEdit();
    text_edit_log->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    push_button_save_log = new QPushButton(tr("Сохранить"));
    push_button_save_log->setMinimumSize(30, 30);
    line_edit_cryption->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QHBoxLayout *H_layout_save = new QHBoxLayout;
    H_layout_save->addWidget(push_button_save_log);

    //main
    QVBoxLayout *V_layout_main = new QVBoxLayout;
    V_layout_main->addLayout(H_layout_main);
     V_layout_main->addSpacing(20);
    V_layout_main->addWidget(label_log);
    V_layout_main->addWidget(text_edit_log);
    V_layout_main->addLayout(H_layout_save);


    setLayout(V_layout_main);
    setWindowTitle(tr("test_GosNIIAS"));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);


    connect(push_button_cryption, &QPushButton::clicked, this, &MainWindow::slot_button_cryption);
    connect(push_button_encryption,  &QPushButton::clicked, this, &MainWindow::slot_button_encryption);
    connect(push_button_save_log,  &QPushButton::clicked, this, &MainWindow::save_log);
}


MainWindow::~MainWindow()
{
}


void MainWindow::slot_button_cryption()
{
    cryption_button_data(cryption::crypt);
}


void MainWindow::slot_button_encryption()
{
    cryption_button_data(cryption::encrypt);
}


void MainWindow::cryption_button_data(cryption cryp)
{
    QString data;
    int type_cryp = 0;
    int offset = 0;

    if (!get_data_form(data, type_cryp, offset)) return;
    QString encryption_data = cryption_data(data, type_cryp, cryp, offset);
    if (encryption_data.isNull()) return;

    line_edit_cryption->setText(encryption_data);

    QString old_log = text_edit_log->toPlainText();

    QString new_log = QString("Дата: %1\n"
                               "Время: %2\n"
                               "Входные данные: %3\n"
                               "Тип шифрования: %4\n"
                               "Операция: %5\n"
                               "Смещение: %6\n"
                               "Выходные данные: %7\n\n\n")
            .arg((QDate::currentDate()).toString())
            .arg((QTime::currentTime()).toString())
            .arg(data)
            .arg(list_cription.at(type_cryp))
            .arg(cryp==cryption::crypt?"Шифрование":"Расшифрование")
            .arg(offset)
            .arg(encryption_data);


    text_edit_log->setText(new_log + old_log);
}

bool MainWindow::get_data_form(QString &Data, int &type_cryp, int &offset)
{
    type_cryp = combo_box_setting->currentIndex();

    if(type_cryp == 0)//для цезаря
    {
        QString offset_str = line_edit_offset->text();
        if (offset_str.isEmpty())
        {
            QMessageBox::warning(this,
                                 "Внимание",
                                 "Не заполнено поле \"смещение\"",
                                 QMessageBox::Ok);
            return false;
        }

        bool ok;
        offset = offset_str.toInt(&ok, 10);
        if (!ok)
        {
            QMessageBox::warning(this,
                                 "Внимание",
                                 "Не заполнено поле \"смещение\"",
                                 QMessageBox::Ok);
            return false;
        }
    }


    Data = line_edit_text->text();
    return true;
}


QString MainWindow::cryption_data(QString &data, int type_cryp, cryption cryp, int offset)
{

    QString cription_data;
    if (type_cryp == 0)
    {
        offset = offset % 25;
        if (cryp == cryption::encrypt)
        {
            offset = -offset;
        }

        for (int i = 0; i < data.size(); i++)
        {
            int curent_unicode = static_cast<int>(data.at(i).unicode());
            int new_unicode = curent_unicode + offset;
            if ((curent_unicode >= MIN_a_z_UNICODE) && (curent_unicode <= MAX_a_z_UNICODE))
            {
                if (new_unicode < MIN_a_z_UNICODE)
                {
                    new_unicode = MAX_a_z_UNICODE - MIN_a_z_UNICODE +new_unicode + 1;
                }
                else if (new_unicode > MAX_a_z_UNICODE)
                {
                    new_unicode = MIN_a_z_UNICODE - MAX_a_z_UNICODE + new_unicode - 1;
                }
            }
            else if((curent_unicode >= MIN_A_Z_UNICODE) && (curent_unicode <= MAX_A_Z_UNICODE))
            {
                if (new_unicode < MIN_A_Z_UNICODE)
                {
                    new_unicode = MAX_A_Z_UNICODE - MIN_A_Z_UNICODE + new_unicode + 1;
                }
                else if (new_unicode > MAX_A_Z_UNICODE)
                {
                    new_unicode = MIN_A_Z_UNICODE - MAX_A_Z_UNICODE + new_unicode - 1;
                }
            }
            else
            {
                new_unicode = curent_unicode;
            }

            cription_data.append(QString(QChar(new_unicode)));
        }


    }
    else if (type_cryp == 1)
    {
        qDebug() << "type_cryp == 1";
        for (int i = 0; i < data.size(); i++)
        {
            int curent_unicode = static_cast<int>(data.at(i).unicode());
            int new_unicode;
            if ((curent_unicode >= MIN_a_z_UNICODE) && (curent_unicode <= MAX_a_z_UNICODE))
            {
                new_unicode = MAX_a_z_UNICODE - (curent_unicode - MIN_a_z_UNICODE);
                qDebug() << "1";

            }
            else if((curent_unicode >= MIN_A_Z_UNICODE) && (curent_unicode <= MAX_A_Z_UNICODE))
            {
                new_unicode = MAX_A_Z_UNICODE - (curent_unicode - MIN_A_Z_UNICODE);
                qDebug() << "2";
            }
            else
            {
                new_unicode = curent_unicode;
                qDebug() << "3";
            }

            cription_data.append(QString(QChar(new_unicode)));
        }
    }

    return cription_data;
}


bool MainWindow::save_log()
{
    QFile file("log.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        return false;
    }

    QTextStream writeStream(&file);
    writeStream << text_edit_log->toPlainText();
    file.close();

    return true;
}
