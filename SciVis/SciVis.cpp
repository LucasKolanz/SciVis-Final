#include "SciVis.h"
#include <QFileDialog>
extern int display_mode;
SciVis::SciVis(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

SciVis::~SciVis()
{}
// void SciVis::on_checkBox_select_face_clicked(bool checked) {
// 	ui.GLWindow->m_select_face = checked;
// }
// void SciVis::on_checkBox_select_vertex_clicked(bool checked) {
// 	ui.GLWindow->m_select_vertex = checked;
// }
void SciVis::on_pushButton_open_file_clicked() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open data"), "../../data/", tr("data Files (*.*)"));
	ui.GLWindow->openFile(fileName.toStdString().c_str());
	ui.GLWindow->update();
}
void SciVis::on_pushButton_display_solid_clicked() {
	display_mode = 1;
	ui.GLWindow->update();
}
void SciVis::on_pushButton_display_wireframes_clicked() {
	display_mode = 2;
	ui.GLWindow->update();
}
void SciVis::on_pushButton_display_vertex_clicked() {
	display_mode = 3;
	ui.GLWindow->vertexColor();
	ui.GLWindow->update();
}
void SciVis::on_pushButton_code1_clicked() {
	ui.GLWindow->algorithm1();
	ui.textEdit_output->append("run algorithm1!");
}
void SciVis::on_pushButton_code2_clicked() {
	ui.GLWindow->algorithm2();
	ui.textEdit_output->append("run algorithm2!");
}
void SciVis::on_pushButton_code3_clicked() {
	ui.GLWindow->algorithm3();
	ui.textEdit_output->append("run algorithm3!");
}

void SciVis::on_pushButton_code4_clicked() {
	ui.GLWindow->algorithm4();
	ui.textEdit_output->append("run algorithm4!");
}
void SciVis::on_pushButton_code5_clicked() {
	ui.GLWindow->algorithm5();
	ui.textEdit_output->append("run algorithm5!");
}
void SciVis::on_pushButton_code6_clicked() {
	ui.GLWindow->algorithm6();
	ui.textEdit_output->append("run algorithm6!");
}
void SciVis::on_pushButton_code7_clicked() {
	ui.GLWindow->algorithm7();
	ui.textEdit_output->append("run algorithm7!");
}
void SciVis::on_pushButton_code8_clicked() {
	ui.GLWindow->algorithm8();
	ui.textEdit_output->append("run algorithm8!");
}
