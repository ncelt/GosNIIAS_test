#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>


enum cryption
{
    crypt = 0,
    encrypt = 1
};

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void slot_button_cryption(); //слот обработки клавиши "шифрование"
    void slot_button_encryption(); //слот обработки клавиши "расшифрование"


private:
    QStringList list_cription;

    QComboBox *combo_box_setting = nullptr;
    QPushButton *push_button_cryption = nullptr;
    QPushButton *push_button_encryption = nullptr;
    QPushButton *push_button_save_log = nullptr;
    QLineEdit *line_edit_offset = nullptr;

    QLineEdit *line_edit_text = nullptr;
    QLineEdit *line_edit_cryption = nullptr;

    QTextEdit *text_edit_log = nullptr;

    void cryption_button_data(cryption cryp);
    QString cryption_data(QString &data, int type_cryp, cryption cryp, int offset); //зашифровка данных
    bool get_data_form(QString &Data, int &type_cryp, int &offset);//сбор данных с формы
    bool save_log(); //сохранение лога
};
#endif // MAINWINDOW_H
