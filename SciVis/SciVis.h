#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SciVis.h"

class SciVis : public QMainWindow
{
    Q_OBJECT

public:
    SciVis(QWidget *parent = nullptr);
    virtual ~SciVis();

public slots:
	/*ui events**/
	// void on_checkBox_select_face_clicked(bool checked);
	// void on_checkBox_select_vertex_clicked(bool checked);
	void on_pushButton_open_file_clicked();
	void on_pushButton_display_solid_clicked();
	void on_pushButton_display_wireframes_clicked();
	void on_pushButton_display_vertex_clicked();

	void on_pushButton_code1_clicked();
	void on_pushButton_code2_clicked();
	void on_pushButton_code3_clicked();
	void on_pushButton_code4_clicked();
	void on_pushButton_code5_clicked();
	void on_pushButton_code6_clicked();
	void on_pushButton_code7_clicked();
	void on_pushButton_code8_clicked();

private:
    Ui::SciVisClass ui;
};
