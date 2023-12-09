#ifndef GLWIDGET_H_
#define GLWIDGET_H_

#include <QOpenGLWidget>
#include <QKeyEvent>
// #include "libraries/glew-2.1.0/include/GL/glew.h"



class GLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit GLWidget(QWidget *parent = 0);

	~GLWidget();

public:
	bool m_select_face;
	bool m_select_vertex;
	bool pointcloudOn = true;

	void openFile(const char* file);

	void algorithm1();
	void algorithm2();
	void algorithm3();
	void algorithm4();
	void algorithm5();
	void algorithm6();
	void algorithm7();
	void algorithm8();

    void vertexColor();
    void togglePtcloud();
protected:

	virtual void paintGL();
	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void wheelEvent(QWheelEvent  *event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void mouseDoubleClickEvent(QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent * event);

	void motion(int x, int y);

private:
	bool mFullScreen;
};

#endif // !GLWIDGET_H_

