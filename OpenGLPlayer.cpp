#include "OpenGLPlayer.h"
#include <QDebug>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

////GLSL3.0�汾��,������attribute�ؼ��֣��Լ�varying�ؼ��֣������Ա���ͳһ��in/out��Ϊǰ�ùؼ���
#define GL_VERSIONS  "#version 330 core\n"
#define GET_GLSTR(x) GL_VERSIONS#x


const char *vsrc = GET_GLSTR(

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}

);

const char *fsrc = GET_GLSTR(

out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texY;
uniform sampler2D texU;
uniform sampler2D texV;

void main()
{
	vec3 yuv;
	vec3 rgb;

	yuv.x = texture(texY, TexCoord).r;
	yuv.y = texture(texU, TexCoord).r - 0.5;
	yuv.z = texture(texV, TexCoord).r - 0.5;

	rgb = mat3(1.0,    1.0,     1.0,
		       0.0,    -0.3455, 1.779,
		       1.4075, -0.7169, 0.0) * yuv;

	FragColor = vec4(rgb, 1.0);
}

);

/*
YUVתRGB:

R = Y + 1.4075 * V;
G = Y - 0.3455 * U - 0.7169*V;
B = Y + 1.779 * U;
*/

OpenGLPlayer::OpenGLPlayer(QWidget *parent)
	: QOpenGLWidget(parent)
	, program(NULL)
	, m_nWidth(0)
	, m_nHeight(0)
{
	for (int i = 0; i < 3; i++)
		m_textureYUV[i] = NULL;
}


OpenGLPlayer::~OpenGLPlayer()
{
	makeCurrent();

	vao.destroy();
	vbo.destroy();

	if (program)
		program->deleteLater();

	for (int i = 0; i < 3; i++)
	{
		if (m_textureYUV[i] != NULL)
			m_textureYUV[i]->destroy();
	}

	doneCurrent();
}

void OpenGLPlayer::showYUV(QByteArray byte, int width, int height)
{
	m_mutex.lock();
	m_byteArray = byte;
	m_nWidth = width;
	m_nHeight = height;
	m_mutex.unlock();
	update();
}


/*virtual*/ void OpenGLPlayer::initializeGL()
{
	//Ϊ��ǰ������ʼ��OpenGL����
	initializeOpenGLFunctions();

	//���ñ���ɫΪ��ɫ
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//��ʼ���������
	for (int i = 0; i < 3; i++)
		m_textureYUV[i] = new QOpenGLTexture(QOpenGLTexture::Target2D);

	program = new  QOpenGLShaderProgram(this);
	program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
	program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
	program->link();
	program->bind();

	//��ʼ��VBO,���������ݴ洢��buffer��,�ȴ�VAO���������ͷ�
	float vertices[] = {
	   // ��������              //��������
	   -1.0f, -1.0f, 0.0f,  0.0f, 1.0f,        //����
	   1.0f , -1.0f, 0.0f,  1.0f, 1.0f,        //����
	   -1.0f, 1.0f,  0.0f,  0.0f, 0.0f,        //����
	   1.0f,  1.0f,  0.0f,  1.0f, 0.0f         //����
	};


	vbo.create();
	vbo.bind();              //�󶨵���ǰ��OpenGL������,
	vbo.allocate(vertices, sizeof(vertices));
	vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);  //����Ϊһ���޸ģ����ʹ��(���겻��,���ֻ�����ص�)

	//��ʼ��VAO,���ö�������״̬(���㣬���ߣ����������)
	vao.create();
	vao.bind();

	// void setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0);
	program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof(float));   //����aPos��������
	program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));   //����aColor������ɫ
	program->enableAttributeArray(0); //ʹ��
	program->enableAttributeArray(1);

	//������ж���
	vao.release();
	vbo.release();
}
/*virtual*/ void OpenGLPlayer::resizeGL(int w, int h)
{
	QOpenGLWidget::resizeGL(w, h);
}
/*virtual*/ void OpenGLPlayer::paintGL()
{
	if (m_nWidth == 0 || m_nHeight == 0)
		return;

	glClear(GL_COLOR_BUFFER_BIT);

	// ��ȾShader
	vao.bind();

	program->setUniformValue("texY", 0);
	program->setUniformValue("texU", 1);
	program->setUniformValue("texV", 2);

	m_mutex.lock();
	for (int i = 0; i < 3; i++)
	{
		int width = m_nWidth;
		int height = m_nHeight;
		if (i == 0)
		{
			yuvArr[i] = m_byteArray.mid(0, m_nWidth*m_nHeight);
		}
		else
		{
			width = m_nWidth / 2;
			height = m_nHeight / 2;

			yuvArr[i] = m_byteArray.mid(m_nWidth * m_nHeight + (m_nWidth*m_nHeight / 4) * (i - 1), m_nWidth * m_nHeight / 4);
		}

		if (m_textureYUV[i]->width() != width || m_textureYUV[i]->height() != height)
		{
			if (m_textureYUV[i]->isCreated())
				m_textureYUV[i]->destroy();

			m_textureYUV[i]->create();
			m_textureYUV[i]->setSize(width, height);
			m_textureYUV[i]->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
			m_textureYUV[i]->setFormat(QOpenGLTexture::R8_UNorm);
			m_textureYUV[i]->allocateStorage();        //�洢����(�Ŵ���С���ˡ���ʽ��size)
		}

		m_textureYUV[i]->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, yuvArr[i]);
		m_textureYUV[i]->bind(i);
	}
	m_mutex.unlock();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	for (int i = 0; i < 3; i++)
		m_textureYUV[i]->release();

	vao.release();       //���
}