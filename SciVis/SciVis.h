#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SciVis.h"

//Max and min values for spinboxes
#define MAX_DX 200
#define MIN_DX 0.25
#define MAX_DY 200
#define MIN_DY 0.25
#define MAX_CONTOURS 250
#define MIN_CONTOURS 1 

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
	void on_pushButton_display_pointcloud_clicked();

	void on_doubleSpinBox_dx_valueChanged(double value);
    void on_doubleSpinBox_dy_valueChanged(double value);
    void on_spinBox_cont_valueChanged(int value);
    void on_pushButton_apply_clicked();


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
