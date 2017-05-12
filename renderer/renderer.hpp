#pragma once

#include <QtWidgets/QMainWindow>
#include <QGLWidget>

#include <iostream>

class renderer_t : public QGLWidget
{
   Q_OBJECT

public:
   renderer_t( QWidget * parent = 0 );

private slots:
   void advanceGears();

protected:
   void initializeGL();
   void paintGL();
   void resizeGL(int width, int height);
   void mousePressEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);

public slots:
   void setXRotation(int angle);
   void setYRotation(int angle);
   void setZRotation(int angle);

signals:
   void xRotationChanged(int angle);
   void yRotationChanged(int angle);
   void zRotationChanged(int angle);

private:
   //tmp
   GLuint makeGear(const GLfloat *reflectance, GLdouble innerRadius,
                               GLdouble outerRadius, GLdouble thickness,
                               GLdouble toothSize, GLint toothCount);
   void normalizeAngle(int *angle);
   void drawGear(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz,
                 GLdouble angle);

   GLuint gear1;
   GLuint gear2;
   GLuint gear3;
   int xRot;
   int yRot;
   int zRot;
   int gear1Rot;

   QPoint lastPos;
};