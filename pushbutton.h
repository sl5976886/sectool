//�Զ��尴ť��ʵ��Normal��Hover��Press
#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
class PushButton:public QPushButton
{
    Q_OBJECT
public:
   explicit PushButton(QWidget *parent = 0);
   ~PushButton();
   void getIconPath(QString icon_path);//�õ�ͼƬ��·�����֣��������ð�ťΪͼƬ�Ĵ�С
   void enterEvent(QEvent *);//�����¼�
   void leaveEvent(QEvent *);//�뿪�¼�
   void mousePressEvent(QMouseEvent *event);  //��갴���¼�
   void mouseReleaseEvent(QMouseEvent *event); //����ͷ��¼�
   void paintEvent(QPaintEvent *);//��ť��ͼ�¼�

private:
   enum ButtonStatus{NORMAL, HOVER, PRESS}; //ö�ٰ�ť�ļ���״̬
   ButtonStatus status;
   QString icon_path;//·����
   bool mouse_press; //��ť����Ƿ���
};
#endif // PUSHBUTTON_H
