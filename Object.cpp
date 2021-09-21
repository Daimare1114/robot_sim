#include <stdio.h>
#include "ObjectData.h"
#include "C:\glfw-3.3.2.bin.WIN64\include\GLFW\glfw3.h"

namespace CG {

	void CGROBOT::SetLighting(void) {
		
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightAmbient);
		glEnable(GL_LIGHTING);	//�Ɩ����f����L���ɂ���
		glEnable(GL_LIGHT0);	//������0�Ԗڂ�L���ɂ���(0�Ԗځ`7�Ԗڂ܂Ŏg�p�\�D�����ɕ����̌�����ݒ�ł���炵��)
		glEnable(GL_COLOR_MATERIAL);	//���̂̎�����L���ɂ���
	}

	//STL�f�[�^�̓ǂݍ���
	void CGROBOT::Read_STData(const char *FileName, STL_ObjectData *Object) {
		FILE *fp;
		char byte80[80];	//STL�t�@�C���̍ŏ��̃R�[�h���i�[����
		unsigned short byte2;	//
		unsigned int n = 0;	//
		float x = 0;
		int i = 0, j = 0;

		//�t�@�C�����J��
		fp = fopen(FileName, "rb");

		//�ŏ��̗]���ȏ�񂾂��ǂݍ��݂�i�߂�
		fread(byte80, sizeof(char), 80, fp);

		//�ʂ̑�����ǂݍ���
		fread(&Object->N, sizeof(unsigned int), 1, fp);
		printf("Object->N:%d\n", Object->N);

		//�I�u�W�F�N�g�̏���ǂݍ���
		for (i = 0; i < Object->N; i++) {
			//�@���x�N�g���̓ǂݍ���
			fread(&Object->Surface[i].NormalVector.x, sizeof(float), 1, fp);
			fread(&Object->Surface[i].NormalVector.y, sizeof(float), 1, fp);
			fread(&Object->Surface[i].NormalVector.z, sizeof(float), 1, fp);

			//�O�p�ʂ𐬂�3���_��ǂݍ���
			for (j = 0; j < 3; j++) {
				fread(&Object->Surface[i].TrianglePoint[j].x, sizeof(float), 1, fp);
				fread(&Object->Surface[i].TrianglePoint[j].y, sizeof(float), 1, fp);
				fread(&Object->Surface[i].TrianglePoint[j].z, sizeof(float), 1, fp);
			}
			//�]���ȏ��̓ǂݍ��݂�i�߂�
			fread(&byte2, sizeof(unsigned short), 1, fp);
		}
		fclose(fp);
	}

	//STL�f�[�^�̕\��
	void CGROBOT::DrawSTL(STL_ObjectData *Object, CADColor *color) {
		int i = 0;
		int j = 0;

		glColor3f(color->red, color->green, color->blue);
		glLineWidth(2.0);
		for (i = 0; i < Object->N; i++) {
			glBegin(GL_POLYGON);
			glNormal3f(Object->Surface[i].NormalVector.x, Object->Surface[i].NormalVector.y, Object->Surface[i].NormalVector.z);//�ʂ̖@�����w��
			for (j = 0; j < 3; j++) {
				glVertex3f(Object->Surface[i].TrianglePoint[j].x, Object->Surface[i].TrianglePoint[j].y, Object->Surface[i].TrianglePoint[j].z);//�ʂ̂�`��
			}
			glEnd();
		}

	}

	//���ʂ̕`��
	void CGROBOT::DrawField(void) {
		glLineWidth(2.0);
		glBegin(GL_POLYGON);
		glColor3d(0., 0.5, 0.);
		glVertex3d(-FieldX, -FieldY, 0);
		glVertex3d(FieldX, -FieldY, 0);
		glVertex3d(FieldX, FieldY, 0);
		glVertex3d(-FieldX, FieldY, 0);
		glEnd();
	}

	//���W�n�̕\��
	void CGROBOT::DrawCoodinate(float AxisSize) {
		glLineWidth(3.0);//���̑������w��
		//���̕`��J�n
		//�����������񏑂��Ƃ��́CGL_LINES��
		glBegin(GL_LINES);
		//x��(��)
		glColor3d(1., 0., 0.);
		glVertex3d(0., 0., 0.);//���̎n�_(x,y,z)
		glVertex3d(AxisSize, 0., 0.);
		//y��(��)
		glColor3d(0., 1., 0.);
		glVertex3d(0., 0., 0.);//���̎n�_(x,y,z)
		glVertex3d(0., AxisSize, 0.);
		//z��(��)
		glColor3d(0., 0., 1.);
		glVertex3d(0., 0., 0.);//���̎n�_(x,y,z)
		glVertex3d(0., 0., AxisSize);
		glEnd();//���̕`��I��
	}

	//�`�F�X�{�[�h�̕`��
	void CGROBOT::DrawChessBoard(float PatternSize) {
		int num = 0;//���W�̔ԍ�

		//�`��
		num = 0;
		for (int i = 0; i < PAT_ROWS - 1; i++)
		{
			for (int j = 0; j < PAT_COLS - 1; j++)
			{
				if (num % 2 == 0) {//�i�q�̍��オ����
					glColor3d(0., 0., 0.);
					glBegin(GL_POLYGON);
					glVertex3d(i * PatternSize, j * PatternSize, 1);
					glVertex3d(i * PatternSize, (j + 1) * PatternSize, 1);
					glVertex3d((i + 1) * PatternSize, (j + 1) * PatternSize, 1);
					glVertex3d((i + 1) * PatternSize, j * PatternSize, 1);
					glEnd();
				}
				else {
					glColor3d(1., 1., 1.);
					glBegin(GL_POLYGON);
					glVertex3d(i * PatternSize, j * PatternSize, 1);
					glVertex3d(i * PatternSize, (j + 1) * PatternSize, 1);
					glVertex3d((i + 1) * PatternSize, (j + 1) * PatternSize, 1);
					glVertex3d((i + 1) * PatternSize, j * PatternSize, 1);
					glEnd();
				}
				num++;
			}
		}

	}

	//SCARA���{�b�g�̕`��
	void CGROBOT::DrawScaraRB(RBModel *Model, float *JointAngle, VECTOR *HandPos) {
		//���{�b�g�x�[�X�̕`��
		glPushMatrix();
		glTranslated(BASE_OFFSET_X, BASE_OFFSET_Y, BASE_OFFSET_Z);
		DrawSTL(&Model->LinkData[0], &Model->LinkColor[0]);

		//�A�[��1�̕`��
		glTranslated(0, 0, 0);
		glRotated(JointAngle[0], 0, 0, 1);
		DrawCoodinate(20);
		DrawSTL(&Model->LinkData[1], &Model->LinkColor[1]);

		//�A�[��2�̕`��
		glTranslated(75.0, 0, 0);
		glRotated(JointAngle[1], 0, 0, 1);
		DrawCoodinate(20);
		DrawSTL(&Model->LinkData[2], &Model->LinkColor[2]);
		glPopMatrix();

		//�G���h�G�t�F�N�^�̕`��
		glPushMatrix();
		glTranslated(HandPos->x, HandPos->y, HandPos->z);
		DrawCoodinate(50);
		glPopMatrix();
	}
}